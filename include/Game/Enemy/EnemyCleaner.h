#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class CleanerState : u32 {
    cPatrolVacuum  = 0,
    cCleaningSpree = 1,
    cSpinStunned   = 2,
    cDefeated      = 3
};

class EnemyCleaner : public GambitActor {
public:
    static constexpr f32 cMaxHp = 80.0f;
    static constexpr f32 cMoveSpeed = 0.065f;
    static constexpr f32 cVacuumRadius = 2.0f;

    EnemyCleaner();
    virtual ~EnemyCleaner() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void updateAi(const sead::Vector3f& playerInkNearbyPos, bool hasPlayerInkNearby);
    void applyDamage(f32 damage, bool isRearHit);

    CleanerState getState() const { return mState; }
    f32 getHp() const { return mHp; }
    bool isRearExposed() const { return mState == CleanerState::cSpinStunned || mState == CleanerState::cPatrolVacuum; }
    const sead::Vector3f& getPosition() const { return mPosition; }

    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    void vacuumInkFloor();

    sead::Vector3f mPosition;
    sead::Vector3f mMoveDirection;
    f32 mHp;
    CleanerState mState;
    s32 mStateTimer;

    undefined mReserved[0x38];
};

} // namespace Game
