#include "Game/MapObj/Obj_Box00L.h"
#include <cmath>
#include <algorithm>

namespace Game {

Obj_Box00L::Obj_Box00L()
    : mPosition(0.0f, 0.0f, 0.0f)
    , mState(BoxState::cState_Normal)
    , mHealth(cMaxHealth)
    , mWobbleScale(1.0f)
    , mTimer(0)
    , mDroppedEggs(0)
{
    std::fill(mReserved, mReserved + sizeof(mReserved), 0);
}

Obj_Box00L::~Obj_Box00L() {
}

void Obj_Box00L::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mState = BoxState::cState_Normal;
    mHealth = cMaxHealth;
    mWobbleScale = 1.0f;
    mTimer = 0;
    mDroppedEggs = 0;
}

void Obj_Box00L::vfunc_3() {
    // 0x025f7448: Model loading (Obj_Box00L.szs, Obj_Break00)
}

void Obj_Box00L::vfunc_5() {
    // 0x025f7480: Durability initialization
    mHealth = cMaxHealth;
}

void Obj_Box00L::vfunc_7() {
    // 0x025f4ba0: Damage reception & wobble update
    update();
}

void Obj_Box00L::vfunc_9() {
    // 0x025f54ec: Shatter fracture event
}

bool Obj_Box00L::takeDamage(f32 damage) {
    if (isBroken()) {
        return false;
    }

    mHealth -= damage;
    mState = BoxState::cState_Wobble;
    mTimer = 0;
    mWobbleScale = 1.15f;

    if (mHealth <= 0.0f) {
        mHealth = 0.0f;
        mState = BoxState::cState_Broken;
        mDroppedEggs = 5; // Drops 5 Power Eggs
        return true;
    }

    return false;
}

void Obj_Box00L::update() {
    mTimer++;

    if (mState == BoxState::cState_Wobble) {
        mWobbleScale *= 0.92f;
        if (mWobbleScale <= 1.01f) {
            mWobbleScale = 1.0f;
            mState = BoxState::cState_Normal;
        }
    }
}

void Obj_Box00L::draw() {
    // Model rendered via ModelSceneMgr
}

} // namespace Game
