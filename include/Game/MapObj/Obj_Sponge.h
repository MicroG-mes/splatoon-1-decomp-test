#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class SpongeState : u32 {
    cIdle       = 0,
    cExpanding  = 1,
    cShrinking  = 2
};

class Obj_Sponge : public GambitActor {
public:
    static constexpr f32 cMinScale = 1.0f;
    static constexpr f32 cMaxScale = 3.0f;
    static constexpr f32 cGrowthStep = 0.25f;
    static constexpr f32 cShrinkStep = 0.35f;

    Obj_Sponge();
    virtual ~Obj_Sponge() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void hitByInk(s32 inkTeam, f32 inkVolume);

    f32 getCurrentScale() const { return mCurrentScale; }
    f32 getTargetScale() const { return mTargetScale; }
    s32 getOwnerTeam() const { return mOwnerTeam; }
    SpongeState getState() const { return mState; }
    const sead::Vector3f& getPosition() const { return mPosition; }

    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mPosition;
    f32 mCurrentScale;
    f32 mTargetScale;
    s32 mOwnerTeam; // -1 = neutral/unpainted, 0 = team 0, 1 = team 1
    SpongeState mState;

    undefined mReserved[0x34];
};

} // namespace Game
