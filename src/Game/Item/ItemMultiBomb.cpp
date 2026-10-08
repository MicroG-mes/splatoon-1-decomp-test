#include "Game/Item/ItemMultiBomb.h"
#include <cmath>

namespace Game {

ItemMultiBomb::ItemMultiBomb()
    : mPosition(0.0f, 0.0f, 0.0f),
      mState(ItemMultiBombState::cState_Wait),
      mSubWeaponType(0),
      mCollectorPlayerId(0),
      mTimer(0),
      mRushDurationFrames(360),
      mHoverPhase(0.0f) {
}

ItemMultiBomb::~ItemMultiBomb() {
}

void ItemMultiBomb::init() {
    GameItemBase::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mState = ItemMultiBombState::cState_Wait;
    mSubWeaponType = 0;
    mCollectorPlayerId = 0;
    mTimer = 0;
    mRushDurationFrames = 360;
    mHoverPhase = 0.0f;
}

void ItemMultiBomb::spawn(const sead::Vector3f& pos, u32 subWeaponType) {
    mPosition = pos;
    mSubWeaponType = subWeaponType;
    mState = ItemMultiBombState::cState_Wait;
    mTimer = 0;
    mRushDurationFrames = 360;
}

bool ItemMultiBomb::checkPlayerPickup(const sead::Vector3f& playerPos, u32 playerId) {
    if (mState != ItemMultiBombState::cState_Wait) {
        return false;
    }

    f32 dx = playerPos.x - mPosition.x;
    f32 dy = playerPos.y - mPosition.y;
    f32 dz = playerPos.z - mPosition.z;
    f32 distSq = dx * dx + dy * dy + dz * dz;

    if (distSq <= (cPickupRadius * cPickupRadius)) {
        mState = ItemMultiBombState::cState_Got;
        mCollectorPlayerId = playerId;
        mTimer = 0;
        return true;
    }

    return false;
}

void ItemMultiBomb::update() {
    switch (mState) {
        case ItemMultiBombState::cState_Wait: {
            mTimer++;
            mHoverPhase += 0.05f;
            // Gentle hovering vertical oscillation
            mPosition.y += std::sin(mHoverPhase) * 0.005f;
            break;
        }

        case ItemMultiBombState::cState_Got: {
            mTimer++;
            if (mRushDurationFrames > 0) {
                mRushDurationFrames--;
            }
            break;
        }

        default:
            break;
    }
}

void ItemMultiBomb::draw() {
    // Model rendering handled by ModelSceneMgr
}

} // namespace Game
