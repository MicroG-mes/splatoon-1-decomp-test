#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class BarrierState : u32 {
    cInactive = 0,
    cActive   = 1,
    cFading   = 2
};

class Obj_Barrier : public GambitActor {
public:
    Obj_Barrier();
    virtual ~Obj_Barrier() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void activate(f32 durationSeconds = 4.5f);
    void applyKnockback(const sead::Vector3f& knockbackForce);
    bool checkTeammateShare(const sead::Vector3f& playerPos, const sead::Vector3f& teammatePos, f32 shareRadius = 2.0f);

    BarrierState getState() const { return mState; }
    bool isActive() const { return mState == BarrierState::cActive; }
    f32 getRemainingTimeRatio() const;

    const sead::Vector3f& getKnockbackVelocity() const { return mKnockbackVelocity; }

protected:
    BarrierState mState;
    s32 mTotalDurationFrames;
    s32 mRemainingFrames;

    sead::Vector3f mKnockbackVelocity;
    f32 mBubblePulsePhase;

    undefined mReserved[0x38];
};

} // namespace Game
