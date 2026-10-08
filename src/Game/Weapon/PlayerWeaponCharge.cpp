#include "Game/Weapon/PlayerWeaponCharge.h"
#include <algorithm>

namespace Game {

PlayerWeaponCharge::PlayerWeaponCharge()
    : mState(ChargeState::cIdle),
      mChargeFrames(0),
      mMaxChargeFrames(60),
      mChargeRatio(0.0f),
      mMinDamage(40.0f),
      mMaxDamage(160.0f),
      mMinRange(15.0f),
      mMaxRange(60.0f),
      mMinVelocity(1.2f),
      mMaxVelocity(3.6f),
      mIsScopeEquipped(false) {
}

PlayerWeaponCharge::~PlayerWeaponCharge() {
}

void PlayerWeaponCharge::init() {
    PlayerWeaponBase::init();
    mState = ChargeState::cIdle;
    mChargeFrames = 0;
    mChargeRatio = 0.0f;
}

void PlayerWeaponCharge::startCharging() {
    if (mState == ChargeState::cIdle) {
        mState = ChargeState::cCharging;
        mChargeFrames = 0;
        mChargeRatio = 0.0f;
    }
}

void PlayerWeaponCharge::releaseCharge() {
    if (mState == ChargeState::cCharging || mState == ChargeState::cFullCharge) {
        mState = ChargeState::cFiring;
        fireChargeShot();
    }
}

f32 PlayerWeaponCharge::computeDamage() const {
    return mMinDamage + (mMaxDamage - mMinDamage) * mChargeRatio;
}

f32 PlayerWeaponCharge::computeRange() const {
    return mMinRange + (mMaxRange - mMinRange) * mChargeRatio;
}

f32 PlayerWeaponCharge::computeVelocity() const {
    return mMinVelocity + (mMaxVelocity - mMinVelocity) * mChargeRatio;
}

void PlayerWeaponCharge::fireChargeShot() {
    // Projectile generation handled by bullet manager
    mState = ChargeState::cCooldown;
}

void PlayerWeaponCharge::update() {
    PlayerWeaponBase::update();

    if (mState == ChargeState::cCharging) {
        mChargeFrames++;
        mChargeRatio = std::min(1.0f, static_cast<f32>(mChargeFrames) / static_cast<f32>(mMaxChargeFrames));
        if (mChargeFrames >= mMaxChargeFrames) {
            mState = ChargeState::cFullCharge;
            mChargeRatio = 1.0f;
        }
    } else if (mState == ChargeState::cCooldown) {
        mChargeFrames = 0;
        mChargeRatio = 0.0f;
        mState = ChargeState::cIdle;
    }
}

void PlayerWeaponCharge::draw() {
    PlayerWeaponBase::draw();
}

} // namespace Game
