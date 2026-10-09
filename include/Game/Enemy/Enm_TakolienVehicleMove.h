#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class VehicleMoveState : u32 {
    cPatrol = 0,
    cChase  = 1,
    cShoot  = 2,
    cBreak  = 3,
    cEject  = 4
};

/**
 * Enm_TakolienVehicleMove / EnemyTakolienEasy
 * Decompiled from PowerPC retail executable Gambit.elf:
 * Vtable @ 0x1008B750
 * Model: content/Model/Enm_TakolienVehicleMove.szs (6,248 vertices)
 * Sub-models: Enm_Break00, Enm_Break01, Enm_Break02, Enm_TakolienVehicleMove
 *
 * Mobile bipedal Octoling assault walker mech capable of navigating
 * complex mission terrain and firing mobile bubble projectile bursts.
 */
class Enm_TakolienVehicleMove : public GambitActor {
public:
    static constexpr f32 cMaxHealth      = 150.0f;
    static constexpr f32 cWalkSpeed      = 1.25f;
    static constexpr f32 cChaseSpeed     = 2.10f;
    static constexpr f32 cDetectRange    = 32.0f;
    static constexpr f32 cAttackRange    = 18.0f;

    Enm_TakolienVehicleMove();
    virtual ~Enm_TakolienVehicleMove() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic PowerPC vfuncs
    virtual void vfunc_3();  // Model loading (Enm_TakolienVehicleMove.szs, 6,248 vertices)
    virtual void vfunc_5();  // Parameter initialization
    virtual void vfunc_7();  // Bipedal motion tick & pursuit AI
    virtual void vfunc_47(); // Rapid bubble cannon volley
    virtual void vfunc_52(); // Mech frame destruction & pilot ejection

    void spawn(const sead::Vector3f& pos, const sead::Vector3f& patrolTarget);
    void updateAi(const sead::Vector3f& playerPos);
    bool takeDamage(f32 damage);

    VehicleMoveState getState() const { return mState; }
    f32 getHealth() const { return mHealth; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    const sead::Vector3f& getMoveDirection() const { return mMoveDirection; }
    f32 getStridePhase() const { return mStridePhase; }
    bool isPilotEjected() const { return mIsPilotEjected; }
    bool isDestroyed() const { return mState == VehicleMoveState::cBreak || mState == VehicleMoveState::cEject; }
    u32 getShotsFired() const { return mShotsFired; }

protected:
    sead::Vector3f mPosition;
    sead::Vector3f mMoveDirection;
    sead::Vector3f mPatrolPointA;
    sead::Vector3f mPatrolPointB;
    bool mPatrolForward;
    f32 mHealth;
    VehicleMoveState mState;
    f32 mStridePhase;
    s32 mActionTimer;
    u32 mShotsFired;
    bool mIsPilotEjected;
    u8 mReserved[0x20];
};

} // namespace Game
