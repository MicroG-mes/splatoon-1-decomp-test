#include "Game/Weapon/GameWeaponCharger.h"
#include "Game/Paint/PaintTextureMgr.h"

namespace Game {

GameWeaponCharger::GameWeaponCharger()
    : mState(ChargerState::cIdle),
      mStateTimer(0),
      mChargeRatio(0.0f),
      mFramesToFullCharge(60),
      mBaseDamage(40.0f),
      mMaxDamage(160.0f),
      mBaseRange(15.0f),
      mMaxRange(38.0f),
      mIsLaserActive(false) {
}

GameWeaponCharger::~GameWeaponCharger() {
}

void GameWeaponCharger::init() {
    GambitActor::init();
    mState = ChargerState::cIdle;
}

f32 GameWeaponCharger::computeDamage() const {
    return mBaseDamage + mChargeRatio * (mMaxDamage - mBaseDamage);
}

f32 GameWeaponCharger::computeRange() const {
    return mBaseRange + mChargeRatio * (mMaxRange - mBaseRange);
}

void GameWeaponCharger::startCharging() {
    if (mState == ChargerState::cIdle) {
        mState = ChargerState::cCharging;
        mChargeRatio = 0.0f;
        mIsLaserActive = true;
        mStateTimer = 0;
    }
}

void GameWeaponCharger::releaseCharge() {
    if (mState == ChargerState::cCharging || mState == ChargerState::cFullCharge) {
        fireShot();
        mState = ChargerState::cFiring;
        mIsLaserActive = false;
        mStateTimer = 0;
    }
}

void GameWeaponCharger::fireShot() {
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        f32 range = computeRange();
        // Paint continuous straight line from player forward
        paint->splatInk(sead::Vector3f(0.0f, 0.0f, range * 0.5f), 1.2f, 0);
    }
}

void GameWeaponCharger::update() {
    mStateTimer++;

    switch (mState) {
        case ChargerState::cCharging:
            mChargeRatio += 1.0f / static_cast<f32>(mFramesToFullCharge);
            if (mChargeRatio >= 1.0f) {
                mChargeRatio = 1.0f;
                mState = ChargerState::cFullCharge;
            }
            break;

        case ChargerState::cFiring:
            if (mStateTimer >= 15) { // Fire recoil cooldown
                mState = ChargerState::cIdle;
                mChargeRatio = 0.0f;
            }
            break;

        default:
            break;
    }
}

void GameWeaponCharger::draw() {
    GambitActor::draw();
}

} // namespace Game
