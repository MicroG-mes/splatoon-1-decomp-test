#include "Game/MapObj/Obj_LiftFall.h"
#include "Game/System/AglParameter.h"

namespace Game {

bool LiftFallParams::load(const char* paramsPath) {
    lifeSeconds = 3.00000000f;

    if (paramsPath) {
        AglParameterObj obj;
        if (obj.loadFromFile(paramsPath)) {
            lifeSeconds = obj.getFloat("mLife", lifeSeconds);
        }
    }

    return true;
}

Obj_LiftFall::Obj_LiftFall()
    : mState(LiftFallState::cState_Idle)
    , mRemainingFrames(180)
    , mRespawnTimer(0)
    , mFallDisplacementY(0.0f)
    , mFallVelocityY(0.0f)
    , mShakeOffsetX(0.0f)
    , mIsPlayerOn(false) {
    mPosition.set(0.0f, 0.0f, 0.0f);
    mParams.load("content/Static/Obj_LiftFall.params");
    mRemainingFrames = static_cast<s32>(mParams.lifeSeconds * cFps);
}

Obj_LiftFall::~Obj_LiftFall() {}

void Obj_LiftFall::init() {
    init(mPosition);
}

void Obj_LiftFall::init(const sead::Vector3f& pos) {
    mPosition = pos;
    mState = LiftFallState::cState_Idle;
    mRespawnTimer = 0;
    mFallDisplacementY = 0.0f;
    mFallVelocityY = 0.0f;
    mShakeOffsetX = 0.0f;
    mIsPlayerOn = false;
    mParams.load("content/Static/Obj_LiftFall.params");
    mRemainingFrames = static_cast<s32>(mParams.lifeSeconds * cFps);
}

void Obj_LiftFall::onPlayerStepOn() {
    mIsPlayerOn = true;
    if (mState == LiftFallState::cState_Idle) {
        mState = LiftFallState::cState_SteppedOn;
    }
}

void Obj_LiftFall::onPlayerStepOff() {
    mIsPlayerOn = false;
}

void Obj_LiftFall::update() {
    switch (mState) {
        case LiftFallState::cState_Idle:
            mShakeOffsetX = 0.0f;
            break;

        case LiftFallState::cState_SteppedOn:
        case LiftFallState::cState_Shaking:
            mRemainingFrames--;
            if (mRemainingFrames <= 60 && mRemainingFrames > 0) {
                mState = LiftFallState::cState_Shaking;
                mShakeOffsetX = (mRemainingFrames % 2 == 0) ? 0.08f : -0.08f;
            } else if (mRemainingFrames <= 0) {
                mState = LiftFallState::cState_Falling;
                mFallVelocityY = 0.0f;
                mFallDisplacementY = 0.0f;
                mShakeOffsetX = 0.0f;
            }
            break;

        case LiftFallState::cState_Falling:
            mFallVelocityY -= 0.5f; // Gravity
            mFallDisplacementY += mFallVelocityY;
            if (mFallDisplacementY < -50.0f) {
                mState = LiftFallState::cState_Respawning;
                mRespawnTimer = cRespawnDurationFrames;
            }
            break;

        case LiftFallState::cState_Respawning:
            mRespawnTimer--;
            if (mRespawnTimer <= 0) {
                mState = LiftFallState::cState_Idle;
                mRemainingFrames = static_cast<s32>(mParams.lifeSeconds * cFps);
                mFallDisplacementY = 0.0f;
                mFallVelocityY = 0.0f;
                mShakeOffsetX = 0.0f;
            }
            break;
    }
}

} // namespace Game
