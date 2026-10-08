#include "Game/Rule/GameRuleRanked.h"
#include <algorithm>
#include <cmath>

namespace Game {

// -----------------------------------------------------------------------------
// GachiAreaTimer (Splat Zones Rule Engine)
// -----------------------------------------------------------------------------

GachiAreaTimer::GachiAreaTimer()
    : mAlphaCount(cMaxCount)
    , mBravoCount(cMaxCount)
    , mAlphaPenalty(0)
    , mBravoPenalty(0)
    , mAlphaAccumSec(0.0f)
    , mBravoAccumSec(0.0f)
    , mLastControl(SplatZoneOwner::cNeutral)
    , mIsOvertime(false)
{
}

GachiAreaTimer::~GachiAreaTimer() {
}

void GachiAreaTimer::reset() {
    mAlphaCount = cMaxCount;
    mBravoCount = cMaxCount;
    mAlphaPenalty = 0;
    mBravoPenalty = 0;
    mAlphaAccumSec = 0.0f;
    mBravoAccumSec = 0.0f;
    mLastControl = SplatZoneOwner::cNeutral;
    mIsOvertime = false;
}

void GachiAreaTimer::update(SplatZoneOwner currentControl, f32 deltaTime) {
    // Detect loss of control to apply Splatoon penalty formula: floor((100 - score) * 0.75)
    if (mLastControl == SplatZoneOwner::cTeamAlpha && currentControl != SplatZoneOwner::cTeamAlpha) {
        if (mAlphaCount < cMaxCount) {
            s32 penalty = static_cast<s32>(std::floor((cMaxCount - mAlphaCount) * cPenaltyMultiplier));
            mAlphaPenalty = (std::max)(mAlphaPenalty, penalty);
        }
    } else if (mLastControl == SplatZoneOwner::cTeamBravo && currentControl != SplatZoneOwner::cTeamBravo) {
        if (mBravoCount < cMaxCount) {
            s32 penalty = static_cast<s32>(std::floor((cMaxCount - mBravoCount) * cPenaltyMultiplier));
            mBravoPenalty = (std::max)(mBravoPenalty, penalty);
        }
    }
    mLastControl = currentControl;

    // Tick countdown for team holding the zones (1 point per second = 60 frames)
    if (currentControl == SplatZoneOwner::cTeamAlpha && mAlphaCount > 0) {
        mAlphaAccumSec += deltaTime;
        while (mAlphaAccumSec >= 1.0f) {
            mAlphaAccumSec -= 1.0f;
            if (mAlphaPenalty > 0) {
                mAlphaPenalty--;
            } else if (mAlphaCount > 0) {
                mAlphaCount--;
            }
        }
    } else {
        mAlphaAccumSec = 0.0f;
    }

    if (currentControl == SplatZoneOwner::cTeamBravo && mBravoCount > 0) {
        mBravoAccumSec += deltaTime;
        while (mBravoAccumSec >= 1.0f) {
            mBravoAccumSec -= 1.0f;
            if (mBravoPenalty > 0) {
                mBravoPenalty--;
            } else if (mBravoCount > 0) {
                mBravoCount--;
            }
        }
    } else {
        mBravoAccumSec = 0.0f;
    }

    // Overtime evaluation
    if (mIsOvertime) {
        // If trailing team loses control, match immediately ends
        u32 leading = getWinnerTeam();
        SplatZoneOwner trailingOwner = (leading == 0) ? SplatZoneOwner::cTeamBravo : SplatZoneOwner::cTeamAlpha;
        if (currentControl != trailingOwner) {
            mIsOvertime = false;
        } else {
            // If trailing team surpasses leader's score, trailing team takes the victory!
            if (leading == 0 && mBravoCount < mAlphaCount) {
                mIsOvertime = false; // Trailing Bravo overtook Alpha
            } else if (leading == 1 && mAlphaCount < mBravoCount) {
                mIsOvertime = false; // Trailing Alpha overtook Bravo
            }
        }
    }
}

u32 GachiAreaTimer::getWinnerTeam() const {
    if (mAlphaCount < mBravoCount) return 0;
    if (mBravoCount < mAlphaCount) return 1;
    // Tie-breaker: whoever has less penalty
    return (mAlphaPenalty <= mBravoPenalty) ? 0 : 1;
}

bool GachiAreaTimer::checkOvertimeNeeded(f32 matchTimeRemaining, SplatZoneOwner currentControl) const {
    if (matchTimeRemaining > 0.0f) return false;
    if (isKnockout()) return false;

    // Overtime occurs if trailing team currently controls the zones when regulation ends
    u32 leader = getWinnerTeam();
    SplatZoneOwner trailingOwner = (leader == 0) ? SplatZoneOwner::cTeamBravo : SplatZoneOwner::cTeamAlpha;
    if (currentControl == trailingOwner) {
        const_cast<GachiAreaTimer*>(this)->mIsOvertime = true;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// GachiHokoPedestal (Rainmaker Goal Pedestal)
// -----------------------------------------------------------------------------

GachiHokoPedestal::GachiHokoPedestal()
    : mAlphaPedestal(0.0f, 0.0f, -50.0f)
    , mBravoPedestal(0.0f, 0.0f, 50.0f)
    , mAlphaBestDist(100)
    , mBravoBestDist(100)
    , mInitialDistance(100.0f)
{
}

GachiHokoPedestal::~GachiHokoPedestal() {
}

void GachiHokoPedestal::init(const sead::Vector3f& alphaPedestalPos, const sead::Vector3f& bravoPedestalPos) {
    mAlphaPedestal = alphaPedestalPos;
    mBravoPedestal = bravoPedestalPos;
    mInitialDistance = (bravoPedestalPos - alphaPedestalPos).length();
    if (mInitialDistance < 1.0f) mInitialDistance = 100.0f;
    reset();
}

void GachiHokoPedestal::reset() {
    mAlphaBestDist = 100;
    mBravoBestDist = 100;
}

s32 GachiHokoPedestal::updateCarrierDistance(const sead::Vector3f& carrierPos, u32 carrierTeam) {
    // Team Alpha advances toward Bravo Pedestal; Team Bravo advances toward Alpha Pedestal
    const sead::Vector3f& targetPedestal = (carrierTeam == 0) ? mBravoPedestal : mAlphaPedestal;
    f32 distToGoal = (carrierPos - targetPedestal).length();

    // Map linear distance to 100 -> 0 counts
    f32 ratio = distToGoal / (mInitialDistance * 0.5f);
    s32 count = (std::max)(1, (std::min)(100, static_cast<s32>(ratio * 100.0f)));

    if (carrierTeam == 0) {
        if (count < mAlphaBestDist) mAlphaBestDist = count;
        return mAlphaBestDist;
    } else {
        if (count < mBravoBestDist) mBravoBestDist = count;
        return mBravoBestDist;
    }
}

bool GachiHokoPedestal::checkGoalTouchdown(const sead::Vector3f& carrierPos, u32 carrierTeam) const {
    const sead::Vector3f& targetPedestal = (carrierTeam == 0) ? mBravoPedestal : mAlphaPedestal;
    sead::Vector3f diff = carrierPos - targetPedestal;
    f32 horizontalDistSq = diff.x * diff.x + diff.z * diff.z;
    f32 verticalDist = std::abs(diff.y);

    // Goal pedestal has a 2.8m radius top plateau
    if (horizontalDistSq <= (2.8f * 2.8f) && verticalDist <= 2.2f) {
        // Touchdown! Instant Knockout!
        if (carrierTeam == 0) {
            const_cast<GachiHokoPedestal*>(this)->mAlphaBestDist = 0;
        } else {
            const_cast<GachiHokoPedestal*>(this)->mBravoBestDist = 0;
        }
        return true;
    }
    return false;
}

u32 GachiHokoPedestal::getWinnerTeam() const {
    return (mAlphaBestDist <= mBravoBestDist) ? 0 : 1;
}

} // namespace Game
