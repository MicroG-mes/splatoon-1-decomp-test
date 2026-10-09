#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include "sead/resource/BfresParser.h"

namespace Game {

enum class OctotrooperState : u32 {
    cIdlePatrol = 0,
    cAimCharge  = 1,
    cShoot      = 2,
    cHopBack    = 3,
    cSplatted   = 4
};

/**
 * EnemyHohei / Enm_Hohei
 * Address: vtable @ 0x1007797C
 * Standard Octarian infantry soldier (Octotrooper).
 *
 * Real PowerPC methods:
 *   vfunc_3  @ 0x02322260 - Model and resource loading
 *   vfunc_5  @ 0x02322798 - Spawn / reset
 *   vfunc_7  @ 0x023230cc - AI tick, parameter struct at 0x70, position at 0x140
 *   vfunc_47 @ 0x023256ec - Animation / model sync
 *   vfunc_52 @ 0x02323c70 - Target nearest player via PlayerMgr
 */
class EnemyHohei : public GambitActor {
public:
    EnemyHohei();
    virtual ~EnemyHohei() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Vtable slots matching Ghidra 0x1007797C
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_47();
    virtual void vfunc_52();

    void applyDamage(f32 damage);
    void updateAi(const sead::Vector3f& playerPos);

    OctotrooperState getState() const { return mState; }
    f32 getRemainingHp() const { return mCurrentHp; }
    const sead::Vector3f& getPosition() const { return mWorldPos; }
    u32 getModelVertexCount() const { return static_cast<u32>(mModel.getTotalVertexCount()); }
    const sead::BfresModel& getModel() const { return mModel; }

protected:
    void fireInkBlob();

    OctotrooperState mState;
    s32 mStateTimer;

    f32 mYaw;
    f32 mMaxHp;
    f32 mCurrentHp;

    // Struct members aligned to Espresso PowerPC offsets:
    // +0x70: Parameter struct pointer
    void* mParamStructPtr;             // 0x70
    undefined mPadding1[0x40];

    void* mSoundComponent;             // 0xB4
    undefined mPadding2[0x84];

    sead::Vector3f mWorldPos;          // 0x140 - 0x14C
    undefined mPadding3[0xA0];

    void* mTrajectoryComponent;        // 0x1F0
    undefined mPadding4[0xC];

    void* mInkMuzzleEmitter;           // 0x200
    undefined mPadding5[0x8];

    void* mStateMachineComponent;      // 0x20C
    sead::BfresModel mModel;
};

// Internal binary alias
using Enm_Hohei = EnemyHohei;

} // namespace Game
