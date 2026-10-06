#include "Game/Bullet/GameBulletBombSucker.h"
#include "Game/Paint/PaintTextureMgr.h"

namespace Game {

GameBulletBombSucker::GameBulletBombSucker()
    : mIsStuck(false),
      mStickNormal(0.0f, 1.0f, 0.0f),
      mFuseFrames(120),       // ~2 seconds after sticking
      mExplosionRadius(4.5f) {
    mDamage = 180.0f;        // Lethal direct hit (Player HP is 100.0f)
    mPaintRadius = 5.2f;
    mLifeSpanFrames = 600;   // Despawn timeout if airborne
}

GameBulletBombSucker::~GameBulletBombSucker() = default;

void GameBulletBombSucker::onHitGround(const sead::Vector3f& hitPos, const sead::Vector3f& normal) {
    if (!mIsStuck) {
        mIsStuck = true;
        mPosition = hitPos;
        mStickNormal = normal;
        mVelocity = sead::Vector3f(0.0f, 0.0f, 0.0f); // Freeze in place
    }
}

void GameBulletBombSucker::onHitWall(const sead::Vector3f& hitPos, const sead::Vector3f& normal) {
    if (!mIsStuck) {
        mIsStuck = true;
        mPosition = hitPos;
        mStickNormal = normal;
        mVelocity = sead::Vector3f(0.0f, 0.0f, 0.0f); // Stick to wall
    }
}

void GameBulletBombSucker::update() {
    if (mIsStuck) {
        // Countdown fuse
        if (--mFuseFrames <= 0) {
            explode();
        }
    } else {
        // Airborne trajectory
        GameBullet::update();
    }
}

void GameBulletBombSucker::explode() {
    if (PaintTextureMgr::instance()) {
        PaintColor c = (mTeamId == 0) ? PaintColor::TeamAlpha : PaintColor::TeamBravo;
        PaintTextureMgr::instance()->paintSplat(mPosition, mPaintRadius, c);
    }
    destroy();
}

} // namespace Game
