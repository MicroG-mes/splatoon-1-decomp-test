#include "Game/Net/AutoWarpPoint.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cmath>

namespace Game {

AutoWarpPoint::AutoWarpPoint()
    : mStartPos(0.0f, 0.0f, 0.0f),
      mTargetPos(0.0f, 0.0f, 0.0f),
      mCurrentPos(0.0f, 0.0f, 0.0f),
      mState(SuperJumpState::cIdle),
      mTimer(0),
      mWindupDuration(cDefaultWindupFrames),
      mFlightDuration(cDefaultFlightFrames),
      mTeamId(0),
      mHasStealthJump(false) {
}

AutoWarpPoint::~AutoWarpPoint() {
}

void AutoWarpPoint::init() {
    GambitActor::init();
    mStartPos.set(0.0f, 0.0f, 0.0f);
    mTargetPos.set(0.0f, 0.0f, 0.0f);
    mCurrentPos.set(0.0f, 0.0f, 0.0f);
    mState = SuperJumpState::cIdle;
    mTimer = 0;
    mWindupDuration = cDefaultWindupFrames;
    mFlightDuration = cDefaultFlightFrames;
    mTeamId = 0;
    mHasStealthJump = false;
}

void AutoWarpPoint::launchSuperJump(const sead::Vector3f& startPos, const sead::Vector3f& targetPos, u32 teamId, bool hasStealthJump, s32 quickJumpDiscountFrames) {
    mStartPos = startPos;
    mTargetPos = targetPos;
    mCurrentPos = startPos;
    mTeamId = teamId;
    mHasStealthJump = hasStealthJump;

    mWindupDuration = cDefaultWindupFrames - quickJumpDiscountFrames;
    if (mWindupDuration < 25) {
        mWindupDuration = 25;
    }

    mFlightDuration = cDefaultFlightFrames;
    mState = SuperJumpState::cWindupSquat;
    mTimer = 0;
}

bool AutoWarpPoint::isLandingMarkerVisible(const sead::Vector3f& observerPos, u32 observerTeam) const {
    if (mState != SuperJumpState::cAirborneParabola) {
        return false;
    }

    // Always visible to teammates
    if (observerTeam == mTeamId) {
        return true;
    }

    // Stealth Jump hides landing marker from enemies unless they are within close proximity (6.0m)
    if (mHasStealthJump) {
        f32 dx = observerPos.x - mTargetPos.x;
        f32 dz = observerPos.z - mTargetPos.z;
        f32 distSq = dx * dx + dz * dz;
        return (distSq <= 6.0f * 6.0f);
    }

    return true; // Standard jump: always visible to enemies
}

void AutoWarpPoint::stepFlightPhysics() {
    f32 t = static_cast<f32>(mTimer) / static_cast<f32>(mFlightDuration);
    if (t > 1.0f) t = 1.0f;

    // Horizontal linear interpolation
    mCurrentPos.x = mStartPos.x + (mTargetPos.x - mStartPos.x) * t;
    mCurrentPos.z = mStartPos.z + (mTargetPos.z - mStartPos.z) * t;

    // Parabolic vertical trajectory: 4 * h * t * (1 - t)
    f32 parabola = 4.0f * cMaxApexHeight * t * (1.0f - t);
    f32 baseY = mStartPos.y + (mTargetPos.y - mStartPos.y) * t;
    mCurrentPos.y = baseY + parabola;
}

void AutoWarpPoint::update() {
    mTimer++;

    switch (mState) {
        case SuperJumpState::cWindupSquat:
            if (mTimer >= mWindupDuration) {
                mState = SuperJumpState::cAirborneParabola;
                mTimer = 0;
            }
            break;

        case SuperJumpState::cAirborneParabola:
            stepFlightPhysics();
            if (mTimer >= mFlightDuration) {
                mState = SuperJumpState::cTouchdownSplash;
                mTimer = 0;
                mCurrentPos = mTargetPos;

                // Ink splash on landing touchdown
                PaintTextureMgr* paint = PaintTextureMgr::instance();
                if (paint) {
                    paint->splatInk(mTargetPos, 2.0f, mTeamId);
                }
            }
            break;

        case SuperJumpState::cTouchdownSplash:
            if (mTimer >= 15) {
                mState = SuperJumpState::cCompleted;
                mTimer = 0;
            }
            break;

        case SuperJumpState::cCompleted:
        case SuperJumpState::cIdle:
        default:
            break;
    }
}

void AutoWarpPoint::draw() {
    if (mState == SuperJumpState::cAirborneParabola) {
        GambitActor::draw();
    }
}

} // namespace Game
