#include "Game/Enemy/GameEnemyHideKing.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cmath>
#include <algorithm>
#include <cstring>

namespace Game {

GameEnemyHideKing::GameEnemyHideKing()
    : mPosition(0.0f, 0.0f, 0.0f),
      mState(OctonozzleState::cRotatingBarrage),
      mPhase(OctonozzlePhase::cPhase1),
      mStateTimer(0),
      mRotationAngle(0.0f),
      mRotationSpeed(0.012f),
      mTentacleHp(cTentacleMaxHp),
      mBarrageCooldown(0) {
    std::memset(mHolesPlugged, 0, sizeof(mHolesPlugged));
}

GameEnemyHideKing::~GameEnemyHideKing() = default;

void GameEnemyHideKing::init() {
    GambitActor::init();
    vfunc_3();
}

/**
 * Enm_CylinderKing__vfunc_1 @ 0x0234a900
 * Boss destruction and Zapfish rescue.
 */
void GameEnemyHideKing::vfunc_1() {
    mState = OctonozzleState::cDefeated;
}

/**
 * Enm_CylinderKing__vfunc_3 @ 0x023485c0
 * Cylinder hull geometry and hole nodes initialization.
 */
void GameEnemyHideKing::vfunc_3() {
    mState = OctonozzleState::cRotatingBarrage;
    mPhase = OctonozzlePhase::cPhase1;
    mStateTimer = 0;
    mRotationAngle = 0.0f;
    mRotationSpeed = 0.012f;
    mTentacleHp = cTentacleMaxHp;
    mBarrageCooldown = 0;
    std::memset(mHolesPlugged, 0, sizeof(mHolesPlugged));
}

/**
 * Enm_CylinderKing__vfunc_5 @ 0x02348c40
 * Reset rotation.
 */
void GameEnemyHideKing::vfunc_5() {
    mRotationAngle = 0.0f;
    mStateTimer = 0;
}

/**
 * Enm_CylinderKing__vfunc_7 @ 0x02349e50
 * Main Octonozzle AI tick.
 */
void GameEnemyHideKing::vfunc_7() {
    mStateTimer++;
    if (mBarrageCooldown > 0) mBarrageCooldown--;

    switch (mState) {
        case OctonozzleState::cRotatingBarrage: {
            mRotationAngle += mRotationSpeed;
            if (mRotationAngle > 6.2831853f) {
                mRotationAngle -= 6.2831853f;
            }

            // Check if all nozzle holes are plugged with player ink
            u32 pluggedCount = 0;
            for (u32 i = 0; i < cTotalNozzleHoles; ++i) {
                if (mHolesPlugged[i] != 0) pluggedCount++;
            }

            if (pluggedCount >= 3) {
                // Stunned: exposes tentacle on top of the cylinder
                mState = OctonozzleState::cTopExposed;
                mStateTimer = 0;
            }
            break;
        }

        case OctonozzleState::cTopExposed:
            if (mStateTimer >= 360) { // 6 seconds exposure window
                // Recovers and unplugs holes
                std::memset(mHolesPlugged, 0, sizeof(mHolesPlugged));
                mState = OctonozzleState::cRotatingBarrage;
                mStateTimer = 0;
            }
            break;

        case OctonozzleState::cPhaseTransition:
            if (mStateTimer >= 90) {
                mState = OctonozzleState::cRotatingBarrage;
                mStateTimer = 0;
            }
            break;

        case OctonozzleState::cDefeated:
            break;
    }
}

/**
 * Enm_CylinderKing__vfunc_11 @ 0x0234a180
 * Tentacle hit reaction & phase progression.
 */
void GameEnemyHideKing::vfunc_11(f32 damage) {
    if (mState != OctonozzleState::cTopExposed) return;

    mTentacleHp -= damage;
    if (mTentacleHp <= 0.0f) {
        advancePhase();
    }
}

void GameEnemyHideKing::advancePhase() {
    if (mPhase == OctonozzlePhase::cPhase1) {
        mPhase = OctonozzlePhase::cPhase2;
        mRotationSpeed = 0.018f;
        mTentacleHp = cTentacleMaxHp;
        std::memset(mHolesPlugged, 0, sizeof(mHolesPlugged));
        mState = OctonozzleState::cPhaseTransition;
        mStateTimer = 0;
    } else if (mPhase == OctonozzlePhase::cPhase2) {
        mPhase = OctonozzlePhase::cPhase3;
        mRotationSpeed = 0.026f;
        mTentacleHp = cTentacleMaxHp;
        std::memset(mHolesPlugged, 0, sizeof(mHolesPlugged));
        mState = OctonozzleState::cPhaseTransition;
        mStateTimer = 0;
    } else {
        mState = OctonozzleState::cDefeated;
        vfunc_1();

        PaintTextureMgr* paint = PaintTextureMgr::instance();
        if (paint) {
            paint->splatInk(mPosition, 6.0f, 0); // Giant burst
        }
    }
}

void GameEnemyHideKing::plugHoleWithInk(u32 holeIndex) {
    if (holeIndex < cTotalNozzleHoles) {
        mHolesPlugged[holeIndex] = 1;
    }
}

void GameEnemyHideKing::setupBoss(const sead::Vector3f& centerPos) {
    mPosition = centerPos;
    vfunc_3();
}

void GameEnemyHideKing::updateAi(const sead::Vector3f& playerPos) {
    (void)playerPos;
    vfunc_7();
}

void GameEnemyHideKing::update() {
    GambitActor::update();
    vfunc_7();
}

void GameEnemyHideKing::draw() {
    GambitActor::draw();
}

} // namespace Game
