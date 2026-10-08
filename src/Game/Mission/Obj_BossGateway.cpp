#include "Game/Mission/Obj_BossGateway.h"

namespace Game {

Obj_BossGateway::Obj_BossGateway()
    : mPosition(0.0f, 0.0f, 35.0f),
      mInteractRadius(3.5f),
      mIsUnlocked(false),
      mIsEntering(false),
      mStateTimer(0) {
}

Obj_BossGateway::~Obj_BossGateway() {
}

void Obj_BossGateway::init() {
    GambitActor::init();
    mIsUnlocked = false;
    mIsEntering = false;
}

void Obj_BossGateway::setUnlockState(bool isUnlocked) {
    mIsUnlocked = isUnlocked;
}

bool Obj_BossGateway::checkPlayerProximity(const sead::Vector3f& playerPos, f32 interactRadius) {
    f32 dx = playerPos.x - mPosition.x;
    f32 dy = playerPos.y - mPosition.y;
    f32 dz = playerPos.z - mPosition.z;
    f32 distSq = dx * dx + dy * dy + dz * dz;

    return (distSq <= interactRadius * interactRadius);
}

void Obj_BossGateway::enterBossStage() {
    if (mIsUnlocked) {
        mIsEntering = true;
        mStateTimer = 0;
    }
}

void Obj_BossGateway::update() {
    mStateTimer++;
}

void Obj_BossGateway::draw() {
    GambitActor::draw();
}

} // namespace Game
