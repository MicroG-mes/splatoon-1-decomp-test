#include "Game/MapObj/Obj_AirBall.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>

namespace Game {

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Obj_AirBall::Obj_AirBall(AirBallType type)
    : mType(type)
{
    switch (mType) {
    case AirBallType::cType_Mission:
        mName = "Obj_AirBallMsn";
        break;
    case AirBallType::cType_Duel:
        mName = "Obj_AirBallDuel";
        break;
    case AirBallType::cType_StaffRoll:
        mName = "Obj_AirBall";
        break;
    }
}

void Obj_AirBall::init() {
    mHealth = mParams.mMaxHp;
    mFrameCounter = 0;
    mBobbingY = 0.0f;
    mBobbingVelocity = 0.0f;
    mStateTimer = 0;
    mTimeRemaining = mParams.mSequenceTimeLimit;
    mChainCompleted = false;

    // First balloon in sequence (step 0) or Duel balloons start appearing immediately
    if (mStepIndex == 0 || mType == AirBallType::cType_Duel) {
        triggerAppear();
    } else {
        mState = AirBallState::cState_Hidden;
        mScale = 0.0f;
    }
}

bool Obj_AirBall::loadParams(const char* filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    // Read raw parameter buffer if present
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    if (size > 0) {
        std::vector<char> buffer(static_cast<size_t>(size));
        file.read(buffer.data(), size);
        return true;
    }
    return false;
}

void Obj_AirBall::setSequence(int seqId, int stepIndex, int maxSteps, int timeLimit) {
    mSequenceId = seqId;
    mStepIndex = stepIndex;
    mMaxSteps = maxSteps > 0 ? maxSteps : 1;
    mParams.mSequenceTimeLimit = timeLimit;
    mTimeRemaining = timeLimit;

    if (mStepIndex == 0) {
        triggerAppear();
    } else {
        mState = AirBallState::cState_Hidden;
        mScale = 0.0f;
    }
}

void Obj_AirBall::triggerAppear() {
    mState = AirBallState::cState_Appear;
    mStateTimer = 0;
    mScale = 0.0f;
    mHealth = mParams.mMaxHp;
    mTimeRemaining = mParams.mSequenceTimeLimit;
}

void Obj_AirBall::update() {
    switch (mState) {
    case AirBallState::cState_Hidden:
        mScale = 0.0f;
        break;

    case AirBallState::cState_Appear: {
        mStateTimer++;
        float progress = static_cast<float>(mStateTimer) / static_cast<float>(mParams.mAppearFrames);
        if (progress > 1.0f) progress = 1.0f;
        // Smooth popping growth curve with slight overshoot (squash/stretch)
        mScale = std::sin(progress * static_cast<float>(M_PI) * 0.5f);
        if (mStateTimer >= mParams.mAppearFrames) {
            mScale = 1.0f;
            mState = AirBallState::cState_Wait;
            mStateTimer = 0;
        }
        break;
    }

    case AirBallState::cState_Wait: {
        mScale = 1.0f;
        mFrameCounter = (mFrameCounter + 1) % mParams.mPeriodFrames;

        // Authentic Splatoon floating bobbing: harmonic sinusoidal wave
        float angle = static_cast<float>(mFrameCounter) * (2.0f * static_cast<float>(M_PI) / static_cast<float>(mParams.mPeriodFrames));
        float targetY = std::sin(angle) * mParams.mBobbingAmplitude;

        // Damped harmonic oscillator tracking
        float displacement = targetY - mBobbingY;
        float springForce = displacement * mParams.mSpringKp;
        float dampingForce = -mBobbingVelocity * mParams.mSpringKd;
        mBobbingVelocity += springForce + dampingForce;
        mBobbingY += mBobbingVelocity;

        // Sequence countdown in mission mode
        if (mType == AirBallType::cType_Mission && mTimeRemaining > 0) {
            mTimeRemaining--;
            if (mTimeRemaining == 0) {
                mState = AirBallState::cState_Timeout;
                mScale = 0.0f;
            }
        }
        break;
    }

    case AirBallState::cState_DamageShot: {
        mStateTimer++;
        // Wobble squish response during ink damage
        float damageProgress = static_cast<float>(mStateTimer) / static_cast<float>(mParams.mDamageShotFrames);
        mScale = 1.0f + 0.2f * std::sin(damageProgress * static_cast<float>(M_PI) * 2.0f);
        if (mStateTimer >= mParams.mDamageShotFrames) {
            mScale = 1.0f;
            mState = AirBallState::cState_Wait;
            mStateTimer = 0;
        }
        break;
    }

    case AirBallState::cState_Burst: {
        // Already popped, stays in burst state for particle & event processing
        mScale = 0.0f;
        break;
    }

    case AirBallState::cState_Timeout: {
        mScale = 0.0f;
        break;
    }
    }
}

bool Obj_AirBall::applyDamage(float damage, u32 teamId) {
    if (mState != AirBallState::cState_Wait && mState != AirBallState::cState_DamageShot) {
        return false;
    }

    mLastHitterTeam = teamId;
    mHealth -= damage;

    if (mHealth <= 0.0f) {
        mHealth = 0.0f;
        mState = AirBallState::cState_Burst;
        mScale = 0.0f;

        // Trigger next balloon in sequence chain
        if (mLinkedNext) {
            mLinkedNext->triggerAppear();
        } else if (mStepIndex == mMaxSteps - 1) {
            mChainCompleted = true;
        }
        return true;
    }

    mState = AirBallState::cState_DamageShot;
    mStateTimer = 0;
    return true;
}

bool Obj_AirBall::checkHit(const sead::Vector3f& targetPos, float targetRadius) const {
    if (mState != AirBallState::cState_Wait && mState != AirBallState::cState_DamageShot) {
        return false;
    }

    sead::Vector3f ballPos = mPosition;
    ballPos.y += mBobbingY;

    float dx = targetPos.x - ballPos.x;
    float dy = targetPos.y - ballPos.y;
    float dz = targetPos.z - ballPos.z;
    float distSq = dx * dx + dy * dy + dz * dz;

    float combinedRadius = mParams.mCollisionRadius + targetRadius;
    return distSq <= (combinedRadius * combinedRadius);
}

} // namespace Game
