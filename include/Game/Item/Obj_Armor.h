#pragma once

#include "types.h"
#include "Game/Item/GameItemBase.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class ItemArmorState : u32 {
    cState_Wait = 0,
    cState_Got  = 1
};

/**
 * Obj_Armor / ItemArmor
 * Hero Armor pickup in Octo Valley single-player campaign.
 * Grants the player an additional defensive suit layer that absorbs lethal hits.
 */
class Obj_Armor : public GameItemBase {
public:
    static constexpr f32 cPickupRadius = 1.6f;
    static constexpr u32 cMaxArmorHits = 2;

    Obj_Armor();
    virtual ~Obj_Armor() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void spawn(const sead::Vector3f& pos);
    bool checkPlayerPickup(const sead::Vector3f& playerPos, u32 playerId);

    ItemArmorState getState() const { return mState; }
    bool isCollected() const { return mState == ItemArmorState::cState_Got; }

protected:
    sead::Vector3f mPosition;
    ItemArmorState mState;
    u32 mCollectorPlayerId;
    s32 mTimer;
    f32 mHoverPhase;
    f32 mRotationAngle;
};

} // namespace Game
