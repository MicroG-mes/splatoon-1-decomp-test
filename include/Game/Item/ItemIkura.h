#pragma once

#include "types.h"
#include "Game/Item/GameItemBase.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class IkuraState : u32 {
    cWait    = 0,
    cGot     = 1,
    cGotPara = 2
};

/**
 * ItemIkura
 * Power Egg currency pickup in Octo Valley single-player campaign.
 */
class ItemIkura : public GameItemBase {
public:
    static constexpr f32 cPickupRadius = 1.2f;
    static constexpr f32 cVacuumRadius = 4.5f;
    static constexpr f32 cGravity = -0.015f;
    static constexpr f32 cTerminalVelocity = -0.35f;
    static constexpr f32 cParachuteTerminalVelocity = -0.06f;

    ItemIkura();
    virtual ~ItemIkura() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void spawn(const sead::Vector3f& pos, u32 eggValue, bool withParachute = false);
    void spawnDropWithVelocity(const sead::Vector3f& pos, const sead::Vector3f& vel, u32 eggValue);
    bool checkPlayerPickup(const sead::Vector3f& playerPos, u32 playerId);

    IkuraState getState() const { return mState; }
    u32 getValue() const { return mEggValue; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    bool isCollected() const { return mState == IkuraState::cGot; }

protected:
    sead::Vector3f mPosition;
    sead::Vector3f mVelocity;
    IkuraState mState;
    u32 mEggValue;
    u32 mCollectorPlayerId;
    s32 mTimer;
    f32 mGroundY;
    bool mIsGrounded;
};

} // namespace Game
