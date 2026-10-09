#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class BridgeUpDownState : u32 {
    cLowered  = 0, // Walkable bridge horizontal position
    cRaising  = 1, // Ascending rotation around hinge
    cRaised   = 2, // Upright barrier position
    cLowering = 3  // Descending return rotation
};

/**
 * Obj_BridgeUpDown00 (Rising Drawbridge Platform)
 * Decompiled from PowerPC retail executable Gambit.elf:
 * Vtable @ 0x100cbcfc
 * Model: content/Model/Obj_BridgeUpDown00.szs (2,624 vertices, Obj_BridgeUpDown00.kcl)
 *
 * Single-pivot mechanical drawbridge used in single-player missions
 * and multiplayer terrain gimmicks to toggle navigable routes.
 */
class Obj_BridgeUpDown00 : public GambitActor {
public:
    static constexpr f32 cMaxAngleDegrees = 75.0f;
    static constexpr f32 cRaiseSpeed      = 1.25f; // Deg/frame
    static constexpr f32 cLowerSpeed      = 1.00f; // Deg/frame
    static constexpr s32 cHoldDuration    = 180;   // 3 seconds held open before lowering

    Obj_BridgeUpDown00();
    virtual ~Obj_BridgeUpDown00() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic PowerPC vfuncs
    virtual void vfunc_3(); // Model & KCL setup
    virtual void vfunc_5(); // Parameter initialization
    virtual void vfunc_7(); // Motion tick
    virtual void vfunc_11(); // Switch / ink trigger activation

    void spawn(const sead::Vector3f& pivotPos, f32 yawAngle = 0.0f);
    void triggerRaise();
    void triggerLower();

    BridgeUpDownState getState() const { return mState; }
    f32 getAngleDegrees() const { return mAngle; }
    f32 getNormalizedElevation() const { return mAngle / cMaxAngleDegrees; }
    const sead::Vector3f& getPivotPosition() const { return mPivotPosition; }
    bool isPassable() const { return mState == BridgeUpDownState::cLowered; }
    bool isFullyRaised() const { return mState == BridgeUpDownState::cRaised; }

protected:
    sead::Vector3f mPivotPosition;
    f32 mYawAngle;
    f32 mAngle;         // Current elevation angle in degrees [0.0f .. 75.0f]
    BridgeUpDownState mState;
    s32 mHoldTimer;
    u8 mReserved[0x28];
};

} // namespace Game
