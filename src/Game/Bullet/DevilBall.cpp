#include "Game/Bullet/DevilBall.h"

namespace Game {

DevilBall::DevilBall()
    : mState(DevilBallState::cFinished)
    , mPosition(0.0f, 0.0f, 0.0f)
    , mVelocity(0.0f, 0.0f, 0.0f)
    , mTeamId(0)
    , mOwnerPlayerId(0)
    , mLifeFrames(0)
{
}

DevilBall::~DevilBall() {}

void DevilBall::init() {
    GambitActor::init();
    mState = DevilBallState::cFinished;
    mLifeFrames = 0;
}

void DevilBall::throwBall(const sead::Vector3f& startPos, const sead::Vector3f& initVel, u32 teamId, u32 ownerPlayerId) {
    mPosition = startPos;
    mVelocity = initVel;
    mTeamId = teamId;
    mOwnerPlayerId = ownerPlayerId;
    mState = DevilBallState::cAirborne;
    mLifeFrames = 180; // 3.0s maximum flight
}

void DevilBall::update() {
    if (mState == DevilBallState::cAirborne) {
        mVelocity.y -= cGravity;
        mPosition += mVelocity;

        mLifeFrames--;
        if (mLifeFrames <= 0) {
            detonate(mPosition);
        }
    } else if (mState == DevilBallState::cDetonated) {
        mState = DevilBallState::cFinished;
    }
}

void DevilBall::draw() {}

void DevilBall::detonate(const sead::Vector3f& impactPos) {
    mPosition = impactPos;
    mState = DevilBallState::cDetonated;
}

bool DevilBall::checkHitAndDebuff(const sead::Vector3f& targetPos, u32 targetTeam, DisruptedDebuff& targetDebuff, bool hasColdBlooded) {
    if (mState != DevilBallState::cDetonated) {
        return false;
    }
    // Cannot disrupt teammates
    if (targetTeam == mTeamId) {
        return false;
    }

    f32 dist = (targetPos - mPosition).length();
    if (dist <= cSplashRadius) {
        targetDebuff.apply(hasColdBlooded);
        return true;
    }
    return false;
}

} // namespace Game
