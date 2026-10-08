#include "Game/MapObj/Obj_PaintingLift.h"

namespace Game {

Obj_PaintingLift::Obj_PaintingLift()
    : mBasePos(0.0f, 0.0f, 0.0f),
      mMaxElevation(10.0f),
      mCurrentHeight(0.0f),
      mFanAngularVelocity(0.0f),
      mFanAngle(0.0f) {
}

Obj_PaintingLift::~Obj_PaintingLift() {
}

void Obj_PaintingLift::init() {
    GambitActor::init();
    mBasePos.set(0.0f, 0.0f, 0.0f);
    mMaxElevation = 10.0f;
    mCurrentHeight = 0.0f;
    mFanAngularVelocity = 0.0f;
    mFanAngle = 0.0f;
}

void Obj_PaintingLift::setupLift(const sead::Vector3f& basePos, f32 maxElevation) {
    mBasePos = basePos;
    mMaxElevation = maxElevation;
    mCurrentHeight = 0.0f;
    mFanAngularVelocity = 0.0f;
    mFanAngle = 0.0f;
}

void Obj_PaintingLift::applyInkToPropeller(f32 inkPower) {
    // Ink jets spin the propeller fan
    mFanAngularVelocity += inkPower * 0.15f;
    if (mFanAngularVelocity > 1.2f) {
        mFanAngularVelocity = 1.2f;
    }
}

void Obj_PaintingLift::update() {
    GambitActor::update();

    mFanAngle += mFanAngularVelocity;

    if (mFanAngularVelocity > 0.01f) {
        // Platform rises proportional to fan rotation
        mCurrentHeight += mFanAngularVelocity * 0.12f;
        if (mCurrentHeight > mMaxElevation) {
            mCurrentHeight = mMaxElevation;
        }

        // Air friction dampens fan
        mFanAngularVelocity *= 0.94f;
    } else {
        mFanAngularVelocity = 0.0f;

        // Gravity slowly lowers platform back to base
        if (mCurrentHeight > 0.0f) {
            mCurrentHeight -= 0.04f;
            if (mCurrentHeight < 0.0f) {
                mCurrentHeight = 0.0f;
            }
        }
    }
}

void Obj_PaintingLift::draw() {
    GambitActor::draw();
}

} // namespace Game
