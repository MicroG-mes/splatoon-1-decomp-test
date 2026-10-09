#include "Game/MapObj/Obj_SeesawLift.h"
#include <cmath>
#include <algorithm>

namespace Game {

Obj_SeesawLift::Obj_SeesawLift()
    : mPivotPosition(0.0f, 0.0f, 0.0f)
    , mTiltAngle(0.0f)
    , mAngularVelocity(0.0f)
    , mExternalTorque(0.0f)
    , mState(SeesawLiftState::cBalanced)
    , mOccupantCount(0)
{
    std::fill(mReserved, mReserved + sizeof(mReserved), 0);
}

Obj_SeesawLift::~Obj_SeesawLift() {
}

void Obj_SeesawLift::init() {
    GambitActor::init();
    mPivotPosition.set(0.0f, 0.0f, 0.0f);
    mTiltAngle = 0.0f;
    mAngularVelocity = 0.0f;
    mExternalTorque = 0.0f;
    mState = SeesawLiftState::cBalanced;
    mOccupantCount = 0;
}

void Obj_SeesawLift::vfunc_3() {
    // 0x025b8838: Model setup & collision initialization
}

void Obj_SeesawLift::vfunc_7() {
    // 0x025b915c / FUN_025b8e18: Dynamic angular simulation tick
    update();
}

void Obj_SeesawLift::vfunc_11() {
    // 0x025b91fc: Transform and collision matrix sync
}

void Obj_SeesawLift::vfunc_53() {
    // 0x025b92fc: Player weight impact event
}

void Obj_SeesawLift::spawn(const sead::Vector3f& pivotPos) {
    mPivotPosition = pivotPos;
    mTiltAngle = 0.0f;
    mAngularVelocity = 0.0f;
    mExternalTorque = 0.0f;
    mState = SeesawLiftState::cBalanced;
    mOccupantCount = 0;
}

void Obj_SeesawLift::applyWeightImpulse(f32 leverArmDistance, f32 weight) {
    // Clamp lever arm to seesaw bounds
    f32 clampedArm = std::clamp(leverArmDistance, -cArmHalfWidth, cArmHalfWidth);
    // Torque = r * F
    f32 torque = clampedArm * weight * cTorqueMultiplier;
    mExternalTorque += torque;
    mOccupantCount = (weight > 0.0f) ? 1 : 0;
    vfunc_53();
}

void Obj_SeesawLift::resetBalance() {
    mTiltAngle = 0.0f;
    mAngularVelocity = 0.0f;
    mExternalTorque = 0.0f;
    mState = SeesawLiftState::cBalanced;
    mOccupantCount = 0;
}

f32 Obj_SeesawLift::getTiltAngleDegrees() const {
    return mTiltAngle * (180.0f / 3.14159265358979323846f);
}

bool Obj_SeesawLift::isLevel() const {
    return std::abs(mTiltAngle) < 0.005f;
}

void Obj_SeesawLift::update() {
    // FUN_025b8e18: Angular dampening, limit clamping and restitution rebound
    mAngularVelocity += mExternalTorque;
    mExternalTorque = 0.0f;

    // If unoccupied, spring restoring torque gently levels the platform back out
    if (mOccupantCount == 0 && std::abs(mTiltAngle) > 0.0001f) {
        mAngularVelocity -= mTiltAngle * cRestoringSpringCoeff;
    }

    mAngularVelocity *= cAngularDamping;
    mTiltAngle += mAngularVelocity;

    // Maximum tilt limit stops with restitution bounce
    if (mTiltAngle > cMaxTiltRadians) {
        mTiltAngle = cMaxTiltRadians;
        if (mAngularVelocity > 0.0f) {
            mAngularVelocity = -mAngularVelocity * cRestitution;
        }
        mState = SeesawLiftState::cMaxTilt;
    } else if (mTiltAngle < -cMaxTiltRadians) {
        mTiltAngle = -cMaxTiltRadians;
        if (mAngularVelocity < 0.0f) {
            mAngularVelocity = -mAngularVelocity * cRestitution;
        }
        mState = SeesawLiftState::cMaxTilt;
    } else {
        if (std::abs(mTiltAngle) < 0.001f && std::abs(mAngularVelocity) < 0.0005f) {
            mState = SeesawLiftState::cBalanced;
        } else {
            mState = SeesawLiftState::cTilting;
        }
    }

    vfunc_11();
}

void Obj_SeesawLift::draw() {
    // Rendered via ModelSceneMgr
}

} // namespace Game
