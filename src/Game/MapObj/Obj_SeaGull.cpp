#include "Game/MapObj/Obj_SeaGull.h"
#include <cmath>
#include <fstream>
#include <vector>

namespace Game {

Obj_SeaGull::Obj_SeaGull()
{
}

void Obj_SeaGull::init() {
    mState = SeaGullState::cState_Wait_random;
    mFlightVelocity = sead::Vector3f(0.0f, 0.0f, 0.0f);
    mTimer = 0;
    mPerchY = mPosition.y;
}

bool Obj_SeaGull::loadParams(const char* filePath) {
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

void Obj_SeaGull::triggerTakeOff(const sead::Vector3f& escapeDir) {
    if (mState != SeaGullState::cState_Wait_random) return;

    mState = SeaGullState::cState_TakeOff;
    mTimer = 0;

    float len = std::sqrt(escapeDir.x * escapeDir.x + escapeDir.z * escapeDir.z);
    sead::Vector3f normDir = (len > 0.01f)
        ? sead::Vector3f(escapeDir.x / len, 0.0f, escapeDir.z / len)
        : sead::Vector3f(0.0f, 0.0f, 1.0f);

    mFlightVelocity = sead::Vector3f(
        normDir.x * mParams.mFlySpeed * 0.6f,
        mParams.mAscentSpeed,
        normDir.z * mParams.mFlySpeed * 0.6f
    );
}

void Obj_SeaGull::update() {
    switch (mState) {
    case SeaGullState::cState_Wait_random: {
        mTimer++;
        // Idle head bobbing & preening
        break;
    }

    case SeaGullState::cState_TakeOff: {
        mTimer++;
        mPosition.x += mFlightVelocity.x;
        mPosition.y += mFlightVelocity.y;
        mPosition.z += mFlightVelocity.z;

        if (mTimer >= mParams.mTakeOffFrames) {
            mState = SeaGullState::cState_Fly;
            // Level out flight into soaring cruise
            mFlightVelocity.y = 0.5f; // Gentle steady climb
            mFlightVelocity.x *= 1.4f;
            mFlightVelocity.z *= 1.4f;
            mTimer = 0;
        }
        break;
    }

    case SeaGullState::cState_Fly: {
        mTimer++;
        mPosition.x += mFlightVelocity.x;
        mPosition.y += mFlightVelocity.y;
        mPosition.z += mFlightVelocity.z;
        break;
    }
    }
}

bool Obj_SeaGull::checkPlayerProximity(const sead::Vector3f& playerPos) {
    if (mState != SeaGullState::cState_Wait_random) return false;

    float dx = playerPos.x - mPosition.x;
    float dy = playerPos.y - mPosition.y;
    float dz = playerPos.z - mPosition.z;
    float distSq = dx * dx + dy * dy + dz * dz;

    if (distSq <= (mParams.mScareRadius * mParams.mScareRadius)) {
        // Escape away from player position
        sead::Vector3f escapeDir = sead::Vector3f(mPosition.x - playerPos.x, 0.0f, mPosition.z - playerPos.z);
        triggerTakeOff(escapeDir);
        return true;
    }
    return false;
}

bool Obj_SeaGull::applyDamage(const sead::Vector3f& hitPos) {
    if (mState != SeaGullState::cState_Wait_random) return false;

    float dx = hitPos.x - mPosition.x;
    float dy = hitPos.y - mPosition.y;
    float dz = hitPos.z - mPosition.z;
    float distSq = dx * dx + dy * dy + dz * dz;

    if (distSq <= (mParams.mShotScareRadius * mParams.mShotScareRadius)) {
        sead::Vector3f escapeDir = sead::Vector3f(mPosition.x - hitPos.x, 0.0f, mPosition.z - hitPos.z);
        triggerTakeOff(escapeDir);
        return true;
    }
    return false;
}

} // namespace Game
