#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class TakopterBombState : u32 {
    cPatrolHover  = 0,
    cAimPlayer    = 1,
    cDropBomb     = 2,
    cHitStagger   = 3,
    cDefeated     = 4
};

/**
 * Enm_TakopterBomb (Octobomber)
 * Heavy aerial Octarian bomber equipped with dual propellers and heavy bomb payload.
 * Hovers overhead and drops devastating bombs onto the player below.
 */
class Enm_TakopterBomb : public GambitActor {
public:
    static constexpr f32 cMaxHp = 120.0f;
    static constexpr f32 cHoverAltitude = 5.5f;
    static constexpr f32 cDetectionRange = 22.0f;
    static constexpr s32 cBombIntervalFrames = 150;

    Enm_TakopterBomb();
    virtual ~Enm_TakopterBomb() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void updateAi(const sead::Vector3f& playerPos);
    void applyDamage(f32 damage, const sead::Vector3f& knockbackDir);

    TakopterBombState getState() const { return mState; }
    f32 getHp() const { return mHp; }
    f32 getPropellerSpeed() const { return mPropellerSpeed; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    bool isDefeated() const { return mState == TakopterBombState::cDefeated; }

    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    void dropBombPayload();

    sead::Vector3f mPosition;
    sead::Vector3f mVelocity;
    f32 mHp;
    f32 mPropellerSpeed;
    f32 mBobPhase;

    TakopterBombState mState;
    s32 mStateTimer;
    s32 mBombCooldown;
    u32 mEggDropCount;
};

} // namespace Game
