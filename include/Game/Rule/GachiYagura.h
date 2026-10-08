#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class YaguraState : u32 {
    cNeutralCenter = 0,
    cAdvancingToBravo = 1, // Controlled by Alpha
    cAdvancingToAlpha = 2, // Controlled by Bravo
    cContested     = 3,
    cRetreating    = 4
};

class GachiYagura : public GambitActor {
public:
    static constexpr f32 cBaseSpeed = 0.08f;
    static constexpr s32 cInitialDistance = 100;
    static constexpr s32 cAbandonRetreatDelay = 300; // 5s before tower returns to center

    GachiYagura();
    virtual ~GachiYagura() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void updateRiders(u32 alphaRiderCount, u32 bravoRiderCount);

    YaguraState getState() const { return mState; }
    s32 getAlphaDistance() const { return mAlphaDistance; }
    s32 getBravoDistance() const { return mBravoDistance; }
    s32 getAlphaBestDistance() const { return mAlphaBestDistance; }
    s32 getBravoBestDistance() const { return mBravoBestDistance; }
    f32 getTrackProgress() const { return mTrackProgress; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    bool isKnockout() const { return mAlphaDistance <= 0 || mBravoDistance <= 0; }

protected:
    void stepMovement();

    sead::Vector3f mPosition;
    YaguraState mState;
    f32 mTrackProgress; // -1.0 (Alpha goal) to 0.0 (center) to +1.0 (Bravo goal)
    f32 mSpeedMultiplier;

    s32 mAlphaDistance;
    s32 mBravoDistance;
    s32 mAlphaBestDistance;
    s32 mBravoBestDistance;

    u32 mAlphaRiders;
    u32 mBravoRiders;
    s32 mAbandonTimer;

    undefined mReserved[0x38];
};

} // namespace Game
