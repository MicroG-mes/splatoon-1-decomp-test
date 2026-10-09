#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class PropellerLiftState : u32 {
    cWait        = 0,
    cStartPoint  = 1,
    cMove        = 2,
    cMoveReturn  = 3,
    cEndPoint    = 4,

    // Legacy backwards-compatible aliases
    cIdle        = 0,
    cMoving      = 2,
    cReturning   = 3
};

/**
 * ScrewLift / Lft_Propeller00 / Lft_Propeller01 / Obj_Propeller01
 * Address: vtable @ 0x100d5f54
 * Ink-powered propeller lift platform and rotating winch gimmick in Octo Valley
 * and multiplayer stages (Ancho-V Games).
 *
 * Real PowerPC methods:
 *   vfunc_1  @ 0x025B09F4 - Teardown
 *   vfunc_3  @ 0x025AE0BC - Resource and model loading
 *   vfunc_5  @ 0x025AE7F0 - Initialization and bone light setup (Screw, Slave, Light)
 *   vfunc_7  @ 0x025AF11C - Main update tick & winch movement
 *   vfunc_9  @ 0x025AFC88 - Linear track evaluation
 *   vfunc_11 @ 0x025B0320 - KCL collision transform update
 *   vfunc_14 @ 0x025B0524 - Ink hit impulse receiver
 *   vfunc_15 @ 0x025B05A8 - Player ride attachment
 *   vfunc_35 @ 0x025B0634 - Switch event broadcast
 *   vfunc_39 @ 0x025B06CC - Sound effect trigger (FastUp, Vernier)
 *   vfunc_47 @ 0x025B1580 - Animation / propeller rotation sync
 *   vfunc_56 @ 0x025B05F4 - Audio pitch update
 */
class Lft_Propeller00 : public GambitActor {
public:
    static constexpr f32 cMaxRpm             = 180.0f;
    static constexpr f32 cRpmDecay           = 0.96f;
    static constexpr f32 cReturnSpeed        = 0.005f;
    static constexpr f32 cMoveSpeedFactor    = 0.00045f;
    static constexpr f32 cMinMoveRpm         = 5.0f;
    static constexpr f32 cInkImpulseMult     = 18.0f;

    Lft_Propeller00();
    virtual ~Lft_Propeller00() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC vtable slots matching Ghidra
    virtual void vfunc_1();
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_9();
    virtual void vfunc_11();
    virtual void vfunc_14();
    virtual void vfunc_15();
    virtual void vfunc_35();
    virtual void vfunc_39();
    virtual void vfunc_47();
    virtual void vfunc_56();

    void setTrack(const sead::Vector3f& startPos, const sead::Vector3f& endPos);
    void hitPropeller(f32 inkPower, u32 teamId = 0);

    PropellerLiftState getState() const { return mState; }
    f32 getProgress() const { return mProgress; }
    f32 getPropellerRpm() const { return mPropellerRpm; }
    f32 getPropellerAngle() const { return mPropellerAngle; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    const sead::Vector3f& getStartPos() const { return mStartPos; }
    const sead::Vector3f& getEndPos() const { return mEndPos; }
    bool isAtEndPoint() const { return mState == PropellerLiftState::cEndPoint; }
    bool isAtStartPoint() const { return mProgress <= 0.001f; }
    bool isSpinning() const { return mPropellerRpm > cMinMoveRpm; }

    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mStartPos;      // 0x140
    sead::Vector3f mEndPos;
    sead::Vector3f mPosition;

    PropellerLiftState mState;
    f32 mProgress;                 // 0.0f to 1.0f
    f32 mPropellerRpm;
    f32 mPropellerAngle;
    f32 mTargetProgress;
    s32 mTimer;
    u32 mLastHitTeam;

    undefined mReserved[0x38];
};

// Internal binary aliases
using ScrewLift        = Lft_Propeller00;
using Lft_Propeller01  = Lft_Propeller00;
using Obj_Propeller01  = Lft_Propeller00;

} // namespace Game
