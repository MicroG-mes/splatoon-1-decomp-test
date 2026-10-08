#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "Game/Enemy/RailKingPilotHouse.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class RailKingPhase : u32 {
    cPhase1 = 1,
    cPhase2 = 2,
    cPhase3 = 3,
    cPhase4 = 4,
    cPhase5 = 5 // Calamari Inkantation final phase
};

enum class RailKingState : u32 {
    cHoverPatrol     = 0,
    cLaunchFist      = 1,
    cFistReflected   = 2,
    cMissileBarrage  = 3,
    cStunnedGrooving = 4,
    cDefeated        = 5
};

/**
 * Enm_RailKing (DJ Octavio Final Boss)
 * Address: vtable @ 0x1008A270
 *
 * Real PowerPC methods:
 *   vfunc_7  @ 0x02375284 - AI update, rocket fist checks, spawner nodes, turntable tick
 *   vfunc_11 @ 0x02375c84 - Fist deflection & damage reflection
 */
class Enm_RailKing : public GambitActor {
public:
    static constexpr f32 cDeflectionThreshold = 40.0f;

    Enm_RailKing();
    virtual ~Enm_RailKing() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Vtable slots matching Ghidra 0x1008A270
    virtual void vfunc_7();
    virtual void vfunc_11();

    void updateBossAi(const sead::Vector3f& playerPos);
    void applyDamageToFist(f32 damage);
    void applyDamageToCockpit(f32 damage);

    RailKingState getState() const { return mState; }
    RailKingPhase getPhase() const { return mPhase; }
    f32 getFistDamageAccumulated() const { return mFistDamageAccumulated; }
    bool isGroovingToInkantation() const { return mPhase == RailKingPhase::cPhase5; }
    const sead::Vector3f& getPosition() const { return mPosition; }

    RailKingPilotHouse* getPilotHouse() { return &mPilotHouse; }

protected:
    void triggerFistReflect();
    void advancePhase();

    RailKingState mState;
    RailKingPhase mPhase;
    s32 mStateTimer;

    sead::Vector3f mPosition;
    sead::Vector3f mFistPosition;
    sead::Vector3f mFistVelocity;

    f32 mFistDamageAccumulated;
    f32 mCockpitHp;

    RailKingPilotHouse mPilotHouse;

    // Struct members aligned to Espresso PowerPC offsets:
    undefined mPaddingNodes[0x40];

    sead::Vector3f mThrustNode0;       // 0x218
    sead::Vector3f mThrustNode1;       // 0x220
    undefined mPaddingSound[0x68];

    u32 mSoundHandle;                  // 0x28C: Wasabi music sync
    undefined mPaddingFists[0x8];
    void* mLeftFistPtr;                // 0x298: Left rocket fist actor
    void* mRightFistPtr;               // 0x29C: Right rocket fist actor
    undefined mPaddingCockpit[0xC];
    void* mCockpitTurntablePtr;        // 0x2AC: Central turntable
    void* mVisualizerPtr;              // 0x2B0: Speaker visualizer

    undefined mPaddingSpk[0x40];
    void* mKillerWailSpeaker1;         // 0x2F4
    void* mKillerWailSpeaker2;         // 0x2F8
    void* mOctocopterSpawner1;         // 0x2FC
    undefined mPaddingSpawn2[4];
    void* mOctocopterSpawner2;         // 0x304
    void* mOctocopterSpawner3;         // 0x308
    undefined mPaddingBomb[4];
    void* mRainmakerLauncher;          // 0x310
};

} // namespace Game
