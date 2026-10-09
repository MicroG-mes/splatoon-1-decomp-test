#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class CommanderBindState : u32 {
    cState_Wait              = 0,
    cState_Talk              = 1,
    cState_TakeCommander     = 2,
    cState_Break             = 3,
    cState_Landing           = 4,
    cState_ReleaseCommander  = 5,
    cState_LastBossDance     = 6
};

/**
 * Npc_CommanderBind / NpcCommanderBind
 * Retail Address: vtable @ 0x100e5100
 * Bound Cap'n Cuttlefish held captive in DJ Octavio's floating snowglobe arena.
 * Roots for Agent 3 during the final battle and dances to the Calamari Inkantation.
 *
 * Real PowerPC methods:
 *   vfunc_3  @ 0x0260f964 - Model loading (Npc_CommanderBind.szs)
 *   vfunc_5  @ 0x0260fcc4 - Bound state initialization (Npc_CommanderBind_AnmItp.params)
 *   vfunc_7  @ 0x0260fd6c - State machine update & dancing cheer
 *   vfunc_9  @ 0x0260fda0 - Snowglobe touch / projectile collision detection
 *   vfunc_11 @ 0x0260fda4 - Break / release event (Break: 20 frames)
 */
class Npc_CommanderBind : public GambitActor {
public:
    static constexpr s32 cBreakFrames = 20; // Break: 20.0 frames in AnmItp.params

    Npc_CommanderBind();
    virtual ~Npc_CommanderBind() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC vtable slots matching Ghidra
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_9();
    virtual void vfunc_11();

    // Combat & sequence events
    void startCheeringDance();
    void triggerRescueBreak();

    // Queries
    CommanderBindState getState() const { return mState; }
    bool isBound() const { return mState != CommanderBindState::cState_ReleaseCommander && mState != CommanderBindState::cState_Landing; }
    bool isDancing() const { return mState == CommanderBindState::cState_LastBossDance; }
    bool isRescued() const { return mState == CommanderBindState::cState_Landing; }

    const sead::Vector3f& getPosition() const { return mPosition; }
    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mPosition;      // 0x140
    CommanderBindState mState;
    s32 mStateTimer;
    f32 mDancePhase;

    undefined mReserved[0x34];
};

using NpcCommanderBind = Npc_CommanderBind;

} // namespace Game
