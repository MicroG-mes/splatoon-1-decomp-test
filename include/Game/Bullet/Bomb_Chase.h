#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class ChaseBombState : u32 {
    cLaunching     = 0,
    cCruisingTrail = 1,
    cHomingTarget  = 2,
    cDetonating    = 3,
    cFinished      = 4
};

class Bomb_Chase : public GambitActor {
public:
    static constexpr f32 cCruiseSpeed = 0.28f;
    static constexpr f32 cHomingSpeed = 0.42f;
    static constexpr f32 cDetectionRange = 18.0f;
    static constexpr f32 cLethalDamage = 180.0f;
    static constexpr f32 cBlastRadius = 4.2f;
    static constexpr s32 cMaxLifetimeFrames = 180; // 3 seconds

    Bomb_Chase();
    virtual ~Bomb_Chase() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void launch(const sead::Vector3f& startPos, f32 yawAngle, u32 teamId, u32 ownerPlayerId);
    void checkEnemyHoming(const sead::Vector3f& enemyPos, u32 enemyTeam);

    ChaseBombState getState() const { return mState; }
    bool isFinished() const { return mState == ChaseBombState::cFinished; }
    f32 getYawAngle() const { return mYawAngle; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    u32 getTeamId() const { return mTeamId; }

protected:
    void triggerDetonation();

    sead::Vector3f mPosition;
    f32 mYawAngle;
    u32 mTeamId;
    u32 mOwnerPlayerId;

    ChaseBombState mState;
    s32 mTimer;
    sead::Vector3f mHomingTargetPos;
    bool mHasTarget;

    undefined mReserved[0x38];
};

} // namespace Game
