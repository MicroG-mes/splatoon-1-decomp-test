#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class OctosniperState : u32 {
    cIdle     = 0,
    cPatrol   = 1,
    cLockOn   = 2,
    cCharging = 4, // 0x04 in FUN_022cfd50: laser guide line active
    cFiring   = 7, // 0x07 in FUN_022cfd50: lethal sniper bolt release
    cCooldown = 8,
    cStunned  = 9
};

/**
 * GameEnemyCharge / Enm_Charge (Octosniper)
 * Address: vtable @ 0x100666C8
 * Authentic Nintendo path: D:/home/Cafe/Gambit/App/Program/Game/Enemy/GameEnemyCharge.cpp
 * Long-range stationary Octarian sniper with laser guide line.
 *
 * Real PowerPC methods:
 *   vfunc_1  @ 0x022d3168 - Reset & actor cleanup
 *   vfunc_3  @ 0x022cc774 - Sniping platform & sight line initialization
 *   vfunc_5  @ 0x022cd640 - Reset aim coordinates
 *   vfunc_7  @ 0x022cfd50 - Aiming tick, charge state machine, firing trigger
 *   vfunc_47 @ 0x022d31a4 - Laser beam draw
 *   vfunc_52 @ 0x022cff7c - Damage reaction & tentacle hit test
 */
class GameEnemyCharge : public GambitActor {
public:
    static constexpr f32 cSniperDamage = 100.0f;
    static constexpr f32 cMaxSniperRange = 35.0f;
    static constexpr s32 cChargeDuration = 60; // 1 second charge warning
    static constexpr s32 cCooldownDuration = 45;

    GameEnemyCharge();
    virtual ~GameEnemyCharge() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic PPC methods
    virtual void vfunc_1();
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_47();
    virtual void vfunc_52(f32 damage, bool isRearHit);

    void setupBunker(const sead::Vector3f& pos, f32 yawAngle);
    void updateAimAtPlayer(const sead::Vector3f& playerPos);

    OctosniperState getState() const { return mState; }
    f32 getHealth() const { return mHealth; }
    bool isAlive() const { return mIsAlive; }
    bool isLaserActive() const { return mState == OctosniperState::cCharging || mState == OctosniperState::cLockOn; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    const sead::Vector3f& getAimTarget() const { return mAimTarget; }

protected:
    void fireSniperShot();

    sead::Vector3f mPosition;
    sead::Vector3f mAimTarget;
    sead::Vector3f mShootDir;

    f32 mHealth;
    f32 mYawAngle;
    bool mIsAlive;
    bool mHasLineOfSight;

    OctosniperState mState; // +0x190 (offset 400)
    s32 mStateTimer;
    s32 mChargeTimer;
};

} // namespace Game
