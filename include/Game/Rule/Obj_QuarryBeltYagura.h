#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class TowerControlState : u32 {
    cNeutral        = 0,
    cMovingAlpha    = 1,
    cMovingBravo    = 2,
    cContested      = 3,
    cCheckpointWait = 4,
    cKnockoutAlpha  = 5,
    cKnockoutBravo  = 6
};

/**
 * Obj_QuarryBeltYagura
 * In-game Tower actor for Tower Control (Gachi Yagura).
 * Rides along a precomputed 3D spline rail across the map.
 *
 * Address: vtable @ 0x100D1A3C
 * Real PowerPC methods:
 *   vfunc_6 @ 0x0243f558 - Main tower spline rail advancement and state step
 *   FUN_0243f3bc         - Tangent velocity projection along rail track
 */
class Obj_QuarryBeltYagura : public GambitActor {
public:
    Obj_QuarryBeltYagura();
    virtual ~Obj_QuarryBeltYagura() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Vtable slot 6 override directly from 0x0243f558
    virtual void vfunc_6();

    void updateRiders(u32 alphaRiderCount, u32 bravoRiderCount);

    TowerControlState getState() const { return mState; }
    f32 getDistanceScoreAlpha() const { return mBestScoreAlpha; }
    f32 getDistanceScoreBravo() const { return mBestScoreBravo; }
    f32 getCurrentRailProgress() const { return mRailProgress; }

    bool isKnockout() const { return mState == TowerControlState::cKnockoutAlpha || mState == TowerControlState::cKnockoutBravo; }

protected:
    void stepMovementPPC();

    TowerControlState mState;
    s32 mStateTimer;

    f32 mRailProgress;     // 0.0 = Center, +1.0 = Bravo Goal (Alpha win), -1.0 = Alpha Goal (Bravo win)
    f32 mBestScoreAlpha;   // 100 to 0
    f32 mBestScoreBravo;   // 100 to 0

    u32 mRidersAlpha;
    u32 mRidersBravo;
    s32 mIdleFrames;

    // Struct members aligned to Espresso PowerPC offsets:
    // +0x728: Rail spline context
    // +0x72C: Spline curve count
    undefined mSplineContext[0x7F0]; // 0x728 - 0xF18

    sead::Vector3f mRailTangent;     // 0xF20: Tangent X, Y, Z
    undefined mPaddingNodes[0x24];

    sead::Vector3f mSplineNode0;     // 0xF50
    sead::Vector3f mSplineNode1;     // 0xF60
    sead::Vector3f mSplineNode2;     // 0xF70
    undefined mPaddingVel[0x2C];

    sead::Vector3f mVelocity;        // 0xFAC: Velocity X, Y, Z
    f32 mTargetSpeed;                // 0xFB4: Speed along rail
    f32 mSmoothing;                  // 0xFB8: Tangent smoothing
    u32 mSoundHandle;                // 0xFBC: Audio/Effect handle
};

} // namespace Game
