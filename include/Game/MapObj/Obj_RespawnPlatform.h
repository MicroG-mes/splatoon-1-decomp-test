#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class RespawnPlatformState : u32 {
    cState_Wait    = 0,
    cState_Respawn = 1
};

enum class RespawnTeam : u32 {
    cTeam_Alpha = 0,
    cTeam_Bravo = 1
};

/**
 * Obj_RespawnPlatform / RespawnPoint
 * Retail Address: vtable @ 0x100d4e4c
 * Team spawn point platform in all competitive versus and battle stages.
 * Provides barrier dome shielding to defend spawning players from enemy attacks.
 *
 * Real PowerPC methods:
 *   vfunc_3  @ 0x025aa410 - Model loading (Obj_RespawnPlatform.szs, M_RespawnPlatform01, tex_mtx1)
 *   vfunc_5  @ 0x025aa3b8 - Spawn barrier collision & team setup (Alpha / Bravo)
 *   vfunc_7  @ 0x025aa520 - Shield pulse animation & ink level update
 *   vfunc_14 @ 0x025aa6b0 - Super jump landing pad & player ink replenishment
 */
class Obj_RespawnPlatform : public GambitActor {
public:
    static constexpr f32 cBarrierRadius      = 4.5f;
    static constexpr f32 cInkRefillRate      = 0.10f; // 10% ink tank refill per frame

    Obj_RespawnPlatform();
    virtual ~Obj_RespawnPlatform() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC vtable slots matching Ghidra
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_14();

    // Match & team mechanics
    void setTeam(RespawnTeam team) { mTeam = team; }
    RespawnTeam getTeam() const { return mTeam; }

    bool isInsideBarrier(const sead::Vector3f& playerPos) const;
    void triggerRespawnEffect();
    f32 refillPlayerInk(f32 currentInk);

    // Queries
    RespawnPlatformState getState() const { return mState; }
    f32 getBarrierPulse() const { return mBarrierPulse; }
    s32 getSpawnsCount() const { return mSpawnsCount; }

    const sead::Vector3f& getPosition() const { return mPosition; }
    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mPosition;      // 0x140
    RespawnPlatformState mState;
    RespawnTeam mTeam;
    s32 mTimer;
    s32 mSpawnsCount;
    f32 mBarrierPulse;

    undefined mReserved[0x34];
};

using RespawnPoint = Obj_RespawnPlatform;

} // namespace Game
