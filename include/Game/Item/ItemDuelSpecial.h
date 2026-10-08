#pragma once

#include "types.h"
#include "Game/Item/GameItemBase.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class ItemDuelSpecialState : u32 {
    cState_Wait = 0,
    cState_Got  = 1
};

/**
 * ItemDuelSpecial / Obj_ItemDuel
 * Battle Dojo (2P local duel) special can collectible.
 * Grants a random or designated special weapon when collected.
 */
class ItemDuelSpecial : public GameItemBase {
public:
    static constexpr f32 cPickupRadius = 1.6f;

    ItemDuelSpecial();
    virtual ~ItemDuelSpecial() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void spawn(const sead::Vector3f& pos, u32 specialWeaponType);
    bool checkPlayerPickup(const sead::Vector3f& playerPos, u32 playerId);

    ItemDuelSpecialState getState() const { return mState; }
    u32 getSpecialWeaponType() const { return mSpecialWeaponType; }
    bool isCollected() const { return mState == ItemDuelSpecialState::cState_Got; }

protected:
    sead::Vector3f mPosition;
    ItemDuelSpecialState mState;
    u32 mSpecialWeaponType;
    u32 mCollectorPlayerId;
    s32 mTimer;
    f32 mHoverPhase;
    f32 mRotationAngle;
};

} // namespace Game
