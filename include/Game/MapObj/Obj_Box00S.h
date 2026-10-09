#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class BoxSmallState : u32 {
    cState_Normal   = 0,
    cState_Broken   = 1
};

/**
 * Obj_Box00S
 * Retail Address: vtable @ 0x100e16b4
 * Small breakable wooden supply crate in Octo Valley and Battle Dojo.
 * Breakeven durability: 20.0 HP (1-2 shots).
 *
 * Real PowerPC methods:
 *   vfunc_3 @ 0x025f8210 - Model loading (Obj_Box00S.szs, Obj_Break00, 1,008 vertices)
 *   vfunc_5 @ 0x025f8240 - Durability initialization (20.0 HP)
 *   vfunc_7 @ 0x025f4ba0 - Hit detection
 *   vfunc_9 @ 0x025f54ec - Break shatter & Power Egg drop
 */
class Obj_Box00S : public GambitActor {
public:
    static constexpr f32 cMaxHealth = 20.0f;

    Obj_Box00S();
    virtual ~Obj_Box00S() override;

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
    BoxSmallState getState() const { return mState; }
    f32 getHealth() const { return mHealth; }
    bool isBroken() const { return mState == BoxSmallState::cState_Broken; }
    s32 getDroppedPowerEggs() const { return mDroppedEggs; }

    const sead::Vector3f& getPosition() const { return mPosition; }
    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mPosition;      // 0x140
    BoxSmallState mState;
    f32 mHealth;
    s32 mDroppedEggs;

    undefined mReserved[0x38];
};

} // namespace Game
