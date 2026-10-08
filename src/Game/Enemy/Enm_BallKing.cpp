#include "Game/Enemy/Enm_BallKing.h"
#include <cstring>
#include <cmath>
#include <algorithm>

namespace Game {

Enm_BallKing::Enm_BallKing()
    : mState(BallKingState::cRevUpEngine),
      mPhase(BallKingPhase::cPhase1),
      mStateTimer(0),
      mPosition(0.0f, 0.0f, 0.0f),
      mMoveDirection(0.0f, 0.0f, 1.0f),
      mCurrentSpeed(0.0f),
      mTentacleHp(100.0f),
      mExposedDurationFrames(180),
      mIsShieldActive(1),
      mAttackMode(1),
      mVulnerabilityTimer(0),
      mStunTimer(0),
      mCollisionMask(0x80000000),
      mShieldResetFlag(0),
      mLeftArmDetached(0),
      mRightArmDetached(0),
      mAnimBehaviorMode(0) {
    std::memset(mPaddingShield, 0, sizeof(mPaddingShield));
    std::memset(mPaddingPhase, 0, sizeof(mPaddingPhase));
    std::memset(mPaddingTimers, 0, sizeof(mPaddingTimers));
    std::memset(mPaddingArms, 0, sizeof(mPaddingArms));
    std::memset(mPaddingArm2, 0, sizeof(mPaddingArm2));
    std::memset(mPaddingArm3, 0, sizeof(mPaddingArm3));
}

Enm_BallKing::~Enm_BallKing() {
}

void Enm_BallKing::init() {
    GambitActor::init();
    mState = BallKingState::cRevUpEngine;
    mPhase = BallKingPhase::cPhase1;
    mTentacleHp = 100.0f;
    mIsShieldActive = 1;
    mAttackMode = 1;
    mVulnerabilityTimer = 0;
    mStunTimer = 0;
    mCollisionMask = 0x80000000;
    mLeftArmDetached = 0;
    mRightArmDetached = 0;
}

/**
 * Enm_BallKing__vfunc_7 @ 0x022af2dc
 * Core AI and vulnerability logic decompiled from PowerPC Espresso.
 * Updates rolling movement, decrements stun & vulnerability timers,
 * and re-engages shield armor when stun timer elapses.
 */
void Enm_BallKing::vfunc_7() {
    // Mode 2: High speed rolling dash
    if (mAttackMode == 2) {
        mAnimBehaviorMode = 1;
    } else {
        if (mState != BallKingState::cFlippedExposed && mState != BallKingState::cSpinoutSkid) {
            mLeftArmDetached = 0;
            mRightArmDetached = 0;
        }
    }

    // Decrement vulnerability timer (0x39C)
    if (mVulnerabilityTimer > 0) {
        mVulnerabilityTimer--;
    }

    // Decrement stun timer (0x3A0)
    if (mStunTimer > 0) {
        mStunTimer--;
        if (mStunTimer == 0) {
            // Stun expired: re-arm shields
            mIsShieldActive = 1;
            mShieldResetFlag = 0;
            mCollisionMask = 0x80000000;
            vfunc_31(); // Close clamshell armor
            if (mState == BallKingState::cFlippedExposed) {
                mState = BallKingState::cRecoverRighting;
                mStateTimer = 0;
            }
        }
    }
}

void Enm_BallKing::vfunc_11() {
    // Spinout on player ink contact
    if (mState == BallKingState::cRollingDash) {
        triggerSpinout();
    }
}

// 0x022C5A38: Decompiled vfunc_26 (Engine rev & torque spin setup)
void Enm_BallKing::vfunc_26() {
    mAttackMode = 2;
    mCollisionMask = 0xC0000000;
}

// 0x022C5AC0: Decompiled vfunc_28 (Collision detachment cleanup)
void Enm_BallKing::vfunc_28() {
    mCollisionMask = 0;
    mCurrentSpeed = 0.0f;
}

// 0x022A7F90: Decompiled vfunc_30 (Eject & detach clamshell armor collision bodies)
void Enm_BallKing::vfunc_30() {
    mLeftArmDetached = 1;
    mRightArmDetached = 1;
    mIsShieldActive = 0;
}

// 0x022A8018: Decompiled vfunc_31 (Attach & close clamshell armor collision bodies)
void Enm_BallKing::vfunc_31() {
    mLeftArmDetached = 0;
    mRightArmDetached = 0;
    mIsShieldActive = 1;
}

void Enm_BallKing::updateBossAi(const sead::Vector3f& playerPos, bool isRollingOnPlayerInk) {
    if (isRollingOnPlayerInk && mState == BallKingState::cRollingDash) {
        vfunc_11();
        return;
    }

    switch (mState) {
        case BallKingState::cRevUpEngine:
            mStateTimer++;
            mAttackMode = 1;
            if (mStateTimer >= 90) {
                mState = BallKingState::cRollingDash;
                vfunc_26(); // Enter rolling attack mode
                mStateTimer = 0;
                f32 dx = playerPos.x - mPosition.x;
                f32 dz = playerPos.z - mPosition.z;
                f32 len = std::sqrt(dx * dx + dz * dz);
                if (len > 0.01f) {
                    mMoveDirection.set(dx / len, 0.0f, dz / len);
                }
            }
            break;

        case BallKingState::cRollingDash: {
            f32 maxSpeed = (mPhase == BallKingPhase::cPhase1) ? cMaxRollSpeedPhase1 :
                           (mPhase == BallKingPhase::cPhase2) ? cMaxRollSpeedPhase2 : cMaxRollSpeedPhase3;
            mCurrentSpeed = std::min(maxSpeed, mCurrentSpeed + 0.02f);
            mPosition.x += mMoveDirection.x * mCurrentSpeed;
            mPosition.z += mMoveDirection.z * mCurrentSpeed;
            break;
        }

        case BallKingState::cSpinoutSkid:
            mStateTimer++;
            mCurrentSpeed *= 0.90f;
            mPosition.x += mMoveDirection.x * mCurrentSpeed;
            mPosition.z += mMoveDirection.z * mCurrentSpeed;
            if (mStateTimer >= 45) {
                mState = BallKingState::cFlippedExposed;
                mStateTimer = 0;
                vfunc_30(); // Eject clamshell armor
                mStunTimer = mExposedDurationFrames;
                mVulnerabilityTimer = mExposedDurationFrames;
            }
            break;

        case BallKingState::cFlippedExposed:
            // Boss disabled on back, tentacle exposed
            break;

        case BallKingState::cRecoverRighting:
            mStateTimer++;
            if (mStateTimer >= 60) {
                mState = BallKingState::cRevUpEngine;
                vfunc_31(); // Re-arm clamshell armor
                mStateTimer = 0;
            }
            break;

        case BallKingState::cDefeated:
            vfunc_28();
            break;
    }
}

void Enm_BallKing::triggerSpinout() {
    mState = BallKingState::cSpinoutSkid;
    mStateTimer = 0;
    mAttackMode = 1;
    vfunc_30(); // Eject clamshell armor immediately on skid
}

void Enm_BallKing::applyTentacleDamage(f32 damage) {
    if (mState != BallKingState::cFlippedExposed) {
        return;
    }

    mTentacleHp -= damage;
    if (mTentacleHp <= 0.0f) {
        if (mPhase == BallKingPhase::cPhase1) {
            mPhase = BallKingPhase::cPhase2;
            mTentacleHp = 100.0f;
            mState = BallKingState::cRecoverRighting;
            mStateTimer = 0;
        } else if (mPhase == BallKingPhase::cPhase2) {
            mPhase = BallKingPhase::cPhase3;
            mTentacleHp = 100.0f;
            mState = BallKingState::cRecoverRighting;
            mStateTimer = 0;
        } else {
            mState = BallKingState::cDefeated;
        }
    }
}

void Enm_BallKing::update() {
    GambitActor::update();
    vfunc_7();
}

void Enm_BallKing::draw() {
    GambitActor::draw();
}

} // namespace Game
