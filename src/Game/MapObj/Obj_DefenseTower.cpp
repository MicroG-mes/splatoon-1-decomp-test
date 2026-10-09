#include "Game/MapObj/Obj_DefenseTower.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

namespace Game {

Obj_DefenseTower::Obj_DefenseTower() {
}

void Obj_DefenseTower::init() {
    loadParams();
    mHealth = mParams.mLife;
    mState = DefenseTowerState::Intact;
    mDamageCooldownTimer = 0;
}

void Obj_DefenseTower::update() {
    if (mState == DefenseTowerState::Destroyed) {
        return;
    }

    if (mDamageCooldownTimer > 0) {
        --mDamageCooldownTimer;
    } else if (mHealth < mParams.mLife) {
        // Recover health at curable rate
        mHealth = std::min(mParams.mLife, mHealth + mParams.mCurableRate * (1.0f / 60.0f));
        if (mHealth >= mParams.mLife) {
            mState = DefenseTowerState::Intact;
        }
    }
}

bool Obj_DefenseTower::loadParams(const char* filePath) {
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
                    } else if (currentParam == "mCurableRate") {
                        mParams.mCurableRate = val;
                    }
                } catch (...) {}
                currentParam.clear();
            }
        }
    }
    return true;
}

bool Obj_DefenseTower::applyDamage(float damage) {
    if (mState == DefenseTowerState::Destroyed) {
        return false;
    }

    mHealth -= damage;
    mDamageCooldownTimer = 120; // 2 seconds delay before regeneration starts

    if (mHealth <= 0.0f) {
        mHealth = 0.0f;
        mState = DefenseTowerState::Destroyed;
    } else {
        mState = DefenseTowerState::Damaged;
    }
    return true;
}

void Obj_DefenseTower::repair(float amount) {
    if (mState == DefenseTowerState::Destroyed) {
        return;
    }
    mHealth = std::min(mParams.mLife, mHealth + amount);
    if (mHealth >= mParams.mLife) {
        mState = DefenseTowerState::Intact;
    }
}

} // namespace Game
