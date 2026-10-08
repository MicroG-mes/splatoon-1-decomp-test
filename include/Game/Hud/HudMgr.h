#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

struct ReticleState {
    bool isTargetLocked;
    f32 targetDistance;
    bool isEffectiveRange;
};

class HudMgr : public GambitActor {
public:
    HudMgr();
    virtual ~HudMgr() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void updateReticle(bool targetLocked, f32 distance, bool effectiveRange);
    void updateInkTank(f32 inkLevel, f32 subWeaponCost);
    void updateSpecialGauge(f32 currentPoints, f32 maxPoints);
    void triggerDamageVignette(f32 damageAmount);

    bool isLowInk() const { return mIsLowInk; }
    bool isSpecialReady() const { return mIsSpecialReady; }
    f32 getInkLevel() const { return mInkLevel; }
    f32 getSpecialRatio() const { return mSpecialRatio; }
    f32 getDamageVignetteAlpha() const { return mDamageVignetteAlpha; }

protected:
    ReticleState mReticle;

    f32 mInkLevel;         // 0.0 to 1.0
    f32 mSubWeaponCost;    // 0.0 to 1.0
    bool mIsLowInk;

    f32 mSpecialPoints;
    f32 mSpecialMaxPoints;
    f32 mSpecialRatio;     // 0.0 to 1.0
    bool mIsSpecialReady;

    f32 mDamageVignetteAlpha; // 0.0 (clean) to 1.0 (heavy ink on screen)
    s32 mFlashAnimTimer;

    undefined mReserved[0x38];
};

} // namespace Game
