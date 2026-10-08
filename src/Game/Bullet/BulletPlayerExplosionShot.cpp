#include "Game/Bullet/BulletPlayerExplosionShot.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cmath>

namespace Game {

BulletPlayerNormalExplosionShotBase::BulletPlayerNormalExplosionShotBase()
    : mType(BlasterType::cNormal),
      mPosition(0.0f, 0.0f, 0.0f),
      mVelocity(0.0f, 0.0f, 0.0f),
      mTeamId(0),
      mDirectDamage(125.0f),
      mMaxSplashDamage(70.0f),
      mMinSplashDamage(50.0f),
      mBlastRadius(3.5f),
      mShieldMultiplier(2.5f),
      mBeakonMultiplier(1.8f),
      mAirBurstTimer(0),
      mMaxFlightFrames(15),
      mIsBurst(false) {
}

BulletPlayerNormalExplosionShotBase::~BulletPlayerNormalExplosionShotBase() {
}

void BulletPlayerNormalExplosionShotBase::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mVelocity.set(0.0f, 0.0f, 0.0f);
    mAirBurstTimer = 0;
    mIsBurst = false;
}

void BulletPlayerNormalExplosionShotBase::fireBlaster(BlasterType type, const sead::Vector3f& pos, const sead::Vector3f& dir, u32 teamId) {
    mType = type;
    mPosition = pos;
    mTeamId = teamId;
    mAirBurstTimer = 0;
    mIsBurst = false;

    f32 speed = 1.6f;
    switch (type) {
        case BlasterType::cNormal:
            mDirectDamage = 125.0f;
            mMaxSplashDamage = 70.0f;
            mMinSplashDamage = 50.0f;
            mBlastRadius = 3.5f;
            mMaxFlightFrames = 15;
            speed = 1.6f;
            break;
        case BlasterType::cRange:
            mDirectDamage = 125.0f;
            mMaxSplashDamage = 70.0f;
            mMinSplashDamage = 50.0f;
            mBlastRadius = 3.5f;
            mMaxFlightFrames = 22;
            speed = 1.7f;
            break;
        case BlasterType::cRapid:
            mDirectDamage = 85.0f;
            mMaxSplashDamage = 35.0f;
            mMinSplashDamage = 17.5f;
            mBlastRadius = 3.0f;
            mMaxFlightFrames = 18;
            speed = 2.0f;
            break;
        case BlasterType::cLuna:
            mDirectDamage = 125.0f;
            mMaxSplashDamage = 70.0f;
            mMinSplashDamage = 50.0f;
            mBlastRadius = 4.2f; // Extra large blast
            mMaxFlightFrames = 10;
            speed = 1.5f;
            break;
    }

    mShieldMultiplier = 2.5f;
    mBeakonMultiplier = 1.8f;

    f32 len = std::sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
    if (len > 0.001f) {
        mVelocity.set(dir.x / len * speed, dir.y / len * speed, dir.z / len * speed);
    } else {
        mVelocity.set(0.0f, 0.0f, speed);
    }
}

// Matches FUN_0225c8f0 decompiled PPC linear interpolation
f32 BulletPlayerNormalExplosionShotBase::computeExplosionDamageAtDistance(f32 distFromBlastCenter) const {
    if (distFromBlastCenter <= 0.2f) {
        return mDirectDamage; // Direct hit
    }
    if (distFromBlastCenter >= mBlastRadius) {
        return 0.0f; // Outside blast sphere
    }

    // Linear interpolation between max splash (center) and min splash (perimeter)
    f32 ratio = 1.0f - (distFromBlastCenter / mBlastRadius);
    return (mMaxSplashDamage - mMinSplashDamage) * ratio + mMinSplashDamage;
}

// Matches BulletPlayerNormalExplosionShotBase__vfunc_88
f32 BulletPlayerNormalExplosionShotBase::computeDamageAgainstTarget(u32 targetType, f32 distFromBlastCenter, u32 targetTeamId) const {
    if (targetTeamId == mTeamId) {
        return 0.0f; // Friendly fire immune
    }

    f32 baseDmg = computeExplosionDamageAtDistance(distFromBlastCenter);

    // targetType 0x0C = Rainmaker Shield
    if (targetType == 12) {
        return baseDmg * mShieldMultiplier;
    }
    // targetType 10 = Squid Beakon / Deployable Trap
    if (targetType == 10) {
        return baseDmg * mBeakonMultiplier;
    }

    return baseDmg;
}

void BulletPlayerNormalExplosionShotBase::triggerAirBurst() {
    if (mIsBurst) return;
    mIsBurst = true;

    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        paint->splatInk(mPosition, mBlastRadius, mTeamId);
    }
}

void BulletPlayerNormalExplosionShotBase::update() {
    if (mIsBurst) return;

    mPosition.x += mVelocity.x;
    mPosition.y += mVelocity.y;
    mPosition.z += mVelocity.z;

    mAirBurstTimer++;
    if (mAirBurstTimer >= mMaxFlightFrames) {
        triggerAirBurst();
    }
}

void BulletPlayerNormalExplosionShotBase::draw() {
    if (!mIsBurst) {
        GambitActor::draw();
    }
}

} // namespace Game
