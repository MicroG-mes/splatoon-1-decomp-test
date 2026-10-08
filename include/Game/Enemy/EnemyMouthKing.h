#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "Game/Enemy/Enm_MouthKingTooth.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class OctomawPhase : u32 {
    cPhase1 = 1,
    cPhase2 = 2,
    cPhase3 = 3
};

enum class OctomawState : u32 {
    cSubmergedSwim   = 0,
    cBreachTarget    = 1,
    cLeapChomp       = 2,
    cSwallowBomb     = 3,
    cStunnedExposed  = 4,
    cSubmergeRecover = 5,
    cDefeated        = 6
};

class EnemyMouthKing : public GambitActor {
public:
    static constexpr u32 cToothCount = 8;

    EnemyMouthKing();
    virtual ~EnemyMouthKing() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void updateBossAi(const sead::Vector3f& playerPos);
    void applyBombToMouth(f32 bombDamage);
    void applyTentacleDamage(f32 damage);

    OctomawState getState() const { return mState; }
    void setState(OctomawState state) { mState = state; }
    OctomawPhase getPhase() const { return mPhase; }
    f32 getTentacleHp() const { return mTentacleHp; }
    bool isTentacleVulnerable() const { return mState == OctomawState::cStunnedExposed; }
    const sead::Vector3f& getPosition() const { return mPosition; }

    Enm_MouthKingTooth* getTooth(u32 index) {
        if (index < cToothCount) {
            return &mTeeth[index];
        }
        return nullptr;
    }

protected:
    void setupPhaseTeeth();
    u32 countBrokenTeeth() const;

    OctomawState mState;
    OctomawPhase mPhase;
    s32 mStateTimer;

    sead::Vector3f mPosition;
    sead::Vector3f mTargetPlayerPos;
    f32 mTentacleHp;

    Enm_MouthKingTooth mTeeth[cToothCount];

    undefined mReserved[0x38];
};

} // namespace Game
