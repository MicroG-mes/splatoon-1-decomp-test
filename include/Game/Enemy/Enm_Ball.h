#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class OctoballState : u32 {
    cWait        = 0,
    cNotice      = 1,
    cMove        = 2,
    cFall        = 3,
    cDie         = 4,
    cDazedUncurl = 5,
    cChance      = 5,

    // Legacy backwards-compatible aliases
    cIdleStand   = 0,
    cCurlingUp   = 1,
    cRollingDash = 2,
    cWallBounce  = 3,
    cDefeated    = 4
};

enum class OctoballVariant : u32 {
    cNormal = 0,
    cReal   = 1,
    cFake   = 2
};

/**
 * Enm_Ball / EnemyBall
 * Address: vtable @ 0x1005f71c
 * Rolling Octarian ball enemy with BarrierGuard invulnerability during roll
 * and vulnerability when sunk into player ink.
 *
 * Real PowerPC methods:
 *   vfunc_1  @ 0x022A45D8 - Destructor / teardown
 *   vfunc_3  @ 0x0229FEF4 - Model & resource loading
 *   vfunc_5  @ 0x022A02BC - Initialization & parameters
 *   vfunc_7  @ 0x022A0C5C - Update tick & status
 *   vfunc_11 @ 0x022A0DF8 - Collision handling
 *   vfunc_47 @ 0x022A4614 - Animation / roll rotation sync
 *   vfunc_52 @ 0x022A1E4C - Player target lock
 *   vfunc_60 @ 0x022C133C - State dispatch
 */
class Enm_Ball : public GambitActor {
public:
    static constexpr f32 cMaxHp                   = 30.0f;
    static constexpr f32 cLifeNormal              = 0.70f;
    static constexpr f32 cLifeReal                = 12.0f;
    static constexpr f32 cLifeFake                = 0.60f;
    static constexpr f32 cRollSpeed               = 0.22f;
    static constexpr f32 cSpeedNormal             = 1.50f;
    static constexpr f32 cSpeedReal               = 0.65f;
    static constexpr f32 cSpeedFake               = 0.65f;
    static constexpr f32 cSpeedPlayerInkNormal    = 0.20f;
    static constexpr f32 cSpeedPlayerInkReal      = 0.06f;
    static constexpr f32 cSpeedPlayerInkFake      = 0.06f;
    static constexpr f32 cEyesightRadius          = 200.0f;
    static constexpr f32 cEyesightAngleNormal     = 120.0f;
    static constexpr f32 cEyesightAngleReal       = 45.0f;
    static constexpr f32 cTrackPaintRadius        = 20.0f;
    static constexpr f32 cDiePaintRadius          = 40.0f;
    static constexpr s32 cChanceSinkFrame         = 20;
    static constexpr f32 cEscapeSpeedRate         = 1.50f;
    static constexpr s32 cEscapeTime              = 60;
    static constexpr s32 cDroppedPowerEggs        = 5;

    Enm_Ball();
    virtual ~Enm_Ball() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC vtable slots matching Ghidra
    virtual void vfunc_1();
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_11();
    virtual void vfunc_47();
    virtual void vfunc_52();
    virtual void vfunc_60();

    // Configuration
    void setVariant(OctoballVariant variant);
    OctoballVariant getVariant() const { return mVariant; }

    // AI & damage
    void updateAi(const sead::Vector3f& playerPos, bool isRollingOnPlayerInk);
    bool applyDamage(f32 damage);

    // Getters
    OctoballState getState() const { return mState; }
    f32 getHp() const { return mHp; }
    f32 getMaxHp() const { return mMaxHp; }
    bool isRolling() const { return mState == OctoballState::cMove; }
    bool isInvulnerable() const { return mMuteki; }
    bool isChanceState() const { return mState == OctoballState::cChance; }
    bool isDefeated() const { return mState == OctoballState::cDie; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    const sead::Vector3f& getMoveDir() const { return mMoveDir; }
    f32 getSpeed() const { return mCurrentSpeed; }
    s32 getDroppedPowerEggs() const { return mDroppedEggs; }
    f32 getBarrierDeflectedDamage() const { return mDeflectedDamage; }

    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    void stepRollingMovement();

    sead::Vector3f mPosition;      // 0x140
    sead::Vector3f mMoveDir;
    f32 mHp;
    f32 mMaxHp;
    f32 mCurrentSpeed;
    f32 mSpeedNormal;
    f32 mSpeedPL;
    f32 mDeflectedDamage;
    OctoballState mState;
    OctoballVariant mVariant;
    s32 mStateTimer;
    s32 mChanceTimer;
    s32 mEscapeTimer;
    s32 mDroppedEggs;
    bool mMuteki;

    undefined mReserved[0x38];
};

// Internal binary aliases
using EnemyBall   = Enm_Ball;
using Enm_BallReal = Enm_Ball;
using Enm_BallFake = Enm_Ball;

} // namespace Game
