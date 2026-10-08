#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class SprinklerState : u32 {
    cAttaching = 0,
    cSpraying  = 1,
    cDestroyed = 2
};

/**
 * Sprinkler
 * Address: vtable @ 0x10035A14
 * Autonomous rotating ink sprinkler sub-weapon.
 *
 * Real PowerPC methods:
 *   vfunc_3   @ 0x0221392c - Model and parameter init
 *   vfunc_167 @ 0x02213b50 - Droplet arc calculation
 *   vfunc_176 @ 0x022142b8 - Reset rotation angle (0x248) and active spray state (0x24A)
 */
class Sprinkler : public GambitActor {
public:
    Sprinkler();
    virtual ~Sprinkler() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Vtable slots matching Ghidra 0x10035A14
    virtual void vfunc_3();
    virtual void vfunc_167();
    virtual void vfunc_176();

    void attachToSurface(const sead::Vector3f& pos, const sead::Vector3f& normal, u32 team);
    void applyDamage(f32 damage);

    SprinklerState getState() const { return mState; }
    f32 getRemainingHp() const { return mCurrentHp; }
    f32 getRotationAngle() const { return mRotationAngle; }
    u16 getRotationAngleRaw() const { return mRotationAngleRaw; }
    bool isActive() const { return mIsActive != 0; }

protected:
    void sprayDroplet();

    SprinklerState mState;
    s32 mStateTimer;

    sead::Vector3f mPosition;
    sead::Vector3f mNormal;
    u32 mTeam;

    f32 mMaxHp;
    f32 mCurrentHp;
    f32 mRotationAngle;
    s32 mSprayTimer;

    // Espresso PowerPC offsets:
    // +0x24: Resource load token
    u32 mResourceInitToken;            // 0x24
    undefined mPadding1[0x180];

    u16 mRotationAngleRaw;             // 0x248: PowerPC 16-bit heading angle
    u8  mIsActive;                     // 0x24A: PowerPC active spray flag
    undefined mPadding2[1];            // 0x24B
};

} // namespace Game
