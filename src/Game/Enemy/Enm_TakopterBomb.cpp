#include "Game/Enemy/Enm_TakopterBomb.h"
#include <cmath>
#include <algorithm>

namespace Game {

Enm_TakopterBomb::Enm_TakopterBomb()
    : mPosition(0.0f, cFlightAltitude, 0.0f)
    , mState(TakopterBombState::cState_Wait)
    , mHealth(cMaxHealth)
    , mDroppedBombs(0)
    , mStateTimer(0)
{
    std::fill(mReserved, mReserved + sizeof(mReserved), 0);
}

Enm_TakopterBomb::~Enm_TakopterBomb() {
}

void Enm_TakopterBomb::init() {
    GambitActor::init();
    mPosition.set(0.0f, cFlightAltitude, 0.0f);
    mState = TakopterBombState::cState_Wait;
    mHealth = cMaxHealth;
    mDroppedBombs = 0;
    mStateTimer = 0;
}

void Enm_TakopterBomb::vfunc_3() {
    // 0x023cf3a4: Model & asset loading (Enm_TakopterBomb.szs, 11,611 vertices)
}

void Enm_TakopterBomb::vfunc_5() {
    // 0x023c7870: Parameter initialization
    mHealth = cMaxHealth;
}

void Enm_TakopterBomb::vfunc_7() {
    // 0x023c8060: Aerial tracking & state update
    update();
}

void Enm_TakopterBomb::vfunc_47() {
    // 0x023d1370: Splat Bomb release
    dropBomb();
}

void Enm_TakopterBomb::spawn(const sead::Vector3f& pos) {
    mPosition = pos;
    mState = TakopterBombState::cState_Wait;
    mHealth = cMaxHealth;
    mDroppedBombs = 0;
    mStateTimer = 0;
}

bool Enm_TakopterBomb::checkSight(const sead::Vector3f& playerPos) const {
    f32 dx = playerPos.x - mPosition.x;
    f32 dy = playerPos.y - mPosition.y;
    f32 dz = playerPos.z - mPosition.z;
    f32 distSq = dx * dx + dy * dy + dz * dz;

    return (distSq <= cEyesightRadius * cEyesightRadius) && (std::abs(dy) <= 180.0f);
}

void Enm_TakopterBomb::dropBomb() {
    mDroppedBombs++;
    mState = TakopterBombState::cState_BombDrop;
    mStateTimer = 0;
}

bool Enm_TakopterBomb::takeDamage(f32 damage) {
    if (mState == TakopterBombState::cState_Die) {
        return false;
    }

    mHealth -= damage;
    if (mHealth <= 0.0f) {
        mHealth = 0.0f;
        mState = TakopterBombState::cState_Die;
        return true;
    }

    return false;
}

void Enm_TakopterBomb::update() {
    mStateTimer++;

    switch (mState) {
        case TakopterBombState::cState_Wait: {
            break;
        }

        case TakopterBombState::cState_Chase: {
            break;
        }

        case TakopterBombState::cState_BombDrop: {
            if (mStateTimer >= 20) {
                mState = TakopterBombState::cState_Wait;
                mStateTimer = 0;
            }
            break;
        }

        case TakopterBombState::cState_Die: {
            break;
        }

        default:
            break;
    }
}

void Enm_TakopterBomb::draw() {
    // Model rendered via ModelSceneMgr
}

} // namespace Game
