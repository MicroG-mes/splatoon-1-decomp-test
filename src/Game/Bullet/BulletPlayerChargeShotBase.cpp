#include "Game/Bullet/BulletPlayerChargeShotBase.h"
#include <cassert>
#include <cmath>

namespace Game {

BulletPlayerChargeShotBase::BulletPlayerChargeShotBase()
    : mChargeRatio(1.0f),
      mBeamLength(30.0f),
      mIsPiercing(false) {
}

BulletPlayerChargeShotBase::~BulletPlayerChargeShotBase() = default;

void BulletPlayerChargeShotBase::init() {
    GameBullet::init();
}

void BulletPlayerChargeShotBase::update() {
    GameBullet::update();
}

void BulletPlayerChargeShotBase::draw() {
    GameBullet::draw();
}

void BulletPlayerChargeShotBase::vfunc_4() {
}

void BulletPlayerChargeShotBase::vfunc_135(u32* param2) {
    if (!param2) {
        assert(param2 != nullptr);
        return;
    }
    *param2 = 0;
}

// =========================================================================
// Charge_Light (0x10047408)
// =========================================================================

Charge_Light::Charge_Light()
    : mMinDamage(40.0f),
      mMaxDamage(100.0f),
      mFullChargeDamage(160.0f),
      mOwnerPlayerId(0),
      mVelocity(0.0f, 0.0f, 0.0f),
      mTraveledDistance(0.0f),
      mBulletStateFlags(0) {
}

Charge_Light::~Charge_Light() = default;

void Charge_Light::init() {
    BulletPlayerChargeShotBase::init();
    mTraveledDistance = 0.0f;
    mBulletStateFlags = 0;
    mIsPiercing = (mChargeRatio >= cFullChargeThreshold);
}

void Charge_Light::setDamageParams(f32 minDamage, f32 maxDamage, f32 fullChargeDamage) {
    mMinDamage = minDamage;
    mMaxDamage = maxDamage;
    mFullChargeDamage = fullChargeDamage;
}

/**
 * Charge_Light__vfunc_7 @ 0x0224d060
 * Integrates charger bullet velocity using fast inverse square root normalization
 * and distance accumulation.
 */
void Charge_Light::vfunc_7() {
    f32 vx = mVelocity.x;
    f32 vy = mVelocity.y;
    f32 vz = mVelocity.z;
    f32 speedSq = vx * vx + vy * vy + vz * vz;

    if (mBulletStateFlags & 0x8000) {
        if (speedSq > 0.0001f) {
            // PowerPC frsqrte + Newton-Raphson inverse square root
            f32 invSpeed = 1.0f / std::sqrt(speedSq);
            mVelocity.x *= invSpeed;
            mVelocity.y *= invSpeed;
            mVelocity.z *= invSpeed;
        }
    }

    mTraveledDistance += std::sqrt(speedSq);
}

void Charge_Light::vfunc_11() {
    // Collision checking against actors and KCL stage mesh
    // If mIsPiercing is true, does not consume bullet on player contact
}

/**
 * Charge_Light__vfunc_88 @ 0x0224eeec
 * Authentic PowerPC damage calculation:
 * - If target is same player as owner -> returns 0.0f (DAT_10048e18)
 * - If chargeRatio < 1.0f -> lerp(minDamage, maxDamage, chargeRatio)
 * - If chargeRatio >= 1.0f -> fullChargeDamage (160.0f)
 * - If targetType == 0xC (Rainmaker Shield) -> damage * 2.8f (DAT_10048e5c)
 * - If targetType is object -> damage * 2.0f (DAT_10048e20)
 */
f64 Charge_Light::vfunc_88(u32 targetType, const s32* pTargetOwnerId) {
    if (pTargetOwnerId && *pTargetOwnerId == mOwnerPlayerId) {
        return static_cast<f64>(cSelfDamageMultiplier);
    }

    f32 damage = 0.0f;
    if (mChargeRatio < cFullChargeThreshold) {
        damage = (mMaxDamage - mMinDamage) * mChargeRatio + mMinDamage;
    } else {
        damage = mFullChargeDamage;
    }

    if (targetType != 0) {
        if (targetType == 0xC) {
            // Shachihoko (Rainmaker) shield receives 2.8x damage
            damage *= cShachihokoShieldMultiplier;
        } else {
            // Generic breakable objects receive 2.0x damage
            damage *= cObjectDamageMultiplier;
        }
    }

    return static_cast<f64>(damage);
}

f32 Charge_Light::getCalculatedDamage(u32 targetType) const {
    s32 dummyId = -1;
    return static_cast<f32>(const_cast<Charge_Light*>(this)->vfunc_88(targetType, &dummyId));
}

void Charge_Light::update() {
    BulletPlayerChargeShotBase::update();
    vfunc_7();
    vfunc_11();
}

void Charge_Light::draw() {
    BulletPlayerChargeShotBase::draw();
}

// =========================================================================
// BulletPlayerChargeHitSplash
// =========================================================================

BulletPlayerChargeHitSplash::BulletPlayerChargeHitSplash()
    : mSplashRadius(1.2f),
      mSplashTimer(0) {
}

BulletPlayerChargeHitSplash::~BulletPlayerChargeHitSplash() = default;

void BulletPlayerChargeHitSplash::init() {
    GameBullet::init();
    mSplashTimer = 0;
}

void BulletPlayerChargeHitSplash::update() {
    GameBullet::update();
    vfunc_7();
}

void BulletPlayerChargeHitSplash::draw() {
    GameBullet::draw();
}

void BulletPlayerChargeHitSplash::vfunc_7() {
    mSplashTimer++;
}

void BulletPlayerChargeHitSplash::vfunc_11() {
}

void BulletPlayerChargeHitSplash::vfunc_135(u32* param2) {
    if (!param2) {
        assert(param2 != nullptr);
        return;
    }
    *param2 = 0;
}

void BulletPlayerChargeHitSplash::vfunc_147() {
}

} // namespace Game
