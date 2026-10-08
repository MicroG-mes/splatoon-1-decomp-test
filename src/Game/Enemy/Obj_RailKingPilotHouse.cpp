#include "Game/Enemy/Obj_RailKingPilotHouse.h"
#include "Game/System/AglParameter.h"
#include <cmath>
#include <algorithm>

namespace Game {

bool RailKingPilotHouseParams::load(const char* paramsPath, const char* anmPath) {
    damageFrame = 12;
    searchRadius = 100.00000000f;
    rotSpeedDeg = 1.00000000f;
    eyeMaxAngleDeg = 45.00000000f;
    eyeLerpRate = 0.10000000f;
    worldWait = 20.00000000f;
    worldWaitHit = 10.00000000f;

    if (paramsPath) {
        AglParameterObj obj;
        if (obj.loadFromFile(paramsPath)) {
            damageFrame = obj.getInt("mDamageFrame", damageFrame);
            searchRadius = obj.getFloat("mSearchRadius", searchRadius);
            rotSpeedDeg = obj.getFloat("mRotSpeedDeg", rotSpeedDeg);

            auto eyeArr = obj.getFloatArray("mEyeUVDirectableParam");
            if (eyeArr.size() >= 3) {
                eyeMaxAngleDeg = eyeArr[1];
                eyeLerpRate = eyeArr[2];
            }
        }
    }

    if (anmPath) {
        AglParameterObj anmObj;
        if (anmObj.loadFromFile(anmPath)) {
            worldWait = anmObj.getFloat("WorldWait", worldWait);
            worldWaitHit = anmObj.getFloat("WorldWaitHit", worldWaitHit);
        }
    }

    return true;
}

Obj_RailKingPilotHouse::Obj_RailKingPilotHouse()
    : mState(OctavioPilotState::cState_Idle)
    , mCurrentYawDeg(0.0f)
    , mTargetYawDeg(0.0f)
    , mEyeDeflectionDeg(0.0f)
    , mDamageTimer(0)
    , mShiokaraPhase(0)
    , mGrooveTimer(0.0f) {
    mPosition.set(0.0f, 0.0f, 0.0f);
    mParams.load("content/Static/Obj_RailKingPilotHouse.params",
                 "content/Static/Obj_RailKingPilotHouse_Pilot_AnmItp.params");
}

Obj_RailKingPilotHouse::~Obj_RailKingPilotHouse() {}

void Obj_RailKingPilotHouse::init() {
    init(mPosition);
}

void Obj_RailKingPilotHouse::init(const sead::Vector3f& pos) {
    mPosition = pos;
    mState = OctavioPilotState::cState_Idle;
    mCurrentYawDeg = 0.0f;
    mTargetYawDeg = 0.0f;
    mEyeDeflectionDeg = 0.0f;
    mDamageTimer = 0;
    mShiokaraPhase = 0;
    mGrooveTimer = 0.0f;
    mParams.load("content/Static/Obj_RailKingPilotHouse.params",
                 "content/Static/Obj_RailKingPilotHouse_Pilot_AnmItp.params");
}

bool Obj_RailKingPilotHouse::trackPlayer(const sead::Vector3f& playerPos) {
    if (mState == OctavioPilotState::cState_Damaged || mState == OctavioPilotState::cState_Defeated) {
        return false;
    }

    f32 dx = playerPos.x - mPosition.x;
    f32 dz = playerPos.z - mPosition.z;
    f32 dist = std::sqrt(dx * dx + dz * dz);

    if (dist <= mParams.searchRadius) {
        mTargetYawDeg = std::atan2(dx, dz) * (180.0f / 3.14159265f);
        if (mState != OctavioPilotState::cState_ShiokaraHypnotized) {
            mState = OctavioPilotState::cState_TrackingPlayer;
        }
        return true;
    }

    if (mState == OctavioPilotState::cState_TrackingPlayer) {
        mState = OctavioPilotState::cState_Idle;
    }
    return false;
}

void Obj_RailKingPilotHouse::takePunchDamage() {
    mState = OctavioPilotState::cState_Damaged;
    mDamageTimer = mParams.damageFrame;
}

void Obj_RailKingPilotHouse::startShiokaraGroove(u32 phase) {
    mState = OctavioPilotState::cState_ShiokaraHypnotized;
    mShiokaraPhase = phase;
    mGrooveTimer = 0.0f;
}

void Obj_RailKingPilotHouse::update() {
    if (mState == OctavioPilotState::cState_Damaged) {
        if (mDamageTimer > 0) {
            mDamageTimer--;
        } else {
            mState = (mShiokaraPhase > 0) ? OctavioPilotState::cState_ShiokaraHypnotized : OctavioPilotState::cState_Idle;
        }
        return;
    }

    // Rotate toward target yaw with authentic 1.0 deg/f clamped speed
    f32 diffYaw = mTargetYawDeg - mCurrentYawDeg;
    while (diffYaw > 180.0f) diffYaw -= 360.0f;
    while (diffYaw < -180.0f) diffYaw += 360.0f;

    if (std::abs(diffYaw) > 0.01f) {
        f32 step = std::clamp(diffYaw, -mParams.rotSpeedDeg, mParams.rotSpeedDeg);
        mCurrentYawDeg += step;
    }

    // Eye UV directable deflection towards target
    f32 targetEye = std::clamp(diffYaw, -mParams.eyeMaxAngleDeg, mParams.eyeMaxAngleDeg);
    mEyeDeflectionDeg += (targetEye - mEyeDeflectionDeg) * mParams.eyeLerpRate;

    // Hypnotic head-bobbing rhythm during Calamari Inkantation
    if (mState == OctavioPilotState::cState_ShiokaraHypnotized) {
        mGrooveTimer += 0.1f;
    }
}

} // namespace Game
