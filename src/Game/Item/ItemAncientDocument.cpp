#include "Game/Item/ItemAncientDocument.h"
#include <cmath>

namespace Game {

ItemAncientDocument::ItemAncientDocument()
    : mPosition(0.0f, 0.0f, 0.0f),
      mState(AncientDocumentState::cState_Wait),
      mScrollId(1),
      mCollectorPlayerId(0),
      mTimer(0),
      mRotationAngle(0.0f),
      mHoverPhase(0.0f) {
}

ItemAncientDocument::~ItemAncientDocument() {
}

void ItemAncientDocument::init() {
    GameItemBase::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mState = AncientDocumentState::cState_Wait;
    mScrollId = 1;
    mCollectorPlayerId = 0;
    mTimer = 0;
    mRotationAngle = 0.0f;
    mHoverPhase = 0.0f;
}

void ItemAncientDocument::spawn(const sead::Vector3f& pos, u32 scrollId) {
    mPosition = pos;
    mScrollId = (scrollId >= 1 && scrollId <= cMaxScrolls) ? scrollId : 1;
    mState = AncientDocumentState::cState_Wait;
    mTimer = 0;
    mRotationAngle = 0.0f;
    mHoverPhase = 0.0f;
}

bool ItemAncientDocument::checkPlayerPickup(const sead::Vector3f& playerPos, u32 playerId) {
    if (mState != AncientDocumentState::cState_Wait) {
        return false;
    }

    f32 dx = playerPos.x - mPosition.x;
    f32 dy = playerPos.y - mPosition.y;
    f32 dz = playerPos.z - mPosition.z;
    f32 distSq = dx * dx + dy * dy + dz * dz;

    if (distSq <= (cPickupRadius * cPickupRadius)) {
        mState = AncientDocumentState::cState_Got;
        mCollectorPlayerId = playerId;
        mTimer = 0;
        return true;
    }

    return false;
}

void ItemAncientDocument::update() {
    switch (mState) {
        case AncientDocumentState::cState_Wait: {
            mTimer++;
            mRotationAngle += 0.035f;
            if (mRotationAngle > 6.2831853f) {
                mRotationAngle -= 6.2831853f;
            }

            mHoverPhase += 0.06f;
            mPosition.y += std::sin(mHoverPhase) * 0.008f;
            break;
        }

        case AncientDocumentState::cState_Got: {
            mTimer++;
            // Floating upward celebration animation before disappearance
            if (mTimer < 45) {
                mPosition.y += 0.05f;
                mRotationAngle += 0.12f;
            }
            break;
        }

        default:
            break;
    }
}

void ItemAncientDocument::draw() {
    // Model rendering handled by ModelSceneMgr
}

} // namespace Game
