#include "Game/Enemy/Enm_TakopterTornado.h"
#include <cmath>
#include <algorithm>

namespace Game {

Enm_TakopterTornado::Enm_TakopterTornado()
    : mPosition(0.0f, cHoverAltitude, 0.0f)
    , mState(TakopterTornadoState::cState_Wait)
    , mHealth(cMaxHealth)
    , mPropellerAngle(0.0f)
    , mBobOffset(0.0f)
    , mTimer(0)
    , mStateTimer(0)
{
    std::fill(mReserved, mReserved + sizeof(mReserved), 0);
}

Enm_TakopterTornado::~Enm_TakopterTornado() {
}

void Enm_TakopterTornado::init() {
    GambitActor::init();
    mPosition.set(0.0f, cHoverAltitude, 0.0f);
    mState = TakopterTornadoState::cState_Wait;
    mHealth = cMaxHealth;
    mPropellerAngle = 0.0f;
    mBobOffset = 0.0f;
    mTimer = 0;
    mStateTimer = 0;
}

void Enm_TakopterTornado::vfunc_3() {
    // 0x023cf3a4: Model & asset loading (Enm_TakopterTornado.szs, 8,675 vertices)
}

void Enm_TakopterTornado::vfunc_5() {
    // 0x023c7870: Parameter initialization
    mHealth = cMaxHealth;
}

void Enm_TakopterTornado::vfunc_7() {
    // 0x023c8060: State machine update
    update();
}

void Enm_TakopterTornado::vfunc_47() {
    // 0x023d1370: Tornado vortex projectile spawn
}

void Enm_TakopterTornado::spawn(const sead::Vector3f& pos) {
    mPosition = pos;
    mState = TakopterTornadoState::cState_Wait;
    mHealth = cMaxHealth;
    mPropellerAngle = 0.0f;
    mBobOffset = 0.0f;
    mTimer = 0;
    mStateTimer = 0;
}

bool Enm_TakopterTornado::checkSight(const sead::Vector3f& playerPos) const {
    f32 dx = playerPos.x - mPosition.x;
    f32 dy = playerPos.y - mPosition.y;
    f32 dz = playerPos.z - mPosition.z;
    f32 distSq = dx * dx + dy * dy + dz * dz;

    return distSq <= (cEyesightRadius * cEyesightRadius);
}

void Enm_TakopterTornado::triggerTornadoAttack() {
    mState = TakopterTornadoState::cState_Attack;
    mStateTimer = 0;
}

bool Enm_TakopterTornado::hitWithInk(f32 damage, const sead::Vector3f& hitDir) {
    (void)hitDir;

    if (mState == TakopterTornadoState::cState_Die) {
        return false;
    }

    mHealth -= damage;
    if (mHealth <= 0.0f) {
        mHealth = 0.0f;
        mState = TakopterTornadoState::cState_Die;
        mStateTimer = 0;
    }

    return true;
}

void Enm_TakopterTornado::update() {
    mTimer++;
    mStateTimer++;

    // Continuous propeller spin
    mPropellerAngle += cPropellerRotSpeed;
    if (mPropellerAngle >= 360.0f) {
        mPropellerAngle -= 360.0f;
    }

    // Natural hover bobbing oscillation (Fuwa)
    f32 cycleRad = static_cast<f32>(mTimer % 180) / 180.0f * 6.2831853f;
    mBobOffset = std::sin(cycleRad) * cFuwaAmplitude;

    switch (mState) {
        case TakopterTornadoState::cState_Wait: {
            // Hover in place or patrol
            break;
        }

        case TakopterTornadoState::cState_Chase: {
            // Move toward target at cChaseSpeed
            break;
        }

        case TakopterTornadoState::cState_Attack: {
            // Charge up and unleash tornado projectile over 20 frames
            if (mStateTimer >= 20) {
                mState = TakopterTornadoState::cState_Wait;
                mStateTimer = 0;
            }
            break;
        }

        case TakopterTornadoState::cState_Escape: {
            // Back away from encroaching player
            if (mStateTimer >= 40) {
                mState = TakopterTornadoState::cState_Wait;
                mStateTimer = 0;
            }
            break;
        }

        case TakopterTornadoState::cState_Die: {
            // Spiral descent and pop
            break;
        }

        default:
            break;
    }
}

void Enm_TakopterTornado::draw() {
    // Model rendered via ModelSceneMgr
}

} // namespace Game
