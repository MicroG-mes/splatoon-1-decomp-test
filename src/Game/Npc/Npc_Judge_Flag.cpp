#include "Game/Npc/Npc_Judge_Flag.h"
#include <cmath>

namespace Game {

Npc_Judge_Flag::Npc_Judge_Flag()
    : mPosition(0.0f, 0.0f, 0.0f),
      mState(JudgeState::cSleeping),
      mStateCode(0),
      mVibeRank(JudgeVibeRank::cDry),
      mWinningTeam(255),
      mAwardedSnails(0),
      mFlagAnimationTimer(0),
      mIsPlayerNearby(false) {
}

Npc_Judge_Flag::~Npc_Judge_Flag() = default;

void Npc_Judge_Flag::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mState = JudgeState::cSleeping;
    mStateCode = 0;
    mVibeRank = JudgeVibeRank::cDry;
    mWinningTeam = 255;
    mAwardedSnails = 0;
    mFlagAnimationTimer = 0;
    mIsPlayerNearby = false;
}

void Npc_Judge_Flag::setup(const sead::Vector3f& cushionPos) {
    mPosition = cushionPos;
    mState = JudgeState::cSleeping;
    mStateCode = 0;
}

void Npc_Judge_Flag::update() {
    GambitActor::update();
    vfunc_3();
}

void Npc_Judge_Flag::draw() {
    GambitActor::draw();
}

/**
 * Npc_Judge_Flag__vfunc_3 @ 0x026186C4
 * AI decision and state cycle. Transitions between sleeping, waking, and flag celebration.
 */
void Npc_Judge_Flag::vfunc_3() {
    if (mState == JudgeState::cJudgingFlag) {
        mFlagAnimationTimer++;
        if (mFlagAnimationTimer > 180) { // 3 seconds celebration
            mState = JudgeState::cAwake;
            mFlagAnimationTimer = 0;
        }
    } else if (mState == JudgeState::cRewarding) {
        mFlagAnimationTimer++;
        if (mFlagAnimationTimer > 120) {
            mState = JudgeState::cAwake;
            mFlagAnimationTimer = 0;
        }
    }
}

u32 Npc_Judge_Flag::vfunc_11() {
    return static_cast<u32>(mState);
}

/**
 * Npc_Judge_Flag__vfunc_60 @ 0x02618FB8
 * Super Sea Snail bonus award query.
 */
u32 Npc_Judge_Flag::vfunc_60() {
    return mAwardedSnails;
}

/**
 * Npc_Judge_Flag__vfunc_47 @ 0x026191AC
 * Raises Alpha (0) or Bravo (1) victory flag.
 */
void Npc_Judge_Flag::vfunc_47(u8 winningTeam) {
    mWinningTeam = winningTeam;
    mState = JudgeState::cJudgingFlag;
    mFlagAnimationTimer = 0;
    mStateCode = (winningTeam == 0) ? 1 : 2;
}

void Npc_Judge_Flag::updateProximity(const sead::Vector3f& playerPos, f32 wakeRadius) {
    f32 dx = playerPos.x - mPosition.x;
    f32 dz = playerPos.z - mPosition.z;
    f32 dist = std::sqrt(dx * dx + dz * dz);

    if (dist <= wakeRadius) {
        mIsPlayerNearby = true;
        if (mState == JudgeState::cSleeping) {
            mState = JudgeState::cAwake; // Judd wakes up and yawns!
        }
    } else {
        mIsPlayerNearby = false;
        if (mState == JudgeState::cAwake && mFlagAnimationTimer == 0) {
            mState = JudgeState::cSleeping; // Judd goes back to sleep on his cushion
        }
    }
}

void Npc_Judge_Flag::judgeMatch(f32 alphaPercent, f32 bravoPercent) {
    if (alphaPercent > bravoPercent) {
        vfunc_47(0); // Team Alpha wins
    } else if (bravoPercent > alphaPercent) {
        vfunc_47(1); // Team Bravo wins
    } else {
        vfunc_47(255); // Tie / Draw
    }
}

u32 Npc_Judge_Flag::awardSuperSeaSnails(f32 vibePoints) {
    if (vibePoints >= 15.0f) {
        mVibeRank = JudgeVibeRank::cSoHot;
        mAwardedSnails = 3; // Max Super Sea Snail payout
    } else if (vibePoints >= 10.0f) {
        mVibeRank = JudgeVibeRank::cSmokin;
        mAwardedSnails = 2;
    } else if (vibePoints >= 4.0f) {
        mVibeRank = JudgeVibeRank::cWarm;
        mAwardedSnails = 1;
    } else {
        mVibeRank = JudgeVibeRank::cDry;
        mAwardedSnails = 0;
    }

    mState = JudgeState::cRewarding;
    mFlagAnimationTimer = 0;
    return mAwardedSnails;
}

} // namespace Game
