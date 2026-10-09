#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class StampState : u32 {
    cState_Wait     = 0,
    cState_Walk     = 1,
    cState_Chase    = 2,
    cState_AttackSt = 3,
    cState_Attack   = 4,
    cState_Chance   = 5,
    cState_StandUp  = 6,
    cState_Die      = 7
};

/**
 * Enm_Stamp / EnemyStamp
 * Retail Address: vtable @ 0x100c8220
 * Octostamp minion (タコスタンプ) in Octo Valley single-player campaign.
 * Heavy armored cube octarian that slams its impenetrable metal face down to crush Agent 3,
 * exposing its soft purple squid tentacle on its back during the Chance vulnerability state.
 *
 * Real PowerPC methods:
 *   vfunc_3  @ 0x0239d05c - Model & resource loading (Enm_Stamp.szs, 3,464 vertices)
 *   vfunc_5  @ 0x0239d064 - Parameter loading from Enm_Stamp.params (mLife: 0.70, mEyesight_Radius: 250)
 *   vfunc_7  @ 0x0239d120 - AI state machine update (Wait, Chase, Attack, Chance, StandUp)
 *   vfunc_9  @ 0x0239d240 - Front shield deflection vs vulnerable weak point damage
 *   vfunc_11 @ 0x0239d350 - Face-plant slam ground shockwave & ink splatter
 */
class Enm_Stamp : public GambitActor {
public:
    static constexpr f32 cMaxHealth         = 70.0f; // mLife: 0.70 in Enm_Stamp.params
    static constexpr f32 cEyesightRadius    = 250.0f;
    static constexpr f32 cDiePaintRadius    = 45.0f;
    static constexpr s32 cChanceFrames      = 60;   // Chance window when face-down
    static constexpr s32 cStandUpFrames     = 25;

    Enm_Stamp();
    virtual ~Enm_Stamp() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC vtable slots matching Ghidra
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_9();
    virtual void vfunc_11();

    // AI & Combat lifecycle
    void spawn(const sead::Vector3f& pos);
    bool checkSight(const sead::Vector3f& playerPos) const;
    void triggerSlamAttack();
    bool takeDamage(f32 damage, const sead::Vector3f& hitPos, const sead::Vector3f& hitDir, bool& outShieldDeflected);

    // Queries
    StampState getState() const { return mState; }
    f32 getHealth() const { return mHealth; }
    bool isChanceState() const { return mState == StampState::cState_Chance; }
    bool isDefeated() const { return mState == StampState::cState_Die; }

    const sead::Vector3f& getPosition() const { return mPosition; }
    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mPosition;      // 0x140
    StampState mState;
    f32 mHealth;
    s32 mStateTimer;
    bool mFaceDown;

    undefined mReserved[0x38];
};

using EnemyStamp = Enm_Stamp;

} // namespace Game
