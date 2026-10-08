#include "Game/Enemy/GameEnemyTakolien.h"
#include "Game/Bullet/GameBullet.h"
#include "Game/Bullet/GameBulletBombSucker.h"
#include "Game/Paint/PaintTextureMgr.h"

namespace Game {

GameEnemyTakolien::GameEnemyTakolien()
    : mCurrentState(TakolienState::Patrol),
      mStateTimer(0),
      mHealth(100.0f),
      mTargetDistance(999.0f),
      mTargetPlayerPos(0.0f, 0.0f, 0.0f),
      mCanSeePlayer(false),
      mIsSwimming(false),
      mAttackCooldown(0),
      mBombCooldown(0),
      mOctolingVariant(0) {
    mScale = sead::Vector3f(1.0f, 1.0f, 1.0f);
}

GameEnemyTakolien::~GameEnemyTakolien() = default;

void GameEnemyTakolien::init() {
    GambitActor::init();
    changeState(TakolienState::Patrol);
}

void GameEnemyTakolien::changeState(TakolienState state) {
    mCurrentState = state;
    mStateTimer = 0;

    switch (mCurrentState) {
        case TakolienState::SwimDodge:
            mIsSwimming = true;
            break;
        case TakolienState::Die:
            mIsSwimming = false;
            // Trigger splat particle and despawn
            destroy();
            break;
        default:
            mIsSwimming = false;
            break;
    }
}

void GameEnemyTakolien::update() {
    mStateTimer++;
    if (mAttackCooldown > 0) mAttackCooldown--;
    if (mBombCooldown > 0) mBombCooldown--;

    updateAI();
    updateCombatMovement();
}

void GameEnemyTakolien::draw() {
    GambitActor::draw();
}

void GameEnemyTakolien::updateAI() {
    switch (mCurrentState) {
        case TakolienState::Patrol:
            // Check line-of-sight to player
            if (mCanSeePlayer && mTargetDistance < 15.0f) {
                changeState(TakolienState::Pursue);
            }
            break;

        case TakolienState::Pursue:
            if (mTargetDistance <= 7.0f) {
                changeState(TakolienState::Shoot);
            } else if (!mCanSeePlayer) {
                changeState(TakolienState::SearchPlayer);
            }
            break;

        case TakolienState::Shoot:
            tryShoot();
            // Occasionally throw a bomb or swim dodge
            if (mBombCooldown == 0 && mTargetDistance > 4.0f) {
                changeState(TakolienState::ThrowBomb);
            } else if (mStateTimer > 60) {
                changeState(TakolienState::SwimDodge);
            }
            break;

        case TakolienState::ThrowBomb:
            tryThrowBomb();
            changeState(TakolienState::Shoot);
            break;

        case TakolienState::SwimDodge:
            // Relocate rapidly under ink for 45 frames
            if (mStateTimer > 45) {
                changeState(TakolienState::Shoot);
            }
            break;

        case TakolienState::SearchPlayer:
            if (mStateTimer > 120) {
                changeState(TakolienState::Patrol);
            }
            break;

        default:
            break;
    }
}

void GameEnemyTakolien::updateCombatMovement() {
    // Basic navigation towards target
    if (mCurrentState == TakolienState::Pursue) {
        sead::Vector3f diff(
            mTargetPlayerPos.x - mPosition.x,
            mTargetPlayerPos.y - mPosition.y,
            mTargetPlayerPos.z - mPosition.z
        );
        f32 len = (diff.x * diff.x) + (diff.z * diff.z);
        if (len > 0.1f) {
            f32 speed = mIsSwimming ? 0.25f : 0.12f;
            mPosition.x += (diff.x / len) * speed;
            mPosition.z += (diff.z / len) * speed;
        }
    }
}

void GameEnemyTakolien::tryShoot() {
    if (mAttackCooldown > 0) return;

    mAttackCooldown = 10; // Rapid fire (every 10 frames)
    // Bullet is spawned along facing direction towards target
}

void GameEnemyTakolien::tryThrowBomb() {
    mBombCooldown = 180; // 3 seconds cooldown between bombs
}

void GameEnemyTakolien::takeDamage(f32 damage, u32 attackerTeam) {
    mHealth -= damage;
    if (mHealth <= 0.0f) {
        changeState(TakolienState::Die);
    } else {
        // High damage causes immediate swim dodge
        if (damage > 30.0f) {
            changeState(TakolienState::SwimDodge);
        }
    }
}

} // namespace Game
