#include "Game/Enemy/GameEnemyCleaner.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cmath>

namespace Game {

GameEnemyCleaner::GameEnemyCleaner()
    : mPosition(0.0f, 0.0f, 0.0f),
      mVelocity(0.0f, 0.0f, 0.0f),
      mHeading(0.0f, 0.0f, 1.0f),
      mHealth(cMaxHealth),
      mScale(1.0f),
      mIsAlive(true),
      mState(EnmCleanerState::cVacuumPatrol),
      mStateTimer(0),
      mCleanTimer(0) {
    mTransformMatrix.makeIdentity();
}

GameEnemyCleaner::~GameEnemyCleaner() = default;

void GameEnemyCleaner::init() {
    GambitActor::init();
    vfunc_3();
}

/**
 * Enm_Cleaner__vfunc_1 @ 0x022dafb8
 * Actor destruction & cleanup.
 */
void GameEnemyCleaner::vfunc_1() {
    mIsAlive = false;
    mState = EnmCleanerState::cDestroyed;
}

/**
 * Enm_Cleaner__vfunc_3 @ 0x022d4ce0
 * Initialization of vacuum brush and navigation nodes.
 */
void GameEnemyCleaner::vfunc_3() {
    mHealth = cMaxHealth;
    mScale = 1.0f;
    mIsAlive = true;
    mState = EnmCleanerState::cVacuumPatrol;
    mStateTimer = 0;
    mCleanTimer = 0;
}

/**
 * Enm_Cleaner__vfunc_5 @ 0x022d5168
 * Reset position and heading.
 */
void GameEnemyCleaner::vfunc_5() {
    mVelocity.set(0.0f, 0.0f, 0.0f);
    mStateTimer = 0;
}

/**
 * Enm_Cleaner__vfunc_7 @ 0x022d9670
 * Main Squee-G AI tick.
 * Updates state, checks timer 0x2B0, and updates steering.
 */
void GameEnemyCleaner::vfunc_7() {
    if (!mIsAlive) return;

    mStateTimer++;

    if (mCleanTimer > 0) {
        mCleanTimer--;
    }

    // Skip movement if in state 5 (cSpinStunned)
    if (mState != EnmCleanerState::cSpinStunned) {
        mPosition.x += mVelocity.x;
        mPosition.y += mVelocity.y;
        mPosition.z += mVelocity.z;
    } else {
        if (mStateTimer >= 45) {
            mState = EnmCleanerState::cVacuumPatrol;
            mStateTimer = 0;
        }
    }
}

/**
 * Enm_Cleaner__vfunc_11 @ 0x022d97fc
 * Updates 4x3 transformation matrix and absorbs ink on the ground.
 */
void GameEnemyCleaner::vfunc_11() {
    if (!mIsAlive) return;

    // Update 4x3 matrix with heading and position
    f32 angle = std::atan2(mHeading.x, mHeading.z);
    f32 cosA = std::cos(angle);
    f32 sinA = std::sin(angle);

    mTransformMatrix.m[0][0] = cosA * mScale;
    mTransformMatrix.m[0][1] = 0.0f;
    mTransformMatrix.m[0][2] = sinA * mScale;
    mTransformMatrix.m[0][3] = mPosition.x;

    mTransformMatrix.m[1][0] = 0.0f;
    mTransformMatrix.m[1][1] = mScale;
    mTransformMatrix.m[1][2] = 0.0f;
    mTransformMatrix.m[1][3] = mPosition.y;

    mTransformMatrix.m[2][0] = -sinA * mScale;
    mTransformMatrix.m[2][1] = 0.0f;
    mTransformMatrix.m[2][2] = cosA * mScale;
    mTransformMatrix.m[2][3] = mPosition.z;

    vacuumInkAtCurrentPos();
}

/**
 * Enm_Cleaner__vfunc_47 @ 0x022daff4
 * Render vacuum model.
 */
void GameEnemyCleaner::vfunc_47() {
}

/**
 * Enm_Cleaner__vfunc_62 @ 0x022d9b2c
 * Damage handling & rear weakpoint test.
 */
void GameEnemyCleaner::vfunc_62(f32 damage, bool isRearHit) {
    if (!mIsAlive) return;

    if (isRearHit) {
        mHealth -= damage;
    } else {
        // Frontal bumper deflection: absorbs damage and stuns briefly
        mState = EnmCleanerState::cSpinStunned;
        mStateTimer = 0;
        mHealth -= damage * 0.1f;
    }

    if (mHealth <= 0.0f) {
        mHealth = 0.0f;
        mIsAlive = false;
        vfunc_1();

        PaintTextureMgr* paint = PaintTextureMgr::instance();
        if (paint) {
            paint->splatInk(mPosition, 2.0f, 0); // Burst
        }
    }
}

void GameEnemyCleaner::spawn(const sead::Vector3f& spawnPos) {
    mPosition = spawnPos;
    vfunc_3();
}

void GameEnemyCleaner::updateNavigation(const sead::Vector3f& targetInkPos, bool foundInk) {
    if (!mIsAlive || mState == EnmCleanerState::cSpinStunned) return;

    if (foundInk) {
        mState = EnmCleanerState::cTargetInk;
        f32 dx = targetInkPos.x - mPosition.x;
        f32 dz = targetInkPos.z - mPosition.z;
        f32 dist = std::sqrt(dx * dx + dz * dz);
        if (dist > 0.1f) {
            mHeading.set(dx / dist, 0.0f, dz / dist);
            mVelocity.set(mHeading.x * cRushSpeed, 0.0f, mHeading.z * cRushSpeed);
        }
    } else {
        mState = EnmCleanerState::cVacuumPatrol;
        mVelocity.set(mHeading.x * cNormalSpeed, 0.0f, mHeading.z * cNormalSpeed);
    }
}

void GameEnemyCleaner::vacuumInkAtCurrentPos() {
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        // Absorbs and removes friendly ink from underneath vacuum chassis
        paint->splatInk(mPosition, cVacuumRadius, 1); // Overwrites with Octarian team color
    }
}

void GameEnemyCleaner::update() {
    GambitActor::update();
    vfunc_7();
    vfunc_11();
}

void GameEnemyCleaner::draw() {
    GambitActor::draw();
    vfunc_47();
}

} // namespace Game
