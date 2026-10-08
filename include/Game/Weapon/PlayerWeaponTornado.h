#pragma once

#include "types.h"
#include "Game/Weapon/PlayerWeaponBase.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class TornadoState : u32 {
    cSelectingTarget = 0,
    cLaunching       = 1,
    cDescending      = 2,
    cVortexActive    = 3,
    cFinished        = 4
};

/**
 * PlayerWeaponTornado (Inkstrike / Tornado)
 * Address: vtable @ 0x100F0EC4
 *
 * Real PowerPC methods:
 *   vfunc_35 @ 0x026da730 - Sub-action controller invocation
 *   vfunc_70 @ 0x026da770 - Export weapon context pointer
 */
class PlayerWeaponTornado : public PlayerWeaponBase {
public:
    PlayerWeaponTornado();
    virtual ~PlayerWeaponTornado() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void startAiming();
    void confirmTarget(const sead::Vector3f& targetPos, u32 team);

    TornadoState getState() const { return mState; }
    bool isFinished() const { return mState == TornadoState::cFinished; }
    const sead::Vector3f& getTargetPosition() const { return mTargetPosition; }

protected:
    void triggerVortexExplosion();

    TornadoState mState;
    s32 mStateTimer;

    sead::Vector3f mTargetPosition;
    sead::Vector3f mMissilePos;
    u32 mTeam;

    f32 mVortexRadius;
    f32 mDamagePerFrame;
};

} // namespace Game
