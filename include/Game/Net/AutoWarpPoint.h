#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class SuperJumpState : u32 {
    cIdle             = 0,
    cWindupSquat      = 1,
    cAirborneParabola = 2,
    cTouchdownSplash  = 3,
    cCompleted        = 4
};

class AutoWarpPoint : public GambitActor {
public:
    static constexpr s32 cDefaultWindupFrames = 75;
    static constexpr s32 cDefaultFlightFrames = 110;
    static constexpr f32 cMaxApexHeight = 22.0f;

    AutoWarpPoint();
    virtual ~AutoWarpPoint() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void launchSuperJump(const sead::Vector3f& startPos, const sead::Vector3f& targetPos, u32 teamId, bool hasStealthJump, s32 quickJumpDiscountFrames = 0);

    SuperJumpState getState() const { return mState; }
    bool isCompleted() const { return mState == SuperJumpState::cCompleted; }
    bool isLandingMarkerVisible(const sead::Vector3f& observerPos, u32 observerTeam) const;
    const sead::Vector3f& getCurrentSquidPos() const { return mCurrentPos; }
    const sead::Vector3f& getTargetLandingPos() const { return mTargetPos; }

protected:
    void stepFlightPhysics();

    sead::Vector3f mStartPos;
    sead::Vector3f mTargetPos;
    sead::Vector3f mCurrentPos;

    SuperJumpState mState;
    s32 mTimer;
    s32 mWindupDuration;
    s32 mFlightDuration;
    u32 mTeamId;
    bool mHasStealthJump;

    undefined mReserved[0x38];
};

} // namespace Game
