#include "Game/MapObj/Obj_BridgeUpDown00.h"
#include <cmath>
#include <algorithm>

namespace Game {

Obj_BridgeUpDown00::Obj_BridgeUpDown00()
    : mPivotPosition(0.0f, 0.0f, 0.0f)
    , mYawAngle(0.0f)
    , mAngle(0.0f)
    , mState(BridgeUpDownState::cLowered)
    , mHoldTimer(0)
{
    std::fill(mReserved, mReserved + sizeof(mReserved), 0);
}

Obj_BridgeUpDown00::~Obj_BridgeUpDown00() {
}

void Obj_BridgeUpDown00::init() {
    GambitActor::init();
    mPivotPosition.set(0.0f, 0.0f, 0.0f);
    mYawAngle = 0.0f;
    mAngle = 0.0f;
    mState = BridgeUpDownState::cLowered;
    mHoldTimer = 0;
}

void Obj_BridgeUpDown00::vfunc_3() {
    // 0x100cbcfc: Model & KCL setup (Obj_BridgeUpDown00.szs, 2,624 vertices)
}

void Obj_BridgeUpDown00::vfunc_5() {
    // Parameter initialization
    mAngle = 0.0f;
    mState = BridgeUpDownState::cLowered;
    mHoldTimer = 0;
}

void Obj_BridgeUpDown00::vfunc_7() {
    // Motion tick
    update();
}

void Obj_BridgeUpDown00::vfunc_11() {
    // Switch trigger activation
    triggerRaise();
}

void Obj_BridgeUpDown00::spawn(const sead::Vector3f& pivotPos, f32 yawAngle) {
    mPivotPosition = pivotPos;
    mYawAngle = yawAngle;
    mAngle = 0.0f;
    mState = BridgeUpDownState::cLowered;
    mHoldTimer = 0;
}

void Obj_BridgeUpDown00::triggerRaise() {
    if (mState == BridgeUpDownState::cLowered || mState == BridgeUpDownState::cLowering) {
        mState = BridgeUpDownState::cRaising;
    }
}

void Obj_BridgeUpDown00::triggerLower() {
    if (mState == BridgeUpDownState::cRaised || mState == BridgeUpDownState::cRaising) {
        mState = BridgeUpDownState::cLowering;
    }
}

void Obj_BridgeUpDown00::update() {
    switch (mState) {
        case BridgeUpDownState::cLowered:
            break;

        case BridgeUpDownState::cRaising:
            mAngle += cRaiseSpeed;
            if (mAngle >= cMaxAngleDegrees) {
                mAngle = cMaxAngleDegrees;
                mState = BridgeUpDownState::cRaised;
                mHoldTimer = cHoldDuration;
            }
            break;

        case BridgeUpDownState::cRaised:
            if (--mHoldTimer <= 0) {
                mState = BridgeUpDownState::cLowering;
            }
            break;

        case BridgeUpDownState::cLowering:
            mAngle -= cLowerSpeed;
            if (mAngle <= 0.0f) {
                mAngle = 0.0f;
                mState = BridgeUpDownState::cLowered;
                mHoldTimer = 0;
            }
            break;
    }
}

void Obj_BridgeUpDown00::draw() {
    // Rendered via ModelSceneMgr
}

} // namespace Game
