#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class LiftTravelState : u32 {
    cMovingForward = 0,
    cPauseAtEnd    = 1,
    cMovingReverse = 2,
    cPauseAtStart  = 3
};

class RollingLift : public GambitActor {
public:
    static constexpr f32 cDefaultTravelSpeed = 0.045f;
    static constexpr s32 cDefaultPauseFrames = 60;

    RollingLift();
    virtual ~RollingLift() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setTrackWaypoints(const sead::Vector3f& startPoint, const sead::Vector3f& endPoint);
    void applyPropellerInk(f32 inkAmount);

    LiftTravelState getTravelState() const { return mState; }
    f32 getProgress() const { return mProgress; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    const sead::Vector3f& getVelocity() const { return mVelocity; }

protected:
    void stepMovement();

    sead::Vector3f mStartPoint;
    sead::Vector3f mEndPoint;
    sead::Vector3f mPosition;
    sead::Vector3f mVelocity;

    f32 mProgress; // 0.0f (Start) to 1.0f (End)
    f32 mSpeed;
    LiftTravelState mState;
    s32 mTimer;
    bool mIsPropellerDriven;

    undefined mReserved[0x38];
};

} // namespace Game
