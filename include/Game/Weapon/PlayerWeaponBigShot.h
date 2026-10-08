#pragma once

#include "types.h"
#include "Game/Weapon/PlayerWeaponBase.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class BigShotState : u32 {
    cReady        = 0,
    cFireWindup   = 1,
    cLaunchVortex = 2,
    cRecoil       = 3,
    cFinished     = 4
};

/**
 * PlayerWeaponBigShot (Inkzooka / Super Shot)
 * Address: vtable @ 0x100EFF24
 *
 * Real PowerPC methods:
 *   vfunc_35 @ 0x026d7774 - Sub-action controller invocation
 *   vfunc_70 @ 0x026d77b4 - Export weapon context pointer
 */
class PlayerWeaponBigShot : public PlayerWeaponBase {
public:
    static constexpr s32 cSpecialDuration = 360; // 6 seconds at 60fps
    static constexpr f32 cVortexDamage = 120.0f;
    static constexpr f32 cVortexSpeed = 1.4f;
    static constexpr f32 cVortexRadius = 2.8f;
    static constexpr u32 cMaxAmmo = 6;

    PlayerWeaponBigShot();
    virtual ~PlayerWeaponBigShot() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void startSpecial(u32 teamId, s32 durationFrames = cSpecialDuration);
    bool fireShot(const sead::Vector3f& muzzlePos, f32 yawAngle);

    BigShotState getState() const { return mState; }
    bool canFire() const { return mState == BigShotState::cReady && mAmmoRemaining > 0; }
    bool isFinished() const { return mState == BigShotState::cFinished; }
    u32 getAmmoRemaining() const { return mAmmoRemaining; }
    s32 getRemainingDuration() const { return mDurationTimer; }

protected:
    BigShotState mState;
    s32 mDurationTimer;
    s32 mStateTimer;
    u32 mAmmoRemaining;
    u32 mTeamId;

    sead::Vector3f mLastShotPos;
    f32 mLastShotYaw;
};

} // namespace Game
