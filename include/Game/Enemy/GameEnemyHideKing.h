#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class OctonozzlePhase : u32 {
    cPhase1 = 1,
    cPhase2 = 2,
    cPhase3 = 3
};

enum class OctonozzleState : u32 {
    cRotatingBarrage = 0,
    cTopExposed      = 1,
    cPhaseTransition = 2,
    cDefeated        = 3
};

/**
 * GameEnemyHideKing / Enm_CylinderKing (The Ravenous Octonozzle Boss)
 * Address: vtable @ 0x10072B28
 * Authentic Nintendo path: D:/home/Cafe/Gambit/App/Program/Game/Enemy/GameEnemyHideKing.cpp
 * 3-tier rotating cylinder cannon boss with ink suction holes and exposed top tentacle.
 *
 * Real PowerPC methods:
 *   vfunc_1  @ 0x0234a900 - Destruction & Zapfish reward release
 *   vfunc_3  @ 0x023485c0 - Cylinder hull geometry and hole nodes initialization
 *   vfunc_5  @ 0x02348c40 - Reset cylinder rotation
 *   vfunc_7  @ 0x02349e50 - AI update, cannon salvo, suction hole climbing check
 *   vfunc_11 @ 0x0234a180 - Tentacle hit reaction & phase progression
 */
class GameEnemyHideKing : public GambitActor {
public:
    static constexpr u32 cTotalNozzleHoles = 6;
    static constexpr f32 cCylinderRadius = 4.2f;
    static constexpr f32 cCylinderHeight = 9.5f;
    static constexpr f32 cTentacleMaxHp = 100.0f;

    GameEnemyHideKing();
    virtual ~GameEnemyHideKing() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    virtual void vfunc_1();
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_11(f32 damage);

    void setupBoss(const sead::Vector3f& centerPos);
    void updateAi(const sead::Vector3f& playerPos);
    void plugHoleWithInk(u32 holeIndex);

    OctonozzleState getState() const { return mState; }
    OctonozzlePhase getPhase() const { return mPhase; }
    f32 getTentacleHp() const { return mTentacleHp; }
    f32 getRotationAngle() const { return mRotationAngle; }
    bool isTopExposed() const { return mState == OctonozzleState::cTopExposed; }
    const sead::Vector3f& getPosition() const { return mPosition; }

protected:
    void advancePhase();

    sead::Vector3f mPosition;
    OctonozzleState mState;
    OctonozzlePhase mPhase;
    s32 mStateTimer;

    f32 mRotationAngle;
    f32 mRotationSpeed;
    f32 mTentacleHp;

    u8 mHolesPlugged[cTotalNozzleHoles];
    s32 mBarrageCooldown;
};

} // namespace Game
