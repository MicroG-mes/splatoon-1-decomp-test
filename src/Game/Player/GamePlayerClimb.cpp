#include "Game/Player/GamePlayerClimb.h"
#include <cmath>

namespace Game {

GamePlayerClimb::GamePlayerClimb()
    : mState(PlayerClimbState::cDetached),
      mWallNormal(0.0f, 0.0f, 1.0f),
      mWallTopY(0.0f),
      mClimbOffsetY(0.0f),
      mClimbRootOffsetY(0.0f),
      mClimbHideOffsetY(-0.5f),
      mCameraFrontAngle(0.0f),
      mTimer(0) {
}

GamePlayerClimb::~GamePlayerClimb() {
}

void GamePlayerClimb::init() {
    GambitActor::init();
    mState = PlayerClimbState::cDetached;
    mWallNormal.set(0.0f, 0.0f, 1.0f);
    mWallTopY = 0.0f;
    mClimbOffsetY = 0.0f;
    mClimbRootOffsetY = 0.0f;
    mClimbHideOffsetY = -0.5f;
    mCameraFrontAngle = 0.0f;
    mTimer = 0;
}

bool GamePlayerClimb::attachToWall(const sead::Vector3f& wallNormal, f32 wallTopY) {
    mWallNormal = wallNormal;
    mWallTopY = wallTopY;
    mState = PlayerClimbState::cAttached;
    mClimbOffsetY = 0.0f;
    mTimer = 0;
    return true;
}

void GamePlayerClimb::detachFromWall() {
    mState = PlayerClimbState::cDetached;
    mClimbOffsetY = 0.0f;
    mTimer = 0;
}

void GamePlayerClimb::updateClimbMotion(f32 stickY, f32 stickX, bool hasFriendlyInk) {
    if (mState == PlayerClimbState::cDetached) {
        return;
    }

    if (!hasFriendlyInk) {
        // Without friendly ink, player slips down the wall
        mState = PlayerClimbState::cSlipDown;
        mClimbOffsetY += cSlipGravity;
        return;
    }

    mState = PlayerClimbState::cSwimming;

    // Vertical climb displacement
    if (stickY > 0.1f) {
        mClimbOffsetY += stickY * cClimbSpeedUp;
    } else if (stickY < -0.1f) {
        mClimbOffsetY += stickY * cClimbSpeedDown;
    }

    // Ledge mantle detection
    if (mClimbOffsetY >= mWallTopY && mWallTopY > 0.0f) {
        mState = PlayerClimbState::cVaultLedge;
    }
}

void GamePlayerClimb::update() {
    if (mState == PlayerClimbState::cDetached) {
        return;
    }

    mTimer++;
    if (mState == PlayerClimbState::cVaultLedge) {
        // Ledge pop-over animation completes after a few frames
        if (mTimer > 10) {
            detachFromWall();
        }
    }
}

void GamePlayerClimb::draw() {
    // Model deformation and ink ripple particles handled by PlayerModel
}

} // namespace Game
