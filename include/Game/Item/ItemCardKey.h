#pragma once

#include "types.h"
#include "Game/Item/GameItemBase.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class ItemCardKeyState : u32 {
    cState_Wait = 0,
    cState_Got  = 1
};

/**
 * ItemCardKey
 * Vault key card required to unlock caged vaults and locked doors in Octo Valley.
 */
class ItemCardKey : public GameItemBase {
public:
    static constexpr f32 cPickupRadius = 1.5f;

    ItemCardKey();
    virtual ~ItemCardKey() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void spawn(const sead::Vector3f& pos, u32 keyId);
    bool checkPlayerPickup(const sead::Vector3f& playerPos, u32 playerId);

    ItemCardKeyState getState() const { return mState; }
    u32 getKeyId() const { return mKeyId; }
    bool isCollected() const { return mState == ItemCardKeyState::cState_Got; }

protected:
    sead::Vector3f mPosition;
    ItemCardKeyState mState;
    u32 mKeyId;
    u32 mCollectorPlayerId;
    s32 mTimer;
    f32 mHoverPhase;
    f32 mRotationAngle;
};

} // namespace Game
