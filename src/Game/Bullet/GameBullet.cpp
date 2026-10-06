#include "Game/Bullet/GameBullet.h"
#include "Game/Paint/PaintTextureMgr.h"

namespace Game {

GameBullet::GameBullet()
    : mVelocity(0.0f, 0.0f, 0.0f),
      mGravity(0.025f),
      mDamage(28.0f), // Typical shooter damage (Splattershot)
      mPaintRadius(1.2f),
      mLifeSpanFrames(30),
      mTeamId(0),
      mOwnerPlayerId(0),
      mHasCollided(false) {}

GameBullet::~GameBullet() = default;

void GameBullet::init() {
    GambitActor::init();
}

void GameBullet::launch(const sead::Vector3f& startPos, const sead::Vector3f& direction, f32 speed, u32 teamId) {
    mPosition = startPos;
    mVelocity.x = direction.x * speed;
    mVelocity.y = direction.y * speed;
    mVelocity.z = direction.z * speed;
    mTeamId = teamId;
    mHasCollided = false;
}

void GameBullet::update() {
    if (mHasCollided) return;

    // Ballistic trajectory
    mVelocity.y -= mGravity;
    mPosition.x += mVelocity.x;
    mPosition.y += mVelocity.y;
    mPosition.z += mVelocity.z;

    // Check lifetime
    if (--mLifeSpanFrames <= 0) {
        destroy();
    }
}

void GameBullet::draw() {
    GambitActor::draw();
}

void GameBullet::onHitGround(const sead::Vector3f& hitPos, const sead::Vector3f& normal) {
    mHasCollided = true;
    if (PaintTextureMgr::instance()) {
        PaintColor c = (mTeamId == 0) ? PaintColor::TeamAlpha : PaintColor::TeamBravo;
        PaintTextureMgr::instance()->paintSplat(hitPos, mPaintRadius, c);
    }
    destroy();
}

void GameBullet::onHitWall(const sead::Vector3f& hitPos, const sead::Vector3f& normal) {
    mHasCollided = true;
    // Paint wall splat via WallPaintMgr
    destroy();
}

void GameBullet::onHitActor(GambitActor* target) {
    mHasCollided = true;
    destroy();
}

} // namespace Game
