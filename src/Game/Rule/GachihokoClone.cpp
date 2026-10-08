#include "Game/Rule/GachihokoClone.h"

namespace Game {

GachihokoClone::GachihokoClone()
    : mState(RainmakerState::cShieldIdle),
      mStateTimer(0),
      mShieldHealth(1000.0f),
      mShieldBias(0.0f),
      mCarrierPlayerId(0),
      mCarrierTeam(0),
      mCarrierFuseFrames(60 * 60),
      mBestScoreAlpha(100.0f),
      mBestScoreBravo(100.0f),
      mPosition(0.0f, 0.0f, 0.0f) {
}

GachihokoClone::~GachihokoClone() {
}

void GachihokoClone::init() {
    GambitActor::init();
    mState = RainmakerState::cShieldIdle;
    mShieldHealth = 1000.0f;
    mShieldBias = 0.0f;
    mBestScoreAlpha = 100.0f;
    mBestScoreBravo = 100.0f;
}

void GachihokoClone::applyShieldDamage(u32 team, f32 damage) {
    if (mState != RainmakerState::cShieldIdle) {
        return;
    }

    if (team == 0) { // Alpha
        mShieldBias += damage * 0.001f;
        mShieldHealth -= damage;
    } else { // Bravo
        mShieldBias -= damage * 0.001f;
        mShieldHealth -= damage;
    }

    if (mShieldHealth <= 0.0f) {
        u32 winningTeam = (mShieldBias >= 0.0f) ? 0 : 1;
        explodeShield(winningTeam);
    }
}

void GachihokoClone::explodeShield(u32 winningTeam) {
    mState = RainmakerState::cShieldPopping;
    mStateTimer = 0;
    // Massive radial paint burst and damage in winning team color
}

void GachihokoClone::pickUp(u32 team, u32 carrierPlayerId) {
    mCarrierTeam = team;
    mCarrierPlayerId = carrierPlayerId;
    mCarrierFuseFrames = 60 * 60; // 60s
    mState = (team == 0) ? RainmakerState::cCarriedAlpha : RainmakerState::cCarriedBravo;
}

void GachihokoClone::drop(const sead::Vector3f& dropPos) {
    mPosition = dropPos;
    mState = RainmakerState::cFreeOnGround;
    mStateTimer = 0;
}

void GachihokoClone::updateCarrierDistance(f32 distanceToGoal) {
    if (mState == RainmakerState::cCarriedAlpha) {
        if (distanceToGoal < mBestScoreAlpha) {
            mBestScoreAlpha = distanceToGoal;
        }
        if (distanceToGoal <= 0.0f) {
            mBestScoreAlpha = 0.0f;
            mState = RainmakerState::cGoalKnockout;
        }
    } else if (mState == RainmakerState::cCarriedBravo) {
        if (distanceToGoal < mBestScoreBravo) {
            mBestScoreBravo = distanceToGoal;
        }
        if (distanceToGoal <= 0.0f) {
            mBestScoreBravo = 0.0f;
            mState = RainmakerState::cGoalKnockout;
        }
    }
}

void GachihokoClone::update() {
    mStateTimer++;

    if (mState == RainmakerState::cShieldPopping) {
        if (mStateTimer > 30) {
            mState = RainmakerState::cFreeOnGround;
            mStateTimer = 0;
        }
    } else if (mState == RainmakerState::cCarriedAlpha || mState == RainmakerState::cCarriedBravo) {
        if (mCarrierFuseFrames > 0) {
            mCarrierFuseFrames--;
            if (mCarrierFuseFrames == 0) {
                // Time up! Carrier detonates
                drop(mPosition);
            }
        }
    } else if (mState == RainmakerState::cFreeOnGround) {
        // If left on the ground for 900 frames (15s), shield regenerates
        if (mStateTimer > 900) {
            mState = RainmakerState::cShieldIdle;
            mShieldHealth = 1000.0f;
            mShieldBias = 0.0f;
        }
    }
}

void GachihokoClone::draw() {
    GambitActor::draw();
}

} // namespace Game
