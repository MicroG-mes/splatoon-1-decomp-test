#include "Game/Enemy/Enm_Stamp.h"
#include <cmath>
#include <algorithm>

namespace Game {

Enm_Stamp::Enm_Stamp()
    : mPosition(0.0f, 0.0f, 0.0f)
    , mState(StampState::cState_Wait)
    , mHealth(cMaxHealth)
    , mStateTimer(0)
    , mFaceDown(false)
{
    std::fill(mReserved, mReserved + sizeof(mReserved), 0);
}

Enm_Stamp::~Enm_Stamp() {
}

void Enm_Stamp::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mState = StampState::cState_Wait;
    mHealth = cMaxHealth;
    mStateTimer = 0;
    mFaceDown = false;
}

void Enm_Stamp::vfunc_3() {
    // 0x0239d05c: Model loading (Enm_Stamp.szs, 3,464 vertices)
}

void Enm_Stamp::vfunc_5() {
    // 0x0239d064: Parameter loading from Enm_Stamp.params
    mHealth = cMaxHealth;
}

void Enm_Stamp::vfunc_7() {
    // 0x0239d120: State machine update
    update();
}

void Enm_Stamp::vfunc_9() {
    // 0x0239d240: Damage callback & front shield deflection
}

void Enm_Stamp::vfunc_11() {
    // 0x0239d350: Slam shockwave & ink splash
}

void Enm_Stamp::spawn(const sead::Vector3f& pos) {
    mPosition = pos;
    mState = StampState::cState_Wait;
    mHealth = cMaxHealth;
    mStateTimer = 0;
    mFaceDown = false;
}

bool Enm_Stamp::checkSight(const sead::Vector3f& playerPos) const {
    f32 dx = playerPos.x - mPosition.x;
    f32 dy = playerPos.y - mPosition.y;
    f32 dz = playerPos.z - mPosition.z;
    f32 distSq = dx * dx + dy * dy + dz * dz;

    return (distSq <= cEyesightRadius * cEyesightRadius) && (std::abs(dy) <= 130.0f);
}

void Enm_Stamp::triggerSlamAttack() {
    mState = StampState::cState_AttackSt;
    mStateTimer = 0;
}

bool Enm_Stamp::takeDamage(f32 damage, const sead::Vector3f& hitPos, const sead::Vector3f& hitDir, bool& outShieldDeflected) {
    outShieldDeflected = false;

    if (mState == StampState::cState_Die) {
        return false;
    }

    // When standing up and hit from the front face (Z < 0): 100% deflected by metal face!
    if (!mFaceDown && hitDir.z < 0.0f) {
        outShieldDeflected = true;
        return false;
    }

    // When face-down in Chance state, or hit from behind: vulnerable squid tentacle takes full damage!
    mHealth -= damage;

    if (mHealth <= 0.0f) {
        mHealth = 0.0f;
        mState = StampState::cState_Die;
        mStateTimer = 0;
    }

    return true;
}

void Enm_Stamp::update() {
    mStateTimer++;

    switch (mState) {
        case StampState::cState_AttackSt: {
            // Jump windup (15 frames)
            if (mStateTimer >= 15) {
                mState = StampState::cState_Attack;
                mStateTimer = 0;
            }
            break;
        }

        case StampState::cState_Attack: {
            // Slam down onto the ground!
            if (mStateTimer >= 10) {
                mState = StampState::cState_Chance;
                mStateTimer = 0;
                mFaceDown = true;
            }
            break;
        }

        case StampState::cState_Chance: {
            // Face-down vulnerability window (60 frames)
            if (mStateTimer >= cChanceFrames) {
                mState = StampState::cState_StandUp;
                mStateTimer = 0;
            }
            break;
        }

        case StampState::cState_StandUp: {
            if (mStateTimer >= cStandUpFrames) {
                mState = StampState::cState_Wait;
                mStateTimer = 0;
                mFaceDown = false;
            }
            break;
        }

        default:
            break;
    }
}

void Enm_Stamp::draw() {
    // Model rendered via ModelSceneMgr
}

} // namespace Game
