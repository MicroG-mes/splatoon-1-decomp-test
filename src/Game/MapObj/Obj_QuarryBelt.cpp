#include "Game/MapObj/Obj_QuarryBelt.h"
#include <cmath>

namespace Game {

Obj_QuarryBelt::Obj_QuarryBelt()
    : mCenter(0.0f, 0.0f, 0.0f),
      mDirection(0.0f, 0.0f, 1.0f),
      mLength(10.0f),
      mWidth(4.0f),
      mSpeed(cDefaultBeltSpeed),
      mUvOffset(0.0f),
      mIsActive(true),
      mTimer(0) {
}

Obj_QuarryBelt::~Obj_QuarryBelt() {
}

void Obj_QuarryBelt::init() {
    GambitActor::init();
    mCenter.set(0.0f, 0.0f, 0.0f);
    mDirection.set(0.0f, 0.0f, 1.0f);
    mLength = 10.0f;
    mWidth = 4.0f;
    mSpeed = cDefaultBeltSpeed;
    mUvOffset = 0.0f;
    mIsActive = true;
    mTimer = 0;
}

void Obj_QuarryBelt::setupBelt(const sead::Vector3f& center, const sead::Vector3f& direction, f32 length, f32 width, f32 speed) {
    mCenter = center;
    mDirection = direction;
    // Normalize direction in XZ plane
    f32 len = std::sqrt(mDirection.x * mDirection.x + mDirection.z * mDirection.z);
    if (len > 0.0001f) {
        mDirection.x /= len;
        mDirection.y = 0.0f;
        mDirection.z /= len;
    }
    mLength = length;
    mWidth = width;
    mSpeed = speed;
    mUvOffset = 0.0f;
    mIsActive = true;
}

bool Obj_QuarryBelt::getBeltVelocityAt(const sead::Vector3f& queryPos, sead::Vector3f* outVel) const {
    if (!mIsActive || !outVel) {
        return false;
    }

    f32 dx = queryPos.x - mCenter.x;
    f32 dy = queryPos.y - mCenter.y;
    f32 dz = queryPos.z - mCenter.z;

    // Check vertical proximity (within 1 unit above belt surface)
    if (dy < -0.2f || dy > 1.2f) {
        return false;
    }

    // Project onto belt local axes:
    // Along direction (length) and perpendicular (width)
    f32 dotAlong = dx * mDirection.x + dz * mDirection.z;
    f32 perpX = -mDirection.z;
    f32 perpZ = mDirection.x;
    f32 dotPerp = dx * perpX + dz * perpZ;

    f32 halfLen = mLength * 0.5f;
    f32 halfWid = mWidth * 0.5f;

    if (std::abs(dotAlong) <= halfLen && std::abs(dotPerp) <= halfWid) {
        outVel->x = mDirection.x * mSpeed;
        outVel->y = 0.0f;
        outVel->z = mDirection.z * mSpeed;
        return true;
    }

    return false;
}

void Obj_QuarryBelt::update() {
    if (!mIsActive) {
        return;
    }

    mTimer++;
    mUvOffset += mSpeed * 0.1f;
    if (mUvOffset > 1.0f) {
        mUvOffset -= 1.0f;
    }
}

void Obj_QuarryBelt::draw() {
    // Model and conveyor UV scroll rendering handled by ModelSceneMgr
}

} // namespace Game
