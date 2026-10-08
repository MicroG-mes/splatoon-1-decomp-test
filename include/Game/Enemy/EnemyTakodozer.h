#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class OctodozerState : u32 {
    cChargeForward = 0,
    cTurnAround    = 1,
    cTentacleHit   = 2,
    cDestroyed     = 3
};

class EnemyTakodozer : public GambitActor {
public:
    EnemyTakodozer();
    virtual ~EnemyTakodozer() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void applyWeakpointDamage(f32 damage);
    void updatePatrol(f32 pathLength);

    OctodozerState getState() const { return mState; }
    f32 getTentacleHp() const { return mTentacleHp; }
    const sead::Vector3f& getPosition() const { return mPosition; }

protected:
    void paintDozerTrail();

    OctodozerState mState;
    s32 mStateTimer;

    sead::Vector3f mPosition;
    f32 mMoveDir; // +1.0 or -1.0
    f32 mSpeed;
    f32 mTentacleHp;

    undefined mReserved[0x38];
};

} // namespace Game
