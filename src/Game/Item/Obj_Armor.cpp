#include "Game/Item/Obj_Armor.h"
#include <cmath>

namespace Game {

Obj_Armor::Obj_Armor()
    : mPosition(0.0f, 0.0f, 0.0f),
      mState(ItemArmorState::cState_Wait),
      mCollectorPlayerId(0),
      mTimer(0),
      mHoverPhase(0.0f),
      mRotationAngle(0.0f) {
}

Obj_Armor::~Obj_Armor() {
}

void Obj_Armor::init() {
    GameItemBase::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mState = ItemArmorState::cState_Wait;
    mCollectorPlayerId = 0;
    mTimer = 0;
    mHoverPhase = 0.0f;
    mRotationAngle = 0.0f;
}

void Obj_Armor::spawn(const sead::Vector3f& pos) {
    mPosition = pos;
    mState = ItemArmorState::cState_Wait;
    mTimer = 0;
    mHoverPhase = 0.0f;
    mRotationAngle = 0.0f;
}

bool Obj_Armor::checkPlayerPickup(const sead::Vector3f& playerPos, u32 playerId) {
    if (mState != ItemArmorState::cState_Wait) {
        return false;
    }

    f32 dx = playerPos.x - mPosition.x;
    f32 dy = playerPos.y - mPosition.y;
    f32 dz = playerPos.z - mPosition.z;
    f32 distSq = dx * dx + dy * dy + dz * dz;

    if (distSq <= (cPickupRadius * cPickupRadius)) {
        mState = ItemArmorState::cState_Got;
        mCollectorPlayerId = playerId;
        mTimer = 0;
        return true;
    }

    return false;
}

void Obj_Armor::update() {
    switch (mState) {
        case ItemArmorState::cState_Wait: {
            mTimer++;
            mRotationAngle += 0.04f;
            if (mRotationAngle > 6.2831853f) {
                mRotationAngle -= 6.2831853f;
            }

            mHoverPhase += 0.05f;
            mPosition.y += std::sin(mHoverPhase) * 0.006f;
            break;
        }

        case ItemArmorState::cState_Got: {
            mTimer++;
            // Upward dissolve burst
            if (mTimer < 30) {
                mPosition.y += 0.08f;
                mRotationAngle += 0.2f;
            }
            break;
        }

        default:
            break;
    }
}

void Obj_Armor::draw() {
    // Model rendering handled by ModelSceneMgr
}

} // namespace Game
