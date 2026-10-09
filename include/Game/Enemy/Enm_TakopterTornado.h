#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class TakopterTornadoState : u32 {
    cState_Wait     = 0,
    cState_Chase    = 1,
    cState_Attack   = 2,
    cState_Escape   = 3,
    cState_Die      = 4
};

/**
 * Enm_TakopterTornado / EnemyTakopterTornado
 * Retail Address: vtable @ 0x1008f420 / 0x1008f55c
 * Octocopter Tornado variant (タコプター トルネード) in Octo Valley single-player campaign.
 * Aerial octarian minion equipped with an aerodynamic rotor blade that fires high-velocity
 * tornado vortex ink blasts from mid-air.
 *
 * Real PowerPC methods:
 *   vfunc_3  @ 0x023cf3a4 - Model & asset loading (Enm_TakopterTornado.szs, 8,675 vertices)
 *   vfunc_5  @ 0x023c7870 - Parameter initialization from Enm_TakopterTornado.params
 *   vfunc_7  @ 0x023c8060 - State machine update (StateTornado::cPatrol @ 0x023d4d9c)
 *   vfunc_47 @ 0x023d1370 - Tornado vortex projectile spawn & ink dispersal
 */
class Enm_TakopterTornado : public GambitActor {
public:
    static constexpr f32 cMaxHealth             = 40.0f; // mLife: 4.0 in params (4 hits)
    static constexpr f32 cEyesightRadius        = 3000.0f; // mEyesight_Radius: 3000.0
    static constexpr f32 cDiePaintRadius        = 55.0f;   // mDiePaintRadius: 55.0
    static constexpr f32 cNormalSpeed           = 1.44f;   // mSpeed: 1.44
    static constexpr f32 cChaseSpeed            = 2.88f;   // mSpeedChase: 2.88
    static constexpr f32 cEscapeSpeed           = 1.00f;   // mSpeedEscape: 1.00
    static constexpr f32 cShotSpeed             = 2.40f;   // mShotSpeed: 2.40
    static constexpr f32 cPropellerRotSpeed     = 19.0f;   // mPropellerRotSpeed: 19.0 deg/f
    static constexpr f32 cFuwaAmplitude         = 2.0f;    // mFuwaWidth: 2.0
    static constexpr f32 cHoverAltitude         = 90.0f;   // mHeight: 90.0

    Enm_TakopterTornado();
    virtual ~Enm_TakopterTornado() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC vtable slots matching Ghidra
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_47();

    // AI & Combat lifecycle
    void spawn(const sead::Vector3f& pos);
    bool checkSight(const sead::Vector3f& playerPos) const;
    void triggerTornadoAttack();
    bool hitWithInk(f32 damage, const sead::Vector3f& hitDir);

    // Queries
    TakopterTornadoState getState() const { return mState; }
    f32 getHealth() const { return mHealth; }
    bool isAirborne() const { return mPosition.y >= cHoverAltitude * 0.5f; }
    bool isDefeated() const { return mState == TakopterTornadoState::cState_Die; }
    f32 getPropellerAngle() const { return mPropellerAngle; }
    f32 getHoverBobOffset() const { return mBobOffset; }

    const sead::Vector3f& getPosition() const { return mPosition; }
    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mPosition;      // 0x140
    TakopterTornadoState mState;
    f32 mHealth;
    f32 mPropellerAngle;
    f32 mBobOffset;
    s32 mTimer;
    s32 mStateTimer;

    undefined mReserved[0x38];
};

using EnemyTakopterTornado = Enm_TakopterTornado;

} // namespace Game
