#include "Game/MapObj/Obj_SighterTarget.h"
#include <cmath>
#include <fstream>
#include <vector>

namespace Game {

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Obj_SighterTarget::Obj_SighterTarget(SighterTargetType type)
    : mType(type)
{
    if (mType == SighterTargetType::cTarget_Strong) {
        mMaxHp = mParams.mHpStrong;
        mTargetScale = mParams.mScaleStrong;
    } else {
        mMaxHp = mParams.mHpNormal;
        mTargetScale = 1.0f;
    }
}

void Obj_SighterTarget::init() {
    mHealth = mMaxHp;
    mCurrentScale = mTargetScale;
    mState = SighterTargetState::cState_Ready;
    mBendAngle = 0.0f;
    mBendVelocity = 0.0f;
    mBendDir = sead::Vector3f(0.0f, 0.0f, 1.0f);
    mTimer = 0;
    mDamageCooldownTimer = 0;
}

bool Obj_SighterTarget::loadParams(const char* filePath) {
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

void Obj_SighterTarget::update() {
    switch (mState) {
    case SighterTargetState::cState_Ready: {
        // Auto-heal when not damaged for 120 frames
        if (mDamageCooldownTimer > 0) {
            mDamageCooldownTimer--;
        } else if (mHealth < mMaxHp) {
            mHealth += 1.0f;
            if (mHealth > mMaxHp) mHealth = mMaxHp;
        }
        break;
    }

    case SighterTargetState::cState_DamageShot:
    case SighterTargetState::cState_DamageShotBend: {
        mTimer++;
        if (mDamageCooldownTimer > 0) mDamageCooldownTimer--;

        // Damped harmonic tether spring physics
        float springForce = -mBendAngle * mParams.mBendKp;
        float dampingForce = -mBendVelocity * mParams.mBendKd;
        mBendVelocity += springForce + dampingForce;
        mBendAngle += mBendVelocity;

        if (mTimer >= 30 && std::abs(mBendAngle) < 0.01f && std::abs(mBendVelocity) < 0.01f) {
            mState = SighterTargetState::cState_Ready;
            mBendAngle = 0.0f;
            mBendVelocity = 0.0f;
            mTimer = 0;
        }
        break;
    }

    case SighterTargetState::cState_Burst: {
        mTimer++;
        mCurrentScale = 0.0f;
        if (mTimer >= mParams.mBurstWaitFrame) {
            mState = SighterTargetState::cState_Respawn;
            mTimer = 0;
            mHealth = mMaxHp;
        }
        break;
    }

    case SighterTargetState::cState_Respawn: {
        mTimer++;
        // Expand inflation curve over 20 frames
        float progress = static_cast<float>(mTimer) / 20.0f;
        if (progress > 1.0f) progress = 1.0f;
        mCurrentScale = mTargetScale * std::sin(progress * static_cast<float>(M_PI * 0.5));

        if (mTimer >= 20) {
            mCurrentScale = mTargetScale;
            mState = SighterTargetState::cState_Ready;
            mTimer = 0;
            mDamageCooldownTimer = 0;
        }
        break;
    }
    }
}

bool Obj_SighterTarget::applyDamage(float damage, const sead::Vector3f& hitPos, const sead::Vector3f& shotDir, u32 teamId) {
    if (mState != SighterTargetState::cState_Ready &&
        mState != SighterTargetState::cState_DamageShot &&
        mState != SighterTargetState::cState_DamageShotBend) {
        return false;
    }

    mLastTeam = teamId;
    mDamageCooldownTimer = mParams.mNoDamageRefreshFrame;
    mHealth -= damage;

    // Shot deflection direction
    float dirLen = std::sqrt(shotDir.x * shotDir.x + shotDir.z * shotDir.z);
    if (dirLen > 0.01f) {
        mBendDir = sead::Vector3f(shotDir.x / dirLen, 0.0f, shotDir.z / dirLen);
    }

    if (mHealth <= 0.0f) {
        mHealth = 0.0f;
        mState = SighterTargetState::cState_Burst;
        mCurrentScale = 0.0f;
        mTimer = 0;
        return true;
    }

    // Heavy damage or high burst causes bend
    if (damage >= 40.0f) {
        mState = SighterTargetState::cState_DamageShotBend;
        mBendAngle = mParams.mMaxBendAngle;
        mBendVelocity = 0.06f;
    } else {
        mState = SighterTargetState::cState_DamageShot;
        mBendAngle = mParams.mMaxBendAngle * 0.5f;
        mBendVelocity = 0.04f;
    }
    mTimer = 0;
    return true;
}

bool Obj_SighterTarget::checkHit(const sead::Vector3f& shotPos, float shotRadius) const {
    if (mState == SighterTargetState::cState_Burst) return false;

    float r = (mType == SighterTargetType::cTarget_Strong)
        ? mParams.mColRadiusBulletStrong
        : mParams.mColRadiusBulletNormal;
    float h = (mType == SighterTargetType::cTarget_Strong)
        ? mParams.mColHeightBulletStrong
        : mParams.mColHeightBulletNormal;

    float dx = shotPos.x - mPosition.x;
    float dz = shotPos.z - mPosition.z;
    float horizDistSq = dx * dx + dz * dz;
    float combinedRadius = r + shotRadius;

    if (horizDistSq <= (combinedRadius * combinedRadius)) {
        if (shotPos.y >= mPosition.y - shotRadius && shotPos.y <= mPosition.y + h + shotRadius) {
            return true;
        }
    }
    return false;
}

} // namespace Game
