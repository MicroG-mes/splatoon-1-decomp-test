#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class SquidGuardState : u32 {
    cState_Idle   = 0,
    cState_Rattle = 1
};

/**
 * Obj_SquidGuard
 * Steel security crowd-control barrier barricade in Inkopolis and competitive stages.
 * Blocks human movement while rattling elastically upon ink impact or collision.
 */
class Obj_SquidGuard : public GambitActor {
public:
    static constexpr f32 cBarrierHalfWidth      = 1.25f;
    static constexpr f32 cBarrierHeight         = 1.10f;
    static constexpr f32 cBarrierThickness      = 0.20f;
    static constexpr f32 cRattleAmplitude       = 0.12f;
    static constexpr f32 cRattleDecay           = 0.88f;

    Obj_SquidGuard();
    virtual ~Obj_SquidGuard() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC vtable slots matching Ghidra
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_11();

    // Interaction & collision
    bool hitWithInk(f32 inkPower, const sead::Vector3f& hitDir);
    bool checkPlayerCollision(const sead::Vector3f& playerPos, f32 radius, sead::Vector3f& outRebound);

    // Status inspection
    SquidGuardState getState() const { return mState; }
    bool isRattling() const { return mState == SquidGuardState::cState_Rattle; }
    f32 getRattleOffset() const { return mRattleOffset; }
    const sead::Vector3f& getPosition() const { return mPosition; }

    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mPosition;      // 0x140

    SquidGuardState mState;
    f32 mRattleOffset;
    f32 mRattleEnergy;
    s32 mRattleTimer;

    undefined mReserved[0x38];
};

} // namespace Game
