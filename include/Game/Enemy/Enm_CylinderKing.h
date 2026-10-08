#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "Game/Enemy/Obj_CylinderKingHole.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class CylinderKingPhase : u32 {
    cPhase1 = 1,
    cPhase2 = 2,
    cPhase3 = 3
};

enum class CylinderKingState : u32 {
    cRotatingBarrage = 0,
    cTopExposed      = 1,
    cPhaseTransition = 2,
    cDefeated        = 3
};

class Enm_CylinderKing : public GambitActor {
public:
    static constexpr u32 cTotalHoles = 6;
    static constexpr f32 cCylinderRadius = 4.0f;
    static constexpr f32 cCylinderHeight = 9.0f;

    Enm_CylinderKing();
    virtual ~Enm_CylinderKing() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void updateBossAi(const sead::Vector3f& playerPos);
    void applyTentacleDamage(f32 damage);

    CylinderKingState getState() const { return mState; }
    CylinderKingPhase getPhase() const { return mPhase; }
    f32 getTentacleHp() const { return mTentacleHp; }
    f32 getRotationAngle() const { return mRotationAngle; }
    bool isTopExposed() const { return mState == CylinderKingState::cTopExposed; }
    const sead::Vector3f& getPosition() const { return mPosition; }

    Obj_CylinderKingHole* getHole(u32 index) {
        if (index < cTotalHoles) {
            return &mHoles[index];
        }
        return nullptr;
    }

protected:
    void setupPhaseConfig();
    u32 countInkedHoles() const;

    CylinderKingState mState;
    CylinderKingPhase mPhase;
    s32 mStateTimer;

    sead::Vector3f mPosition;
    f32 mRotationAngle;
    f32 mRotationSpeed;
    f32 mTentacleHp;

    Obj_CylinderKingHole mHoles[cTotalHoles];

    undefined mReserved[0x38];
};

} // namespace Game
