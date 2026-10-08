#include "Game/Bullet/BulletBombDevil.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cstring>
#include <cmath>

namespace Game {

BulletBombDevil::BulletBombDevil()
    : mBombState(DevilBombState::cDetonated),
      mTeamId(0),
      mOwnerPlayerId(0),
      mBlastSphereRadius(cBlastRadius),
      mExplosionDamageRadius(cBlastRadius),
      mDetonationPos(0.0f, 0.0f, 0.0f),
      mStateFlags(0),
      mFuseTimer(0),
      mVelocity(0.0f, 0.0f, 0.0f),
      mSurfaceNormal(0.0f, 1.0f, 0.0f) {
    std::memset(mPadding1, 0, sizeof(mPadding1));
    std::memset(mPadding2, 0, sizeof(mPadding2));
    std::memset(mPadding3, 0, sizeof(mPadding3));
}

BulletBombDevil::~BulletBombDevil() {
}

void BulletBombDevil::init() {
    GambitActor::init();
    mBombState = DevilBombState::cDetonated;
    mFuseTimer = 0;
    mStateFlags = 0;
    mBlastSphereRadius = cBlastRadius;
    mExplosionDamageRadius = cBlastRadius;
}

void BulletBombDevil::throwBomb(const sead::Vector3f& startPos, const sead::Vector3f& initVel, u32 team, u32 ownerPlayerId) {
    mPosition = startPos;
    mVelocity = initVel;
    mTeamId = team;
    mOwnerPlayerId = ownerPlayerId;
    mBombState = DevilBombState::cAirborne;
    mFuseTimer = 0;
    mStateFlags = 0;
}

void BulletBombDevil::attachToSurface(const sead::Vector3f& contactPos, const sead::Vector3f& normal) {
    mPosition = contactPos;
    mSurfaceNormal = normal;
    mVelocity.set(0.0f, 0.0f, 0.0f);
    mBombState = DevilBombState::cAttachedSurface;
    mFuseTimer = 0;
}

/**
 * BulletBombDevilCore__vfunc_7 @ 0x0220bf50
 * HitSplash trigger and sets state flag bit 0 on *(this + 0xF4).
 */
void BulletBombDevil::vfunc_7() {
    mStateFlags |= 1;
}

/**
 * BulletBombDevilCore__vfunc_11 @ 0x0220bf88
 * Detonates the suction bomb at coordinates (0xE8, 0xEC, 0xF0).
 */
void BulletBombDevil::vfunc_11() {
    mDetonationPos = mPosition;
    vfunc_7();
    explode();
}

/**
 * BulletBombDevilCore__vfunc_88 @ 0x0220bfe4
 * Calculates damage within blast radius: 180.0 HP maximum damage to enemy players.
 */
f64 BulletBombDevil::vfunc_88(u32 param2, const u32* targetTeam, const sead::Vector3f* targetPos) {
    (void)param2;
    if ((mStateFlags & 1) == 0 || !targetPos) {
        return 0.0;
    }

    f32 dx = mDetonationPos.x - targetPos->x;
    f32 dy = mDetonationPos.y - targetPos->y;
    f32 dz = mDetonationPos.z - targetPos->z;
    f32 distSq = dx * dx + dy * dy + dz * dz;

    f32 radiusSq = mExplosionDamageRadius * mExplosionDamageRadius;
    if (distSq < radiusSq) {
        if (targetTeam && *targetTeam != mTeamId) {
            f32 dist = std::sqrt(distSq);
            f32 falloff = 1.0f - (dist / mExplosionDamageRadius) * 0.5f;
            return static_cast<f64>(cBaseDamage * falloff);
        }
    }
    return 0.0;
}

void BulletBombDevil::explode() {
    mBombState = DevilBombState::cDetonated;

    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        paint->splatInk(mPosition, cBlastRadius, mTeamId);
    }
}

void BulletBombDevil::update() {
    GambitActor::update();

    if (mBombState == DevilBombState::cAirborne) {
        mPosition.x += mVelocity.x;
        mPosition.y += mVelocity.y;
        mPosition.z += mVelocity.z;
        mVelocity.y -= cGravity;

        // Ground / wall latching: suction cup sticks on contact
        if (mPosition.y <= 0.0f) {
            mPosition.y = 0.0f;
            attachToSurface(mPosition, sead::Vector3f(0.0f, 1.0f, 0.0f));
        }
    } else if (mBombState == DevilBombState::cAttachedSurface) {
        mFuseTimer++;
        if (mFuseTimer >= cFuseDuration) {
            vfunc_11();
        }
    }
}

void BulletBombDevil::draw() {
    GambitActor::draw();
}

} // namespace Game
