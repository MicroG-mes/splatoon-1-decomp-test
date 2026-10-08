#include "Game/Player/Tank_Ink.h"

namespace Game {

Tank_Ink::Tank_Ink()
    : mModelType(TankModelType::cStandard),
      mInkAmount(cMaxCapacity),
      mCapacity(cMaxCapacity),
      mSubCost(cDefaultSubCost),
      mEmptyPenaltyTimer(0),
      mRefillCooldownTimer(0) {
}

Tank_Ink::~Tank_Ink() {
}

void Tank_Ink::init() {
    GambitActor::init();
    mModelType = TankModelType::cStandard;
    mInkAmount = cMaxCapacity;
    mCapacity = cMaxCapacity;
    mSubCost = cDefaultSubCost;
    mEmptyPenaltyTimer = 0;
    mRefillCooldownTimer = 0;
}

void Tank_Ink::setupModel(TankModelType modelType) {
    mModelType = modelType;
}

bool Tank_Ink::consumeInk(f32 amount) {
    if (mInkAmount < amount) {
        // Dry-fire penalty
        mEmptyPenaltyTimer = 30; // 0.5s lockout before refill starts
        return false;
    }

    mInkAmount -= amount;
    if (mInkAmount < 0.0f) mInkAmount = 0.0f;
    mRefillCooldownTimer = 15; // 0.25s firing delay
    return true;
}

void Tank_Ink::refillInk(bool isSubmerged) {
    if (mEmptyPenaltyTimer > 0 || mRefillCooldownTimer > 0) {
        return; // Currently on firing delay or empty penalty
    }

    if (isSubmerged) {
        // Fast submerged squid recovery: 3.33% per frame (refills in ~30 frames / 0.5s)
        mInkAmount += 3.33f;
    } else {
        // Slow humanoid standing recovery: 0.33% per frame
        mInkAmount += 0.33f;
    }

    if (mInkAmount > mCapacity) {
        mInkAmount = mCapacity;
    }
}

void Tank_Ink::setSubMarkerCost(f32 subCostPercent) {
    mSubCost = subCostPercent;
}

// Matches Tank_Ink__vfunc_52 @ 0x026D526C
f32 Tank_Ink::computeLiquidHeight() const {
    f32 ratio = getInkRatio();
    // Clamped between -0.85f (empty bottom) and 0.85f (full top)
    f32 height = -0.85f + (ratio * 1.70f);
    if (height < -0.85f) height = -0.85f;
    if (height > 0.85f) height = 0.85f;
    return height;
}

void Tank_Ink::update() {
    if (mEmptyPenaltyTimer > 0) {
        mEmptyPenaltyTimer--;
    }
    if (mRefillCooldownTimer > 0) {
        mRefillCooldownTimer--;
    }
}

void Tank_Ink::draw() {
    GambitActor::draw();
}

} // namespace Game
