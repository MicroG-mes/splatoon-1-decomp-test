#include "Game/MapObj/Obj_ColorCone.h"
#include <cmath>
#include <algorithm>

namespace Game {

Obj_ColorCone::Obj_ColorCone()
    : mPosition(0.0f, 0.0f, 0.0f),
      mTiltAxis(1.0f, 0.0f, 0.0f),
      mState(ColorConeState::cState_Upright),
      mTiltAngle(0.0f),
      mTiltVelocity(0.0f),
      mStateTimer(0) {
}

Obj_ColorCone::~Obj_ColorCone() {
}

void Obj_ColorCone::init() {
    GambitActor::init();
    mState = ColorConeState::cState_Upright;
    mTiltAngle = 0.0f;
    mTiltVelocity = 0.0f;
    mStateTimer = 0;
    mTiltAxis.set(1.0f, 0.0f, 0.0f);
}

void Obj_ColorCone::vfunc_3() {
    // Model & resource load (Obj_ColorCone.szs)
}

void Obj_ColorCone::vfunc_5() {
    // Parameter init
}

void Obj_ColorCone::vfunc_7() {
    // Physics tick
    update();
}

void Obj_ColorCone::vfunc_11() {
    // Collision detection
}

void Obj_ColorCone::applyImpulse(const sead::Vector3f& force) {
    f32 mag = std::sqrt(force.x * force.x + force.z * force.z);
    if (mag < 0.01f) return;

    if (mag >= cToppleThreshold) {
        // Heavy impact topples cone
        mState = ColorConeState::cState_Toppled;
        mTiltAngle = cMaxTiltAngle;
        mTiltVelocity = 0.0f;
        mTiltAxis.set(-force.z / mag, 0.0f, force.x / mag);
        return;
    }

    // Elastic tilt impulse
    mState = ColorConeState::cState_Tilted;
    mTiltVelocity += mag * 0.15f;
    mTiltAxis.set(-force.z / mag, 0.0f, force.x / mag);
}

bool Obj_ColorCone::checkPlayerBrush(const sead::Vector3f& playerPos, const sead::Vector3f& playerVel, f32 playerRadius) {
    f32 dx = playerPos.x - mPosition.x;
    f32 dz = playerPos.z - mPosition.z;
    f32 dist = std::sqrt(dx * dx + dz * dz);

    if (dist <= (cBaseRadius + playerRadius)) {
        applyImpulse(playerVel * 2.0f);
        return true;
    }
    return false;
}

bool Obj_ColorCone::checkBulletImpact(const sead::Vector3f& bulletPos, const sead::Vector3f& bulletVel, f32 damage) {
    f32 dx = bulletPos.x - mPosition.x;
    f32 dy = bulletPos.y - mPosition.y;
    f32 dz = bulletPos.z - mPosition.z;
    f32 distH = std::sqrt(dx * dx + dz * dz);

    if (distH <= cBaseRadius && dy >= 0.0f && dy <= cHeight) {
        sead::Vector3f imp = bulletVel * (damage * 0.05f);
        applyImpulse(imp);
        return true;
    }
    return false;
}

void Obj_ColorCone::update() {
    mStateTimer++;

    if (mState == ColorConeState::cState_Tilted) {
        // Torsional spring-damper restoring torque
        f32 springAccel = -cTiltSpringKp * mTiltAngle - cTiltDamperKd * mTiltVelocity;
        mTiltVelocity += springAccel;
        mTiltAngle += mTiltVelocity;

        if (std::abs(mTiltAngle) < 0.005f && std::abs(mTiltVelocity) < 0.005f) {
            mTiltAngle = 0.0f;
            mTiltVelocity = 0.0f;
            mState = ColorConeState::cState_Upright;
        }
    }
}

void Obj_ColorCone::draw() {
    GambitActor::draw();
}

} // namespace Game
