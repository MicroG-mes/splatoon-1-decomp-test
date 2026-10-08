#pragma once

#include "types.h"
#include "Game/Bullet/GameBullet.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * BulletPlayerChargeShotBase
 * Address: vtable @ 0x10048E8C
 * High-velocity piercing projectile and paint beam for Charger weapons.
 *
 * Real PowerPC methods:
 *   vfunc_4   @ 0x0224d05c - No-op stub
 *   vfunc_135 @ 0x022501e8 - Reset context parameter
 */
class BulletPlayerChargeShotBase : public GameBullet {
public:
    BulletPlayerChargeShotBase();
    virtual ~BulletPlayerChargeShotBase() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    virtual void vfunc_4();
    virtual void vfunc_135(u32* param2);

    void setChargePower(f32 chargeRatio) { mChargeRatio = chargeRatio; }
    f32 getChargePower() const { return mChargeRatio; }

    bool isPiercing() const { return mIsPiercing; }

protected:
    f32 mChargeRatio;
    f32 mBeamLength;
    bool mIsPiercing;
};

/**
 * Charge_Light
 * Address: vtable @ 0x10047408
 * Retail Splat Charger projectile with charge-scaled ballistics and damage.
 *
 * Real PowerPC methods:
 *   vfunc_7   @ 0x0224d060 - Velocity normalization & drag integration
 *   vfunc_11  @ 0x0224e6e4 - Collision detection & penetration
 *   vfunc_88  @ 0x0224eeec - Damage interpolation curve with Rainmaker multiplier
 */
class Charge_Light : public BulletPlayerChargeShotBase {
public:
    // Authentic constants from .rodata (0x10048E18, 0x10048E20, 0x10048E24, 0x10048E5C)
    static constexpr f32 cSelfDamageMultiplier = 0.0f;
    static constexpr f32 cObjectDamageMultiplier = 2.0f;
    static constexpr f32 cFullChargeThreshold = 1.0f;
    static constexpr f32 cShachihokoShieldMultiplier = 2.8f;

    Charge_Light();
    virtual ~Charge_Light() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    virtual void vfunc_7();
    virtual void vfunc_11();
    virtual f64 vfunc_88(u32 targetType, const s32* pTargetOwnerId);

    void setDamageParams(f32 minDamage, f32 maxDamage, f32 fullChargeDamage);
    f32 getCalculatedDamage(u32 targetType = 0) const;

protected:
    f32 mMinDamage;        // +0x870
    f32 mMaxDamage;        // +0x8D0
    f32 mFullChargeDamage; // +0x930
    s32 mOwnerPlayerId;    // +0x2C
    sead::Vector3f mVelocity; // 0xC4, 0xC8, 0xCC
    f32 mTraveledDistance;    // 0x16C
    u32 mBulletStateFlags;    // 0x170
};

/**
 * BulletPlayerChargeHitSplash
 * Address: vtable @ 0x10047FD4
 * Paint droplets and ground splash created along the charger line of fire.
 */
class BulletPlayerChargeHitSplash : public GameBullet {
public:
    BulletPlayerChargeHitSplash();
    virtual ~BulletPlayerChargeHitSplash() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    virtual void vfunc_7();
    virtual void vfunc_11();
    virtual void vfunc_135(u32* param2);
    virtual void vfunc_147();

protected:
    f32 mSplashRadius;
    s32 mSplashTimer;
};

} // namespace Game
