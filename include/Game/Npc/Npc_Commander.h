#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class CommanderState : u32 {
    cState_Wait        = 0,
    cState_DemoWait    = 1,
    cState_Appear      = 2,
    cState_Hide        = 3,
    cState_Talk_St     = 4,
    cState_Talk_A      = 5,
    cState_Talk_Ed     = 6,
    cState_Pose_A      = 7,
    cState_Pose_B      = 8,
    cState_Flick       = 9,
    cState_Dance       = 10
};

/**
 * Npc_Commander / NpcCommander
 * Retail Address: vtable @ 0x100e4ce8
 * Cap'n Craig Cuttlefish (アタリメ司令) in Octo Valley overworld and Inkopolis Plaza manhole.
 * Veteran leader of the New Squidbeak Splatoon who mentors Agent 3.
 *
 * Real PowerPC methods:
 *   vfunc_3 @ 0x0260d940 - Resource loading (Npc_Commander.szs, head, M_Eye, tex_mtx0)
 *   vfunc_5 @ 0x0260dc10 - Parameter initialization (Npc_Commander_AnmItp.params)
 *   vfunc_7 @ 0x0260e2c0 - State update: talking gestures, cane flicking, eye tracking
 *   vfunc_9 @ 0x0260e524 - Player proximity check & dialogue prompt trigger
 */
class Npc_Commander : public GambitActor {
public:
    static constexpr f32 cTalkRadius        = 3.5f;
    static constexpr s32 cTalkStFrames      = 5;    // Talk_St: 5 frames in AnmItp.params
    static constexpr s32 cTalkEdFrames      = 7;    // Talk_Ed: 7 frames in AnmItp.params
    static constexpr s32 cPoseAFrames       = 30;   // Pose_A: 30 frames in AnmItp.params
    static constexpr s32 cFlickFrames       = 12;   // Flick: 12 frames in AnmItp.params

    Npc_Commander();
    virtual ~Npc_Commander() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC vtable slots matching Ghidra
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_9();

    // Interaction & dialogue
    bool checkPlayerProximity(const sead::Vector3f& playerPos);
    void startTalking();
    void endTalking();
    void triggerCaneFlick();
    void triggerHeroPose();

    // Queries
    CommanderState getState() const { return mState; }
    bool isTalking() const { return mState == CommanderState::cState_Talk_St || mState == CommanderState::cState_Talk_A || mState == CommanderState::cState_Talk_Ed; }
    s32 getTalkCount() const { return mTalkCount; }

    const sead::Vector3f& getPosition() const { return mPosition; }
    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mPosition;      // 0x140
    CommanderState mState;
    s32 mStateTimer;
    s32 mTalkCount;
    bool mPlayerNearby;

    undefined mReserved[0x34];
};

using NpcCommander = Npc_Commander;

} // namespace Game
