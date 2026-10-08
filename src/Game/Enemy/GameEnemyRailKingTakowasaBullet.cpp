#include "Game/Enemy/GameEnemyRailKingTakowasaBullet.h"

namespace Game {

GameEnemyRailKingTakowasaBullet::GameEnemyRailKingTakowasaBullet()
    : mIsReflected(false),
      mHitsToReflect(3),
      mCurrentHits(0),
      mBossTargetPos(0.0f, 0.0f, 0.0f),
      mRotationAngle(0.0f) {
    mDamage = 60.0f;
    mPaintRadius = 3.5f;
    mLifeSpanFrames = 900;
}

GameEnemyRailKingTakowasaBullet::~GameEnemyRailKingTakowasaBullet() = default;

void GameEnemyRailKingTakowasaBullet::init() {
    GameBullet::init();
}

void GameEnemyRailKingTakowasaBullet::update() {
    mRotationAngle += 0.08f;

    if (mIsReflected) {
        // Homing back towards DJ Octavio
        sead::Vector3f diff(
            mBossTargetPos.x - mPosition.x,
            mBossTargetPos.y - mPosition.y,
            mBossTargetPos.z - mPosition.z
        );
        f32 distSq = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
        if (distSq > 0.5f) {
            f32 speed = 0.45f;
            mVelocity.x = (diff.x / distSq) * speed;
            mVelocity.y = (diff.y / distSq) * speed;
            mVelocity.z = (diff.z / distSq) * speed;
        } else {
            // Impact with boss
            destroy();
            return;
        }
    }

    GameBullet::update();
}

void GameEnemyRailKingTakowasaBullet::onHitByPlayerShot(f32 shotPower) {
    if (mIsReflected) return;

    mCurrentHits++;
    if (mCurrentHits >= mHitsToReflect) {
        reflectTowardsBoss(mBossTargetPos);
    }
}

void GameEnemyRailKingTakowasaBullet::reflectTowardsBoss(const sead::Vector3f& bossPos) {
    mIsReflected = true;
    mBossTargetPos = bossPos;
    mTeamId = 0; // Switches to Player's ink color
}

} // namespace Game
