#include "Game/MapObj/Obj_Box00S.h"
#include <cmath>
#include <algorithm>

namespace Game {

Obj_Box00S::Obj_Box00S()
    : mPosition(0.0f, 0.0f, 0.0f)
    , mState(BoxSmallState::cState_Normal)
    , mHealth(cMaxHealth)
    , mDroppedEggs(0)
{
    std::fill(mReserved, mReserved + sizeof(mReserved), 0);
}

Obj_Box00S::~Obj_Box00S() {
}

void Obj_Box00S::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mState = BoxSmallState::cState_Normal;
    mHealth = cMaxHealth;
    mDroppedEggs = 0;
}

void Obj_Box00S::vfunc_3() {
    // 0x025f8210: Model loading (Obj_Box00S.szs)
}

void Obj_Box00S::vfunc_5() {
    // 0x025f8240: Durability initialization
    mHealth = cMaxHealth;
}

void Obj_Box00S::vfunc_7() {
    // 0x025f4ba0: Hit detection
    update();
}

void Obj_Box00S::vfunc_9() {
    // 0x025f54ec: Break shatter event
}

bool Obj_Box00S::takeDamage(f32 damage) {
    if (isBroken()) {
        return false;
    }

    mHealth -= damage;

    if (mHealth <= 0.0f) {
        mHealth = 0.0f;
        mState = BoxSmallState::cState_Broken;
        mDroppedEggs = 2; // Drops 2 Power Eggs
        return true;
    }

    return false;
}

void Obj_Box00S::update() {
    // Idle physics
}

void Obj_Box00S::draw() {
    // Model rendered via ModelSceneMgr
}

} // namespace Game
