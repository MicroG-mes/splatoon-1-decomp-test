#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class VehicleFixState : u32 {
    cIdle   = 0,
    cAiming = 1,
    cFiring = 2,
    cBreak  = 3,
    cEject  = 4
};

/**
 * Enm_TakolienVehicleFix / EnemyTakolienFixed
 * Decompiled from PowerPC retail executable Gambit.elf:
 * Vtable @ 0x1008C8A8
 * Projectile: BulletEnemyBubbleShotFixedTakolien @ 0x02224038
 * Model: content/Model/Enm_TakolienVehicleFix.szs (8,049 vertices)
 * Sub-models: Enm_Break00, Enm_Break01, Enm_Break02, Enm_TakolienVehicleFix
 *
 * Heavy stationary Octoling armored cannon turret mounted with
 * reinforced blast canopy and bubble mortar battery.
 */
class Enm_TakolienVehicleFix : public GambitActor {
public:
    static constexpr f32 cMaxHealth          = 200.0f;
    static constexpr f32 cCanopyShield       = 100.0f;
    static constexpr f32 cMaxAimRange        = 40.0f;
    static constexpr f32 cBubbleVelocity     = 18.0f;
    static constexpr s32 cFireCadenceFrames  = 24;

    Enm_TakolienVehicleFix();
    virtual ~Enm_TakolienVehicleFix() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic PowerPC vfuncs
    virtual void vfunc_3();  // Model loading (Enm_TakolienVehicleFix.szs, 8,049 vertices)
    virtual void vfunc_5();  // Parameter initialization
    virtual void vfunc_7();  // Turret tracking & burst logic
    virtual void vfunc_47(); // Bubble mortar discharge (BulletEnemyBubbleShotFixedTakolien)
    virtual void vfunc_52(); // Armor break & pilot ejection

    void spawn(const sead::Vector3f& pos, f32 baseYaw = 0.0f);
    void updateAim(const sead::Vector3f& targetPos);
    bool takeDamage(f32 damage, bool hitCanopy);

    VehicleFixState getState() const { return mState; }
    f32 getHealth() const { return mHealth; }
    f32 getCanopyHealth() const { return mCanopyHealth; }
    f32 getTurretYaw() const { return mTurretYaw; }
    f32 getTurretPitch() const { return mTurretPitch; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    bool isPilotEjected() const { return mIsPilotEjected; }
    bool isDestroyed() const { return mState == VehicleFixState::cBreak || mState == VehicleFixState::cEject; }
    u32 getShotsFired() const { return mShotsFired; }

protected:
    void fireBubbleShot();

    sead::Vector3f mPosition;
    sead::Vector3f mTargetPosition;
    f32 mTurretYaw;
    f32 mTurretPitch;
    f32 mHealth;
    f32 mCanopyHealth;
    VehicleFixState mState;
    s32 mFireTimer;
    u32 mShotsFired;
    bool mIsPilotEjected;
    u8 mReserved[0x24];
};

} // namespace Game
