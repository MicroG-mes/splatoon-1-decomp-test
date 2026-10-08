#include "Game/MapObj/Obj_Grate.h"
#include <cmath>

namespace Game {

Obj_Grate::Obj_Grate()
    : mCenter(0.0f, 0.0f, 0.0f),
      mWidth(4.0f),
      mLength(4.0f),
      mThickness(0.2f),
      mIsEnabled(true) {
}

Obj_Grate::~Obj_Grate() {
}

void Obj_Grate::init() {
    GambitActor::init();
    mCenter.set(0.0f, 0.0f, 0.0f);
    mWidth = 4.0f;
    mLength = 4.0f;
    mThickness = 0.2f;
    mIsEnabled = true;
}

void Obj_Grate::setupGrate(const sead::Vector3f& center, f32 width, f32 length, f32 thickness) {
    mCenter = center;
    mWidth = width;
    mLength = length;
    mThickness = thickness;
    mIsEnabled = true;
}

bool Obj_Grate::checkPlayerCollision(const sead::Vector3f& playerPos, bool isSquidForm, sead::Vector3f* outSupportPos) const {
    if (!mIsEnabled) {
        return false;
    }

    // Squids instantly fall through metal grates
    if (isSquidForm) {
        return false;
    }

    f32 dx = playerPos.x - mCenter.x;
    f32 dz = playerPos.z - mCenter.z;
    f32 halfW = mWidth * 0.5f;
    f32 halfL = mLength * 0.5f;

    // Check horizontal footprint
    if (std::abs(dx) <= halfW && std::abs(dz) <= halfL) {
        f32 topY = mCenter.y + mThickness * 0.5f;
        f32 bottomY = mCenter.y - mThickness * 0.5f;

        // Player standing on or slightly intersecting top surface
        if (playerPos.y >= bottomY && playerPos.y <= topY + 0.4f) {
            if (outSupportPos) {
                outSupportPos->x = playerPos.x;
                outSupportPos->y = topY;
                outSupportPos->z = playerPos.z;
            }
            return true;
        }
    }

    return false;
}

void Obj_Grate::update() {
    // Static environmental actor
}

void Obj_Grate::draw() {
    // Model rendering handled by ModelSceneMgr
}

} // namespace Game
