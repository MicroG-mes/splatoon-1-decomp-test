#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class TakopterState : u32 {
    cPatrolHover   = 0,
    cNoticePlayer  = 1,
    cShootInk      = 2,
    cHitStagger    = 3,
    cDefeated      = 4
};

class Enm_Takopter : public GambitActor {
public:
    static constexpr f32 cMaxHp = 40.0f;
    static constexpr f32 cHoverAltitude = 3.2f;

    Enm_Takopter();
    virtual ~Enm_Takopter() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void updateAi(const sead::Vector3f& playerPos);
    void applyDamage(f32 damage, const sead::Vector3f& knockbackDir);

    TakopterState getState() const { return mState; }
    f32 getHp() const { return mHp; }
    f32 getPropellerSpeed() const { return mPropellerSpeed; }
    const sead::Vector3f& getPosition() const { return mPosition; }

    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    void fireInkShot(const sead::Vector3f& targetPos);

    sead::Vector3f mPosition;
    sead::Vector3f mVelocity;
    f32 mHp;
    f32 mPropellerSpeed;
    f32 mBobPhase;

    TakopterState mState;
    s32 mStateTimer;
    s32 mShotCooldown;

    undefined mReserved[0x38];
};

} // namespace Game
