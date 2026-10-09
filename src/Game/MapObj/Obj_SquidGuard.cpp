#include "Game/MapObj/Obj_SquidGuard.h"
#include <cmath>
#include <algorithm>

namespace Game {

Obj_SquidGuard::Obj_SquidGuard()
    : mPosition(0.0f, 0.0f, 0.0f),
      mState(SquidGuardState::cState_Idle),
      mRattleOffset(0.0f),
      mRattleEnergy(0.0f),
      mRattleTimer(0) {
}

Obj_SquidGuard::~Obj_SquidGuard() {
}

void Obj_SquidGuard::init() {
    GambitActor::init();
    mState = SquidGuardState::cState_Idle;
    mRattleOffset = 0.0f;
    mRattleEnergy = 0.0f;
    mRattleTimer = 0;
}

void Obj_SquidGuard::vfunc_3() {
    // Model & resource load (Obj_SquidGuard.szs)
}

void Obj_SquidGuard::vfunc_5() {
    // Collision shape initialization
}

void Obj_SquidGuard::vfunc_7() {
    update();
}

void Obj_SquidGuard::vfunc_11() {
    // Barrier mesh collision
}

bool Obj_SquidGuard::hitWithInk(f32 inkPower, const sead::Vector3f& hitDir) {
    mState = SquidGuardState::cState_Rattle;
    mRattleEnergy = std::min(mRattleEnergy + inkPower * 0.03f, cRattleAmplitude);
    mRattleTimer = 0;
    return true;
}

bool Obj_SquidGuard::checkPlayerCollision(const sead::Vector3f& playerPos, f32 radius, sead::Vector3f& outRebound) {
    f32 dx = playerPos.x - mPosition.x;
    f32 dy = playerPos.y - mPosition.y;
    f32 dz = playerPos.z - mPosition.z;

    // AABB check against barrier bounds
    if (std::abs(dx) <= (cBarrierHalfWidth + radius) &&
        dy >= 0.0f && dy <= (cBarrierHeight + radius) &&
        std::abs(dz) <= (cBarrierThickness + radius)) {

        // Rebound impulse pushback out of barrier
        f32 pushZ = (dz >= 0.0f) ? (cBarrierThickness + radius - dz) : (-cBarrierThickness - radius - dz);
        outRebound.set(0.0f, 0.0f, pushZ);

        mState = SquidGuardState::cState_Rattle;
        mRattleEnergy = cRattleAmplitude * 0.5f;
        return true;
    }

    outRebound.set(0.0f, 0.0f, 0.0f);
    return false;
}

void Obj_SquidGuard::update() {
    if (mState == SquidGuardState::cState_Rattle) {
        mRattleTimer++;
        // Rapid 6-frame harmonic oscillation
        f32 phase = (static_cast<f32>(mRattleTimer % 6) / 6.0f) * 6.2831853f;
        mRattleOffset = std::sin(phase) * mRattleEnergy;

        mRattleEnergy *= cRattleDecay;
        if (mRattleEnergy < 0.005f) {
            mRattleEnergy = 0.0f;
            mRattleOffset = 0.0f;
            mState = SquidGuardState::cState_Idle;
        }
    }
}

void Obj_SquidGuard::draw() {
    GambitActor::draw();
}

} // namespace Game
