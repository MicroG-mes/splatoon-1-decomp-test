#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class OctoballState : u32 {
    cIdleStand   = 0,
    cCurlingUp   = 1,
    cRollingDash = 2,
    cWallBounce  = 3,
    cDazedUncurl = 4,
    cDefeated    = 5
};

class Enm_Ball : public GambitActor {
public:
    static constexpr f32 cMaxHp = 30.0f;
    static constexpr f32 cRollSpeed = 0.22f;

    Enm_Ball();
    virtual ~Enm_Ball() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void updateAi(const sead::Vector3f& playerPos, bool isRollingOnPlayerInk);
    void applyDamage(f32 damage);

    OctoballState getState() const { return mState; }
    f32 getHp() const { return mHp; }
    bool isRolling() const { return mState == OctoballState::cRollingDash; }
    const sead::Vector3f& getPosition() const { return mPosition; }

    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    void stepRollingMovement();

    sead::Vector3f mPosition;
    sead::Vector3f mMoveDir;
    f32 mHp;
    OctoballState mState;
    s32 mStateTimer;

    undefined mReserved[0x38];
};

} // namespace Game
