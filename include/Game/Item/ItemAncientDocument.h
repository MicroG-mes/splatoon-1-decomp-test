#pragma once

#include "types.h"
#include "Game/Item/GameItemBase.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class AncientDocumentState : u32 {
    cState_Wait = 0,
    cState_Got  = 1
};

/**
 * ItemAncientDocument
 * Sunken Scroll collectible containing Octo Valley lore and blueprints (28 scrolls total).
 */
class ItemAncientDocument : public GameItemBase {
public:
    static constexpr f32 cPickupRadius = 1.8f;
    static constexpr u32 cMaxScrolls = 28;

    ItemAncientDocument();
    virtual ~ItemAncientDocument() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void spawn(const sead::Vector3f& pos, u32 scrollId);
    bool checkPlayerPickup(const sead::Vector3f& playerPos, u32 playerId);

    AncientDocumentState getState() const { return mState; }
    u32 getScrollId() const { return mScrollId; }
    bool isCollected() const { return mState == AncientDocumentState::cState_Got; }

protected:
    sead::Vector3f mPosition;
    AncientDocumentState mState;
    u32 mScrollId;
    u32 mCollectorPlayerId;
    s32 mTimer;
    f32 mRotationAngle;
    f32 mHoverPhase;
};

} // namespace Game
