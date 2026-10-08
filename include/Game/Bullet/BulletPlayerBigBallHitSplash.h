#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

// Decompiled from PPC: BulletPlayerBigBallHitSplash @ 0x0224A004, 0x0224A3E0
class BulletPlayerBigBallHitSplash : public GambitActor {
public:
    BulletPlayerBigBallHitSplash();
    virtual ~BulletPlayerBigBallHitSplash() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void launchSlosh(const sead::Vector3f& pos, const sead::Vector3f& initialVel, f32 maxRange, u32 teamId);

    bool isGrounded() const { return mIsGrounded; }
    f32 getTraveledDistance() const { return mTraveledDist; }
    f32 getMaxRange() const { return mMaxRange; }
    f32 getDamage() const { return mDamage; }
    u32 getTeamId() const { return mTeamId; }
    const sead::Vector3f& getPosition() const { return mPosition; }

private:
    sead::Vector3f mPosition;
    sead::Vector3f mVelocity;
    u32 mTeamId;
    f32 mDamage;
    f32 mMaxRange;
    f32 mTraveledDist;
    f32 mGravity;
    bool mIsGrounded;
};

} // namespace Game
