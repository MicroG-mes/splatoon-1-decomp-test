#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class TurnPlateState : u32 {
    cRotatingClockwise        = 0,
    cRotatingCounterClockwise = 1,
    cPaused                   = 2
};

class Lft_TurnPlate : public GambitActor {
public:
    static constexpr f32 cDefaultAngularSpeed = 0.01745f; // 1 degree per frame (~60 deg/sec)
    static constexpr f32 cDefaultRadius = 5.0f;

    Lft_TurnPlate();
    virtual ~Lft_TurnPlate() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setupTurntable(const sead::Vector3f& centerPos, f32 radius, f32 angularSpeedRad);
    sead::Vector3f calculateRiderVelocity(const sead::Vector3f& riderPos) const;
    bool isRiderOnPlate(const sead::Vector3f& testPos) const;

    TurnPlateState getState() const { return mState; }
    f32 getCurrentAngle() const { return mCurrentAngleRad; }
    f32 getRadius() const { return mRadius; }
    const sead::Vector3f& getCenterPos() const { return mCenterPos; }

protected:
    void stepRotation();

    sead::Vector3f mCenterPos;
    f32 mRadius;
    f32 mCurrentAngleRad;
    f32 mAngularSpeedRad;
    TurnPlateState mState;
    s32 mStateTimer;

    undefined mReserved[0x38];
};

} // namespace Game
