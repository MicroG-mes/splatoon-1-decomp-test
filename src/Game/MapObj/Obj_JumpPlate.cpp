#include "Game/MapObj/Obj_JumpPlate.h"

namespace Game {

Obj_JumpPlate::Obj_JumpPlate()
    : mState(JumpPlateState::cIdle),
      mStateTimer(0),
      mPosition(0.0f, 0.0f, 0.0f),
      mDestinationPos(0.0f, 0.0f, 0.0f),
      mRiderPlayerId(0) {
}

Obj_JumpPlate::~Obj_JumpPlate() {
}

void Obj_JumpPlate::init() {
    GambitActor::init();
    mState = JumpPlateState::cIdle;
    mStateTimer = 0;
    mPosition.set(0.0f, 0.0f, 0.0f);
    mDestinationPos.set(0.0f, 0.0f, 0.0f);
    mRiderPlayerId = 0;
}

void Obj_JumpPlate::setupPad(const sead::Vector3f& padPos, const sead::Vector3f& destPos) {
    mPosition = padPos;
    mDestinationPos = destPos;
    mState = JumpPlateState::cIdle;
    mStateTimer = 0;
}

// Matches Obj_JumpPlate__vfunc_54 @ 0x0255F8E0
bool Obj_JumpPlate::stepOnPad(u32 playerId) {
    if (mState != JumpPlateState::cIdle) {
        return false;
    }

    mRiderPlayerId = playerId;
    mState = JumpPlateState::cCoiling; // Sets spring compression flag
    mStateTimer = 0;
    return true;
}

sead::Vector3f Obj_JumpPlate::computeBallisticVelocity(f32 arcHeight, s32 flightFrames) const {
    f32 t = static_cast<f32>(flightFrames);
    if (t < 1.0f) t = 1.0f;

    f32 vx = (mDestinationPos.x - mPosition.x) / t;
    f32 vz = (mDestinationPos.z - mPosition.z) / t;

    // Standard parabolic trajectory with gravity = 0.038f
    f32 gravity = 0.038f;
    f32 vy = ((mDestinationPos.y - mPosition.y) + (0.5f * gravity * t * t)) / t;

    return sead::Vector3f(vx, vy, vz);
}

void Obj_JumpPlate::update() {
    GambitActor::update();

    mStateTimer++;

    switch (mState) {
        case JumpPlateState::cCoiling:
            if (mStateTimer >= 15) { // 15 frames coil squat
                mState = JumpPlateState::cLaunch;
                mStateTimer = 0;
            }
            break;

        case JumpPlateState::cLaunch:
            // Spring extends launching player
            mState = JumpPlateState::cCooldown;
            mStateTimer = 0;
            break;

        case JumpPlateState::cCooldown:
            if (mStateTimer >= 30) {
                mState = JumpPlateState::cIdle;
                mStateTimer = 0;
            }
            break;

        case JumpPlateState::cIdle:
        default:
            break;
    }
}

void Obj_JumpPlate::draw() {
    GambitActor::draw();
}

} // namespace Game
