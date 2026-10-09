#include "Game/MapObj/Obj_RubberPole00.h"
#include <cmath>

namespace Game {

Obj_RubberPole00::Obj_RubberPole00()
{
}

void Obj_RubberPole00::init() {
    mState = RubberPoleState::cState_Idle;
    mDeflectionAngle = 0.0f;
    mDeflectionVelocity = 0.0f;
    mDeflectionDir = sead::Vector3f(0.0f, 0.0f, 1.0f);
    mTimer = 0;
}

void Obj_RubberPole00::update() {
    if (mState == RubberPoleState::cState_Deflected) {
        mTimer++;
        float springForce = -mDeflectionAngle * mParams.mSpringKp;
        float dampingForce = -mDeflectionVelocity * mParams.mSpringKd;
        mDeflectionVelocity += springForce + dampingForce;
        mDeflectionAngle += mDeflectionVelocity;

        if (mTimer >= 30 && (std::abs(mDeflectionAngle) < 0.02f || mTimer >= 50)) {
            mState = RubberPoleState::cState_Idle;
            mDeflectionAngle = 0.0f;
            mDeflectionVelocity = 0.0f;
            mTimer = 0;
        }
    }
}

bool Obj_RubberPole00::checkPlayerCollision(const sead::Vector3f& playerPos, float playerRadius, sead::Vector3f& outReboundVel) {
    float dx = playerPos.x - mPosition.x;
    float dz = playerPos.z - mPosition.z;
    float horizDistSq = dx * dx + dz * dz;

    float combinedRadius = mParams.mRadius + playerRadius;
    if (horizDistSq <= (combinedRadius * combinedRadius)) {
        if (playerPos.y >= mPosition.y && playerPos.y <= mPosition.y + mParams.mHeight) {
            float dist = std::sqrt(horizDistSq);
            sead::Vector3f normal = (dist > 0.001f)
                ? sead::Vector3f(dx / dist, 0.0f, dz / dist)
                : sead::Vector3f(0.0f, 0.0f, 1.0f);

            // Elastic rebound knockback
            outReboundVel = sead::Vector3f(normal.x * mParams.mReboundSpeed, 0.3f, normal.z * mParams.mReboundSpeed);

            // Pole deflects in player's approach direction
            mDeflectionDir = sead::Vector3f(-normal.x, 0.0f, -normal.z);
            mDeflectionAngle = mParams.mMaxDeflection;
            mDeflectionVelocity = -0.05f;
            mState = RubberPoleState::cState_Deflected;
            mTimer = 0;
            return true;
        }
    }
    return false;
}

bool Obj_RubberPole00::applyShotDamage(const sead::Vector3f& hitPos, const sead::Vector3f& shotDir) {
    float dx = hitPos.x - mPosition.x;
    float dz = hitPos.z - mPosition.z;
    float horizDistSq = dx * dx + dz * dz;

    if (horizDistSq <= (mParams.mRadius * mParams.mRadius * 2.0f)) {
        float dirLen = std::sqrt(shotDir.x * shotDir.x + shotDir.z * shotDir.z);
        if (dirLen > 0.01f) {
            mDeflectionDir = sead::Vector3f(shotDir.x / dirLen, 0.0f, shotDir.z / dirLen);
        }
        mDeflectionAngle = mParams.mMaxDeflection * 0.5f;
        mDeflectionVelocity = 0.08f;
        mState = RubberPoleState::cState_Deflected;
        mTimer = 0;
        return true;
    }
    return false;
}

} // namespace Game
