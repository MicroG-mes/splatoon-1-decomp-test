#include "Game/Enemy/Enm_TakolienVehicleMove.h"
#include <cmath>
#include <algorithm>

namespace Game {

Enm_TakolienVehicleMove::Enm_TakolienVehicleMove()
    : mPosition(0.0f, 0.0f, 0.0f)
    , mMoveDirection(0.0f, 0.0f, 1.0f)
    , mPatrolPointA(0.0f, 0.0f, 0.0f)
    , mPatrolPointB(0.0f, 0.0f, 10.0f)
    , mPatrolForward(true)
    , mHealth(cMaxHealth)
    , mState(VehicleMoveState::cPatrol)
    , mStridePhase(0.0f)
    , mActionTimer(0)
    , mShotsFired(0)
    , mIsPilotEjected(false)
{
    std::fill(mReserved, mReserved + sizeof(mReserved), 0);
}

Enm_TakolienVehicleMove::~Enm_TakolienVehicleMove() {
}

void Enm_TakolienVehicleMove::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mMoveDirection.set(0.0f, 0.0f, 1.0f);
    mPatrolPointA.set(0.0f, 0.0f, 0.0f);
    mPatrolPointB.set(0.0f, 0.0f, 10.0f);
    mPatrolForward = true;
    mHealth = cMaxHealth;
    mState = VehicleMoveState::cPatrol;
    mStridePhase = 0.0f;
    mActionTimer = 0;
    mShotsFired = 0;
    mIsPilotEjected = false;
}

void Enm_TakolienVehicleMove::vfunc_3() {
    // 0x1008B750: Model loading (Enm_TakolienVehicleMove.szs, 6,248 vertices)
}

void Enm_TakolienVehicleMove::vfunc_5() {
    // Parameter initialization
    mHealth = cMaxHealth;
    mState = VehicleMoveState::cPatrol;
}

void Enm_TakolienVehicleMove::vfunc_7() {
    // Bipedal motion tick & pursuit AI
    update();
}

void Enm_TakolienVehicleMove::vfunc_47() {
    // Rapid bubble cannon volley
    mShotsFired++;
}

void Enm_TakolienVehicleMove::vfunc_52() {
    // Mech frame destruction & pilot ejection
    mState = VehicleMoveState::cEject;
    mIsPilotEjected = true;
}

void Enm_TakolienVehicleMove::spawn(const sead::Vector3f& pos, const sead::Vector3f& patrolTarget) {
    mPosition = pos;
    mPatrolPointA = pos;
    mPatrolPointB = patrolTarget;
    mPatrolForward = true;
    mHealth = cMaxHealth;
    mState = VehicleMoveState::cPatrol;
    mStridePhase = 0.0f;
    mActionTimer = 0;
    mShotsFired = 0;
    mIsPilotEjected = false;

    sead::Vector3f diff = mPatrolPointB - mPatrolPointA;
    f32 len = std::sqrt(diff.x * diff.x + diff.z * diff.z);
    if (len > 0.001f) {
        mMoveDirection.set(diff.x / len, 0.0f, diff.z / len);
    }
}

void Enm_TakolienVehicleMove::updateAi(const sead::Vector3f& playerPos) {
    if (isDestroyed()) return;

    f32 dx = playerPos.x - mPosition.x;
    f32 dz = playerPos.z - mPosition.z;
    f32 distH = std::sqrt(dx * dx + dz * dz);

    if (distH <= cAttackRange) {
        mState = VehicleMoveState::cShoot;
        mMoveDirection.set(dx / distH, 0.0f, dz / distH);
    } else if (distH <= cDetectRange) {
        mState = VehicleMoveState::cChase;
        mMoveDirection.set(dx / distH, 0.0f, dz / distH);
    } else {
        mState = VehicleMoveState::cPatrol;
    }
}

bool Enm_TakolienVehicleMove::takeDamage(f32 damage) {
    if (isDestroyed()) return false;

    mHealth -= damage;
    if (mHealth <= 0.0f) {
        mHealth = 0.0f;
        mState = VehicleMoveState::cBreak;
        vfunc_52();
        return true;
    }
    return false;
}

void Enm_TakolienVehicleMove::update() {
    if (isDestroyed()) return;

    switch (mState) {
        case VehicleMoveState::cPatrol: {
            const sead::Vector3f& target = mPatrolForward ? mPatrolPointB : mPatrolPointA;
            f32 dx = target.x - mPosition.x;
            f32 dz = target.z - mPosition.z;
            f32 dist = std::sqrt(dx * dx + dz * dz);

            if (dist < 0.5f) {
                mPatrolForward = !mPatrolForward;
            } else {
                mMoveDirection.set(dx / dist, 0.0f, dz / dist);
                mPosition.x += mMoveDirection.x * cWalkSpeed * 0.1f;
                mPosition.z += mMoveDirection.z * cWalkSpeed * 0.1f;
                mStridePhase += 0.15f;
            }
            break;
        }

        case VehicleMoveState::cChase: {
            mPosition.x += mMoveDirection.x * cChaseSpeed * 0.1f;
            mPosition.z += mMoveDirection.z * cChaseSpeed * 0.1f;
            mStridePhase += 0.25f;
            break;
        }

        case VehicleMoveState::cShoot: {
            if (++mActionTimer >= 20) {
                vfunc_47();
                mActionTimer = 0;
            }
            break;
        }

        default:
            break;
    }
}

void Enm_TakolienVehicleMove::draw() {
    // Rendered via ModelSceneMgr
}

} // namespace Game
