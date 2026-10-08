#include "Game/Bullet/BulletBombInstant.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cstring>
#include <cmath>

namespace Game {

BulletBombInstant::BulletBombInstant()
    : mBombState(BurstBombState::cDetonated),
      mTeamId(0),
      mOwnerPlayerId(0),
      mBlastSphereRadius(cBlastRadius),
      mVelocity(0.0f, 0.0f, 0.0f),
      mTimer(0) {
    std::memset(mPadding1, 0, sizeof(mPadding1));
}

BulletBombInstant::~BulletBombInstant() {
}

void BulletBombInstant::init() {
    GambitActor::init();
    mBombState = BurstBombState::cDetonated;
    mTimer = 0;
    mBlastSphereRadius = cBlastRadius;
}

void BulletBombInstant::throwBomb(const sead::Vector3f& startPos, const sead::Vector3f& initVel, u32 team, u32 ownerPlayerId) {
    mPosition = startPos;
    mVelocity = initVel;
    mTeamId = team;
    mOwnerPlayerId = ownerPlayerId;
    mBombState = BurstBombState::cAirborne;
    mTimer = 0;
}

void BulletBombInstant::impactSurface(const sead::Vector3f& hitPos) {
    mPosition = hitPos;
    mBombState = BurstBombState::cContact;
    vfunc_178();
}

/**
 * BulletBombInstant_StaffRoll__vfunc_176 @ 0x0220d9ac
 * Reset / cleanup.
 */
void BulletBombInstant::vfunc_176() {
    mBombState = BurstBombState::cDetonated;
    mVelocity.set(0.0f, 0.0f, 0.0f);
}

/**
 * BulletBombInstant_StaffRoll__vfunc_178 @ 0x0220de38
 * Triggers instant detonation if state == 2 (contact) or 3 (detonating).
 */
void BulletBombInstant::vfunc_178() {
    if (mBombState == BurstBombState::cContact || mBombState == BurstBombState::cDetonated) {
        explode();
    }
}

/**
 * BulletBombInstant_StaffRoll__vfunc_87 @ 0x0220d9e4
 * Damage query for target within blast radius.
 */
f32 BulletBombInstant::vfunc_87(u32 targetTeam, const sead::Vector3f& targetPos) {
    if (targetTeam == mTeamId) {
        return 0.0f;
    }

    f32 dx = mPosition.x - targetPos.x;
    f32 dy = mPosition.y - targetPos.y;
    f32 dz = mPosition.z - targetPos.z;
    f32 dist = std::sqrt(dx * dx + dy * dy + dz * dz);

    if (dist <= 0.8f) {
        return cDirectDamage; // Direct hit 60.0
    } else if (dist <= 1.8f) {
        return cNearSplashDamage; // Near splash 35.0
    } else if (dist <= cBlastRadius) {
        return cFarSplashDamage; // Far splash 20.0
    }
    return 0.0f;
}

void BulletBombInstant::explode() {
    mBombState = BurstBombState::cDetonated;

    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        paint->splatInk(mPosition, cBlastRadius, mTeamId);
    }
}

void BulletBombInstant::update() {
    GambitActor::update();

    if (mBombState == BurstBombState::cAirborne) {
        mPosition.x += mVelocity.x;
        mPosition.y += mVelocity.y;
        mPosition.z += mVelocity.z;
        mVelocity.y -= cGravity;

        // Instant burst on any surface contact
        if (mPosition.y <= 0.0f) {
            mPosition.y = 0.0f;
            impactSurface(mPosition);
        }
    }
}

void BulletBombInstant::draw() {
    GambitActor::draw();
}

} // namespace Game
