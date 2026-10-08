#include "Game/Amiibo/Obj_PlazaAmiibo.h"

namespace Game {

Obj_PlazaAmiibo::Obj_PlazaAmiibo()
    : mPosition(14.2f, 0.0f, -8.5f),
      mInteractRadius(3.0f),
      mIsPlayerNearby(false),
      mIsOpen(false),
      mGlowAnimTimer(0) {
}

Obj_PlazaAmiibo::~Obj_PlazaAmiibo() {
}

void Obj_PlazaAmiibo::init() {
    GambitActor::init();
    mIsPlayerNearby = false;
    mIsOpen = false;
}

bool Obj_PlazaAmiibo::checkPlayerProximity(const sead::Vector3f& playerPos, f32 interactRadius) {
    f32 dx = playerPos.x - mPosition.x;
    f32 dy = playerPos.y - mPosition.y;
    f32 dz = playerPos.z - mPosition.z;
    f32 distSq = dx * dx + dy * dy + dz * dz;

    mIsPlayerNearby = (distSq <= interactRadius * interactRadius);
    return mIsPlayerNearby;
}

void Obj_PlazaAmiibo::triggerOpenBox() {
    if (mIsPlayerNearby) {
        mIsOpen = true;
    }
}

void Obj_PlazaAmiibo::closeBox() {
    mIsOpen = false;
}

void Obj_PlazaAmiibo::update() {
    mGlowAnimTimer++;
}

void Obj_PlazaAmiibo::draw() {
    GambitActor::draw();
}

} // namespace Game
