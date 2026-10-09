#include "Game/Enemy/Obj_CylinderKingBall.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

namespace Game {

Obj_CylinderKingBall::Obj_CylinderKingBall(CylinderBallType type)
    : mType(type) {
}

void Obj_CylinderKingBall::init() {
    if (mType == CylinderBallType::Small) {
        loadParamsSmall();
    } else if (mType == CylinderBallType::Big) {
        loadParamsBig();
    } else {
        loadParams();
    }
    mHealth = mParams.mLife;
    mCurrentSpeed = mParams.mVel;
    mState = CylinderBallState::Rolling;
    mFrameCounter = 0;
    mTrailDroppedThisFrame = false;
    mTotalTrailsDropped = 0;
}

void Obj_CylinderKingBall::update() {
    if (mState == CylinderBallState::Popped) {
        mTrailDroppedThisFrame = false;
        return;
    }

    float targetVel = mIsOnPlayerInk ? mParams.mVelOnPlayerInk : mParams.mVel;
    float rate = mIsOnPlayerInk ? (mParams.mAccOnPlayerInk * 0.2f) : (mParams.mAcc * 0.2f);
    mCurrentSpeed = mCurrentSpeed + (targetVel - mCurrentSpeed) * rate;

    mPosition.x += mDirection.x * mCurrentSpeed;
    mPosition.y += mDirection.y * mCurrentSpeed;
    mPosition.z += mDirection.z * mCurrentSpeed;

    ++mFrameCounter;
    int interval = mParams.mTrackPaintableRepeatFrame > 0 ? mParams.mTrackPaintableRepeatFrame : 3;
    if (mFrameCounter % interval == 0) {
        mTrailDroppedThisFrame = true;
        ++mTotalTrailsDropped;
    } else {
        mTrailDroppedThisFrame = false;
    }
}

bool Obj_CylinderKingBall::loadParams(const char* filePath) {
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
                    } else if (currentParam == "mGndColRadius") {
                        mParams.mGndColRadius = val;
                    } else if (currentParam == "mVel") {
                        mParams.mVel = val;
                        mCurrentSpeed = val;
                    } else if (currentParam == "mVelOnPlayerInk") {
                        mParams.mVelOnPlayerInk = val;
                    } else if (currentParam == "mAcc") {
                        mParams.mAcc = val;
                    } else if (currentParam == "mAccOnPlayerInk") {
                        mParams.mAccOnPlayerInk = val;
                    } else if (currentParam == "mPlayerDamage") {
                        mParams.mPlayerDamage = val;
                    } else if (currentParam == "mImpactToPlayer") {
                        mParams.mImpactToPlayer = val;
                    } else if (currentParam == "mDiePaintRadius") {
                        mParams.mDiePaintRadius = val;
                    } else if (currentParam == "mTrackPaintableRepeatFrame") {
                        mParams.mTrackPaintableRepeatFrame = static_cast<int>(val);
                    }
                } catch (...) {}
                currentParam.clear();
            }
        }
    }
    return true;
}

bool Obj_CylinderKingBall::loadParamsSmall(const char* filePath) {
    return loadParams(filePath);
}

bool Obj_CylinderKingBall::loadParamsBig(const char* filePath) {
    return loadParams(filePath);
}

void Obj_CylinderKingBall::setRollingDirection(const sead::Vector3f& dir) {
    float len = dir.length();
    if (len > 0.001f) {
        mDirection = dir * (1.0f / len);
    }
}

bool Obj_CylinderKingBall::applyDamage(float damage) {
    if (mState == CylinderBallState::Popped) {
        return false;
    }

    mHealth -= damage;
    if (mHealth <= 0.0f) {
        mHealth = 0.0f;
        mState = CylinderBallState::Popped;
        return true;
    }
    return false;
}

bool Obj_CylinderKingBall::checkPlayerContact(const sead::Vector3f& playerPos, float& outDamage, sead::Vector3f& outKnockback) {
    if (mState == CylinderBallState::Popped) {
        return false;
    }

    float dx = playerPos.x - mPosition.x;
    float dy = playerPos.y - mPosition.y;
    float dz = playerPos.z - mPosition.z;
    float dist = std::sqrt(dx * dx + dy * dy + dz * dz);

    if (dist <= mParams.mGndColRadius) {
        outDamage = mParams.mPlayerDamage;
        if (dist > 0.001f) {
            outKnockback = sead::Vector3f(dx / dist, 0.2f, dz / dist) * mParams.mImpactToPlayer;
        } else {
            outKnockback = sead::Vector3f(0.0f, 0.2f, 1.0f) * mParams.mImpactToPlayer;
        }
        return true;
    }
    return false;
}

} // namespace Game
