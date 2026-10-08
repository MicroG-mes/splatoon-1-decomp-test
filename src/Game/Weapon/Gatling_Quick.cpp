#include "Game/Weapon/Gatling_Quick.h"
#include "Game/Paint/PaintTextureMgr.h"

namespace Game {

Gatling_Quick::Gatling_Quick()
    : mState(GatlingState::cIdle),
      mChargeLevel(0.0f),
      mBarrelSpinSpeed(0.0f),
      mRemainingBullets(0),
      mShotTimer(0),
      mTeamId(0) {
}

Gatling_Quick::~Gatling_Quick() {
}

void Gatling_Quick::init() {
    GambitActor::init();
    mState = GatlingState::cIdle;
    mChargeLevel = 0.0f;
    mBarrelSpinSpeed = 0.0f;
    mRemainingBullets = 0;
    mShotTimer = 0;
    mTeamId = 0;
}

void Gatling_Quick::startCharging() {
    if (mState == GatlingState::cIdle || mState == GatlingState::cCooldown) {
        mState = GatlingState::cCharging;
        mChargeLevel = 0.0f;
    }
}

void Gatling_Quick::releaseTrigger() {
    if (mState == GatlingState::cCharging) {
        if (mChargeLevel >= 0.2f) {
            // Convert charge level to bullet count: up to ~30 bullets at 2.0 full charge
            mRemainingBullets = static_cast<u32>(mChargeLevel * 15.0f);
            mState = GatlingState::cFiring;
            mShotTimer = 0;
        } else {
            // Not enough charge for burst
            mState = GatlingState::cCooldown;
            mRemainingBullets = 0;
        }
    }
}

bool Gatling_Quick::canShoot() const {
    return mState == GatlingState::cFiring && mRemainingBullets > 0;
}

void Gatling_Quick::fireBullet() {
    if (mRemainingBullets == 0) {
        return;
    }

    mRemainingBullets--;
    
    // Splat bullet impact forward
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        sead::Vector3f bulletImpact(0.0f, 0.0f, 12.0f);
        paint->splatInk(bulletImpact, 1.2f, mTeamId);
    }
}

void Gatling_Quick::update() {
    switch (mState) {
        case GatlingState::cIdle:
            if (mBarrelSpinSpeed > 0.0f) {
                mBarrelSpinSpeed -= 0.05f;
                if (mBarrelSpinSpeed < 0.0f) {
                    mBarrelSpinSpeed = 0.0f;
                }
            }
            break;

        case GatlingState::cCharging:
            mChargeLevel += cChargeRate;
            if (mChargeLevel > cMaxCharge) {
                mChargeLevel = cMaxCharge;
            }
            // Accelerate barrel rotation with charge
            mBarrelSpinSpeed = mChargeLevel * 0.5f;
            break;

        case GatlingState::cFiring:
            mShotTimer++;
            if (mShotTimer >= cFireIntervalFrames) {
                mShotTimer = 0;
                fireBullet();

                if (mRemainingBullets == 0) {
                    mState = GatlingState::cCooldown;
                    mChargeLevel = 0.0f;
                }
            }
            break;

        case GatlingState::cCooldown:
            mBarrelSpinSpeed -= 0.08f;
            if (mBarrelSpinSpeed <= 0.0f) {
                mBarrelSpinSpeed = 0.0f;
                mState = GatlingState::cIdle;
            }
            break;
    }
}

void Gatling_Quick::draw() {
    GambitActor::draw();
}

} // namespace Game
