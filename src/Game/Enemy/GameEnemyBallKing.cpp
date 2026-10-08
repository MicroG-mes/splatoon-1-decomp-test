#include "Game/Enemy/GameEnemyBallKing.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cmath>
#include <algorithm>

namespace Game {

GameEnemyBallKing::GameEnemyBallKing()
    : mState(OctowhirlState::cRevUpEngine),
      mPhase(OctowhirlPhase::cPhase1),
      mStateTimer(0),
      mPosition(0.0f, 0.0f, 0.0f),
      mVelocity(0.0f, 0.0f, 0.0f),
      mRollDirection(0.0f, 0.0f, 1.0f),
      mCurrentSpeed(0.0f),
      mTentacleHp(cTentacleMaxHp),
      mIsShieldActive(1),
      mVulnerabilityTimer(0),
      mStunRecoveryTimer(0) {
}

GameEnemyBallKing::~GameEnemyBallKing() = default;

void GameEnemyBallKing::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mVelocity.set(0.0f, 0.0f, 0.0f);
    mState = OctowhirlState::cRevUpEngine;
    mPhase = OctowhirlPhase::cPhase1;
    mStateTimer = 0;
    mCurrentSpeed = 0.0f;
    mTentacleHp = cTentacleMaxHp;
    mIsShieldActive = 1;
    mVulnerabilityTimer = 0;
    mStunRecoveryTimer = 0;
}

/**
 * Enm_BallKing__vfunc_7 @ 0x022af2dc
 * Main boss update tick: integrates timers, manages shell states, and recovers armor.
 */
void GameEnemyBallKing::vfunc_7() {
    mStateTimer++;

    if (mVulnerabilityTimer > 0) {
        mVulnerabilityTimer--;
        if (mVulnerabilityTimer == 0 && mState == OctowhirlState::cFlippedExposed) {
            mState = OctowhirlState::cRecoverRighting;
            mStateTimer = 0;
        }
    }

    if (mStunRecoveryTimer > 0) {
        mStunRecoveryTimer--;
    }

    switch (mState) {
        case OctowhirlState::cRevUpEngine:
            if (mStateTimer >= 60) {
                mState = OctowhirlState::cRollingDash;
                mStateTimer = 0;
            }
            break;

        case OctowhirlState::cRollingDash: {
            f32 maxSpeed = cMaxRollSpeedPhase1;
            if (mPhase == OctowhirlPhase::cPhase2) maxSpeed = cMaxRollSpeedPhase2;
            else if (mPhase == OctowhirlPhase::cPhase3) maxSpeed = cMaxRollSpeedPhase3;

            mCurrentSpeed = std::min(maxSpeed, mCurrentSpeed + 0.02f);
            mVelocity.x = mRollDirection.x * mCurrentSpeed;
            mVelocity.z = mRollDirection.z * mCurrentSpeed;

            mPosition.x += mVelocity.x;
            mPosition.z += mVelocity.z;

            // Lay wide ink trail behind rolling clam
            PaintTextureMgr* paint = PaintTextureMgr::instance();
            if (paint) {
                paint->splatInk(mPosition, 2.8f, 1); // Enemy team ink
            }

            if (mStateTimer >= 180) { // Turn around after 3 seconds
                mState = OctowhirlState::cRevUpEngine;
                mStateTimer = 0;
                mCurrentSpeed = 0.0f;
            }
            break;
        }

        case OctowhirlState::cSpinoutSkid:
            mCurrentSpeed *= 0.88f; // Friction deceleration
            mPosition.x += mRollDirection.x * mCurrentSpeed;
            mPosition.z += mRollDirection.z * mCurrentSpeed;

            if (mCurrentSpeed < 0.02f) {
                mState = OctowhirlState::cFlippedExposed;
                mStateTimer = 0;
                mVulnerabilityTimer = 300; // 5 seconds vulnerability window
                mIsShieldActive = 0;
            }
            break;

        case OctowhirlState::cFlippedExposed:
            // Vulnerable to player fire
            break;

        case OctowhirlState::cRecoverRighting:
            if (mStateTimer >= 45) {
                mIsShieldActive = 1;
                mState = OctowhirlState::cRevUpEngine;
                mStateTimer = 0;
            }
            break;

        case OctowhirlState::cDefeated:
            break;
    }
}

/**
 * Enm_BallKing__vfunc_11 @ 0x022af5b4
 * Ink collision check: when rolling onto player ink, the clam skids and flips over!
 */
void GameEnemyBallKing::vfunc_11() {
    if (mState == OctowhirlState::cRollingDash) {
        triggerSpinout();
    }
}

void GameEnemyBallKing::triggerSpinout() {
    mState = OctowhirlState::cSpinoutSkid;
    mStateTimer = 0;
}

void GameEnemyBallKing::applyDamage(f32 damage, bool hitExposedTentacle) {
    if (mState == OctowhirlState::cFlippedExposed && hitExposedTentacle) {
        mTentacleHp -= damage;
        if (mTentacleHp <= 0.0f) {
            advancePhase();
        }
    }
}

void GameEnemyBallKing::advancePhase() {
    if (mPhase == OctowhirlPhase::cPhase1) {
        mPhase = OctowhirlPhase::cPhase2;
        mTentacleHp = cTentacleMaxHp;
        mState = OctowhirlState::cRecoverRighting;
        mStateTimer = 0;
        mVulnerabilityTimer = 0;
    } else if (mPhase == OctowhirlPhase::cPhase2) {
        mPhase = OctowhirlPhase::cPhase3;
        mTentacleHp = cTentacleMaxHp;
        mState = OctowhirlState::cRecoverRighting;
        mStateTimer = 0;
        mVulnerabilityTimer = 0;
    } else {
        mPhase = OctowhirlPhase::cPhase3;
        mState = OctowhirlState::cDefeated;
        mStateTimer = 0;

        PaintTextureMgr* paint = PaintTextureMgr::instance();
        if (paint) {
            paint->splatInk(mPosition, 5.0f, 0); // Massive Zapfish burst
        }
    }
}

void GameEnemyBallKing::updateBossAi(const sead::Vector3f& playerPos, bool isRollingOnPlayerInk) {
    if (mState == OctowhirlState::cRevUpEngine) {
        f32 dx = playerPos.x - mPosition.x;
        f32 dz = playerPos.z - mPosition.z;
        f32 dist = std::sqrt(dx * dx + dz * dz);
        if (dist > 0.1f) {
            mRollDirection.set(dx / dist, 0.0f, dz / dist);
        }
    }

    if (isRollingOnPlayerInk && mState == OctowhirlState::cRollingDash) {
        vfunc_11();
    }
}

void GameEnemyBallKing::update() {
    GambitActor::update();
    vfunc_7();
}

void GameEnemyBallKing::draw() {
    GambitActor::draw();
}

} // namespace Game
