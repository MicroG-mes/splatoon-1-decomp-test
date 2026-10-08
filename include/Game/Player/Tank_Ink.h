#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class TankModelType : u32 {
    cStandard = 0, // PlayerTank
    cDuel1    = 1, // DuelPlayerTankV1 (Battle Dojo Player 1)
    cDuel2    = 2  // DuelPlayerTankV2 (Battle Dojo Player 2)
};

// Decompiled from PPC: Tank_Ink @ 0x026D50B4, 0x026D51E8, 0x026D526C, 0x026D53AC
class Tank_Ink : public GambitActor {
public:
    static constexpr f32 cMaxCapacity = 100.0f;
    static constexpr f32 cDefaultSubCost = 70.0f; // Splat Bomb default cost (70%)

    Tank_Ink();
    virtual ~Tank_Ink() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setupModel(TankModelType modelType);
    bool consumeInk(f32 amount);
    void refillInk(bool isSubmerged);
    void setSubMarkerCost(f32 subCostPercent);

    // Liquid mesh level evaluation (matches Tank_Ink__vfunc_52 @ 0x026D526C)
    f32 computeLiquidHeight() const;

    bool hasEnoughForSub() const { return mInkAmount >= mSubCost; }
    f32 getInkAmount() const { return mInkAmount; }
    f32 getCapacity() const { return mCapacity; }
    f32 getInkRatio() const { return mInkAmount / mCapacity; }
    f32 getSubCost() const { return mSubCost; }
    TankModelType getModelType() const { return mModelType; }
    bool isSubMarkerLit() const { return hasEnoughForSub(); }

private:
    TankModelType mModelType;
    f32 mInkAmount;
    f32 mCapacity;
    f32 mSubCost;
    s32 mEmptyPenaltyTimer;
    s32 mRefillCooldownTimer;
};

} // namespace Game
