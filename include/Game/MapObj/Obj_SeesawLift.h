#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class SeesawLiftState : u32 {
    cBalanced = 0,
    cTilting  = 1,
    cMaxTilt  = 2
};

/**
 * Obj_SeesawLift / SeesawLift (Balance Seesaw Platform)
 * Decompiled from PowerPC retail executable Gambit.elf:
 * Vtable @ 0x100d7468 / 0x100d7558
 * Source file: GameSeesawLift.cpp
 *
 * Authentic PowerPC Methods:
 *   vfunc_3  @ 0x025b8838 - Model and collision setup
 *   vfunc_7  @ 0x025b915c - Main physics simulation tick
 *   vfunc_11 @ 0x025b91fc - Collision transform matrix update
 *   vfunc_53 @ 0x025b92fc - Player weight impact & moment torque calculation
 *   FUN_025b8e18 @ 0x025b8e18 - Angular velocity dampening, angle limits, restitution rebound
 */
class Obj_SeesawLift : public GambitActor {
public:
    static constexpr f32 cMaxTiltRadians      = 0.349066f; // ~20.0 degrees limit
    static constexpr f32 cAngularDamping       = 0.96f;     // Air and bearing friction
    static constexpr f32 cRestitution          = 0.35f;     // Rebound elasticity on hard detent stops
    static constexpr f32 cRestoringSpringCoeff = 0.003f;    // Spring leveling torque when unoccupied
    static constexpr f32 cTorqueMultiplier     = 0.00015f;  // Moment of force per unit weight-distance
    static constexpr f32 cArmHalfWidth         = 6.0f;      // Half-length of seesaw board

    Obj_SeesawLift();
    virtual ~Obj_SeesawLift() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic Ghidra Decompiled vfuncs
    virtual void vfunc_3();  // 0x025b8838
    virtual void vfunc_7();  // 0x025b915c
    virtual void vfunc_11(); // 0x025b91fc
    virtual void vfunc_53(); // 0x025b92fc

    void spawn(const sead::Vector3f& pivotPos);
    void applyWeightImpulse(f32 leverArmDistance, f32 weight);
    void resetBalance();

    SeesawLiftState getState() const { return mState; }
    f32 getTiltAngle() const { return mTiltAngle; }
    f32 getTiltAngleDegrees() const;
    f32 getAngularVelocity() const { return mAngularVelocity; }
    const sead::Vector3f& getPivotPosition() const { return mPivotPosition; }
    bool isAtMaxTilt() const { return mState == SeesawLiftState::cMaxTilt; }
    bool isLevel() const;

protected:
    sead::Vector3f mPivotPosition;
    f32 mTiltAngle;          // Current rotation angle in radians (offset +0x1D8)
    f32 mAngularVelocity;    // Angular velocity (offset +0x1DC)
    f32 mExternalTorque;     // Applied torque from player weight
    SeesawLiftState mState;
    s32 mOccupantCount;
    u8 mReserved[0x30];
};

} // namespace Game
