#include "Game/Bullet/BulletEnemyBomb.h"
#include "Game/Paint/PaintTextureMgr.h"

namespace Game {

BulletEnemyBomb::BulletEnemyBomb()
    : mPosition(0.0f, 0.0f, 0.0f),
      mVelocity(0.0f, 0.0f, 0.0f),
      mState(EnemyBombState::cFinished),
      mFuseTimer(0),
      mTimer(0) {
}

BulletEnemyBomb::~BulletEnemyBomb() {
}

void BulletEnemyBomb::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mVelocity.set(0.0f, 0.0f, 0.0f);
    mState = EnemyBombState::cFinished;
    mFuseTimer = 0;
    mTimer = 0;
}

void BulletEnemyBomb::launch(const sead::Vector3f& startPos, const sead::Vector3f& initVel) {
    mPosition = startPos;
    mVelocity = initVel;
    mState = EnemyBombState::cInAirAirborne;
    mFuseTimer = 0;
    mTimer = 0;
}

void BulletEnemyBomb::triggerDetonation() {
    mState = EnemyBombState::cDetonating;
    mTimer = 0;

    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        paint->splatInk(mPosition, cBlastRadius, 1); // Enemy purple ink explosion
    }
}

void BulletEnemyBomb::update() {
    mTimer++;

    switch (mState) {
        case EnemyBombState::cInAirAirborne:
            mPosition.x += mVelocity.x;
            mPosition.y += mVelocity.y;
            mPosition.z += mVelocity.z;
            mVelocity.y -= cGravity;

            // Ground impact bounce
            if (mPosition.y <= 0.0f) {
                mPosition.y = 0.0f;
                mVelocity.y = -mVelocity.y * cBounceElasticity;
                mVelocity.x *= 0.70f;
                mVelocity.z *= 0.70f;

                // Start ground fuse timer
                mState = EnemyBombState::cGroundedFuse;
            }
            break;

        case EnemyBombState::cGroundedFuse:
            mFuseTimer++;
            // Damped rolling
            mPosition.x += mVelocity.x;
            mPosition.z += mVelocity.z;
            mVelocity.x *= 0.90f;
            mVelocity.z *= 0.90f;

            if (mFuseTimer >= cFuseDurationFrames) {
                triggerDetonation();
            }
            break;

        case EnemyBombState::cDetonating:
            if (mTimer >= 20) {
                mState = EnemyBombState::cFinished;
            }
            break;

        case EnemyBombState::cFinished:
        default:
            break;
    }
}

void BulletEnemyBomb::draw() {
    if (mState == EnemyBombState::cInAirAirborne || mState == EnemyBombState::cGroundedFuse) {
        GambitActor::draw();
    }
}

} // namespace Game
