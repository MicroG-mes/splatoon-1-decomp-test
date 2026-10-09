#include "Game/MapObj/Obj_Tree00.h"
#include <cmath>
#include <fstream>
#include <vector>

namespace Game {

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Obj_Tree00::Obj_Tree00(int variant)
    : mVariant(variant)
{
    mName = (variant == 1) ? "Obj_Tree01" : "Obj_Tree00";
}

void Obj_Tree00::init() {
    mState = TreeState::cState_Wait;
    mSwayAngle = 0.0f;
    mSwayVelocity = 0.0f;
    mSwayDir = sead::Vector3f(0.0f, 0.0f, 1.0f);
    mTimer = 0;
    mWindPhase = std::fmod(std::abs(mPosition.x * 37.11f + mPosition.z * 91.73f), static_cast<float>(M_PI * 2.0));
}

bool Obj_Tree00::loadParams(const char* filePath) {
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

void Obj_Tree00::update() {
    switch (mState) {
    case TreeState::cState_Wait: {
        mTimer++;
        // Idle ambient tree sway: slow gentle breathing oscillation
        float angle = static_cast<float>(mTimer) * 0.025f + mWindPhase;
        mSwayAngle = std::sin(angle) * 0.035f;
        mSwayDir = sead::Vector3f(std::cos(mWindPhase), 0.0f, std::sin(mWindPhase));
        break;
    }

    case TreeState::cState_DamageShot:
    case TreeState::cState_DamageShotBend: {
        mTimer++;
        // Spring damped oscillation
        float springForce = -mSwayAngle * mParams.mSpringKp;
        float dampingForce = -mSwayVelocity * mParams.mSpringKd;
        mSwayVelocity += springForce + dampingForce;
        mSwayAngle += mSwayVelocity;

        if (mTimer >= mParams.mRecoveryFrames && std::abs(mSwayAngle) < 0.005f && std::abs(mSwayVelocity) < 0.005f) {
            mState = TreeState::cState_Wait;
            mSwayAngle = 0.0f;
            mSwayVelocity = 0.0f;
            mTimer = 0;
        }
        break;
    }
    }
}

bool Obj_Tree00::applyShotDamage(const sead::Vector3f& hitPos, const sead::Vector3f& shotDir) {
    // Check hit against cylinder trunk and canopy sphere
    float dx = hitPos.x - mPosition.x;
    float dz = hitPos.z - mPosition.z;
    float horizDistSq = dx * dx + dz * dz;

    bool hitTrunk = (horizDistSq <= (mParams.mTrunkRadius * mParams.mTrunkRadius) &&
                     hitPos.y >= mPosition.y && hitPos.y <= mPosition.y + mParams.mTrunkHeight);

    float dyCanopy = hitPos.y - (mPosition.y + mParams.mTrunkHeight);
    float canopyDistSq = horizDistSq + dyCanopy * dyCanopy;
    bool hitCanopy = (canopyDistSq <= (mParams.mCanopyRadius * mParams.mCanopyRadius));

    if (hitTrunk || hitCanopy) {
        float dirLen = std::sqrt(shotDir.x * shotDir.x + shotDir.z * shotDir.z);
        if (dirLen > 0.01f) {
            mSwayDir = sead::Vector3f(shotDir.x / dirLen, 0.0f, shotDir.z / dirLen);
        }

        if (hitCanopy) {
            mState = TreeState::cState_DamageShotBend;
            mSwayAngle = mParams.mMaxSwayAngle;
            mSwayVelocity = 0.04f;
        } else {
            mState = TreeState::cState_DamageShot;
            mSwayAngle = mParams.mMaxSwayAngle * 0.5f;
            mSwayVelocity = 0.08f;
        }
        mTimer = 0;
        return true;
    }
    return false;
}

} // namespace Game
