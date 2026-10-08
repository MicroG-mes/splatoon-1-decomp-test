#include "Game/Item/ItemCardKey.h"
#include <cmath>

namespace Game {

ItemCardKey::ItemCardKey()
    : mPosition(0.0f, 0.0f, 0.0f),
      mState(ItemCardKeyState::cState_Wait),
      mKeyId(0),
      mCollectorPlayerId(0),
      mTimer(0),
      mHoverPhase(0.0f),
      mRotationAngle(0.0f) {
}

ItemCardKey::~ItemCardKey() {
}

void ItemCardKey::init() {
    GameItemBase::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mState = ItemCardKeyState::cState_Wait;
    mKeyId = 0;
    mCollectorPlayerId = 0;
    mTimer = 0;
    mHoverPhase = 0.0f;
    mRotationAngle = 0.0f;
}

void ItemCardKey::spawn(const sead::Vector3f& pos, u32 keyId) {
    mPosition = pos;
    mKeyId = keyId;
    mState = ItemCardKeyState::cState_Wait;
    mTimer = 0;
    mHoverPhase = 0.0f;
    mRotationAngle = 0.0f;
}

bool ItemCardKey::checkPlayerPickup(const sead::Vector3f& playerPos, u32 playerId) {
    if (mState != ItemCardKeyState::cState_Wait) {
        return false;
    }

    f32 dx = playerPos.x - mPosition.x;
    f32 dy = playerPos.y - mPosition.y;
    f32 dz = playerPos.z - mPosition.z;
    f32 distSq = dx * dx + dy * dy + dz * dz;

    if (distSq <= (cPickupRadius * cPickupRadius)) {
        mState = ItemCardKeyState::cState_Got;
        mCollectorPlayerId = playerId;
        mTimer = 0;
        return true;
    }

    return false;
}

void ItemCardKey::update() {
    switch (mState) {
        case ItemCardKeyState::cState_Wait: {
            mTimer++;
            mRotationAngle += 0.045f;
            if (mRotationAngle > 6.2831853f) {
                mRotationAngle -= 6.2831853f;
            }

            mHoverPhase += 0.06f;
            mPosition.y += std::sin(mHoverPhase) * 0.007f;
            break;
        }

        case ItemCardKeyState::cState_Got: {
            mTimer++;
            if (mTimer < 35) {
                mPosition.y += 0.06f;
                mRotationAngle += 0.15f;
            }
            break;
        }

        default:
            break;
    }
}

void ItemCardKey::draw() {
    // Model rendering handled by ModelSceneMgr
}

} // namespace Game
