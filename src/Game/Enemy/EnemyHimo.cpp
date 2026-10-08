#include "Game/Enemy/EnemyHimo.h"
#include "Game/Paint/PaintTextureMgr.h"

namespace Game {

EnemyHimo::EnemyHimo()
    : mState(OctosniperState::cIdleScan),
      mStateTimer(0),
      mPosition(0.0f, 6.0f, 0.0f),
      mTargetPos(0.0f, 0.0f, 0.0f),
      mMaxHp(40.0f),
      mCurrentHp(40.0f) {
}

EnemyHimo::~EnemyHimo() {
}

void EnemyHimo::init() {
    GambitActor::init();
    mCurrentHp = mMaxHp;
    mState = OctosniperState::cIdleScan;
}

void EnemyHimo::applyDamage(f32 damage) {
    if (mState == OctosniperState::cSplatted) {
        return;
    }

    mCurrentHp -= damage;
    if (mCurrentHp <= 0.0f) {
        mCurrentHp = 0.0f;
        mState = OctosniperState::cSplatted;
        mStateTimer = 0;
    }
}

void EnemyHimo::fireSniperBeam() {
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        // Splatters straight line towards player target position
        paint->splatInk(mTargetPos, 1.8f, 1); // Enemy ink
    }
}

void EnemyHimo::updateAi(const sead::Vector3f& playerPos) {
    if (mState == OctosniperState::cSplatted) {
        return;
    }

    f32 dx = playerPos.x - mPosition.x;
    f32 dz = playerPos.z - mPosition.z;
    f32 distSq = dx * dx + dz * dz;

    switch (mState) {
        case OctosniperState::cIdleScan:
            if (distSq < 35.0f * 35.0f) { // Long 35m sniper view
                mTargetPos = playerPos;
                mState = OctosniperState::cLockLaser;
                mStateTimer = 0;
            }
            break;

        case OctosniperState::cLockLaser:
            // Laser smoothly tracks player
            mTargetPos.x += (playerPos.x - mTargetPos.x) * 0.08f;
            mTargetPos.z += (playerPos.z - mTargetPos.z) * 0.08f;

            if (mStateTimer >= 90) { // 1.5s lock on
                mState = OctosniperState::cFireSnipe;
                mStateTimer = 0;
                fireSniperBeam();
            }
            break;

        case OctosniperState::cFireSnipe:
            if (mStateTimer >= 20) {
                mState = OctosniperState::cCooldown;
                mStateTimer = 0;
            }
            break;

        case OctosniperState::cCooldown:
            if (mStateTimer >= 75) { // 1.25s reload cooldown
                mState = OctosniperState::cIdleScan;
            }
            break;

        default:
            break;
    }
}

void EnemyHimo::update() {
    mStateTimer++;
}

void EnemyHimo::draw() {
    if (mState != OctosniperState::cSplatted) {
        GambitActor::draw();
    }
}

} // namespace Game
