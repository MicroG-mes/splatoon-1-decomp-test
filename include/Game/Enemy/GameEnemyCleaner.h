#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include "sead/math/seadMatrix.h"

namespace Game {

enum class EnmCleanerState : u32 {
    cVacuumPatrol = 0,
    cTargetInk    = 1,
    cSpinStunned  = 5, // 0x05 in FUN_022d9670 line 44
    cDestroyed    = 6
};

/**
 * GameEnemyCleaner / Enm_Cleaner (Squee-G)
 * Address: vtable @ 0x10067AF4
 * Authentic Nintendo path: D:/home/Cafe/Gambit/App/Program/Game/Enemy/GameEnemyCleaner.cpp
 * Automatic ink absorption vacuum robot.
 *
 * Real PowerPC methods:
 *   vfunc_1  @ 0x022dafb8 - Actor destruction & effect cleanup
 *   vfunc_3  @ 0x022d4ce0 - Initialization of vacuum brush and navigation nodes
 *   vfunc_5  @ 0x022d5168 - Reset position and heading
 *   vfunc_7  @ 0x022d9670 - AI tick, ink query, dirty check, movement integration
 *   vfunc_11 @ 0x022d97fc - 3D transform matrix update & player ink vacuum suction
 *   vfunc_47 @ 0x022daff4 - Render vacuum chassis & wiper brushes
 *   vfunc_62 @ 0x022d9b2c - Damage handling & rear weakpoint test
 */
class GameEnemyCleaner : public GambitActor {
public:
    static constexpr f32 cMaxHealth = 80.0f;
    static constexpr f32 cVacuumRadius = 2.2f;
    static constexpr f32 cNormalSpeed = 0.08f;
    static constexpr f32 cRushSpeed = 0.16f;

    GameEnemyCleaner();
    virtual ~GameEnemyCleaner() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic PPC methods
    virtual void vfunc_1();
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_11();
    virtual void vfunc_47();
    virtual void vfunc_62(f32 damage, bool isRearHit);

    void spawn(const sead::Vector3f& spawnPos);
    void updateNavigation(const sead::Vector3f& targetInkPos, bool foundInk);

    EnmCleanerState getState() const { return mState; }
    f32 getHealth() const { return mHealth; }
    bool isAlive() const { return mIsAlive; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    const sead::Vector3f& getVelocity() const { return mVelocity; }

protected:
    void vacuumInkAtCurrentPos();

    sead::Vector3f mPosition;
    sead::Vector3f mVelocity;
    sead::Vector3f mHeading;

    f32 mHealth;
    f32 mScale;              // 0x17C
    bool mIsAlive;

    EnmCleanerState mState;   // 0x1D0
    s32 mStateTimer;
    s32 mCleanTimer;         // 0x2B0

    // 4x3 transform matrix (0x140 / 0x208)
    sead::Matrix34f mTransformMatrix;
};

} // namespace Game
