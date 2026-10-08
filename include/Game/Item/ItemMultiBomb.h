#pragma once

#include "types.h"
#include "Game/Item/GameItemBase.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class ItemMultiBombState : u32 {
    cState_Wait = 0,
    cState_Got  = 1
};

/**
 * ItemMultiBomb
 * Special weapon pickup canister: Bomb Rush activation and rush gauge
 */
class ItemMultiBomb : public GameItemBase {
public:
    static constexpr f32 cPickupRadius = 1.6f;
    static constexpr f32 cBombRushDurationSeconds = 6.0f;

    ItemMultiBomb();
    virtual ~ItemMultiBomb() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void spawn(const sead::Vector3f& pos, u32 subWeaponType);
    bool checkPlayerPickup(const sead::Vector3f& playerPos, u32 playerId);

    ItemMultiBombState getState() const { return mState; }
    u32 getSubWeaponType() const { return mSubWeaponType; }
    f32 getRemainingRushDuration() const { return mRushDurationFrames / 60.0f; }

protected:
    sead::Vector3f mPosition;
    ItemMultiBombState mState;
    u32 mSubWeaponType;
    u32 mCollectorPlayerId;
    s32 mTimer;
    s32 mRushDurationFrames;
    f32 mHoverPhase;
};

} // namespace Game
