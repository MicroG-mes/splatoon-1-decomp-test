#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include "sead/resource/BfresParser.h"

namespace Game {

enum class HeavyCraneState : u32 {
    cIdle = 0,
    cRotating = 1,
    cTrolleyMove = 2,
    cHoistLift = 3,
    cBreak0 = 4,
    cBreak1 = 5
};

/**
 * Lft_HeavyCraneMachine
 * Industrial Tower Crane Platform Lift.
 *
 * Retail Wii U binary:
 *   vtable @ 0x100CBD60
 *   Parameters: content/Static/Lft_HeavyCraneMachine_AnmItp.params
 *     mDefaultAnimFrame: 0.0f
 *     EnterBreak_0: 5.0f
 *     EnterBreak_1: 5.0f
 *     EnterMove: 10.0f
 *   Model: content/Model/Lft_HeavyCraneMachine.szs
 *   Meshes:
 *     - CopperNetTop (24 vertices)
 *     - TowerCraneRed (3,584 vertices)
 *     - TowerCraneScarlet (7,185 vertices)
 *     - TowerCraneScarletParts (2,962 vertices)
 *   Total Vertices: 13,755 authentic BFRES vertices.
 */
class Lft_HeavyCraneMachine : public GambitActor {
public:
    Lft_HeavyCraneMachine();
    virtual ~Lft_HeavyCraneMachine() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC Ghidra vtable methods
    virtual void vfunc_3();  // 0x02573210 - Model & param loading
    virtual void vfunc_5();  // 0x025733A0 - Reset & spawn
    virtual void vfunc_7();  // 0x025734E0 - Kinematic rotation & hoist update

    void setTargetAngle(f32 angleDeg);
    void setTargetTrolleyDist(f32 distMeters);
    void setTargetHoistHeight(f32 heightMeters);

    HeavyCraneState getState() const { return mState; }
    f32 getCurrentAngle() const { return mCurrentAngle; }
    f32 getTrolleyDist() const { return mTrolleyDist; }
    f32 getHoistHeight() const { return mHoistHeight; }
    sead::Vector3f getPlatformWorldPos() const;

    f32 getEnterMoveDuration() const { return mEnterMoveFrames; }
    f32 getEnterBreak0Duration() const { return mEnterBreak0Frames; }
    f32 getEnterBreak1Duration() const { return mEnterBreak1Frames; }

    u32 getModelVertexCount() const { return static_cast<u32>(mModel.getTotalVertexCount()); }

private:
    HeavyCraneState mState;
    f32 mCurrentAngle;       // Slew rotation in degrees [0, 360)
    f32 mTargetAngle;
    f32 mRotateSpeed;        // deg/sec (default 12.0)

    f32 mTrolleyDist;        // Trolley position along jib [5.0m, 40.0m]
    f32 mTargetTrolleyDist;
    f32 mTrolleySpeed;       // m/s (default 2.5)

    f32 mHoistHeight;        // Hook hoist elevation [0.0m, 25.0m]
    f32 mTargetHoistHeight;
    f32 mHoistSpeed;         // m/s (default 1.8)

    // Params loaded from Lft_HeavyCraneMachine_AnmItp.params
    f32 mDefaultAnimFrame;
    f32 mEnterBreak0Frames;
    f32 mEnterBreak1Frames;
    f32 mEnterMoveFrames;

    sead::Vector3f mBasePosition;
    sead::BfresModel mModel;
};

} // namespace Game
