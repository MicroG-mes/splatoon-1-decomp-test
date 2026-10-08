#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class RainmakerState : u32 {
    cShieldIdle     = 0,
    cShieldPopping  = 1,
    cFreeOnGround   = 2,
    cCarriedAlpha   = 3,
    cCarriedBravo   = 4,
    cGoalKnockout   = 5,
    cOutOfBoundsReset = 6
};

class GachihokoClone : public GambitActor {
public:
    GachihokoClone();
    virtual ~GachihokoClone() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void applyShieldDamage(u32 team, f32 damage);
    void pickUp(u32 team, u32 carrierPlayerId);
    void drop(const sead::Vector3f& dropPos);
    void updateCarrierDistance(f32 distanceToGoal);

    RainmakerState getState() const { return mState; }
    f32 getShieldHealth() const { return mShieldHealth; }
    f32 getShieldBias() const { return mShieldBias; } // -1.0 (Bravo) to +1.0 (Alpha)
    s32 getCarrierFuseSeconds() const { return mCarrierFuseFrames / 60; }

    f32 getBestDistanceAlpha() const { return mBestScoreAlpha; }
    f32 getBestDistanceBravo() const { return mBestScoreBravo; }
    bool isKnockout() const { return mState == RainmakerState::cGoalKnockout; }

protected:
    void explodeShield(u32 winningTeam);

    RainmakerState mState;
    s32 mStateTimer;

    f32 mShieldHealth;
    f32 mShieldBias;

    u32 mCarrierPlayerId;
    u32 mCarrierTeam;
    s32 mCarrierFuseFrames; // 60s * 60 = 3600 frames

    f32 mBestScoreAlpha;
    f32 mBestScoreBravo;

    sead::Vector3f mPosition;
    undefined mReserved[0x38];
};

} // namespace Game
