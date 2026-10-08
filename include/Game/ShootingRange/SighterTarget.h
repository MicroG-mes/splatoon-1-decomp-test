#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class TargetState : u32 {
    cWait        = 0,
    cHitRecoil   = 1,
    cPopped      = 2,
    cRegenerating = 3
};

class SighterTarget : public GambitActor {
public:
    SighterTarget();
    virtual ~SighterTarget() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setup(const sead::Vector3f& pos, f32 maxHp, u32 defenseLevel, bool isMoving);
    void applyDamage(f32 damage);

    TargetState getState() const { return mState; }
    f32 getRemainingHp() const { return mCurrentHp; }
    f32 getLastDamageTaken() const { return mLastDamage; }
    u32 getDefenseLevel() const { return mDefenseLevel; }

    const sead::Vector3f& getPosition() const { return mPosition; }

protected:
    void calculateEffectiveDamage(f32 rawDamage);

    TargetState mState;
    s32 mStateTimer;

    sead::Vector3f mPosition;
    sead::Vector3f mBasePosition;
    bool mIsMoving;
    f32 mMovePhase;

    f32 mMaxHp;
    f32 mCurrentHp;
    u32 mDefenseLevel; // 0 to 3 Defense Up stacks
    f32 mLastDamage;

    f32 mWobbleAngle;
    f32 mWobbleVelocity;

    undefined mReserved[0x38];
};

} // namespace Game
