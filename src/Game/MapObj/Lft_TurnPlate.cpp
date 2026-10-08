#include "Game/MapObj/Lft_TurnPlate.h"
#include <cmath>

namespace Game {

Lft_TurnPlate::Lft_TurnPlate()
    : mCenterPos(0.0f, 0.0f, 0.0f),
      mRadius(cDefaultRadius),
      mCurrentAngleRad(0.0f),
      mAngularSpeedRad(cDefaultAngularSpeed),
      mState(TurnPlateState::cRotatingClockwise),
      mStateTimer(0) {
}

Lft_TurnPlate::~Lft_TurnPlate() {
}

void Lft_TurnPlate::init() {
    GambitActor::init();
    mCenterPos.set(0.0f, 0.0f, 0.0f);
    mRadius = cDefaultRadius;
    mCurrentAngleRad = 0.0f;
    mAngularSpeedRad = cDefaultAngularSpeed;
    mState = TurnPlateState::cRotatingClockwise;
    mStateTimer = 0;
}

void Lft_TurnPlate::setupTurntable(const sead::Vector3f& centerPos, f32 radius, f32 angularSpeedRad) {
    mCenterPos = centerPos;
    mRadius = radius;
    mAngularSpeedRad = angularSpeedRad;
    mCurrentAngleRad = 0.0f;
}

bool Lft_TurnPlate::isRiderOnPlate(const sead::Vector3f& testPos) const {
    f32 dx = testPos.x - mCenterPos.x;
    f32 dz = testPos.z - mCenterPos.z;
    f32 distSq = dx * dx + dz * dz;

    if (distSq <= mRadius * mRadius) {
        f32 dy = std::abs(testPos.y - mCenterPos.y);
        return dy <= 0.8f; // Within surface contact plane
    }

    return false;
}

sead::Vector3f Lft_TurnPlate::calculateRiderVelocity(const sead::Vector3f& riderPos) const {
    if (!isRiderOnPlate(riderPos) || mState == TurnPlateState::cPaused) {
        return sead::Vector3f(0.0f, 0.0f, 0.0f);
    }

    f32 rx = riderPos.x - mCenterPos.x;
    f32 rz = riderPos.z - mCenterPos.z;

    f32 omega = (mState == TurnPlateState::cRotatingClockwise) ? mAngularSpeedRad : -mAngularSpeedRad;

    // Tangential linear velocity: v = (-omega * rz, 0, omega * rx)
    return sead::Vector3f(-omega * rz, 0.0f, omega * rx);
}

void Lft_TurnPlate::stepRotation() {
    f32 omega = (mState == TurnPlateState::cRotatingClockwise) ? mAngularSpeedRad : -mAngularSpeedRad;
    mCurrentAngleRad += omega;

    if (mCurrentAngleRad > 6.283185f) {
        mCurrentAngleRad -= 6.283185f;
    } else if (mCurrentAngleRad < 0.0f) {
        mCurrentAngleRad += 6.283185f;
    }
}

void Lft_TurnPlate::update() {
    mStateTimer++;
    if (mState != TurnPlateState::cPaused) {
        stepRotation();
    }
}

void Lft_TurnPlate::draw() {
    GambitActor::draw();
}

} // namespace Game
