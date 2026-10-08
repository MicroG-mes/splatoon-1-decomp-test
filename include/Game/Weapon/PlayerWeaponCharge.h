#pragma once

#include "types.h"
#include "Game/Weapon/PlayerWeaponBase.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class ChargeState : u32 {
    cIdle       = 0,
    cCharging   = 1,
    cFullCharge = 2,
    cFiring     = 3,
    cCooldown   = 4
};

/**
 * PlayerWeaponCharge (Charger Class - Splat Charger, E-Liter, Squiffer)
 * Address: vtable @ 0x100F0314
 *
 * Real PowerPC methods:
 *   vfunc_35 @ 0x026d7b04 - Sub-action controller invocation
 *   vfunc_70 @ 0x026d7b44 - Export weapon context pointer
 */
class PlayerWeaponCharge : public PlayerWeaponBase {
public:
    PlayerWeaponCharge();
    virtual ~PlayerWeaponCharge() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void startCharging();
    void releaseCharge();

    ChargeState getState() const { return mState; }
    f32 getChargeRatio() const { return mChargeRatio; }
    bool isFullyCharged() const { return mState == ChargeState::cFullCharge; }

    f32 computeDamage() const;
    f32 computeRange() const;
    f32 computeVelocity() const;

protected:
    void fireChargeShot();

    ChargeState mState;
    s32 mChargeFrames;
    s32 mMaxChargeFrames;

    f32 mChargeRatio; // 0.0 to 1.0

    f32 mMinDamage;
    f32 mMaxDamage;
    f32 mMinRange;
    f32 mMaxRange;
    f32 mMinVelocity;
    f32 mMaxVelocity;

    bool mIsScopeEquipped;
};

} // namespace Game
