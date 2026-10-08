#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class CylinderHoleState : u32 {
    cClosed       = 0,
    cOpenFiring   = 1,
    cInkedClimb   = 2
};

/**
 * Obj_CylinderKingHole (Octonozzle Suction Hole / Inking Vent)
 * Decompiled from PowerPC retail executable Gambit.elf:
 * Vtable @ 0x100C3DE0
 *
 * Authentic PowerPC Methods:
 *   vfunc_11 @ 0x0251DC48: Transform hierarchy update relative to cylinder body
 *   vfunc_47 @ 0x0251DDBC: Stage collision registration & ink penetration logic
 */
class Obj_CylinderKingHole : public GambitActor {
public:
    Obj_CylinderKingHole();
    virtual ~Obj_CylinderKingHole() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic Ghidra Decompiled vfuncs
    virtual void vfunc_11(); // 0x0251DC48: Transform update
    virtual void vfunc_47(); // 0x0251DDBC: Collision & ink check

    void setupHole(u32 holeIndex, f32 heightOffset, f32 angleRad);
    void openAndFire();
    void hitWithInk(f32 inkAmount);
    void closeHole();

    CylinderHoleState getState() const { return mState; }
    bool isInkedForClimbing() const { return mState == CylinderHoleState::cInkedClimb; }
    f32 getHeightOffset() const { return mHeightOffset; }
    f32 getAngleRad() const { return mAngleRad; }
    u32 getHoleIndex() const { return mHoleIndex; }
    f32 getInkedAmount() const { return mInkedAmount; }

protected:
    u32 mHoleIndex;
    f32 mHeightOffset;
    f32 mAngleRad;
    CylinderHoleState mState;
    s32 mTimer;
    f32 mInkedAmount;

    u8 mReserved[0x28];
};

} // namespace Game
