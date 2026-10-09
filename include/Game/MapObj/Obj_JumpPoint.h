#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class JumpPointState : u32 {
    cState_Idle      = 0,
    cState_Pulse     = 1,
    cState_Break     = 2
};

/**
 * Obj_JumpPoint / JumpPoint / Wsb_Beacon
 * Retail Address: vtable @ 0x100e0b8c
 * Deployable Super Jump Beacon (ジャンプビーコン) and map jump destination.
 * Allows teammates to super jump directly to its coordinates on the stage.
 *
 * Real PowerPC methods:
 *   vfunc_3  @ 0x025f2a24 - Model loading (Obj_JumpPoint.szs, emission pulse)
 *   vfunc_5  @ 0x025f2b1c - Parameter initialization (Obj_JumpPoint.params, mLife: 3.0)
 *   vfunc_7  @ 0x025f2e64 - Radar sonar broadcast update
 *   vfunc_9  @ 0x025f2f2c - Signal attention beacon to GameWarpAttentionMgr
 *   vfunc_11 @ 0x025f30a8 - Super jump landing callback & durability consumption
 */
class Obj_JumpPoint : public GambitActor {
public:
    static constexpr f32 cMaxLife            = 3.0f; // mLife: 3.0 in Obj_JumpPoint.params
    static constexpr f32 cRadarDetectionDist = 15.0f;
    static constexpr s32 cRadarInterval      = 45;

    Obj_JumpPoint();
    virtual ~Obj_JumpPoint() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC vtable slots matching Ghidra
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_9();
    virtual void vfunc_11();

    // Beacon mechanics
    void deploy(const sead::Vector3f& pos, u32 teamId);
    bool onSuperJumpLanded();
    bool takeDamage(f32 damage);

    // Queries
    JumpPointState getState() const { return mState; }
    f32 getDurability() const { return mDurability; }
    bool isActive() const { return mState != JumpPointState::cState_Break && mDurability > 0.0f; }
    bool isRadarPulseActive() const { return mRadarActive; }
    u32 getTeamId() const { return mTeamId; }

    const sead::Vector3f& getPosition() const { return mPosition; }
    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mPosition;      // 0x140
    JumpPointState mState;
    f32 mDurability;
    u32 mTeamId;
    s32 mTimer;
    s32 mJumpsCount;
    bool mRadarActive;

    undefined mReserved[0x34];
};

using JumpPoint = Obj_JumpPoint;

} // namespace Game
