#include "Game/Rival/GameRivalSquad.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cmath>
#include <algorithm>

namespace Game {

GameRivalSquad::GameRivalSquad()
    : mPosition(0.0f, 0.0f, 0.0f),
      mVelocity(0.0f, 0.0f, 0.0f),
      mTargetPatrolPoint(0.0f, 0.0f, 0.0f),
      mHealth(cMaxHealth),
      mInkLevel(1.0f),
      mIsAlive(true),
      mIsSubmerged(false),
      mCurrentPlan(RivalPlanId::cStandby),
      mDifficulty(RivalDifficulty::cLevel1),
      mPlanTimer(0),
      mBurstFireCooldown(0),
      mBombCooldown(0),
      mSquadMemberIndex(0) {
}

GameRivalSquad::~GameRivalSquad() = default;

void GameRivalSquad::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mVelocity.set(0.0f, 0.0f, 0.0f);
    mHealth = cMaxHealth;
    mInkLevel = 1.0f;
    mIsAlive = true;
    mIsSubmerged = false;
    mCurrentPlan = RivalPlanId::cStandby;
    mPlanTimer = 0;
    mBurstFireCooldown = 0;
    mBombCooldown = 0;
}

void GameRivalSquad::spawn(const sead::Vector3f& spawnPos, RivalDifficulty difficulty) {
    mPosition = spawnPos;
    mTargetPatrolPoint = spawnPos;
    mDifficulty = difficulty;
    mHealth = cMaxHealth;
    mInkLevel = 1.0f;
    mIsAlive = true;
    mIsSubmerged = false;
    mCurrentPlan = RivalPlanId::cStandby;
    mPlanTimer = 0;
}

void GameRivalSquad::applyDamage(f32 damage) {
    if (!mIsAlive) return;

    mHealth -= damage;
    if (mHealth <= 0.0f) {
        mHealth = 0.0f;
        mIsAlive = false;
        mCurrentPlan = RivalPlanId::cStandby;

        // Splat burst in opposing team color
        PaintTextureMgr* paint = PaintTextureMgr::instance();
        if (paint) {
            paint->splatInk(mPosition, 2.0f, 0);
        }
    } else if (mHealth < cRetreatHealthThreshold) {
        // High-level Octolings automatically dive into ink and retreat
        mCurrentPlan = RivalPlanId::cRetreat;
        mPlanTimer = 0;
    }
}

void GameRivalSquad::triggerRespawn(const sead::Vector3f& respawnBeacon) {
    spawn(respawnBeacon, mDifficulty);
}

void GameRivalSquad::planPatrol(const sead::Vector3f& playerPos) {
    (void)playerPos;
    // Advance towards patrol waypoint
    f32 dx = mTargetPatrolPoint.x - mPosition.x;
    f32 dz = mTargetPatrolPoint.z - mPosition.z;
    f32 dist = std::sqrt(dx * dx + dz * dz);

    if (dist > 0.5f) {
        f32 moveSpeed = (mDifficulty == RivalDifficulty::cLevel3) ? 0.35f : 0.25f;
        mVelocity.x = (dx / dist) * moveSpeed;
        mVelocity.z = (dz / dist) * moveSpeed;
    } else {
        mVelocity.x = 0.0f;
        mVelocity.z = 0.0f;
    }

    // Paint path ahead
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint && (mPlanTimer % 15 == 0)) {
        paint->splatInk(mPosition, 1.2f, 1); // Team 1 Octarian ink
    }
}

void GameRivalSquad::planEngage(const sead::Vector3f& playerPos) {
    f32 dx = playerPos.x - mPosition.x;
    f32 dz = playerPos.z - mPosition.z;
    f32 dist = std::sqrt(dx * dx + dz * dz);

    // Lateral strafing movement
    f32 strafeSpeed = (mDifficulty == RivalDifficulty::cLevel3) ? 0.40f : 0.22f;
    f32 perpX = -dz / (dist + 0.001f);
    f32 perpZ = dx / (dist + 0.001f);

    f32 strafeDir = ((mPlanTimer / 30) % 2 == 0) ? 1.0f : -1.0f;
    mVelocity.x = perpX * strafeSpeed * strafeDir;
    mVelocity.z = perpZ * strafeSpeed * strafeDir;

    // Firing bursts
    if (mBurstFireCooldown <= 0) {
        mBurstFireCooldown = (mDifficulty == RivalDifficulty::cLevel3) ? 4 : 8;
        PaintTextureMgr* paint = PaintTextureMgr::instance();
        if (paint) {
            sead::Vector3f bulletPos(
                mPosition.x + (dx / dist) * 2.0f,
                mPosition.y + 1.0f,
                mPosition.z + (dz / dist) * 2.0f
            );
            paint->splatInk(bulletPos, 1.0f, 1);
        }
    }
}

void GameRivalSquad::planRetreat(const sead::Vector3f& playerPos) {
    f32 dx = mPosition.x - playerPos.x;
    f32 dz = mPosition.z - playerPos.z;
    f32 dist = std::sqrt(dx * dx + dz * dz);

    // Fast swim backwards
    f32 swimSpeed = (mDifficulty == RivalDifficulty::cLevel3) ? 0.55f : 0.40f;
    if (dist > 0.001f) {
        mVelocity.x = (dx / dist) * swimSpeed;
        mVelocity.z = (dz / dist) * swimSpeed;
    }

    mIsSubmerged = true;
    mHealth = std::min(cMaxHealth, mHealth + 0.2f); // Rapid ink recovery
    if (mHealth >= 80.0f || dist > cEngagementDistance) {
        mIsSubmerged = false;
        mCurrentPlan = RivalPlanId::cPatrol;
    }
}

void GameRivalSquad::updateTactics(const sead::Vector3f& playerPos) {
    if (!mIsAlive) return;

    mPlanTimer++;
    if (mBurstFireCooldown > 0) mBurstFireCooldown--;
    if (mBombCooldown > 0) mBombCooldown--;

    f32 dx = playerPos.x - mPosition.x;
    f32 dy = playerPos.y - mPosition.y;
    f32 dz = playerPos.z - mPosition.z;
    f32 dist = std::sqrt(dx * dx + dy * dy + dz * dz);

    // State machine transitions
    if (mCurrentPlan != RivalPlanId::cRetreat) {
        if (dist <= cShootingRange) {
            mCurrentPlan = RivalPlanId::cEngage;
        } else if (dist <= cEngagementDistance) {
            mCurrentPlan = RivalPlanId::cPatrol;
            mTargetPatrolPoint = playerPos; // Advance towards player
        } else {
            mCurrentPlan = RivalPlanId::cStandby;
        }
    }

    switch (mCurrentPlan) {
        case RivalPlanId::cStandby:
            mVelocity.set(0.0f, 0.0f, 0.0f);
            break;
        case RivalPlanId::cPatrol:
            planPatrol(playerPos);
            break;
        case RivalPlanId::cEngage:
            planEngage(playerPos);
            break;
        case RivalPlanId::cRetreat:
            planRetreat(playerPos);
            break;
        default:
            break;
    }

    // Integrate position
    mPosition.x += mVelocity.x;
    mPosition.y += mVelocity.y;
    mPosition.z += mVelocity.z;
}

void GameRivalSquad::update() {
    GambitActor::update();
}

void GameRivalSquad::draw() {
    GambitActor::draw();
}

} // namespace Game
