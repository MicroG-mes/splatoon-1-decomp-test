#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class TakopterBombState : u32 {
    cState_Wait     = 0,
    cState_Chase    = 1,
    cState_BombDrop = 2,
    cState_Die      = 3
};

/**
 * Enm_TakopterBomb / EnemyTakopterBomb
 * Retail Address: vtable @ 0x1008d510
 * Octobomber (タコプターボム) in Octo Valley single-player campaign.
 * Heavy aerial octarian bomber minion with high durability that flies high above the arena,
 * tracking Agent 3 and dropping Splat Bombs from above.
 *
 * Real PowerPC methods:
 *   vfunc_3  @ 0x023cf3a4 - Model & asset loading (Enm_TakopterBomb.szs, 11,611 vertices)
 *   vfunc_5  @ 0x023c7870 - Parameter loading from Enm_TakopterBomb.params (mLife: 1.40 / 140.0 HP)
 *   vfunc_7  @ 0x023c8060 - Aerial tracking & bomb drop schedule (280m eyesight)
 *   vfunc_47 @ 0x023d1370 - Splat Bomb release downward
 */
class Enm_TakopterBomb : public GambitActor {
public:
    static constexpr f32 cMaxHealth         = 140.0f; // mLife: 1.40 in params
    static constexpr f32 cEyesightRadius    = 280.0f; // mEyesight_Radius: 280.0
    static constexpr f32 cDiePaintRadius    = 40.0f;  // mDiePaintRadius: 40.0
    static constexpr f32 cFlightAltitude    = 100.0f;

    Enm_TakopterBomb();
    virtual ~Enm_TakopterBomb() override;

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
    void dropBomb();
    bool takeDamage(f32 damage);

    // Queries
    TakopterBombState getState() const { return mState; }
    f32 getHealth() const { return mHealth; }
    bool isAirborne() const { return mPosition.y >= cFlightAltitude * 0.5f; }
    bool isDefeated() const { return mState == TakopterBombState::cState_Die; }
    s32 getDroppedBombCount() const { return mDroppedBombs; }

    const sead::Vector3f& getPosition() const { return mPosition; }
    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mPosition;      // 0x140
    TakopterBombState mState;
    f32 mHealth;
    s32 mDroppedBombs;
    s32 mStateTimer;

    undefined mReserved[0x38];
};

using EnemyTakopterBomb = Enm_TakopterBomb;

} // namespace Game
