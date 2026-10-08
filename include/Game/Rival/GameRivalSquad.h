#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class RivalPlanId : u32 {
    cStandby   = 0, // 0x0272D248: Submerged in ink, ambush ready
    cPatrol    = 1, // Moving between tactical waypoints
    cEngage    = 2, // Firing Octoshot / strafing
    cBombToss  = 3, // Launching Splat Bomb
    cRetreat   = 4, // Diving to recover ink / health
    cSuperJump = 5  // Super Jumping to safety
};

enum class RivalDifficulty : u32 {
    cLevel1 = 0, // Standard Octoling
    cLevel2 = 1, // Advanced Octoling
    cLevel3 = 2  // Elite Kelp Octoling (Octoling Elite)
};

/**
 * GameRivalSquad / GameRivalSquadController
 * Address: vtable @ 0x100F6234
 * Authentic path: D:/home/Cafe/Gambit/App/Program/Game/Rival/GameRivalSquad.cpp
 * Coordinates tactical behavior for an Octoling combatant.
 */
class GameRivalSquad : public GambitActor {
public:
    static constexpr f32 cEngagementDistance = 18.0f;
    static constexpr f32 cShootingRange = 14.0f;
    static constexpr f32 cRetreatHealthThreshold = 35.0f;
    static constexpr f32 cMaxHealth = 100.0f;

    GameRivalSquad();
    virtual ~GameRivalSquad() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void spawn(const sead::Vector3f& spawnPos, RivalDifficulty difficulty = RivalDifficulty::cLevel1);
    void updateTactics(const sead::Vector3f& playerPos);
    void applyDamage(f32 damage);
    void triggerRespawn(const sead::Vector3f& respawnBeacon);

    RivalPlanId getCurrentPlan() const { return mCurrentPlan; }
    RivalDifficulty getDifficulty() const { return mDifficulty; }
    f32 getHealth() const { return mHealth; }
    bool isAlive() const { return mIsAlive; }
    bool isElite() const { return mDifficulty == RivalDifficulty::cLevel3; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    const sead::Vector3f& getVelocity() const { return mVelocity; }

protected:
    void planPatrol(const sead::Vector3f& playerPos);
    void planEngage(const sead::Vector3f& playerPos);
    void planRetreat(const sead::Vector3f& playerPos);

    sead::Vector3f mPosition;
    sead::Vector3f mVelocity;
    sead::Vector3f mTargetPatrolPoint;

    f32 mHealth;
    f32 mInkLevel;
    bool mIsAlive;
    bool mIsSubmerged;

    RivalPlanId mCurrentPlan;
    RivalDifficulty mDifficulty;
    s32 mPlanTimer;
    s32 mBurstFireCooldown;
    s32 mBombCooldown;

    u8 mSquadMemberIndex;
};

} // namespace Game
