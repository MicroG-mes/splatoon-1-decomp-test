#include "Game/Item/ItemDuelSpecial.h"
#include <cmath>

namespace Game {

ItemDuelSpecial::ItemDuelSpecial()
    : mPosition(0.0f, 0.0f, 0.0f),
      mState(ItemDuelSpecialState::cState_Wait),
      mSpecialWeaponType(0),
      mCollectorPlayerId(0),
      mTimer(0),
      mHoverPhase(0.0f),
      mRotationAngle(0.0f) {
}

ItemDuelSpecial::~ItemDuelSpecial() {
}

void ItemDuelSpecial::init() {
    GameItemBase::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mState = ItemDuelSpecialState::cState_Wait;
    mSpecialWeaponType = 0;
    mCollectorPlayerId = 0;
    mTimer = 0;
    mHoverPhase = 0.0f;
    mRotationAngle = 0.0f;
}

void ItemDuelSpecial::spawn(const sead::Vector3f& pos, u32 specialWeaponType) {
    mPosition = pos;
    mSpecialWeaponType = specialWeaponType;
    mState = ItemDuelSpecialState::cState_Wait;
    mTimer = 0;
    mHoverPhase = 0.0f;
    mRotationAngle = 0.0f;
}

bool ItemDuelSpecial::checkPlayerPickup(const sead::Vector3f& playerPos, u32 playerId) {
    if (mState != ItemDuelSpecialState::cState_Wait) {
        return false;
    }

    f32 dx = playerPos.x - mPosition.x;
    f32 dy = playerPos.y - mPosition.y;
    f32 dz = playerPos.z - mPosition.z;
    f32 distSq = dx * dx + dy * dy + dz * dz;

    if (distSq <= (cPickupRadius * cPickupRadius)) {
        mState = ItemDuelSpecialState::cState_Got;
        mCollectorPlayerId = playerId;
        mTimer = 0;
        return true;
    }

    return false;
}

void ItemDuelSpecial::update() {
    switch (mState) {
        case ItemDuelSpecialState::cState_Wait: {
            mTimer++;
            mRotationAngle += 0.05f;
            if (mRotationAngle > 6.2831853f) {
                mRotationAngle -= 6.2831853f;
            }

            mHoverPhase += 0.07f;
            mPosition.y += std::sin(mHoverPhase) * 0.007f;
            break;
        }

        case ItemDuelSpecialState::cState_Got: {
            mTimer++;
            if (mTimer < 30) {
                mPosition.y += 0.07f;
                mRotationAngle += 0.18f;
            }
            break;
        }

        default:
            break;
    }
}

void ItemDuelSpecial::draw() {
    // Model rendering handled by ModelSceneMgr
}

} // namespace Game
