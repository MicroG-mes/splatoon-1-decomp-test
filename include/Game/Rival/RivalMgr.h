#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "Game/Rival/GameRivalSquad.h"

namespace Game {

/**
 * RivalMgr
 * Address: vtable @ 0x100F5BC4
 * Central manager for Octoling squad battles in Single Player campaign.
 *
 * Real PowerPC methods:
 *   vfunc_3  @ 0x0270ba24 - Squad allocation & initialization
 *   vfunc_4  @ 0x0270ba80 - Pre-reset
 *   vfunc_5  @ 0x0270bbf4 - Reset waypoints and difficulty
 *   vfunc_6  @ 0x0270c3cc - Enter scene
 *   vfunc_7  @ 0x0270c224 - AI tick, tactical re-plan, actor updates
 *   vfunc_8  @ 0x0270da78 - Post-calc
 *   vfunc_9  @ 0x0270c278 - Render
 *   vfunc_10 @ 0x0270dad4 - Octoling splatted event, flags |= 0x200
 *   vfunc_11 @ 0x0270c2d4 - Respawn event & beacon jump
 *   vfunc_12 @ 0x0270d674 - Target acquired event
 *   vfunc_13 @ 0x0270d614 - Target lost event
 *   vfunc_33 @ 0x0270db30 - Terminate & unload
 *   vfunc_34 @ 0x0270bfd4 - Difficulty configuration
 */
class RivalMgr : public GambitActor {
public:
    static constexpr u32 cMaxOctolings = 4;

    static RivalMgr* instance() { return sInstance; }

    RivalMgr();
    virtual ~RivalMgr() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Vtable lifecycle overrides directly from Espresso PPC
    virtual void vfunc_3();
    virtual void vfunc_4();
    virtual void vfunc_5();
    virtual void vfunc_6();
    virtual void vfunc_7();
    virtual void vfunc_8();
    virtual void vfunc_9();
    virtual void vfunc_10(u32 octolingIndex);
    virtual void vfunc_11(u32 octolingIndex);
    virtual void vfunc_12(const sead::Vector3f& playerPos);
    virtual void vfunc_13();
    virtual void vfunc_33();
    virtual void vfunc_34(RivalDifficulty difficulty);

    void spawnSquad(const sead::Vector3f spawnPoints[cMaxOctolings], RivalDifficulty difficulty);
    void updateSquadAi(const sead::Vector3f& playerPos);

    u32 getActiveCount() const;
    u32 getStateFlags() const { return mStateFlags; }
    GameRivalSquad* getOctoling(u32 index) {
        return (index < cMaxOctolings) ? &mOctolings[index] : nullptr;
    }

protected:
    static RivalMgr* sInstance;

    undefined mPaddingMgr[0x230 - sizeof(GambitActor)];

    // +0x230: Lifecycle and state bitmask
    u32 mStateFlags; // 0x230

    undefined mPaddingDirty[0x450 - 0x234];
    u8 mNeedsReplan; // 0x450

    GameRivalSquad mOctolings[cMaxOctolings];
    u32 mSquadSize;
    RivalDifficulty mDifficulty;
    sead::Vector3f mLastPlayerPos;
};

} // namespace Game
