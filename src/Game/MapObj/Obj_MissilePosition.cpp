#include "Game/MapObj/Obj_MissilePosition.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

namespace Game {

Obj_MissilePosition::Obj_MissilePosition() {
    mName = "Obj_MissilePosition";
}

void Obj_MissilePosition::init() {
    loadParams();
    mState = State::cWait;
    mOccupantPlayerId = -1;
    mBindFrameCounter = 0;
    mShotCooldownTimer = 0;
    mOverheatTimer = 0;
    mHeatRate = 0.0f;
    mIsOverheated = false;
    mFiredMissileCount = 0;
}

void Obj_MissilePosition::update() {
    if (mShotCooldownTimer > 0) {
        --mShotCooldownTimer;
    }

    if (mState == State::cShot) {
        // After firing burst, return to cockpit bind state
        if (mShotCooldownTimer <= mParams.mMinShotInterval - 10) {
            mState = State::cBind;
        }
    }

    if (mState == State::cBind) {
        ++mBindFrameCounter;
    }

    // VS Heat mechanics
    if (mIsVsMode) {
        if (mIsOverheated) {
            if (mOverheatTimer > 0) {
                --mOverheatTimer;
            }
            if (mOverheatTimer <= 0) {
                mIsOverheated = false;
                mHeatRate = 0.0f;
            }
        } else if (mHeatRate > 0.0f) {
            // Decay heat over cooling frame duration
            float decayPerFrame = 100.0f / (mParams.mCoolingFrame > 0 ? (float)mParams.mCoolingFrame : 600.0f);
            mHeatRate = std::max(0.0f, mHeatRate - decayPerFrame);
        }
    }
}

bool Obj_MissilePosition::loadParams(const char* filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return false;
    }

    mIsVsMode = false;
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
                    if (currentParam == "mBindNoShootFrame") {
                        mParams.mBindNoShootFrame = static_cast<int>(val);
                    } else if (currentParam == "mMissileSpeed") {
                        mParams.mMissileSpeed = val;
                    } else if (currentParam == "mMinShotInterval") {
                        mParams.mMinShotInterval = static_cast<int>(val);
                    } else if (currentParam == "mActionGuideRadius") {
                        mParams.mActionGuideRadius = val;
                    } else if (currentParam == "mActionGuideOffsetY") {
                        mParams.mActionGuideOffsetY = val;
                    } else if (currentParam == "mCoolingFrame") {
                        mParams.mCoolingFrame = static_cast<int>(val);
                    } else if (currentParam == "mCoolingFrameOverheat") {
                        mParams.mCoolingFrameOverheat = static_cast<int>(val);
                    } else if (currentParam == "mHeatRatePerShoot") {
                        mParams.mHeatRatePerShoot = val;
                    }
                } catch (...) {}
                currentParam.clear();
            }
        }
    }
    return true;
}

bool Obj_MissilePosition::loadParamsVS(const char* filePath) {
    bool ok = loadParams(filePath);
    if (ok) {
        mIsVsMode = true;
    }
    return ok;
}

bool Obj_MissilePosition::enterCockpit(int playerId) {
    if (mState != State::cWait) {
        return false;
    }
    mState = State::cBind;
    mOccupantPlayerId = playerId;
    mBindFrameCounter = 0;
    return true;
}

void Obj_MissilePosition::exitCockpit() {
    mState = State::cWait;
    mOccupantPlayerId = -1;
    mBindFrameCounter = 0;
}

void Obj_MissilePosition::setAim(float pitchDeg, float yawDeg) {
    mBones.mBarrelPitchDeg = std::clamp(pitchDeg, -mParams.mMuzzlePitchRangeDeg, mParams.mMuzzlePitchRangeDeg);
    mBones.mTurretYawDeg = yawDeg;
}

bool Obj_MissilePosition::fireMissile(const sead::Vector3f& targetPos) {
    if (mState != State::cBind) {
        return false;
    }
    // Check warm-up period after entry
    if (mBindFrameCounter < mParams.mBindNoShootFrame) {
        return false;
    }
    // Check refire cooldown
    if (mShotCooldownTimer > 0) {
        return false;
    }
    // Check VS overheat
    if (mIsVsMode && mIsOverheated) {
        return false;
    }

    mState = State::cShot;
    mShotCooldownTimer = mParams.mMinShotInterval;
    ++mFiredMissileCount;

    // Apply heat in VS mode
    if (mIsVsMode) {
        mHeatRate += mParams.mHeatRatePerShoot;
        if (mHeatRate >= 100.0f) {
            mHeatRate = 100.0f;
            mIsOverheated = true;
            mOverheatTimer = mParams.mCoolingFrameOverheat;
        }
    }

    return true;
}

bool Obj_MissilePosition::isPlayerInGuideRadius(const sead::Vector3f& playerPos) const {
    float dx = playerPos.x - mPosition.x;
    float dz = playerPos.z - mPosition.z;
    float distSq = dx * dx + dz * dz;
    return distSq <= (mParams.mActionGuideRadius * mParams.mActionGuideRadius);
}

sead::Vector3f Obj_MissilePosition::getGuideMarkerPos() const {
    return sead::Vector3f(mPosition.x, mPosition.y + mParams.mActionGuideOffsetY, mPosition.z);
}

} // namespace Game
