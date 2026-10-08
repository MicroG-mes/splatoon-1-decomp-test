#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class FlyswatterState : u32 {
    cHoverIdle   = 0,
    cWindupRise  = 1,
    cDropSlam    = 2,
    cFlatRest    = 3,
    cRecoverRise = 4,
    cDefeated    = 5
};

class Enm_Flyswatter : public GambitActor {
public:
    static constexpr f32 cMaxHp = 60.0f;
    static constexpr f32 cApexHeight = 6.0f;
    static constexpr f32 cSlamRadius = 3.5f;

    Enm_Flyswatter();
    virtual ~Enm_Flyswatter() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void updateAi(const sead::Vector3f& playerPos);
    void applyDamage(f32 damage);

    FlyswatterState getState() const { return mState; }
    f32 getHp() const { return mHp; }
    bool isGroundedWeakpointExposed() const { return mState == FlyswatterState::cFlatRest; }
    const sead::Vector3f& getPosition() const { return mPosition; }

    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    void slamImpact();

    sead::Vector3f mPosition;
    sead::Vector3f mTargetPlayerPos;
    f32 mHp;
    FlyswatterState mState;
    s32 mStateTimer;

    undefined mReserved[0x38];
};

} // namespace Game
