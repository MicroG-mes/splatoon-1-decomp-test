#include "Game/Enemy/Obj_ZakoPointUFO.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

namespace Game {

Obj_ZakoPointUFO::Obj_ZakoPointUFO(UfoType type)
    : mUfoType(type) {
    mName = "Obj_ZakoPointUFO";
}

void Obj_ZakoPointUFO::init() {
    loadParams();
    mHealth = mParams.mLife;
    mHoverY = mParams.mCruiseAltitudeY;
    mPosition.y = mHoverY;
    mState = State::Hovering;
    mSpawnTimer = 0;
    mTotalSpawnedCount = 0;
    mActiveEnemyCount = 0;
}

void Obj_ZakoPointUFO::update() {
    if (mState == State::Destroyed) {
        return;
    }

    if (mState == State::Retreating) {
        mPosition.y += 0.5f;
        return;
    }

    ++mSpawnTimer;

    if (mState == State::Hovering) {
        // Floating hovering animation
        mPosition.y = mHoverY + 0.25f * std::sin(static_cast<float>(mSpawnTimer) * 0.05f);

        if (mSpawnTimer >= mParams.mSpawnIntervalFrames) {
            if (mActiveEnemyCount < mParams.mMaxActiveEnemies) {
                mState = State::Deploying;
            }
        }
    } else if (mState == State::Deploying) {
        // Spawn reinforcement wave
        ++mTotalSpawnedCount;
        ++mActiveEnemyCount;
        mSpawnTimer = 0;
        mState = State::CoolingDown;
    } else if (mState == State::CoolingDown) {
        if (mSpawnTimer >= 60) {
            mSpawnTimer = 0;
            mState = State::Hovering;
        }
    }
}

bool Obj_ZakoPointUFO::loadParams(const char* filePath) {
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
                        mHealth = val;
                    }
                } catch (...) {}
                currentParam.clear();
            }
        }
    }
    return true;
}

void Obj_ZakoPointUFO::applyDamage(float damage) {
    if (mState == State::Destroyed) {
        return;
    }

    mHealth -= damage;
    if (mHealth <= 0.0f) {
        mHealth = 0.0f;
        mState = State::Destroyed;
    }
}

bool Obj_ZakoPointUFO::triggerSpawnWave() {
    if (mState == State::Destroyed || mState == State::Retreating) {
        return false;
    }
    if (mActiveEnemyCount >= mParams.mMaxActiveEnemies) {
        return false;
    }

    ++mTotalSpawnedCount;
    ++mActiveEnemyCount;
    mSpawnTimer = 0;
    mState = State::CoolingDown;
    return true;
}

void Obj_ZakoPointUFO::onEnemyDefeated() {
    if (mActiveEnemyCount > 0) {
        --mActiveEnemyCount;
    }
}

} // namespace Game
