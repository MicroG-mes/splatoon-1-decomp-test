#include "Game/MapObj/Obj_AreaGate.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

namespace Game {

Obj_AreaGate::Obj_AreaGate(int areaId, int requiredPowerMW)
    : mAreaId(areaId), mRequiredPowerMW(requiredPowerMW) {
}

void Obj_AreaGate::init() {
    loadParams();
    mState = AreaGateState::cState_Locked;
    mShakeTimer = 0;
}

void Obj_AreaGate::update() {
    if (mState == AreaGateState::cState_Shaking) {
        ++mShakeTimer;
        if (mShakeTimer >= mParams.mShakeCancelFrame) {
            mState = AreaGateState::cState_Opening;
            mShakeTimer = 0;
        }
    } else if (mState == AreaGateState::cState_Opening) {
        ++mShakeTimer;
        if (mShakeTimer >= 15) {
            mState = AreaGateState::cState_Opened;
        }
    }
}

bool Obj_AreaGate::loadParams(const char* filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    std::string currentParam;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        while (ss >> token) {
            if (token.front() == '"' && token.back() == '"') {
                currentParam = token.substr(1, token.length() - 2);
            } else if (!currentParam.empty() && token != "{" && token != "}") {
                try {
                    float val = std::stof(token);
                    if (currentParam == "mBarrierWidth") {
                        mParams.mBarrierWidth = val;
                    } else if (currentParam == "mBarrierHeight") {
                        mParams.mBarrierHeight = val;
                    } else if (currentParam == "mBarrierBoundVelLen") {
                        mParams.mBarrierBoundVelLen = val;
                    } else if (currentParam == "mBarrierBoundVelY") {
                        mParams.mBarrierBoundVelY = val;
                    } else if (currentParam == "mShakeCancelFrame") {
                        mParams.mShakeCancelFrame = static_cast<int>(val);
                    }
                } catch (...) {}
                currentParam.clear();
            }
        }
    }
    return true;
}

bool Obj_AreaGate::tryUnlock(int currentPowerMW) {
    if (mState != AreaGateState::cState_Locked) {
        return false;
    }
    if (currentPowerMW >= mRequiredPowerMW) {
        mState = AreaGateState::cState_Shaking;
        mShakeTimer = 0;
        return true;
    }
    return false;
}

bool Obj_AreaGate::checkPlayerCollision(const sead::Vector3f& playerPos, sead::Vector3f& outReboundVel) {
    if (mState != AreaGateState::cState_Locked) {
        return false;
    }

    float dx = playerPos.x - mPosition.x;
    float dy = playerPos.y - mPosition.y;
    float dz = playerPos.z - mPosition.z;

    // Check bounds: within width and height
    float halfWidth = mParams.mBarrierWidth * 0.5f;
    if (std::abs(dx) <= halfWidth && dy >= 0.0f && dy <= mParams.mBarrierHeight && std::abs(dz) <= 5.0f) {
        // Player pushed back along Z axis
        float pushDirZ = dz >= 0.0f ? 1.0f : -1.0f;
        outReboundVel.x = 0.0f;
        outReboundVel.y = mParams.mBarrierBoundVelY;
        outReboundVel.z = pushDirZ * mParams.mBarrierBoundVelLen;
        return true;
    }
    return false;
}

} // namespace Game
