#include "Game/ShootingRange/LytShootingRangeHandler.h"

namespace Game {

LytShootingRangeHandler::LytShootingRangeHandler()
    : mTotalDamage(0.0f),
      mCurrentDps(0.0f),
      mDamageInWindow(0.0f),
      mDpsWindowTimer(0),
      mIsExitRequested(false),
      mAnimTimer(0) {
}

LytShootingRangeHandler::~LytShootingRangeHandler() {
}

void LytShootingRangeHandler::init() {
    GambitActor::init();
    mField.init();
    mTotalDamage = 0.0f;
    mCurrentDps = 0.0f;
    mDamageInWindow = 0.0f;
    mDpsWindowTimer = 0;
    mIsExitRequested = false;
}

void LytShootingRangeHandler::recordDamageDealt(f32 damage) {
    mTotalDamage += damage;
    mDamageInWindow += damage;
}

void LytShootingRangeHandler::handleInput(const VPADStatus& vpad) {
    // Touch on DRC reset button or press Minus button
    if ((vpad.trigger & VPAD_BUTTON_MINUS) || (vpad.tpData.touched && vpad.tpData.x < 300 && vpad.tpData.y > 600)) {
        mField.resetAllInk();
        mTotalDamage = 0.0f;
        mCurrentDps = 0.0f;
        mDamageInWindow = 0.0f;
    }

    // Press B to leave shooting range and return to Ammo Knights
    if (vpad.trigger & VPAD_BUTTON_B) {
        mField.exitToShop();
        mIsExitRequested = true;
    }
}

void LytShootingRangeHandler::update() {
    mAnimTimer++;
    mDpsWindowTimer++;

    // Calculate rolling 1-second DPS
    if (mDpsWindowTimer >= 60) {
        mCurrentDps = mDamageInWindow;
        mDamageInWindow = 0.0f;
        mDpsWindowTimer = 0;
    }

    mField.update();
}

void LytShootingRangeHandler::draw() {
    GambitActor::draw();
    mField.draw();
}

} // namespace Game
