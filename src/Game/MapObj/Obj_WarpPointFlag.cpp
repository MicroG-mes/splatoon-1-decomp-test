#include "Game/MapObj/Obj_WarpPointFlag.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

namespace Game {

Obj_WarpPointFlag::Obj_WarpPointFlag(int checkpointIndex)
    : mCheckpointIndex(checkpointIndex) {
}

void Obj_WarpPointFlag::init() {
    loadParams();
    mRemainingCharges = static_cast<int>(mParams.mLife);
    mState = WarpFlagState::Unchecked;
    mFlagRiseProgress = 0.0f;
}

void Obj_WarpPointFlag::update() {
    if (mState == WarpFlagState::Raised && mFlagRiseProgress < 1.0f) {
        mFlagRiseProgress = std::min(1.0f, mFlagRiseProgress + 0.05f);
    }
}

bool Obj_WarpPointFlag::loadParams(const char* filePath) {
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
                    if (currentParam == "mLife") {
                        mParams.mLife = val;
                        mRemainingCharges = static_cast<int>(val);
                    }
                } catch (...) {}
                currentParam.clear();
            }
        }
    }
    return true;
}

bool Obj_WarpPointFlag::tryActivate(const sead::Vector3f& playerPos, float touchRadius) {
    float dx = playerPos.x - mPosition.x;
    float dy = playerPos.y - mPosition.y;
    float dz = playerPos.z - mPosition.z;
    float dist = std::sqrt(dx * dx + dy * dy + dz * dz);

    if (dist <= touchRadius) {
        if (mState == WarpFlagState::Unchecked) {
            mState = WarpFlagState::Raised;
            mFlagRiseProgress = 0.0f;
            return true;
        }
    }
    return false;
}

bool Obj_WarpPointFlag::consumeRespawnCharge(sead::Vector3f& outRespawnPos) {
    if (mState != WarpFlagState::Raised || mRemainingCharges <= 0) {
        return false;
    }

    outRespawnPos = mPosition;
    --mRemainingCharges;
    if (mRemainingCharges <= 0) {
        mState = WarpFlagState::Exhausted;
    }
    return true;
}

} // namespace Game
