#include "Game/MapObj/Obj_GateManhole.h"
#include <fstream>
#include <sstream>
#include <cmath>

namespace Game {

Obj_GateManhole::Obj_GateManhole(const std::string& destinationStage, bool isBossKettle)
    : mDestinationStage(destinationStage), mIsBossKettle(isBossKettle) {
    if (mIsBossKettle) {
        mName = "Obj_BossGateway";
    }
}

void Obj_GateManhole::init() {
    if (mIsBossKettle) {
        loadBossParams();
    } else {
        loadParams();
    }
    mState = GateManholeState::cState_Hidden;
    mWarpTriggered = false;
    mWarpTimer = 0;
}

void Obj_GateManhole::update() {
    if (mState == GateManholeState::cState_Warping) {
        if (mWarpTimer > 0) {
            --mWarpTimer;
        }
        if (mWarpTimer <= 0) {
            mWarpTriggered = true;
        }
    }
}

bool Obj_GateManhole::loadParams(const char* filePath) {
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
                    if (currentParam == "mWarpColRadius") {
                        mParams.mWarpColRadius = val;
                    } else if (currentParam == "mCommanderSearchProbability") {
                        mParams.mCommanderSearchProbability = static_cast<int>(val);
                    } else if (currentParam == "mCommanderNoSearchFrame") {
                        mParams.mCommanderNoSearchFrame = static_cast<int>(val);
                    }
                } catch (...) {}
                currentParam.clear();
            }
        }
    }
    return true;
}

bool Obj_GateManhole::loadBossParams(const char* filePath) {
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
                    if (currentParam == "mWarpColRadius") {
                        mParams.mWarpColRadius = val;
                    } else if (currentParam == "mStageIconOffsetY") {
                        mParams.mStageIconOffsetY = val;
                    } else if (currentParam == "mOpenDemoWaitFrame") {
                        mParams.mOpenDemoWaitFrame = static_cast<int>(val);
                    } else if (currentParam == "mInWaitFrame") {
                        mParams.mInWaitFrame = static_cast<int>(val);
                    }
                } catch (...) {}
                currentParam.clear();
            }
        }
    }
    return true;
}

void Obj_GateManhole::reveal() {
    if (mState == GateManholeState::cState_Hidden) {
        mState = GateManholeState::cState_Revealed;
    }
}

void Obj_GateManhole::setCleared(bool cleared) {
    if (cleared) {
        mState = GateManholeState::cState_Cleared;
    } else {
        mState = GateManholeState::cState_Revealed;
    }
}

bool Obj_GateManhole::tryEnterWarp(const sead::Vector3f& playerPos, bool isSquidForm) {
    if (mState == GateManholeState::cState_Hidden) {
        return false;
    }
    if (!isSquidForm) {
        return false;
    }

    float dx = playerPos.x - mPosition.x;
    float dz = playerPos.z - mPosition.z;
    float distSq = dx * dx + dz * dz;

    if (distSq <= mParams.mWarpColRadius * mParams.mWarpColRadius) {
        mState = GateManholeState::cState_Warping;
        mWarpTimer = mParams.mInWaitFrame > 0 ? mParams.mInWaitFrame : 35;
        mWarpTriggered = false;
        return true;
    }
    return false;
}

} // namespace Game
