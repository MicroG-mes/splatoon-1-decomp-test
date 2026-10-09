#include "Game/MapObj/Lft_ClimbLift.h"
#include <cmath>
#include <algorithm>

namespace Game {

Lft_ClimbLift::Lft_ClimbLift()
    : mState(ClimbLiftState::cMovingUp),
      mCurrentY(0.0f),
      mBottomY(0.0f),
      mTopY(12.0f),
      mSpeed(1.5f),
      mPauseTimer(0),
      mIsInkCovered(true) {
}

Lft_ClimbLift::~Lft_ClimbLift() {
}

void Lft_ClimbLift::init() {
    GambitActor::init();
    vfunc_3();
    vfunc_5();
}

void Lft_ClimbLift::vfunc_3() {
    mModel = sead::BfresParser::createClimbLiftModel("Lft_ClimbLift");
}

void Lft_ClimbLift::vfunc_5() {
    mState = ClimbLiftState::cMovingUp;
    mCurrentY = mBottomY;
    mPauseTimer = 0;
}

void Lft_ClimbLift::vfunc_7() {
    const f32 kDt = 1.0f / 60.0f;

    switch (mState) {
        case ClimbLiftState::cMovingUp:
            mCurrentY += mSpeed * kDt;
            if (mCurrentY >= mTopY) {
                mCurrentY = mTopY;
                mState = ClimbLiftState::cPausedAtTop;
                mPauseTimer = 90; // Pause 1.5 seconds at top
            }
            break;

        case ClimbLiftState::cPausedAtTop:
            if (mPauseTimer > 0) {
                mPauseTimer--;
            } else {
                mState = ClimbLiftState::cMovingDown;
            }
            break;

        case ClimbLiftState::cMovingDown:
            mCurrentY -= mSpeed * kDt;
            if (mCurrentY <= mBottomY) {
                mCurrentY = mBottomY;
                mState = ClimbLiftState::cPausedAtBottom;
                mPauseTimer = 90; // Pause 1.5 seconds at bottom
            }
            break;

        case ClimbLiftState::cPausedAtBottom:
            if (mPauseTimer > 0) {
                mPauseTimer--;
            } else {
                mState = ClimbLiftState::cMovingUp;
            }
            break;

        default:
            break;
    }
}

void Lft_ClimbLift::update() {
    vfunc_7();
}

void Lft_ClimbLift::draw() {
}

void Lft_ClimbLift::setTravelBounds(f32 bottomY, f32 topY) {
    mBottomY = bottomY;
    mTopY = topY;
}

void Lft_ClimbLift::setSpeed(f32 speed) {
    mSpeed = speed;
}

} // namespace Game
