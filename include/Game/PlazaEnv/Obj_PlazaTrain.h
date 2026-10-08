#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class TrainState : u32 {
    cWaitingOffscreen = 0,
    cApproaching      = 1,
    cCrossingPlaza    = 2,
    cDeparting        = 3
};

class Obj_PlazaTrain : public GambitActor {
public:
    Obj_PlazaTrain();
    virtual ~Obj_PlazaTrain() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void triggerPass();

    TrainState getState() const { return mState; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    f32 getSpeed() const { return mSpeed; }

protected:
    TrainState mState;
    s32 mCycleTimer;
    s32 mCrossTimer;

    sead::Vector3f mPosition;
    f32 mSpeed;
    f32 mTrackSplineProgress; // 0.0 to 1.0

    undefined mReserved[0x38];
};

} // namespace Game
