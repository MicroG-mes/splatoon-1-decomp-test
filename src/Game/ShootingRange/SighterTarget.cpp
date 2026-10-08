#include "Game/ShootingRange/SighterTarget.h"

namespace Game {

SighterTarget::SighterTarget()
    : mState(TargetState::cWait),
      mStateTimer(0),
      mPosition(0.0f, 0.0f, 0.0f),
      mBasePosition(0.0f, 0.0f, 0.0f),
      mIsMoving(false),
      mMovePhase(0.0f),
      mMaxHp(100.0f),
      mCurrentHp(100.0f),
      mDefenseLevel(0),
      mLastDamage(0.0f),
      mWobbleAngle(0.0f),
      mWobbleVelocity(0.0f) {
}

SighterTarget::~SighterTarget() {
}

void SighterTarget::init() {
    GambitActor::init();
    mCurrentHp = mMaxHp;
    mState = TargetState::cWait;
}

void SighterTarget::setup(const sead::Vector3f& pos, f32 maxHp, u32 defenseLevel, bool isMoving) {
    mBasePosition = pos;
    mPosition = pos;
    mMaxHp = maxHp;
    mCurrentHp = maxHp;
    mDefenseLevel = defenseLevel;
    mIsMoving = isMoving;
    mMovePhase = 0.0f;
    mState = TargetState::cWait;
}

void SighterTarget::calculateEffectiveDamage(f32 rawDamage) {
    // Defense Up reduction formula in Splatoon: ~9% reduction per Defense Up tier
    f32 factor = 1.0f;
    if (mDefenseLevel == 1) factor = 0.91f;
    else if (mDefenseLevel == 2) factor = 0.83f;
    else if (mDefenseLevel >= 3) factor = 0.77f;

    mLastDamage = rawDamage * factor;
}

void SighterTarget::applyDamage(f32 damage) {
    if (mState == TargetState::cPopped || mState == TargetState::cRegenerating) {
        return;
    }

    calculateEffectiveDamage(damage);
    mCurrentHp -= mLastDamage;
    mWobbleVelocity += 0.25f; // Physics recoil

    if (mCurrentHp <= 0.0f) {
        mCurrentHp = 0.0f;
        mState = TargetState::cPopped;
        mStateTimer = 0;
    } else {
        mState = TargetState::cHitRecoil;
        mStateTimer = 0;
    }
}

void SighterTarget::update() {
    mStateTimer++;

    // Harmonic spring wobble physics
    mWobbleAngle += mWobbleVelocity;
    mWobbleVelocity -= mWobbleAngle * 0.15f; // Spring tension
    mWobbleVelocity *= 0.88f;                // Friction damping

    // Rail movement
    if (mIsMoving && mState != TargetState::cPopped) {
        mMovePhase += 0.02f;
        mPosition.x = mBasePosition.x + 3.5f * __builtin_sinf(mMovePhase);
    }

    switch (mState) {
        case TargetState::cHitRecoil:
            if (mStateTimer > 15) {
                mState = TargetState::cWait;
            }
            break;

        case TargetState::cPopped:
            // Pop deflation animation (45 frames)
            if (mStateTimer > 45) {
                mState = TargetState::cRegenerating;
                mStateTimer = 0;
            }
            break;

        case TargetState::cRegenerating:
            // Re-inflate target dummy
            if (mStateTimer > 30) {
                mCurrentHp = mMaxHp;
                mState = TargetState::cWait;
            }
            break;

        default:
            break;
    }
}

void SighterTarget::draw() {
    GambitActor::draw();
}

} // namespace Game
