#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class CleanerState : u32 {
    cState_Patrol   = 0,
    cState_Clean    = 1,
    cState_Turn     = 2
};

/**
 * Enm_Cleaner / EnemyCleaner
 * Retail Address: vtable @ 0x10072b20
 * Squee-G Industrial Floor Cleaner (タコクリーナー) in Octo Valley single-player campaign.
 * Armored cleaning automaton that patrols surfaces and sucks up friendly Inkling paint,
 * returning the floor to a spotless un-inked state. Nearly invulnerable to standard weapons.
 *
 * Real PowerPC methods:
 *   vfunc_3  @ 0x022d4ce0 - Model & collision loading (Enm_Cleaner.szs, 1,339 vertices)
 *   vfunc_5  @ 0x022d5168 - Parameter loading from Enm_Cleaner.params (mLife: 100.0 HP)
 *   vfunc_7  @ 0x022d9670 - Vacuum sweep physics & ink absorption loop
 *   vfunc_11 @ 0x022d97fc - Wall / edge bump bounce & 180-degree turnaround
 *   vfunc_37 @ 0x022d9cbc - Squeegee brush rotation & scrubber particle emission
 *   vfunc_47 @ 0x022daff4 - Armor plate deflection & damage immunity
 */
class Enm_Cleaner : public GambitActor {
public:
    static constexpr f32 cMaxHealth         = 100.0f; // mLife: 100.0 (high durability)
    static constexpr f32 cEyesightRadius    = 200.0f; // mEyesight_Radius: 200.0
    static constexpr f32 cCleanRadius       = 8.0f;
    static constexpr f32 cMoveSpeed         = 1.20f;  // cruising sweep speed

    Enm_Cleaner();
    virtual ~Enm_Cleaner() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC vtable slots matching Ghidra
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_11();
    virtual void vfunc_37();
    virtual void vfunc_47();

    // Cleaning & interaction
    void spawn(const sead::Vector3f& pos, const sead::Vector3f& moveDir);
    f32 absorbInk(f32 amount);
    void bumpWall();
    bool hitWithInk(f32 damage, bool& outArmorDeflected);

    // Queries
    CleanerState getState() const { return mState; }
    f32 getHealth() const { return mHealth; }
    f32 getTotalCleanedInk() const { return mTotalCleaned; }
    f32 getBrushRotation() const { return mBrushAngle; }
    bool isCleaning() const { return mState == CleanerState::cState_Clean; }

    const sead::Vector3f& getPosition() const { return mPosition; }
    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mPosition;      // 0x140
    sead::Vector3f mMoveDirection;
    CleanerState mState;
    f32 mHealth;
    f32 mTotalCleaned;
    f32 mBrushAngle;
    s32 mTurnTimer;

    undefined mReserved[0x38];
};

using EnemyCleaner = Enm_Cleaner;

} // namespace Game
