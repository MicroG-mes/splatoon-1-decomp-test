#include "Game/Enemy/Enm_TakolienVehicleFix.h"
#include <cmath>
#include <algorithm>

namespace Game {

Enm_TakolienVehicleFix::Enm_TakolienVehicleFix()
    : mPosition(0.0f, 0.0f, 0.0f)
    , mTargetPosition(0.0f, 0.0f, 0.0f)
    , mTurretYaw(0.0f)
    , mTurretPitch(0.0f)
    , mHealth(cMaxHealth)
    , mCanopyHealth(cCanopyShield)
    , mState(VehicleFixState::cIdle)
    , mFireTimer(0)
    , mShotsFired(0)
    , mIsPilotEjected(false)
{
    std::fill(mReserved, mReserved + sizeof(mReserved), 0);
}

Enm_TakolienVehicleFix::~Enm_TakolienVehicleFix() {
}

void Enm_TakolienVehicleFix::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mTargetPosition.set(0.0f, 0.0f, 0.0f);
    mTurretYaw = 0.0f;
    mTurretPitch = 0.0f;
    mHealth = cMaxHealth;
    mCanopyHealth = cCanopyShield;
    mState = VehicleFixState::cIdle;
    mFireTimer = 0;
    mShotsFired = 0;
    mIsPilotEjected = false;
}

void Enm_TakolienVehicleFix::vfunc_3() {
    // 0x1008C8A8: Model loading (Enm_TakolienVehicleFix.szs, 8,049 vertices)
}

void Enm_TakolienVehicleFix::vfunc_5() {
    // Parameter initialization
    mHealth = cMaxHealth;
    mCanopyHealth = cCanopyShield;
    mState = VehicleFixState::cIdle;
}

void Enm_TakolienVehicleFix::vfunc_7() {
    // Turret tracking & burst logic
    update();
}

void Enm_TakolienVehicleFix::vfunc_47() {
    // BulletEnemyBubbleShotFixedTakolien @ 0x02224038 discharge
    fireBubbleShot();
}

void Enm_TakolienVehicleFix::vfunc_52() {
    // Armor break & pilot ejection
    mState = VehicleFixState::cEject;
    mIsPilotEjected = true;
}

void Enm_TakolienVehicleFix::spawn(const sead::Vector3f& pos, f32 baseYaw) {
    mPosition = pos;
    mTargetPosition = pos;
    mTurretYaw = baseYaw;
    mTurretPitch = 0.0f;
    mHealth = cMaxHealth;
    mCanopyHealth = cCanopyShield;
    mState = VehicleFixState::cIdle;
    mFireTimer = 0;
    mShotsFired = 0;
    mIsPilotEjected = false;
}

void Enm_TakolienVehicleFix::updateAim(const sead::Vector3f& targetPos) {
    mTargetPosition = targetPos;
    f32 dx = targetPos.x - mPosition.x;
    f32 dy = targetPos.y - mPosition.y;
    f32 dz = targetPos.z - mPosition.z;
    f32 distH = std::sqrt(dx * dx + dz * dz);

    if (distH <= cMaxAimRange && distH > 0.1f) {
        mTurretYaw = std::atan2(dx, dz) * (180.0f / 3.14159265358979323846f);
        mTurretPitch = -std::atan2(dy, distH) * (180.0f / 3.14159265358979323846f);

        if (mState == VehicleFixState::cIdle) {
            mState = VehicleFixState::cAiming;
            mFireTimer = 15; // Aim lock duration
        }
    } else {
        if (mState == VehicleFixState::cAiming) {
            mState = VehicleFixState::cIdle;
        }
    }
}

void Enm_TakolienVehicleFix::fireBubbleShot() {
    mShotsFired++;
    mFireTimer = cFireCadenceFrames;
}

bool Enm_TakolienVehicleFix::takeDamage(f32 damage, bool hitCanopy) {
    if (isDestroyed()) return false;

    if (hitCanopy && mCanopyHealth > 0.0f) {
        mCanopyHealth -= damage;
        if (mCanopyHealth <= 0.0f) {
            mCanopyHealth = 0.0f;
        }
        return false;
    }

    mHealth -= damage;
    if (mHealth <= 0.0f) {
        mHealth = 0.0f;
        mState = VehicleFixState::cBreak;
        vfunc_52();
        return true;
    }
    return false;
}

void Enm_TakolienVehicleFix::update() {
    if (isDestroyed()) return;

    if (mState == VehicleFixState::cAiming) {
        if (--mFireTimer <= 0) {
            mState = VehicleFixState::cFiring;
            vfunc_47();
        }
    } else if (mState == VehicleFixState::cFiring) {
        if (--mFireTimer <= 0) {
            mState = VehicleFixState::cAiming;
            mFireTimer = cFireCadenceFrames;
        }
    }
}

void Enm_TakolienVehicleFix::draw() {
    // Rendered via ModelSceneMgr
}

} // namespace Game
