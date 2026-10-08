#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class GeyserState : u32 {
    cClosed    = 0,
    cOpening   = 1,
    cActive    = 2,
    cClosing   = 3,
    cCooldown  = 4
};

class Obj_Geyser : public GambitActor {
public:
    static constexpr f32 cDefaultMaxHeight = 6.0f;
    static constexpr f32 cActivationThreshold = 40.0f;
    static constexpr s32 cActiveDuration = 300; // 5 seconds at 60fps
    static constexpr s32 cCooldownDuration = 60;

    Obj_Geyser();
    virtual ~Obj_Geyser() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void applyInkHit(s32 teamId, f32 amount);
    void triggerEruption(s32 teamId);

    GeyserState getState() const { return mState; }
    f32 getCurrentHeight() const { return mCurrentHeight; }
    f32 getMaxHeight() const { return mMaxHeight; }
    s32 getActiveTeam() const { return mActiveTeam; }
    bool isClimbable() const { return mState == GeyserState::cActive && mCurrentHeight > 1.0f; }
    const sead::Vector3f& getPosition() const { return mPosition; }

    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }
    void setMaxHeight(f32 height) { mMaxHeight = height; }

protected:
    sead::Vector3f mPosition;
    GeyserState mState;
    s32 mStateTimer;
    s32 mActiveTeam;
    f32 mInkAccumulated;
    f32 mCurrentHeight;
    f32 mMaxHeight;

    undefined mReserved[0x38];
};

} // namespace Game
