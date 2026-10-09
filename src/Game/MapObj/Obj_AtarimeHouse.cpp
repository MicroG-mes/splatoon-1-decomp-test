#include "Game/MapObj/Obj_AtarimeHouse.h"
#include <cmath>
#include <algorithm>

namespace Game {

Obj_AtarimeHouse::Obj_AtarimeHouse()
    : mPosition(0.0f, 0.0f, 0.0f)
    , mState(AtarimeHouseState::cState_Normal)
    , mTimer(0)
    , mAntennaSway(0.0f)
{
    std::fill(mReserved, mReserved + sizeof(mReserved), 0);
}

Obj_AtarimeHouse::~Obj_AtarimeHouse() {
}

void Obj_AtarimeHouse::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mState = AtarimeHouseState::cState_Normal;
    mTimer = 0;
    mAntennaSway = 0.0f;
}

void Obj_AtarimeHouse::vfunc_3() {
    // 0x0245a1c0: Model loading (Obj_AtarimeHouse.szs)
}

void Obj_AtarimeHouse::vfunc_5() {
    // 0x0245b0a4: Compound setup & smoke anchor
    mState = AtarimeHouseState::cState_Normal;
}

void Obj_AtarimeHouse::vfunc_7() {
    // 0x0245b630: Environmental fabric sway
    update();
}

void Obj_AtarimeHouse::vfunc_14() {
    // 0x0245bf18: Player proximity check
}

bool Obj_AtarimeHouse::isPlayerInCompound(const sead::Vector3f& playerPos) const {
    f32 dx = playerPos.x - mPosition.x;
    f32 dz = playerPos.z - mPosition.z;
    return (dx * dx + dz * dz) <= (cCompoundRadius * cCompoundRadius);
}

void Obj_AtarimeHouse::setVacant(bool vacant) {
    mState = vacant ? AtarimeHouseState::cState_Vacant : AtarimeHouseState::cState_Normal;
}

void Obj_AtarimeHouse::update() {
    mTimer++;

    // Ambient antenna / cloth sway in Octo Valley mountain breeze
    mAntennaSway = std::sin(mTimer * 0.04f) * 0.12f;
}

void Obj_AtarimeHouse::draw() {
    // Model rendered via ModelSceneMgr
}

} // namespace Game
