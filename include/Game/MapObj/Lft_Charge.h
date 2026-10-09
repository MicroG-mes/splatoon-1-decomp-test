#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class ChargeLiftState : u32 {
    cBottom     = 0,
    cAscending  = 1,
    cTop        = 2,
    cDescending = 3
};

/**
 * Lft_Charge (Octosniper Perch Mechanical Lift)
 * Decompiled from PowerPC retail executable Gambit.elf:
 * Vtable @ 0x100cbcf4
 * Model: content/Model/Lft_Charge.szs (3,064 vertices, Lft_Charge.kcl)
 *
 * Heavy vertical lift platform used to elevate Octosnipers (Enm_Charge)
 * to elevated battle stations and high sniper vantage nests.
 */
class Lft_Charge : public GambitActor {
public:
    static constexpr f32 cMaxElevationHeight = 25.0f;
    static constexpr f32 cAscentSpeed        = 0.35f;
    static constexpr f32 cDescentSpeed       = 0.25f;
    static constexpr s32 cTopDwellDuration   = 120; // 2 seconds dwell at high perch

    Lft_Charge();
    virtual ~Lft_Charge() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic PowerPC vfuncs
    virtual void vfunc_3(); // Model loading (Lft_Charge.szs, 3,064 vertices)
    virtual void vfunc_5(); // Parameter loading
    virtual void vfunc_7(); // Hydraulic motion tick
    virtual void vfunc_11(); // Trigger elevate / lower

    void spawn(const sead::Vector3f& basePos, f32 maxElevation = cMaxElevationHeight);
    void triggerAscend();
    void triggerDescend();

    ChargeLiftState getState() const { return mState; }
    f32 getCurrentHeight() const { return mCurrentHeight; }
    f32 getNormalizedHeight() const { return mCurrentHeight / mTargetElevation; }
    const sead::Vector3f& getBasePosition() const { return mBasePosition; }
    sead::Vector3f getCurrentWorldPosition() const;
    bool isAtTop() const { return mState == ChargeLiftState::cTop; }
    bool isAtBottom() const { return mState == ChargeLiftState::cBottom; }

protected:
    sead::Vector3f mBasePosition;
    f32 mCurrentHeight;
    f32 mTargetElevation;
    ChargeLiftState mState;
    s32 mDwellTimer;
    u8 mReserved[0x20];
};

} // namespace Game
