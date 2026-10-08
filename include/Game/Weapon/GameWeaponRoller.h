#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class RollerState : u32 {
    cIdle        = 0,
    cSwingWindup = 1,
    cSwingRelease = 2,
    cRolling     = 3,
    cEmptyTank   = 4 // TankEmptyRoller
};

enum class RollerType : u32 {
    cCompact     = 0, // Carbon Roller (Roller_Compact)
    cNormal      = 1, // Splat Roller (Roller_Normal)
    cHeavy       = 2, // Dynamo Roller (Roller_Heavy)
    cBrushMini   = 3, // Inkbrush (Roller_BrushMini)
    cBrushNormal = 4  // Octobrush (Roller_BrushNormal)
};

/**
 * GameWeaponRoller / PlayerWeaponRoller
 * Address: vtable @ 0x100F0690
 *
 * Real PowerPC methods:
 *   vfunc_14 @ 0x026D8908 - Velocity reset & swing initialization
 *   vfunc_53 @ 0x026D9178 - Collision track evaluation
 *   vfunc_70 @ 0x026D95AC - TankEmptyRoller state check and event dispatch
 *   FUN_026D8C38 - Droplet painting timer, PaintOff flag integration
 */
class GameWeaponRoller : public GambitActor {
public:
    GameWeaponRoller();
    virtual ~GameWeaponRoller() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic PPC methods
    virtual void vfunc_14();
    virtual void vfunc_70(s32* pOutState);
    void updatePaintTimer(u32 param2, u8 param3);

    void setRollerType(RollerType type);
    RollerType getRollerType() const { return mRollerType; }

    void startFling();
    void startRolling();
    void stopRolling();

    void updateInkConsumption(f32 currentInk);

    RollerState getState() const { return mState; }
    f32 getRollSpeed() const { return mRollSpeed; }
    f32 getPaintWidth() const { return mPaintWidth; }
    f32 getSquishDamage() const { return mSquishDamage; }
    f32 getFlingDamage() const { return mFlingDamage; }
    bool isPainting() const { return mIsPainting; }

protected:
    void flingInkWave();

    RollerType mRollerType;
    RollerState mState;
    s32 mStateTimer;

    f32 mPaintWidth;
    f32 mRollSpeed;
    f32 mSquishDamage;
    f32 mFlingDamage;

    s32 mWindupFrames;
    f32 mFlingRange;
    f32 mInkCostFling;
    f32 mInkCostRollPerFrame;

    bool mHasInk;
    bool mIsPainting;

    // Authentic binary fields from 0x026D8C38
    f32 mSwingSpeedX;    // 0x200
    f32 mSwingSpeedY;    // 0x204
    f32 mPaintTimer;     // 0x248
    u8  mPaintOffFlag;   // 0x25B
};

} // namespace Game
