#include "Game/Rival/RivalMgr.h"
#include <cstring>

namespace Game {

RivalMgr* RivalMgr::sInstance = nullptr;

RivalMgr::RivalMgr()
    : mStateFlags(0),
      mNeedsReplan(0),
      mSquadSize(0),
      mDifficulty(RivalDifficulty::cLevel1),
      mLastPlayerPos(0.0f, 0.0f, 0.0f) {
    sInstance = this;
    std::memset(mPaddingMgr, 0, sizeof(mPaddingMgr));
    std::memset(mPaddingDirty, 0, sizeof(mPaddingDirty));
}

RivalMgr::~RivalMgr() {
    if (sInstance == this) {
        sInstance = nullptr;
    }
}

void RivalMgr::init() {
    GambitActor::init();
    vfunc_3();
}

void RivalMgr::update() {
    GambitActor::update();
    vfunc_7();
}

void RivalMgr::draw() {
    GambitActor::draw();
    vfunc_9();
}

/**
 * RivalMgr__vfunc_3 @ 0x0270ba24
 * Squad initialization, sets state flag 0x1.
 */
void RivalMgr::vfunc_3() {
    mStateFlags |= 0x1;
    mNeedsReplan = 1;
}

/**
 * RivalMgr__vfunc_4 @ 0x0270ba80
 * Pre-reset, sets state flag 0x2.
 */
void RivalMgr::vfunc_4() {
    mStateFlags |= 0x2;
}

/**
 * RivalMgr__vfunc_5 @ 0x0270bbf4
 * Reset squad state, sets state flag 0x4.
 */
void RivalMgr::vfunc_5() {
    mStateFlags |= 0x4;
    mNeedsReplan = 1;
}

/**
 * RivalMgr__vfunc_6 @ 0x0270c3cc
 * Enter battle scene, sets state flag 0x8.
 */
void RivalMgr::vfunc_6() {
    mStateFlags |= 0x8;
}

/**
 * RivalMgr__vfunc_7 @ 0x0270c224
 * Main squad AI update loop.
 * Evaluates dirty flag *(param_1 + 0x450), recalculates tactical routes,
 * and updates each Octoling actor.
 */
void RivalMgr::vfunc_7() {
    if (mNeedsReplan != 0) {
        mNeedsReplan = 0;
        // Recalculate tactical waypoints / patrol goals
    }

    for (u32 i = 0; i < mSquadSize; ++i) {
        if (mOctolings[i].isAlive()) {
            mOctolings[i].updateTactics(mLastPlayerPos);
        }
    }
}

/**
 * RivalMgr__vfunc_8 @ 0x0270da78
 * Post-calc, sets state flag 0x80.
 */
void RivalMgr::vfunc_8() {
    mStateFlags |= 0x80;
}

/**
 * RivalMgr__vfunc_9 @ 0x0270c278
 * Draw tick, sets state flag 0x100.
 */
void RivalMgr::vfunc_9() {
    mStateFlags |= 0x100;
    for (u32 i = 0; i < mSquadSize; ++i) {
        mOctolings[i].draw();
    }
}

/**
 * RivalMgr__vfunc_10 @ 0x0270dad4
 * Octoling splatted event handler: sets state flag 0x200.
 */
void RivalMgr::vfunc_10(u32 octolingIndex) {
    if (octolingIndex < cMaxOctolings) {
        mOctolings[octolingIndex].applyDamage(100.0f);
    }
    mStateFlags |= 0x200;
    mNeedsReplan = 1; // Trigger tactical re-evaluation
}

/**
 * RivalMgr__vfunc_11 @ 0x0270c2d4
 * Octoling respawn event handler.
 */
void RivalMgr::vfunc_11(u32 octolingIndex) {
    if (octolingIndex < cMaxOctolings) {
        mOctolings[octolingIndex].triggerRespawn(sead::Vector3f(0.0f, 0.0f, 20.0f));
    }
    mStateFlags |= 0x400;
}

/**
 * RivalMgr__vfunc_12 @ 0x0270d674
 * Player spotted event: acquires target coordinate.
 */
void RivalMgr::vfunc_12(const sead::Vector3f& playerPos) {
    mLastPlayerPos = playerPos;
    mStateFlags |= 0x800;
}

/**
 * RivalMgr__vfunc_13 @ 0x0270d614
 * Target lost event.
 */
void RivalMgr::vfunc_13() {
    mStateFlags |= 0x1000;
}

/**
 * RivalMgr__vfunc_33 @ 0x0270db30
 * Terminate & cleanup squad.
 */
void RivalMgr::vfunc_33() {
    mStateFlags = 0;
    mSquadSize = 0;
}

/**
 * RivalMgr__vfunc_34 @ 0x0270bfd4
 * Sets difficulty profile for squad.
 */
void RivalMgr::vfunc_34(RivalDifficulty difficulty) {
    mDifficulty = difficulty;
    mNeedsReplan = 1;
}

void RivalMgr::spawnSquad(const sead::Vector3f spawnPoints[cMaxOctolings], RivalDifficulty difficulty) {
    mDifficulty = difficulty;
    mSquadSize = cMaxOctolings;
    for (u32 i = 0; i < cMaxOctolings; ++i) {
        mOctolings[i].init();
        mOctolings[i].spawn(spawnPoints[i], difficulty);
    }
    mNeedsReplan = 1;
}

void RivalMgr::updateSquadAi(const sead::Vector3f& playerPos) {
    vfunc_12(playerPos);
    vfunc_7();
}

u32 RivalMgr::getActiveCount() const {
    u32 count = 0;
    for (u32 i = 0; i < mSquadSize; ++i) {
        if (mOctolings[i].isAlive()) {
            count++;
        }
    }
    return count;
}

} // namespace Game
