#include "Game/Enemy/Enm_CylinderKing.h"
#include "Game/Paint/PaintTextureMgr.h"

namespace Game {

Enm_CylinderKing::Enm_CylinderKing()
    : mState(CylinderKingState::cRotatingBarrage),
      mPhase(CylinderKingPhase::cPhase1),
      mStateTimer(0),
      mPosition(0.0f, 0.0f, 0.0f),
      mRotationAngle(0.0f),
      mRotationSpeed(0.015f),
      mTentacleHp(100.0f) {
}

Enm_CylinderKing::~Enm_CylinderKing() {
}

void Enm_CylinderKing::init() {
    GambitActor::init();
    mPhase = CylinderKingPhase::cPhase1;
    mState = CylinderKingState::cRotatingBarrage;
    mStateTimer = 0;
    mPosition.set(0.0f, 0.0f, 0.0f);
    mRotationAngle = 0.0f;
    mRotationSpeed = 0.015f;
    mTentacleHp = 100.0f;

    setupPhaseConfig();
}

void Enm_CylinderKing::setupPhaseConfig() {
    f32 speedTable[] = { 0.015f, 0.025f, 0.040f };
    mRotationSpeed = speedTable[static_cast<u32>(mPhase) - 1];

    for (u32 i = 0; i < cTotalHoles; ++i) {
        mHoles[i].init();
        f32 height = 1.5f + (static_cast<f32>(i % 3) * 2.5f);
        f32 angle = (static_cast<f32>(i) / static_cast<f32>(cTotalHoles)) * 6.283185f;
        mHoles[i].setupHole(i, height, angle);
    }
}

u32 Enm_CylinderKing::countInkedHoles() const {
    u32 count = 0;
    for (u32 i = 0; i < cTotalHoles; ++i) {
        if (mHoles[i].isInkedForClimbing()) {
            count++;
        }
    }
    return count;
}

void Enm_CylinderKing::applyTentacleDamage(f32 damage) {
    if (mState != CylinderKingState::cTopExposed) {
        return;
    }

    mTentacleHp -= damage;
    if (mTentacleHp <= 0.0f) {
        if (mPhase == CylinderKingPhase::cPhase1) {
            mPhase = CylinderKingPhase::cPhase2;
            mTentacleHp = 100.0f;
            mState = CylinderKingState::cPhaseTransition;
            mStateTimer = 0;
        } else if (mPhase == CylinderKingPhase::cPhase2) {
            mPhase = CylinderKingPhase::cPhase3;
            mTentacleHp = 100.0f;
            mState = CylinderKingState::cPhaseTransition;
            mStateTimer = 0;
        } else {
            mState = CylinderKingState::cDefeated;
            mStateTimer = 0;
        }
    }
}

void Enm_CylinderKing::updateBossAi(const sead::Vector3f& playerPos) {
    if (mState == CylinderKingState::cDefeated) {
        return;
    }

    switch (mState) {
        case CylinderKingState::cRotatingBarrage: {
            // Continually rotate tower
            mRotationAngle += mRotationSpeed;
            if (mRotationAngle > 6.283185f) {
                mRotationAngle -= 6.283185f;
            }

            // Periodically trigger holes to open and fire ink volleys
            if (mStateTimer % 90 == 0) {
                u32 fireIndex = (mStateTimer / 90) % cTotalHoles;
                mHoles[fireIndex].openAndFire();
            }

            // If player has linked enough climbable rungs to summit, top opens
            if (countInkedHoles() >= 2 || mStateTimer >= 600) {
                mState = CylinderKingState::cTopExposed;
                mStateTimer = 0;
            }
            break;
        }

        case CylinderKingState::cTopExposed:
            // Slower rotation while tentacle is exposed at the peak
            mRotationAngle += mRotationSpeed * 0.5f;
            if (mStateTimer >= 360) { // 6 seconds exposed
                mState = CylinderKingState::cPhaseTransition;
                mStateTimer = 0;
            }
            break;

        case CylinderKingState::cPhaseTransition:
            // Shake off ink and reset
            if (mStateTimer >= 60) {
                setupPhaseConfig();
                mState = CylinderKingState::cRotatingBarrage;
                mStateTimer = 0;
            }
            break;

        case CylinderKingState::cDefeated:
        default:
            break;
    }
}

void Enm_CylinderKing::update() {
    mStateTimer++;
    for (u32 i = 0; i < cTotalHoles; ++i) {
        mHoles[i].update();
    }
}

void Enm_CylinderKing::draw() {
    if (mState != CylinderKingState::cDefeated) {
        GambitActor::draw();
        for (u32 i = 0; i < cTotalHoles; ++i) {
            mHoles[i].draw();
        }
    }
}

} // namespace Game
