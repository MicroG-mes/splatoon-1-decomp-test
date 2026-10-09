#include "Game/MapObj/Obj_PaintLiftTurn.h"
#include <cmath>
#include <algorithm>

namespace Game {

Obj_PaintLiftTurn::Obj_PaintLiftTurn()
    : mPosition(0.0f, 0.0f, 0.0f)
    , mState(TurnLiftState::cState_Idle)
    , mCurrentAngle(0.0f)
    , mAngularVelocity(0.0f)
    , mRotSpeed(cDefaultRotSpeed)
    , mPauseTimer(0)
{
    std::fill(mReserved, mReserved + sizeof(mReserved), 0);
}

Obj_PaintLiftTurn::~Obj_PaintLiftTurn() {
}

void Obj_PaintLiftTurn::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mState = TurnLiftState::cState_Idle;
    mCurrentAngle = 0.0f;
    mAngularVelocity = 0.0f;
    mRotSpeed = cDefaultRotSpeed;
    mPauseTimer = 0;
}

void Obj_PaintLiftTurn::vfunc_3() {
    // 0x0258fef4: Model & paint canvas binding (Lft_TurnLift00.szs, 1,468 vertices)
}

void Obj_PaintLiftTurn::vfunc_5() {
    // 0x02590054: Rotary motor setup
    mRotSpeed = cDefaultRotSpeed;
}

void Obj_PaintLiftTurn::vfunc_7() {
    // 0x0259119c: Rotation physics update
    update();
}

void Obj_PaintLiftTurn::vfunc_11() {
    // 0x02591320: Ink torque reception
    applyInkTorque(2.0f);
}

void Obj_PaintLiftTurn::vfunc_30() {
    // 0x02591368: Detent alignment
    mCurrentAngle = std::round(mCurrentAngle / 90.0f) * 90.0f;
}

void Obj_PaintLiftTurn::applyInkTorque(f32 torque) {
    mAngularVelocity += torque;
    if (mAngularVelocity > 8.0f) {
        mAngularVelocity = 8.0f;
    }
    mState = TurnLiftState::cState_Rotating;
}

bool Obj_PaintLiftTurn::checkPassenger(const sead::Vector3f& playerPos) const {
    f32 dx = playerPos.x - mPosition.x;
    f32 dz = playerPos.z - mPosition.z;
    f32 distSq = dx * dx + dz * dz;

    // Must be on the disc platform within radius and near deck elevation
    return (distSq <= cPlatformRadius * cPlatformRadius) &&
           (playerPos.y >= mPosition.y && playerPos.y <= mPosition.y + 2.5f);
}

void Obj_PaintLiftTurn::update() {
    if (mState == TurnLiftState::cState_Rotating) {
        mCurrentAngle += mAngularVelocity;
        while (mCurrentAngle >= 360.0f) {
            mCurrentAngle -= 360.0f;
        }
        while (mCurrentAngle < 0.0f) {
            mCurrentAngle += 360.0f;
        }

        // Friction dampening
        mAngularVelocity *= 0.96f;
        if (mAngularVelocity < 0.05f) {
            mAngularVelocity = 0.0f;
            vfunc_30(); // Snap to closest detent
            mState = TurnLiftState::cState_DetentPause;
            mPauseTimer = 30; // 30-frame pause
        }
    } else if (mState == TurnLiftState::cState_DetentPause) {
        if (--mPauseTimer <= 0) {
            mState = TurnLiftState::cState_Idle;
        }
    }
}

void Obj_PaintLiftTurn::draw() {
    // Model rendered via ModelSceneMgr
}

} // namespace Game
