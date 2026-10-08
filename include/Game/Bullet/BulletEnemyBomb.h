#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class EnemyBombState : u32 {
    cInAirAirborne = 0,
    cGroundedFuse  = 1,
    cDetonating    = 2,
    cFinished      = 3
};

class BulletEnemyBomb : public GambitActor {
public:
    static constexpr f32 cLethalDamage = 120.0f;
    static constexpr f32 cBlastRadius = 5.0f;
    static constexpr s32 cFuseDurationFrames = 120; // 2 seconds fuse
    static constexpr f32 cGravity = 0.038f;
    static constexpr f32 cBounceElasticity = 0.65f;

    BulletEnemyBomb();
    virtual ~BulletEnemyBomb() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void launch(const sead::Vector3f& startPos, const sead::Vector3f& initVel);

    EnemyBombState getState() const { return mState; }
    bool isFinished() const { return mState == EnemyBombState::cFinished; }
    f32 getFuseProgress() const { return static_cast<f32>(mFuseTimer) / static_cast<f32>(cFuseDurationFrames); }
    const sead::Vector3f& getPosition() const { return mPosition; }

protected:
    void triggerDetonation();

    sead::Vector3f mPosition;
    sead::Vector3f mVelocity;
    EnemyBombState mState;
    s32 mFuseTimer;
    s32 mTimer;

    undefined mReserved[0x38];
};

} // namespace Game
