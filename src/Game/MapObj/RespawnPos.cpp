#include "Game/MapObj/RespawnPos.h"
#include <cmath>

namespace Game {

RespawnPos::RespawnPos()
    : mPosition(0.0f, 0.0f, 0.0f),
      mTeamId(0),
      mBarrierPulsePhase(0.0f) {
}

RespawnPos::~RespawnPos() {
}

void RespawnPos::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mTeamId = 0;
    mBarrierPulsePhase = 0.0f;
}

void RespawnPos::setupSpawn(const sead::Vector3f& pos, u32 teamId) {
    mPosition = pos;
    mTeamId = teamId;
    mBarrierPulsePhase = 0.0f;
}

bool RespawnPos::checkBarrierCollision(const sead::Vector3f& testPos, u32 testTeam) const {
    if (testTeam == mTeamId) {
        return false; // Friendly players pass through barrier freely
    }

    f32 dx = testPos.x - mPosition.x;
    f32 dz = testPos.z - mPosition.z;
    f32 distSq = dx * dx + dz * dz;

    // Repulse enemy shots and enemy players from entering spawn barrier
    if (distSq <= cBarrierRadius * cBarrierRadius) {
        f32 dy = testPos.y - mPosition.y;
        if (dy >= 0.0f && dy <= cBarrierHeight) {
            return true; // Collision detected, deflect enemy projectile
        }
    }

    return false;
}

bool RespawnPos::isInsideBarrier(const sead::Vector3f& playerPos) const {
    f32 dx = playerPos.x - mPosition.x;
    f32 dz = playerPos.z - mPosition.z;
    f32 distSq = dx * dx + dz * dz;

    if (distSq <= cBarrierRadius * cBarrierRadius) {
        f32 dy = playerPos.y - mPosition.y;
        return (dy >= 0.0f && dy <= cBarrierHeight);
    }

    return false;
}

void RespawnPos::update() {
    mBarrierPulsePhase += 0.04f;
    if (mBarrierPulsePhase > 6.283185f) {
        mBarrierPulsePhase -= 6.283185f;
    }
}

void RespawnPos::draw() {
    GambitActor::draw();
}

} // namespace Game
