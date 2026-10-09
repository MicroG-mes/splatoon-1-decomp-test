#include "Game/Enemy/Enm_TakolienVehicleSubmarine.h"
#include <cmath>
#include <algorithm>

namespace Game {

Enm_TakolienVehicleSubmarine::Enm_TakolienVehicleSubmarine()
    : mPosition(0.0f, 0.0f, 0.0f)
    , mMoveDirection(0.0f, 0.0f, 1.0f)
    , mCurrentDepth(cSubmergedDepth)
    , mVerticalVelocity(0.0f)
    , mHealth(cMaxHealth)
    , mState(VehicleSubmarineState::cSubmerged)
    , mStateTimer(0)
    , mBreachCount(0)
    , mIsPilotEjected(false)
{
    std::fill(mReserved, mReserved + sizeof(mReserved), 0);
}

Enm_TakolienVehicleSubmarine::~Enm_TakolienVehicleSubmarine() {
}

void Enm_TakolienVehicleSubmarine::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mMoveDirection.set(0.0f, 0.0f, 1.0f);
    mCurrentDepth = cSubmergedDepth;
    mVerticalVelocity = 0.0f;
    mHealth = cMaxHealth;
    mState = VehicleSubmarineState::cSubmerged;
    mStateTimer = 0;
    mBreachCount = 0;
    mIsPilotEjected = false;
}

void Enm_TakolienVehicleSubmarine::vfunc_3() {
    // 0x1008B768: Model loading (Enm_TakolienVehicleSubmarine.szs, 8,172 vertices)
}

void Enm_TakolienVehicleSubmarine::vfunc_5() {
    // Parameter initialization
    mHealth = cMaxHealth;
    mState = VehicleSubmarineState::cSubmerged;
}

void Enm_TakolienVehicleSubmarine::vfunc_7() {
    // Submersible fluid dynamics & breach update
    update();
}

void Enm_TakolienVehicleSubmarine::vfunc_47() {
    // Submarine ink eruption torpedo discharge
}

void Enm_TakolienVehicleSubmarine::vfunc_52() {
    // Hull rupture & pilot ejection
    mState = VehicleSubmarineState::cEject;
    mIsPilotEjected = true;
}

void Enm_TakolienVehicleSubmarine::spawn(const sead::Vector3f& pos) {
    mPosition = pos;
    mMoveDirection.set(0.0f, 0.0f, 1.0f);
    mCurrentDepth = cSubmergedDepth;
    mVerticalVelocity = 0.0f;
    mHealth = cMaxHealth;
    mState = VehicleSubmarineState::cSubmerged;
    mStateTimer = 0;
    mBreachCount = 0;
    mIsPilotEjected = false;
}

void Enm_TakolienVehicleSubmarine::updateSubmarineAi(const sead::Vector3f& targetPos) {
    if (isDestroyed()) return;

    f32 dx = targetPos.x - mPosition.x;
    f32 dz = targetPos.z - mPosition.z;
    f32 distH = std::sqrt(dx * dx + dz * dz);

    if (distH > 0.1f) {
        mMoveDirection.set(dx / distH, 0.0f, dz / distH);
    }

    if (mState == VehicleSubmarineState::cSubmerged && distH <= 20.0f) {
        mState = VehicleSubmarineState::cPeriscope;
        mCurrentDepth = cPeriscopeDepth;
    } else if (mState == VehicleSubmarineState::cPeriscope && distH <= 8.0f) {
        triggerBreach();
    }
}

void Enm_TakolienVehicleSubmarine::triggerBreach() {
    if (mState == VehicleSubmarineState::cSubmerged || mState == VehicleSubmarineState::cPeriscope) {
        mState = VehicleSubmarineState::cBreachJump;
        mVerticalVelocity = cBreachJumpVel;
        mBreachCount++;
        vfunc_47();
    }
}

bool Enm_TakolienVehicleSubmarine::takeDamage(f32 damage) {
    if (isDestroyed()) return false;

    mHealth -= damage;
    if (mHealth <= 0.0f) {
        mHealth = 0.0f;
        mState = VehicleSubmarineState::cBreak;
        vfunc_52();
        return true;
    }
    return false;
}

void Enm_TakolienVehicleSubmarine::update() {
    if (isDestroyed()) return;

    switch (mState) {
        case VehicleSubmarineState::cSubmerged:
        case VehicleSubmarineState::cPeriscope:
            mPosition.x += mMoveDirection.x * cSwimSpeed * 0.1f;
            mPosition.z += mMoveDirection.z * cSwimSpeed * 0.1f;
            break;

        case VehicleSubmarineState::cBreachJump:
            mCurrentDepth += mVerticalVelocity * 0.1f;
            mVerticalVelocity -= 0.5f; // Gravity descent
            if (mVerticalVelocity <= 0.0f && mCurrentDepth >= 0.0f) {
                mState = VehicleSubmarineState::cSurfaced;
                mStateTimer = 60; // 1 second surfaced firing window
            }
            break;

        case VehicleSubmarineState::cSurfaced:
            if (--mStateTimer <= 0) {
                mState = VehicleSubmarineState::cSubmerged;
                mCurrentDepth = cSubmergedDepth;
            }
            break;

        default:
            break;
    }
}

void Enm_TakolienVehicleSubmarine::draw() {
    // Rendered via ModelSceneMgr
}

} // namespace Game
