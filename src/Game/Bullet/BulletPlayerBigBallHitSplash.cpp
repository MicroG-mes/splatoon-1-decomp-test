#include "Game/Bullet/BulletPlayerBigBallHitSplash.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cmath>

namespace Game {

BulletPlayerBigBallHitSplash::BulletPlayerBigBallHitSplash()
    : mPosition(0.0f, 0.0f, 0.0f),
      mVelocity(0.0f, 0.0f, 0.0f),
      mTeamId(0),
      mDamage(70.0f),
      mMaxRange(18.0f),
      mTraveledDist(0.0f),
      mGravity(0.022f),
      mIsGrounded(false) {
}

BulletPlayerBigBallHitSplash::~BulletPlayerBigBallHitSplash() {
}

void BulletPlayerBigBallHitSplash::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mVelocity.set(0.0f, 0.0f, 0.0f);
    mTraveledDist = 0.0f;
    mIsGrounded = false;
}

void BulletPlayerBigBallHitSplash::launchSlosh(const sead::Vector3f& pos, const sead::Vector3f& initialVel, f32 maxRange, u32 teamId) {
    mPosition = pos;
    mVelocity = initialVel;
    mMaxRange = maxRange;
    mTeamId = teamId;
    mTraveledDist = 0.0f;
    mIsGrounded = false;
    mDamage = 70.0f;
}

// Matches PPC BulletPlayerBigBallHitSplash__vfunc_11 @ 0x0224A004
void BulletPlayerBigBallHitSplash::update() {
    if (mIsGrounded) return;

    // Fast reciprocal square root emulation
    f32 horizSpeedSq = mVelocity.x * mVelocity.x + mVelocity.z * mVelocity.z;
    f32 horizSpeed = 0.0f;
    if (horizSpeedSq > 0.0001f) {
        f32 invSqrt = 1.0f / std::sqrt(horizSpeedSq);
        horizSpeed = horizSpeedSq * invSqrt;
    }

    mPosition.x += mVelocity.x;
    mPosition.y += mVelocity.y;
    mPosition.z += mVelocity.z;

    mVelocity.y -= mGravity; // Parabolic lob arc

    mTraveledDist += horizSpeed;

    // Floor impact or max range limit reached
    if (mPosition.y <= 0.0f || mTraveledDist >= mMaxRange) {
        mPosition.y = 0.0f;
        mIsGrounded = true;

        PaintTextureMgr* paint = PaintTextureMgr::instance();
        if (paint) {
            paint->splatInk(mPosition, 3.2f, mTeamId);
        }
    }
}

void BulletPlayerBigBallHitSplash::draw() {
    if (!mIsGrounded) {
        GambitActor::draw();
    }
}

} // namespace Game
