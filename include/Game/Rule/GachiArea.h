#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class ZoneControlState : u32 {
    cNeutral       = 0,
    cControlledP1  = 1, // Controlled by Alpha
    cControlledP2  = 2, // Controlled by Bravo
    cContested     = 3
};

class GachiArea : public GambitActor {
public:
    static constexpr f32 cCaptureThreshold = 0.70f; // 70% ink coverage to seize zone
    static constexpr u32 cInitialCounter = 100;

    GachiArea();
    virtual ~GachiArea() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setZoneBounds(const sead::Vector3f& minBounds, const sead::Vector3f& maxBounds);
    void updatePaintCoverage(f32 alphaPercent, f32 bravoPercent);

    ZoneControlState getControlState() const { return mControlState; }
    s32 getAlphaCounter() const { return mAlphaCounter; }
    s32 getBravoCounter() const { return mBravoCounter; }
    s32 getAlphaPenalty() const { return mAlphaPenalty; }
    s32 getBravoPenalty() const { return mBravoPenalty; }
    bool isOvertimeActive() const { return mIsOvertime; }
    bool isGameOver() const { return mAlphaCounter <= 0 || mBravoCounter <= 0; }

protected:
    void decrementTimer();

    sead::Vector3f mMinBounds;
    sead::Vector3f mMaxBounds;

    ZoneControlState mControlState;
    f32 mAlphaCoverage;
    f32 mBravoCoverage;

    s32 mAlphaCounter;
    s32 mBravoCounter;
    s32 mAlphaPenalty;
    s32 mBravoPenalty;

    s32 mTimerTick;
    bool mIsOvertime;

    undefined mReserved[0x38];
};

} // namespace Game
