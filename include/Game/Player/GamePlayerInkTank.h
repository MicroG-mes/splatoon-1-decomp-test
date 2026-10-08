#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"

namespace Game {

enum class InkRecoveryState : u32 {
    cDelayWait = 0,
    cSquidFast = 1,
    cHumanSlow = 2,
    cBlocked   = 3
};

/**
 * GamePlayerInkTank
 * Manages player ink reservoir, consumption for main/sub weapons,
 * recharge rates during squid submergence, and low-ink UI threshold warnings.
 */
class GamePlayerInkTank : public GambitActor {
public:
    static constexpr f32 cMaxInk = 100.0f;
    static constexpr f32 cSquidRechargePerFrame = 100.0f / 180.0f; // 3 seconds full refill
    static constexpr f32 cHumanRechargePerFrame = 100.0f / 600.0f; // 10 seconds full refill
    static constexpr s32 cDefaultDelayFrames = 20;

    GamePlayerInkTank();
    virtual ~GamePlayerInkTank() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    bool canConsume(f32 amount) const { return mInkLevel >= amount; }
    bool consumeInk(f32 amount);
    void replenishInk(f32 amount);
    void updateRecharge(bool isSquidSubmerged, bool isHumanoid, bool inFriendlyInk);

    f32 getInkLevel() const { return mInkLevel; }
    f32 getInkRatio() const { return mInkLevel / cMaxInk; }
    f32 getSubWeaponThreshold() const { return mSubWeaponThreshold; }
    void setSubWeaponThreshold(f32 threshold) { mSubWeaponThreshold = threshold; }

    bool canUseSubWeapon() const { return mInkLevel >= mSubWeaponThreshold; }
    bool isLowInk() const { return mInkLevel < mSubWeaponThreshold; }
    bool isLowInkBlinking() const { return isLowInk() && ((mBlinkTimer % 20) < 10); }

protected:
    f32 mInkLevel;
    f32 mSubWeaponThreshold;
    s32 mRecoveryDelayTimer;
    s32 mBlinkTimer;
    InkRecoveryState mRecoveryState;
};

} // namespace Game
