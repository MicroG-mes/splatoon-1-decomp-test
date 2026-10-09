#include "Game/MapObj/Obj_BombFlower.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

namespace Game {

Obj_BombFlower::Obj_BombFlower() {
}

void Obj_BombFlower::init() {
    loadParams();
    mHealth = mParams.mMaxHp;
    mState = BombFlowerState::cState_Ready;
    mLastHitterTeam = 0;
    mTimer = 0;
}

void Obj_BombFlower::update() {
    if (mState == BombFlowerState::cState_Burst) {
        mState = BombFlowerState::cState_Respawning;
        mTimer = 0;
    } else if (mState == BombFlowerState::cState_Respawning) {
        ++mTimer;
        if (mTimer >= mParams.mBurstWaitFrame) {
            mState = BombFlowerState::cState_Growing;
            mTimer = 0;
        }
    } else if (mState == BombFlowerState::cState_Growing) {
        ++mTimer;
        if (mTimer >= mParams.mAppearFrame) {
            mState = BombFlowerState::cState_Ready;
            mHealth = mParams.mMaxHp;
            mTimer = 0;
        }
    }
}

bool Obj_BombFlower::loadParams(const char* filePath) {
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
                    if (currentParam == "mMaxHp") {
                        mParams.mMaxHp = val;
                        mHealth = val;
                    } else if (currentParam == "mBalloonRadius") {
                        mParams.mBalloonRadius = val;
                    } else if (currentParam == "mBalloonOffsetY") {
                        mParams.mBalloonOffsetY = val;
                    } else if (currentParam == "mBurstWaitFrame") {
                        mParams.mBurstWaitFrame = static_cast<int>(val);
                    } else if (currentParam == "mAppearFrame") {
                        mParams.mAppearFrame = static_cast<int>(val);
                    } else if (currentParam == "mAppearWaitFrame") {
                        mParams.mAppearWaitFrame = static_cast<int>(val);
                    } else if (currentParam == "mBombCorePaintRadius") {
                        mParams.mBombCorePaintRadius = val;
                    } else if (currentParam == "mBombCoreDamageRadius") {
                        mParams.mBombCoreDamageRadius = val;
                    } else if (currentParam == "mBombCoreDamage") {
                        mParams.mBombCoreDamage = val;
                    } else if (currentParam == "mSplashNum") {
                        mParams.mSplashNum = static_cast<int>(val);
                    } else if (currentParam == "mBalloonKp") {
                        mParams.mBalloonKp = val;
                    } else if (currentParam == "mBalloonKd") {
                        mParams.mBalloonKd = val;
                    }
                } catch (...) {}
                currentParam.clear();
            }
        }
    }
    return true;
}

bool Obj_BombFlower::applyDamage(float damage, u32 teamId) {
    if (mState != BombFlowerState::cState_Ready) {
        return false;
    }

    mHealth -= damage;
    mLastHitterTeam = teamId;

    if (mHealth <= 0.0f) {
        mHealth = 0.0f;
        mState = BombFlowerState::cState_Burst;
        mTimer = 0;
    }
    return true;
}

bool Obj_BombFlower::checkBlastHit(const sead::Vector3f& targetPos, float& outDamage) const {
    sead::Vector3f center(mPosition.x, mPosition.y + mParams.mBalloonOffsetY, mPosition.z);
    float dx = targetPos.x - center.x;
    float dy = targetPos.y - center.y;
    float dz = targetPos.z - center.z;
    float dist = std::sqrt(dx * dx + dy * dy + dz * dz);

    if (dist <= mParams.mBombCoreDamageRadius) {
        float falloff = std::max(0.0f, 1.0f - (dist / mParams.mBombCoreDamageRadius));
        outDamage = mParams.mBombCoreDamage * falloff;
        return true;
    }
    outDamage = 0.0f;
    return false;
}

} // namespace Game
