#include "Game/Enemy/Enm_TakopterBomb.h"
#include <cmath>

namespace Game {

Enm_TakopterBomb::Enm_TakopterBomb()
    : mPosition(0.0f, cHoverAltitude, 0.0f),
      mVelocity(0.0f, 0.0f, 0.0f),
      mHp(cMaxHp),
      mPropellerSpeed(1.0f),
      mBobPhase(0.0f),
      mState(TakopterBombState::cPatrolHover),
      mStateTimer(0),
      mBombCooldown(cBombIntervalFrames),
      mEggDropCount(5) {
}

Enm_TakopterBomb::~Enm_TakopterBomb() {
}

void Enm_TakopterBomb::init() {
    GambitActor::init();
    mPosition.set(0.0f, cHoverAltitude, 0.0f);
    mVelocity.set(0.0f, 0.0f, 0.0f);
    mHp = cMaxHp;
    mPropellerSpeed = 1.0f;
    mBobPhase = 0.0f;
    mState = TakopterBombState::cPatrolHover;
    mStateTimer = 0;
    mBombCooldown = cBombIntervalFrames;
    mEggDropCount = 5;
}

void Enm_TakopterBomb::dropBombPayload() {
    mBombCooldown = cBombIntervalFrames;
    // In full engine, spawns BulletEnemyBomb instance dropped downwards
}

void Enm_TakopterBomb::applyDamage(f32 damage, const sead::Vector3f& knockbackDir) {
    if (mState == TakopterBombState::cDefeated) {
        return;
    }

    mHp -= damage;
    mVelocity.x += knockbackDir.x * 0.1f;
    mVelocity.y += knockbackDir.y * 0.1f;
    mVelocity.z += knockbackDir.z * 0.1f;

    if (mHp <= 0.0f) {
        mHp = 0.0f;
        mState = TakopterBombState::cDefeated;
        mStateTimer = 0;
    } else {
        mState = TakopterBombState::cHitStagger;
        mStateTimer = 0;
    }
}

void Enm_TakopterBomb::updateAi(const sead::Vector3f& playerPos) {
    if (mState == TakopterBombState::cDefeated) {
        return;
    }

    f32 dx = playerPos.x - mPosition.x;
    f32 dz = playerPos.z - mPosition.z;
    f32 distSq = dx * dx + dz * dz;

    if (mBombCooldown > 0) {
        mBombCooldown--;
    }

    switch (mState) {
        case TakopterBombState::cPatrolHover: {
            if (distSq < (cDetectionRange * cDetectionRange)) {
                mState = TakopterBombState::cAimPlayer;
                mStateTimer = 0;
            }
            break;
        }

        case TakopterBombState::cAimPlayer: {
            // Glide towards overhead position above player
            f32 dist = std::sqrt(distSq);
            if (dist > 1.0f) {
                mPosition.x += (dx / dist) * 0.05f;
                mPosition.z += (dz / dist) * 0.05f;
            }

            if (mBombCooldown == 0 && dist < 8.0f) {
                mState = TakopterBombState::cDropBomb;
                mStateTimer = 0;
            }
            break;
        }

        case TakopterBombState::cDropBomb: {
            mStateTimer++;
            if (mStateTimer >= 15) {
                dropBombPayload();
                mState = TakopterBombState::cAimPlayer;
            }
            break;
        }

        case TakopterBombState::cHitStagger: {
            mStateTimer++;
            if (mStateTimer > 12) {
                mState = TakopterBombState::cAimPlayer;
            }
            break;
        }

        default:
            break;
    }
}

void Enm_TakopterBomb::update() {
    if (mState == TakopterBombState::cDefeated) {
        mStateTimer++;
        mVelocity.y -= 0.02f; // Plummet
        mPosition.y += mVelocity.y;
        return;
    }

    mBobPhase += 0.04f;
    mPosition.y += std::sin(mBobPhase) * 0.01f;

    mPosition.x += mVelocity.x;
    mPosition.y += mVelocity.y;
    mPosition.z += mVelocity.z;

    mVelocity.x *= 0.9f;
    mVelocity.y *= 0.9f;
    mVelocity.z *= 0.9f;
}

void Enm_TakopterBomb::draw() {
    // Model and dual propeller spinning rendering handled by ModelSceneMgr
}

} // namespace Game
