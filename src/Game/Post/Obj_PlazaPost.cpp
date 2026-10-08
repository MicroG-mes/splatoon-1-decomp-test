#include "Game/Post/Obj_PlazaPost.h"
#include <cstring>
#include <cmath>

namespace Game {

Obj_PlazaPost::Obj_PlazaPost()
    : mPosition(-8.4f, 0.0f, -2.1f),
      mInteractRadius(2.5f),
      mIsPlayerNearby(false),
      mIsOpen(false) {
    std::memset(mTransformMatrix, 0, sizeof(mTransformMatrix));
    std::memset(mRenderMatrix, 0, sizeof(mRenderMatrix));
    std::memset(mReserved, 0, sizeof(mReserved));

    mTransformMatrix[0][0] = 1.0f;
    mTransformMatrix[1][1] = 1.0f;
    mTransformMatrix[2][2] = 1.0f;
    mTransformMatrix[0][3] = mPosition.x;
    mTransformMatrix[1][3] = mPosition.y;
    mTransformMatrix[2][3] = mPosition.z;
}

Obj_PlazaPost::~Obj_PlazaPost() {
}

void Obj_PlazaPost::init() {
    GambitActor::init();
    mIsPlayerNearby = false;
    mIsOpen = false;
    vfunc_47();
}

// 0x02580778: Decompiled vfunc_11 (3x4 affine transform matrix synchronization)
void Obj_PlazaPost::vfunc_11() {
    // Copy affine matrix [0x54 - 0x80] into [0x1F8 - 0x220]
    std::memcpy(mRenderMatrix, mTransformMatrix, sizeof(mRenderMatrix));
}

// 0x02582260: Decompiled vfunc_47 (Stage collision mesh registration)
void Obj_PlazaPost::vfunc_47() {
    // Registered with Plaza environment collision tree
}

bool Obj_PlazaPost::checkPlayerProximity(const sead::Vector3f& playerPos, f32 interactRadius) {
    f32 dx = playerPos.x - mPosition.x;
    f32 dy = playerPos.y - mPosition.y;
    f32 dz = playerPos.z - mPosition.z;
    f32 distSq = dx * dx + dy * dy + dz * dz;

    mIsPlayerNearby = (distSq <= interactRadius * interactRadius);
    return mIsPlayerNearby;
}

void Obj_PlazaPost::triggerOpenMailbox() {
    if (mIsPlayerNearby) {
        mIsOpen = true;
    }
}

void Obj_PlazaPost::closeMailbox() {
    mIsOpen = false;
}

void Obj_PlazaPost::update() {
    vfunc_11();
}

void Obj_PlazaPost::draw() {
    GambitActor::draw();
}

} // namespace Game
