#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include "sead/resource/BfresParser.h"

namespace Game {

enum class JudgeSleepState : u32 {
    cDeepSleep   = 0,
    cBreathing   = 1,
    cTwitch      = 2,
    cSnoreBubble = 3,
    cStartleWake = 4
};

/**
 * Npc_JudgeSleep
 * Sleeping Judd the Cat NPC in Inkopolis Plaza.
 *
 * Retail Wii U binary:
 *   vtable @ 0x100E6BD4
 *   Camera: content/Static/Plaza_JudgeLook.camera.params
 *     mAt: (0.8, 16.8, 0.0)
 *     mPos: (-1.1, 21.3, 36.9)
 *   Model: content/Model/Npc_JudgeSleep.szs
 *   Meshes:
 *     - Npc_JudgeSleep__M_Eye (58 vertices)
 *     - Npc_JudgeSleep__M_Judge (2,832 vertices)
 *     - Zabuton__M_Zabuton (627 vertices)
 *   Total Vertices: 3,517 authentic BFRES vertices.
 */
class Npc_JudgeSleep : public GambitActor {
public:
    Npc_JudgeSleep();
    virtual ~Npc_JudgeSleep() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC Ghidra vtable methods
    virtual void vfunc_3();  // 0x02618DE4 - Model load & cushion setup
    virtual void vfunc_5();  // 0x02618DF8 - Reset & enter sleep
    virtual void vfunc_7();  // 0x02618DFC - Sleep breath & animation cycle

    void wakeUp();
    void fallAsleep();

    JudgeSleepState getState() const { return mState; }
    f32 getChestExpansion() const { return mChestExpansion; }
    f32 getSnoreBubbleSize() const { return mSnoreBubbleSize; }
    bool isAwake() const { return mState == JudgeSleepState::cStartleWake; }

    const sead::Vector3f& getLookAtPos() const { return mLookAtPos; }
    const sead::Vector3f& getCameraPos() const { return mCameraPos; }

    u32 getModelVertexCount() const { return static_cast<u32>(mModel.getTotalVertexCount()); }

private:
    JudgeSleepState mState;
    u32 mAnimFrame;
    f32 mBreathCycle;        // Rhythmic breathing 0.0 to 1.0
    f32 mChestExpansion;     // Expansion scale [1.0, 1.08]
    f32 mSnoreBubbleSize;    // Bubble size [0.0, 1.0]
    u32 mTwitchTimer;

    sead::Vector3f mLookAtPos;
    sead::Vector3f mCameraPos;

    sead::BfresModel mModel;
};

} // namespace Game
