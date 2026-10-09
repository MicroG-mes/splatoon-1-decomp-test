#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class BoxState : u32 {
    cState_Normal   = 0,
    cState_Wobble   = 1,
    cState_Broken   = 2
};

/**
 * Obj_Box00L
 * Retail Address: vtable @ 0x100e123c
 * Large breakable wooden supply crate in Octo Valley single-player campaign and Battle Dojo.
 * Drops Power Eggs, armor upgrades, canned specials, or Sunken Scrolls when smashed.
 *
 * Real PowerPC methods:
 *   vfunc_3 @ 0x025f7448 - Model loading (Obj_Box00L.szs, Obj_Break00, 1,028 vertices)
 *   vfunc_5 @ 0x025f7480 - Durability initialization (50.0 HP)
 *   vfunc_7 @ 0x025f4ba0 - Ink bullet hit damage accumulation & wobble impulse
 *   vfunc_9 @ 0x025f54ec - Shatter fracture event & drop item spawn
 */
class Obj_Box00L : public GambitActor {
public:
    static constexpr f32 cMaxHealth = 50.0f; // Takes ~50.0 damage to shatter

    Obj_Box00L();
    virtual ~Obj_Box00L() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC vtable slots matching Ghidra
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_9();

    // Damage & breaking
    bool takeDamage(f32 damage);

    // Queries
    BoxState getState() const { return mState; }
    f32 getHealth() const { return mHealth; }
    bool isBroken() const { return mState == BoxState::cState_Broken; }
    f32 getWobbleScale() const { return mWobbleScale; }
    s32 getDroppedPowerEggs() const { return mDroppedEggs; }

    const sead::Vector3f& getPosition() const { return mPosition; }
    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mPosition;      // 0x140
    BoxState mState;
    f32 mHealth;
    f32 mWobbleScale;
    s32 mTimer;
    s32 mDroppedEggs;

    undefined mReserved[0x38];
};

} // namespace Game
