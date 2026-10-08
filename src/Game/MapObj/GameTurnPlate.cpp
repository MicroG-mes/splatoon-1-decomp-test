#include "Game/MapObj/GameTurnPlate.h"
#include <cmath>

namespace Game {

GameTurnPlate::GameTurnPlate()
    : mCenter(0.0f, 0.0f, 0.0f),
      mRadius(cDefaultRadius),
      mCurrentAngle(0.0f),
      mAngularSpeed(cDefaultAngularSpeed),
      mRotationTimer(0) {
    mTransformMatrix.makeIdentity();
}

GameTurnPlate::~GameTurnPlate() = default;

void GameTurnPlate::init() {
    GambitActor::init();
    vfunc_3();
}

/**
 * Obj_TurnPlate__vfunc_3 @ 0x025e4ef8
 * Turntable platform setup and collision registration.
 */
void GameTurnPlate::vfunc_3() {
    mCurrentAngle = 0.0f;
    mRotationTimer = 0;
    mTransformMatrix.makeIdentity();
}

/**
 * Obj_TurnPlate__vfunc_5 @ 0x025e4fd0
 * Reset angle and matrix.
 */
void GameTurnPlate::vfunc_5() {
    mCurrentAngle = 0.0f;
    mRotationTimer = 0;
    mTransformMatrix.makeIdentity();
}

/**
 * Obj_TurnPlate__vfunc_7 @ 0x025e5bf0
 * Rotation update step: integrates angle, builds rotation matrix around Y,
 * and updates local-to-world transform.
 */
void GameTurnPlate::vfunc_7() {
    mCurrentAngle += mAngularSpeed;
    if (mCurrentAngle > 6.2831853f) {
        mCurrentAngle -= 6.2831853f;
    } else if (mCurrentAngle < -6.2831853f) {
        mCurrentAngle += 6.2831853f;
    }

    if (mRotationTimer > 0) {
        mRotationTimer--;
    }

    // Construct 4x3 rotation matrix around Y axis at mCenter
    f32 cosA = std::cos(mCurrentAngle);
    f32 sinA = std::sin(mCurrentAngle);

    mTransformMatrix.m[0][0] = cosA;
    mTransformMatrix.m[0][1] = 0.0f;
    mTransformMatrix.m[0][2] = sinA;
    mTransformMatrix.m[0][3] = mCenter.x;

    mTransformMatrix.m[1][0] = 0.0f;
    mTransformMatrix.m[1][1] = 1.0f;
    mTransformMatrix.m[1][2] = 0.0f;
    mTransformMatrix.m[1][3] = mCenter.y;

    mTransformMatrix.m[2][0] = -sinA;
    mTransformMatrix.m[2][1] = 0.0f;
    mTransformMatrix.m[2][2] = cosA;
    mTransformMatrix.m[2][3] = mCenter.z;
}

/**
 * Obj_TurnPlate__vfunc_11 @ 0x025e68dc
 * Surface contact and rider query.
 */
void GameTurnPlate::vfunc_11() {
}

void GameTurnPlate::setupPlatform(const sead::Vector3f& center, f32 radius, f32 angularSpeed) {
    mCenter = center;
    mRadius = radius;
    mAngularSpeed = angularSpeed;
    vfunc_3();
}

sead::Vector3f GameTurnPlate::computeLinearVelocityAtPoint(const sead::Vector3f& worldPos) const {
    f32 relX = worldPos.x - mCenter.x;
    f32 relZ = worldPos.z - mCenter.z;

    // Linear tangential velocity v = omega x r
    // For rotation around Y: vx = -omega * relZ, vz = omega * relX
    return sead::Vector3f(-mAngularSpeed * relZ, 0.0f, mAngularSpeed * relX);
}

bool GameTurnPlate::isPointOnPlatform(const sead::Vector3f& worldPos) const {
    f32 dx = worldPos.x - mCenter.x;
    f32 dy = std::abs(worldPos.y - mCenter.y);
    f32 dz = worldPos.z - mCenter.z;
    return (dy < 1.0f) && ((dx * dx + dz * dz) <= (mRadius * mRadius));
}

void GameTurnPlate::update() {
    GambitActor::update();
    vfunc_7();
    vfunc_11();
}

void GameTurnPlate::draw() {
    GambitActor::draw();
}

} // namespace Game
