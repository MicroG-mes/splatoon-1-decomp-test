#include "Game/MapObj/Obj_RespawnPlatform.h"
#include <cmath>
#include <algorithm>

namespace Game {

Obj_RespawnPlatform::Obj_RespawnPlatform()
    : mPosition(0.0f, 0.0f, 0.0f)
    , mState(RespawnPlatformState::cState_Wait)
    , mTeam(RespawnTeam::cTeam_Alpha)
    , mTimer(0)
    , mSpawnsCount(0)
    , mBarrierPulse(1.0f)
{
    std::fill(mReserved, mReserved + sizeof(mReserved), 0);
}

Obj_RespawnPlatform::~Obj_RespawnPlatform() {
}

void Obj_RespawnPlatform::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mState = RespawnPlatformState::cState_Wait;
    mTeam = RespawnTeam::cTeam_Alpha;
    mTimer = 0;
    mSpawnsCount = 0;
    mBarrierPulse = 1.0f;
}

void Obj_RespawnPlatform::vfunc_3() {
    // 0x025aa410: Model loading (Obj_RespawnPlatform.szs, M_RespawnPlatform01, tex_mtx1)
}

void Obj_RespawnPlatform::vfunc_5() {
    // 0x025aa3b8: Spawn barrier collision & team setup
    mState = RespawnPlatformState::cState_Wait;
}

void Obj_RespawnPlatform::vfunc_7() {
    // 0x025aa520: Shield pulse animation
    update();
}

void Obj_RespawnPlatform::vfunc_14() {
    // 0x025aa6b0: Super jump landing pad & player ink replenishment
}

bool Obj_RespawnPlatform::isInsideBarrier(const sead::Vector3f& playerPos) const {
    f32 dx = playerPos.x - mPosition.x;
    f32 dz = playerPos.z - mPosition.z;
    f32 dy = playerPos.y - mPosition.y;

    return (dx * dx + dz * dz <= cBarrierRadius * cBarrierRadius) && (dy >= -0.5f && dy <= 5.0f);
}

void Obj_RespawnPlatform::triggerRespawnEffect() {
    mState = RespawnPlatformState::cState_Respawn;
    mTimer = 0;
    mSpawnsCount++;
    mBarrierPulse = 1.3f;
}

f32 Obj_RespawnPlatform::refillPlayerInk(f32 currentInk) {
    currentInk += cInkRefillRate;
    if (currentInk > 1.0f) {
        currentInk = 1.0f;
    }
    return currentInk;
}

void Obj_RespawnPlatform::update() {
    mTimer++;

    // Harmonic barrier glow pulsation
    f32 pulsePhase = mTimer * 0.05f;
    mBarrierPulse = 1.0f + std::sin(pulsePhase) * 0.08f;

    if (mState == RespawnPlatformState::cState_Respawn && mTimer >= 20) {
        mState = RespawnPlatformState::cState_Wait;
    }
}

void Obj_RespawnPlatform::draw() {
    // Model rendered via ModelSceneMgr
}

} // namespace Game
