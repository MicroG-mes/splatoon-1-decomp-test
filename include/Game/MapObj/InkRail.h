#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class InkRailState : u32 {
    cDormant    = 0,
    cActivating = 1,
    cActive     = 2,
    cDeactivating = 3
};

class InkRail : public GambitActor {
public:
    static constexpr f32 cGrindSpeed = 0.35f;
    static constexpr s32 cActiveLifetimeFrames = 600; // 10 seconds

    InkRail();
    virtual ~InkRail() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic Ghidra Decompiled vfuncs (from Obj_InkRail vtable 0x100C9EAC)
    virtual void vfunc_3(); // 0x0254D50C: Rail spline data setup
    virtual void vfunc_7(); // 0x0254F438: 4-stage rail traversal update

    void setupSpline(const sead::Vector3f& startPos, const sead::Vector3f& endPos);
    void activateByInk(u32 teamId);

    sead::Vector3f evaluateSplinePos(f32 progressT) const;
    f32 stepGrindProgress(f32 currentT) const;

    InkRailState getState() const { return mState; }
    bool isActive() const { return mState == InkRailState::cActive; }
    u32 getTeamId() const { return mTeamId; }
    const sead::Vector3f& getStartPos() const { return mStartPos; }
    const sead::Vector3f& getEndPos() const { return mEndPos; }

protected:
    u8 mReserved0_0x8[0x48];
    void* mSplineResourcePtr; // 0x50
    u8 mReserved1_0x54[0x350];
    void* mSplineDataPtr;     // 0x3A4: evaluated catenary spline curve

    sead::Vector3f mStartPos;
    sead::Vector3f mEndPos;
    u32 mTeamId;
    InkRailState mState;
    s32 mTimer;
    f32 mActivationProgress;
};

} // namespace Game
