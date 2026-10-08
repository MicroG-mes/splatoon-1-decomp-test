#include "Game/Bullet/BulletBombNormal.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cstring>

namespace Game {

BulletBombNormal::BulletBombNormal()
    : mBombState(0),
      mState(SplatBombState::cInAirAirborne),
      mFuseTimer(0),
      mPosition(0.0f, 0.0f, 0.0f),
      mVelocity(0.0f, 0.0f, 0.0f),
      mTeam(0),
      mInnerDamage(180.0f),
      mOuterDamage(30.0f),
      mBlastRadius(4.5f) {
    std::memset(mReserved0_0x8, 0, sizeof(mReserved0_0x8));
}

BulletBombNormal::~BulletBombNormal() {
}

void BulletBombNormal::init() {
    GambitActor::init();
    mBombState = 0;
    mState = SplatBombState::cInAirAirborne;
    mFuseTimer = 0;
}

// 0x0221266C: Decompiled BulletBombNormal__vfunc_178
void BulletBombNormal::vfunc_178() {
    // If fuse is active (state 2 or 3) and canExplode() == 0, trigger detonation
    if ((mBombState == 2 || mBombState == 3) && canExplode() == 0) {
        explode(nullptr);
    }
}

void BulletBombNormal::explode(const void* effectParam) {
    mBombState = 3;
    mState = SplatBombState::cExploding;

    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        paint->splatInk(mPosition, mBlastRadius, mTeam);
    }
}

void BulletBombNormal::throwBomb(const sead::Vector3f& startPos, const sead::Vector3f& initVel, u32 team) {
    mPosition = startPos;
    mVelocity = initVel;
    mTeam = team;
    mBombState = 1;
    mState = SplatBombState::cInAirAirborne;
    mFuseTimer = 0;
}

void BulletBombNormal::update() {
    switch (mState) {
        case SplatBombState::cInAirAirborne:
            // Parabolic gravity flight
            mVelocity.y -= 0.035f;
            mPosition.x += mVelocity.x;
            mPosition.y += mVelocity.y;
            mPosition.z += mVelocity.z;

            // Ground impact
            if (mPosition.y <= 0.0f) {
                mPosition.y = 0.0f;
                mVelocity.y = -mVelocity.y * 0.35f; // Bounce dampening
                mVelocity.x *= 0.65f;
                mVelocity.z *= 0.65f;
                mState = SplatBombState::cGroundedFuse;
                mBombState = 2;
                mFuseTimer = 0;
            }
            break;

        case SplatBombState::cGroundedFuse:
            mFuseTimer++;
            // Roll deceleration
            mPosition.x += mVelocity.x;
            mPosition.z += mVelocity.z;
            mVelocity.x *= 0.92f;
            mVelocity.z *= 0.92f;

            // 60-frame (1.0s) fuse
            if (mFuseTimer >= 60) {
                mBombState = 3;
                vfunc_178();
            }
            break;

        case SplatBombState::cExploding:
            mState = SplatBombState::cFinished;
            break;

        default:
            break;
    }
}

void BulletBombNormal::draw() {
    GambitActor::draw();
}

} // namespace Game
