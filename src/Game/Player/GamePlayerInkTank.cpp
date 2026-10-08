#include "Game/Player/GamePlayerInkTank.h"

namespace Game {

GamePlayerInkTank::GamePlayerInkTank()
    : mInkLevel(cMaxInk),
      mSubWeaponThreshold(70.0f),
      mRecoveryDelayTimer(0),
      mBlinkTimer(0),
      mRecoveryState(InkRecoveryState::cHumanSlow) {
}

GamePlayerInkTank::~GamePlayerInkTank() {
}

void GamePlayerInkTank::init() {
    GambitActor::init();
    mInkLevel = cMaxInk;
    mSubWeaponThreshold = 70.0f;
    mRecoveryDelayTimer = 0;
    mBlinkTimer = 0;
    mRecoveryState = InkRecoveryState::cHumanSlow;
}

bool GamePlayerInkTank::consumeInk(f32 amount) {
    if (mInkLevel < amount) {
        return false;
    }

    mInkLevel -= amount;
    if (mInkLevel < 0.0f) {
        mInkLevel = 0.0f;
    }

    // Reset delay before ink begins recovering
    mRecoveryDelayTimer = cDefaultDelayFrames;
    return true;
}

void GamePlayerInkTank::replenishInk(f32 amount) {
    mInkLevel += amount;
    if (mInkLevel > cMaxInk) {
        mInkLevel = cMaxInk;
    }
}

void GamePlayerInkTank::updateRecharge(bool isSquidSubmerged, bool isHumanoid, bool inFriendlyInk) {
    if (mRecoveryDelayTimer > 0) {
        mRecoveryDelayTimer--;
        mRecoveryState = InkRecoveryState::cDelayWait;
        return;
    }

    if (isSquidSubmerged && inFriendlyInk) {
        // Fast recharge when submerged in friendly ink
        mRecoveryState = InkRecoveryState::cSquidFast;
        replenishInk(cSquidRechargePerFrame);
    } else if (isHumanoid) {
        // Slow passive recharge
        mRecoveryState = InkRecoveryState::cHumanSlow;
        replenishInk(cHumanRechargePerFrame);
    } else {
        mRecoveryState = InkRecoveryState::cBlocked;
    }
}

void GamePlayerInkTank::update() {
    if (isLowInk()) {
        mBlinkTimer++;
    } else {
        mBlinkTimer = 0;
    }
}

void GamePlayerInkTank::draw() {
    // Ink tank gauge HUD and back-mounted tank model visual levels handled by PlayerModel
}

} // namespace Game
