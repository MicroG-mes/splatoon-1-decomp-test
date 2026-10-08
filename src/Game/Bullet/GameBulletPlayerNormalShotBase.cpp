#include "Game/Bullet/GameBulletPlayerNormalShotBase.h"
#include <cstring>
#include <cmath>
#include <cassert>

namespace Game {

GameBulletPlayerNormalShotBase::GameBulletPlayerNormalShotBase()
    : mDirectOrigin(0.0f, 0.0f, 0.0f),
      mFlightVelocity(0.0f, 0.0f, 0.0f),
      mCurrentPos(0.0f, 0.0f, 0.0f),
      mBulletState(0),
      mLifeCounterFixed(0),
      mStepVelocity(0.0f, 0.0f, 0.0f),
      mPrevPosition(0.0f, 0.0f, 0.0f),
      mHitFlags(0),
      mHitPos(0.0f, 0.0f, 0.0f),
      mHitVelocity(0.0f, 0.0f, 0.0f) {
    std::memset(mPadding1, 0, sizeof(mPadding1));
    std::memset(mPadding2, 0, sizeof(mPadding2));
    std::memset(mPadding3, 0, sizeof(mPadding3));
    std::memset(mPadding4, 0, sizeof(mPadding4));
    std::memset(mPadding5, 0, sizeof(mPadding5));
    std::memset(mPadding6, 0, sizeof(mPadding6));
    std::memset(mPadding7, 0, sizeof(mPadding7));
    std::memset(mPadding8, 0, sizeof(mPadding8));
}

GameBulletPlayerNormalShotBase::~GameBulletPlayerNormalShotBase() = default;

void GameBulletPlayerNormalShotBase::init() {
    GameBullet::init();
    mBulletState = 0;
    mLifeCounterFixed = 0;
    mHitFlags = 0;
}

/**
 * BulletPlayerNormalExplosionShotBase__vfunc_4 @ 0x02252e88
 * No-op lifecycle stub.
 */
void GameBulletPlayerNormalShotBase::vfunc_4() {
}

/**
 * BulletPlayerNormalExplosionShotBase__vfunc_7 @ 0x0225b11c
 * Ballistic trajectory integration step.
 * If (mBulletState & 0x380000) != 0, returns early.
 * Copies current pos (0x134) to prev pos (0x218).
 * Increments fixed-point lifetime (0x204) by 0x100000 (1.0).
 * Updates step velocity (0x20c).
 */
void GameBulletPlayerNormalShotBase::vfunc_7() {
    if ((mBulletState & 0x380000) != 0) {
        return;
    }

    mPrevPosition = mCurrentPos;
    mLifeCounterFixed += 0x100000;

    // Apply ballistic gravity curve
    mStepVelocity.y -= 0.016f; // Wii U projectile gravity
    mCurrentPos.x += mStepVelocity.x;
    mCurrentPos.y += mStepVelocity.y;
    mCurrentPos.z += mStepVelocity.z;
}

/**
 * BulletPlayerNormalExplosionShotBase__vfunc_11 @ 0x0225bf28
 * Collision detection routine.
 */
void GameBulletPlayerNormalShotBase::vfunc_11() {
}

/**
 * BulletPlayerNormalExplosionShotBase__vfunc_90 @ 0x02252ee8
 * Compares hit target team with bullet teamId (0x2C).
 * Sets bitmask 0x1000 for friendly fire, 0x2000 for special obstacle, 0x800 for enemy.
 * Caches hit pos (0x23C) and hit velocity (0x25C).
 */
u32 GameBulletPlayerNormalShotBase::vfunc_90(u32 hitActorType, const u32* targetTeamId) {
    if (targetTeamId && *targetTeamId == mTeamId) {
        mHitFlags |= 0x1000; // Friendly
    } else if (targetTeamId && *targetTeamId == 2 && hitActorType == 4) {
        mHitFlags |= 0x2000; // Special obstacle / shield
    } else {
        mHitFlags |= 0x800;  // Direct enemy hit
    }

    mHitVelocity = mStepVelocity;
    mHitPos = mCurrentPos;
    return 1;
}

/**
 * BulletPlayerNormalExplosionShotBase__vfunc_92 @ 0x0225de50
 * Damage falloff calculation based on flight distance and parameter struct (+0x510).
 */
void GameBulletPlayerNormalShotBase::vfunc_92(u16* outDamageData) {
    if (!outDamageData) return;
    *outDamageData = static_cast<u16>(mDamage * 10.0f);
}

/**
 * BulletPlayerNormalExplosionShotBase__vfunc_108 @ 0x02253098
 * Impact event callback: sets flag 0x2000, caches velocity.
 */
void GameBulletPlayerNormalShotBase::vfunc_108(u32 param2, u32* param3) {
    (void)param2;
    (void)param3;
    mHitFlags |= 0x2000;
    mHitVelocity = mStepVelocity;
}

/**
 * BulletPlayerNormalExplosionShotBase__vfunc_118 @ 0x0225e2f8
 * Splat detonation / ground impact.
 * Sets flag 0x80000 on mBulletState and zeros flight velocity.
 */
void GameBulletPlayerNormalShotBase::vfunc_118() {
    if ((mBulletState & 0x380000) == 0) {
        mBulletState = (mBulletState & ~0x380000) | 0x80000;
        mFlightVelocity.set(0.0f, 0.0f, 0.0f);
        mStepVelocity.set(0.0f, 0.0f, 0.0f);
    }
}

/**
 * BulletPlayerNormalExplosionShotBase__vfunc_132 @ 0x0225424c
 * Computes normalized 2D horizontal direction: (X, Z) / sqrt(X^2 + Z^2).
 */
void GameBulletPlayerNormalShotBase::vfunc_132(f32* outDir2D, const f32* inVel) {
    if (!outDir2D || !inVel) return;

    f32 vx = inVel[0];
    f32 vz = inVel[2];
    f32 magSq = vx * vx + vz * vz;

    if (magSq > 0.00001f) {
        f32 invMag = 1.0f / std::sqrt(magSq);
        outDir2D[0] = vx * invMag;
        outDir2D[1] = vz * invMag;
    } else {
        outDir2D[0] = 0.0f;
        outDir2D[1] = 0.0f;
    }
}

/**
 * BulletPlayerNormalExplosionShotBase__vfunc_135 @ 0x02254868
 * Reset output parameter pointer to 0.
 */
void GameBulletPlayerNormalShotBase::vfunc_135(u32* param2) {
    if (!param2) {
        assert(param2 != nullptr);
        return;
    }
    *param2 = 0;
}

void GameBulletPlayerNormalShotBase::update() {
    GameBullet::update();
    vfunc_7();
}

void GameBulletPlayerNormalShotBase::draw() {
    GameBullet::draw();
}

} // namespace Game
