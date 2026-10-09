#include "Game/MapObj/Obj_Grass00.h"
#include <cmath>
#include <fstream>
#include <vector>

namespace Game {

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Obj_Grass00::Obj_Grass00(bool isRuins)
    : mIsRuins(isRuins)
{
    mName = mIsRuins ? "Obj_GrassRuins00" : "Obj_Grass00";
}

void Obj_Grass00::init() {
    mState = GrassState::cState_Wait_random;
    mBendAngle = 0.0f;
    mBendVelocity = 0.0f;
    mBendDir = sead::Vector3f(0.0f, 0.0f, 1.0f);
    mTimer = 0;
    // Pseudorandom phase based on initial coordinate
    mWindPhase = std::fmod(std::abs(mPosition.x * 12.9898f + mPosition.z * 78.233f), static_cast<float>(M_PI * 2.0));
}

bool Obj_Grass00::loadParams(const char* filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) return false;
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    if (size > 0) {
        std::vector<char> buf(static_cast<size_t>(size));
        file.read(buf.data(), size);
        return true;
    }
    return false;
}

void Obj_Grass00::update() {
    switch (mState) {
    case GrassState::cState_Wait_random: {
        mTimer++;
        // Idle gentle breeze sway
        float angle = static_cast<float>(mTimer) * 0.04f + mWindPhase;
        mBendAngle = std::sin(angle) * 0.06f;
        mBendDir = sead::Vector3f(std::cos(mWindPhase), 0.0f, std::sin(mWindPhase));
        break;
    }

    case GrassState::cState_Rub:
    case GrassState::cState_RubBend:
    case GrassState::cState_DamageShot:
    case GrassState::cState_DamageShotBend: {
        mTimer++;
        // Damped harmonic restoration back to neutral
        float springForce = -mBendAngle * mParams.mSpringKp;
        float dampingForce = -mBendVelocity * mParams.mSpringKd;
        mBendVelocity += springForce + dampingForce;
        mBendAngle += mBendVelocity;

        if (mTimer >= mParams.mRecoveryFrames && std::abs(mBendAngle) < 0.01f && std::abs(mBendVelocity) < 0.01f) {
            mState = GrassState::cState_Wait_random;
            mBendAngle = 0.0f;
            mBendVelocity = 0.0f;
            mTimer = 0;
        }
        break;
    }
    }
}

bool Obj_Grass00::checkPlayerTouch(const sead::Vector3f& playerPos, const sead::Vector3f& playerVel, bool isSquid) {
    float dx = playerPos.x - mPosition.x;
    float dz = playerPos.z - mPosition.z;
    float distSq = dx * dx + dz * dz;

    if (distSq <= (mParams.mTouchRadius * mParams.mTouchRadius)) {
        float speed = std::sqrt(playerVel.x * playerVel.x + playerVel.z * playerVel.z);
        if (speed > 0.01f) {
            mBendDir = sead::Vector3f(playerVel.x / speed, 0.0f, playerVel.z / speed);
        }

        if (isSquid || speed > 1.5f) {
            mState = GrassState::cState_RubBend;
            mBendAngle = mParams.mMaxBendAngle;
            mBendVelocity = 0.05f;
        } else {
            mState = GrassState::cState_Rub;
            mBendAngle = mParams.mMaxBendAngle * 0.5f;
            mBendVelocity = 0.02f;
        }
        mTimer = 0;
        return true;
    }
    return false;
}

bool Obj_Grass00::applyShotDamage(const sead::Vector3f& hitPos, const sead::Vector3f& shotDir) {
    float dx = hitPos.x - mPosition.x;
    float dz = hitPos.z - mPosition.z;
    float distSq = dx * dx + dz * dz;

    if (distSq <= (mParams.mShotHitRadius * mParams.mShotHitRadius)) {
        float dirLen = std::sqrt(shotDir.x * shotDir.x + shotDir.z * shotDir.z);
        if (dirLen > 0.01f) {
            mBendDir = sead::Vector3f(shotDir.x / dirLen, 0.0f, shotDir.z / dirLen);
        }
        mState = GrassState::cState_DamageShotBend;
        mBendAngle = mParams.mMaxBendAngle * 0.9f;
        mBendVelocity = 0.08f;
        mTimer = 0;
        return true;
    }
    return false;
}

} // namespace Game
