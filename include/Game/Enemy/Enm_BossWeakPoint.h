#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class BossWeakPointState : u32 {
    cState_Appear    = 0,
    cState_Wait      = 1,
    cState_Damage    = 2,
    cState_Chance    = 3,
    cState_Disappear = 4,
    cState_Die       = 5,
    cState_Return    = 6,
    cState_Find      = 7
};

/**
 * Enm_BossWeakPoint / BossWeakPoint
 * Retail Address: vtable @ 0x10065694
 * Exposed giant tentacle weak point core spawned by campaign boss encounters
 * (Octostomp, Octonozzle, Octowhirl, Octomaw, DJ Octavio).
 *
 * Real PowerPC methods:
 *   vfunc_3  @ 0x022c82b4 - Resource loading (Model/Enm_BossWeakPoint.szs, root bone, scale)
 *   vfunc_5  @ 0x022de850 - Parameter initialization (Enm_BossWeakPoint_AnmItp.params)
 *   vfunc_7  @ 0x02332a20 - State machine update & pulsating scale reaction
 *   vfunc_14 @ 0x0239ed08 - Ink shot hit detection & damage accumulation
 *   vfunc_47 @ 0x0236aee0 - EnemyParametersMgr parameter synchronization
 */
class Enm_BossWeakPoint : public GambitActor {
public:
    static constexpr f32 cMaxHealth         = 100.0f;
    static constexpr f32 cCollisionRadius   = 3.2f;
    static constexpr f32 cCollisionHeight   = 4.5f;
    static constexpr s32 cWaitTimeFrames    = 20;   // Wait: 20.0 frames in AnmItp.params
    static constexpr s32 cDisappearFrames   = 3;    // Disappear: 3.0 frames in AnmItp.params
    static constexpr f32 cMaxPulseScale     = 1.35f;

    Enm_BossWeakPoint();
    virtual ~Enm_BossWeakPoint() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC vtable slots matching Ghidra
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_14();
    virtual void vfunc_47();

    // Core combat lifecycle
    void appear(const sead::Vector3f& spawnPos);
    void disappear();
    bool hitWithInk(f32 damage, const sead::Vector3f& hitDir);

    // Queries
    BossWeakPointState getState() const { return mState; }
    f32 getHealth() const { return mHealth; }
    f32 getPulseScale() const { return mPulseScale; }
    bool isVulnerable() const { return mState == BossWeakPointState::cState_Wait || mState == BossWeakPointState::cState_Chance || mState == BossWeakPointState::cState_Damage; }
    bool isDefeated() const { return mState == BossWeakPointState::cState_Die; }
    s32 getSplatsTriggered() const { return mSplatsTriggered; }

    const sead::Vector3f& getPosition() const { return mPosition; }
    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mPosition;      // 0x140
    BossWeakPointState mState;
    f32 mHealth;
    f32 mPulseScale;
    f32 mPulseVelocity;
    s32 mStateTimer;
    s32 mHitRecoveryTimer;
    s32 mSplatsTriggered;

    undefined mReserved[0x34];
};

} // namespace Game
