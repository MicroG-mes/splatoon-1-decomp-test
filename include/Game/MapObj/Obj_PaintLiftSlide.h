#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class SlideLiftState : u32 {
    cState_Moving   = 0,
    cState_WaitEnd  = 1,
    cState_Paused   = 2
};

/**
 * Obj_PaintLiftSlide
 * Retail Address: vtable @ 0x100d1074 / 0x100d12a4
 * Sliding rail moving platform with wire grating (スライドリフト / 網リフト).
 * Moves back and forth along defined rail endpoints (Lft_WireNettingPlate00.szs, 1,552 vertices).
 * Wire mesh allows ink bullets and swimming squids to pass through, but holds humanoid Inklings.
 *
 * Real PowerPC methods:
 *   vfunc_3  @ 0x02591e2c - Model loading (Lft_WireNettingPlate00.szs, 1,552 vertices)
 *   vfunc_5  @ 0x02591e84 - Rail waypoint interpolation setup
 *   vfunc_7  @ 0x025927c0 - Translation update & platform momentum transfer
 *   vfunc_11 @ 0x025932ac - Terminal bounce & return transit trigger
 */
class Obj_PaintLiftSlide : public GambitActor {
public:
    static constexpr f32 cDefaultSpeed = 0.015f; // rail progress per frame
    static constexpr f32 cPlateWidth   = 8.0f;
    static constexpr f32 cPlateLength  = 8.0f;

    Obj_PaintLiftSlide();
    virtual ~Obj_PaintLiftSlide() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC vtable slots matching Ghidra
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_11();

    // Spline & Rail Navigation
    void setRailPoints(const sead::Vector3f& start, const sead::Vector3f& end);
    void setSpeed(f32 speed) { mSpeed = speed; }

    // Physical Interaction
    bool checkPassenger(const sead::Vector3f& playerPos, bool isSquid) const;
    bool canSquidPassThrough() const { return true; } // Wire grating attribute

    // Queries
    SlideLiftState getState() const { return mState; }
    f32 getProgress() const { return mProgress; }
    f32 getDirection() const { return mDirection; }
    const sead::Vector3f& getCurrentPos() const { return mPosition; }

    const sead::Vector3f& getPosition() const { return mPosition; }
    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mPosition;      // 0x140
    sead::Vector3f mRailStart;
    sead::Vector3f mRailEnd;
    SlideLiftState mState;
    f32 mProgress;
    f32 mDirection;
    f32 mSpeed;
    s32 mEndWaitTimer;

    undefined mReserved[0x38];
};

} // namespace Game
