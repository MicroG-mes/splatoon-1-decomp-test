#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class OctosniperState : u32 {
    cIdleScan  = 0,
    cLockLaser = 1,
    cFireSnipe = 2,
    cCooldown  = 3,
    cSplatted  = 4
};

class EnemyHimo : public GambitActor {
public:
    EnemyHimo();
    virtual ~EnemyHimo() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void applyDamage(f32 damage);
    void updateAi(const sead::Vector3f& playerPos);

    OctosniperState getState() const { return mState; }
    f32 getRemainingHp() const { return mCurrentHp; }
    bool isLaserActive() const { return mState == OctosniperState::cLockLaser; }

protected:
    void fireSniperBeam();

    OctosniperState mState;
    s32 mStateTimer;

    sead::Vector3f mPosition;
    sead::Vector3f mTargetPos;
    f32 mMaxHp;
    f32 mCurrentHp;

    undefined mReserved[0x38];
};

} // namespace Game
