#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class LiftFallState : u32 {
    cState_Idle = 0,
    cState_SteppedOn,
    cState_Shaking,
    cState_Falling,
    cState_Respawning
};

struct LiftFallParams {
    f32 lifeSeconds; // mLife (3.0)

    bool load(const char* paramsPath);
};

/**
 * Obj_LiftFall
 * Collapsing timed fall platform in Octo Valley stages.
 * Collapses 3.0 seconds after player stands on it, shaking violently before dropping.
 */
class Obj_LiftFall : public GambitActor {
public:
    static constexpr s32 cFps = 60;
    static constexpr s32 cRespawnDurationFrames = 180; // 3.0s respawn

    Obj_LiftFall();
    virtual ~Obj_LiftFall() override;

    virtual void init() override;
    void init(const sead::Vector3f& pos);
    virtual void update() override;

    // Player contact
    void onPlayerStepOn();
    void onPlayerStepOff();

    // Getters
    LiftFallState getState() const { return mState; }
    bool isSolid() const { return mState <= LiftFallState::cState_Shaking; }
    bool isFalling() const { return mState == LiftFallState::cState_Falling; }
    f32 getFallDisplacementY() const { return mFallDisplacementY; }
    f32 getShakeOffsetX() const { return mShakeOffsetX; }
    s32 getRemainingLifeFrames() const { return mRemainingFrames; }
    const LiftFallParams& getParams() const { return mParams; }

private:
    LiftFallState mState;
    LiftFallParams mParams;
    s32 mRemainingFrames;
    s32 mRespawnTimer;
    f32 mFallDisplacementY;
    f32 mFallVelocityY;
    f32 mShakeOffsetX;
    bool mIsPlayerOn;
};

} // namespace Game
