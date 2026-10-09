#include "Game/MapObj/Lft_HeavyCraneMachine.h"
#include <cmath>
#include <algorithm>

namespace Game {

Lft_HeavyCraneMachine::Lft_HeavyCraneMachine()
    : mState(HeavyCraneState::cIdle),
      mCurrentAngle(0.0f),
      mTargetAngle(0.0f),
      mRotateSpeed(12.0f),
      mTrolleyDist(15.0f),
      mTargetTrolleyDist(15.0f),
      mTrolleySpeed(2.5f),
      mHoistHeight(10.0f),
      mTargetHoistHeight(10.0f),
      mHoistSpeed(1.8f),
      mDefaultAnimFrame(0.0f),
      mEnterBreak0Frames(5.0f),
      mEnterBreak1Frames(5.0f),
      mEnterMoveFrames(10.0f),
      mBasePosition(0.0f, 0.0f, 0.0f) {
}

Lft_HeavyCraneMachine::~Lft_HeavyCraneMachine() {
}

void Lft_HeavyCraneMachine::init() {
    GambitActor::init();
    vfunc_3();
    vfunc_5();
}

void Lft_HeavyCraneMachine::vfunc_3() {
    mModel = sead::BfresParser::createHeavyCraneMachineModel("Lft_HeavyCraneMachine");
}

void Lft_HeavyCraneMachine::vfunc_5() {
    mState = HeavyCraneState::cIdle;
    mCurrentAngle = 0.0f;
    mTargetAngle = 0.0f;
    mTrolleyDist = 15.0f;
    mTargetTrolleyDist = 15.0f;
    mHoistHeight = 10.0f;
    mTargetHoistHeight = 10.0f;
}

void Lft_HeavyCraneMachine::vfunc_7() {
    const f32 kDt = 1.0f / 60.0f;
    bool isMoving = false;

    // 1. Slew rotation
    if (std::abs(mTargetAngle - mCurrentAngle) > 0.05f) {
        f32 dir = (mTargetAngle > mCurrentAngle) ? 1.0f : -1.0f;
        mCurrentAngle += dir * mRotateSpeed * kDt;
        if ((dir > 0.0f && mCurrentAngle > mTargetAngle) || (dir < 0.0f && mCurrentAngle < mTargetAngle)) {
            mCurrentAngle = mTargetAngle;
        }
        isMoving = true;
    }

    // 2. Trolley radial translation along horizontal jib
    if (std::abs(mTargetTrolleyDist - mTrolleyDist) > 0.02f) {
        f32 dir = (mTargetTrolleyDist > mTrolleyDist) ? 1.0f : -1.0f;
        mTrolleyDist += dir * mTrolleySpeed * kDt;
        if ((dir > 0.0f && mTrolleyDist > mTargetTrolleyDist) || (dir < 0.0f && mTrolleyDist < mTargetTrolleyDist)) {
            mTrolleyDist = mTargetTrolleyDist;
        }
        isMoving = true;
    }

    // 3. Hoist cable vertical elevation
    if (std::abs(mTargetHoistHeight - mHoistHeight) > 0.02f) {
        f32 dir = (mTargetHoistHeight > mHoistHeight) ? 1.0f : -1.0f;
        mHoistHeight += dir * mHoistSpeed * kDt;
        if ((dir > 0.0f && mHoistHeight > mTargetHoistHeight) || (dir < 0.0f && mHoistHeight < mTargetHoistHeight)) {
            mHoistHeight = mTargetHoistHeight;
        }
        isMoving = true;
    }

    if (isMoving) {
        mState = HeavyCraneState::cRotating;
    } else {
        mState = HeavyCraneState::cIdle;
    }
}

void Lft_HeavyCraneMachine::update() {
    vfunc_7();
}

void Lft_HeavyCraneMachine::draw() {
}

void Lft_HeavyCraneMachine::setTargetAngle(f32 angleDeg) {
    mTargetAngle = angleDeg;
}

void Lft_HeavyCraneMachine::setTargetTrolleyDist(f32 distMeters) {
    mTargetTrolleyDist = std::clamp(distMeters, 5.0f, 40.0f);
}

void Lft_HeavyCraneMachine::setTargetHoistHeight(f32 heightMeters) {
    mTargetHoistHeight = std::clamp(heightMeters, 0.0f, 25.0f);
}

sead::Vector3f Lft_HeavyCraneMachine::getPlatformWorldPos() const {
    f32 rad = mCurrentAngle * 0.017453292f;
    f32 posX = mBasePosition.x + mTrolleyDist * std::sin(rad);
    f32 posZ = mBasePosition.z + mTrolleyDist * std::cos(rad);
    f32 posY = mBasePosition.y + mHoistHeight;
    return sead::Vector3f(posX, posY, posZ);
}

} // namespace Game
