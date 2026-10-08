#include "Game/Bullet/BulletBombMarking.h"
#include <cstring>
#include <cmath>

namespace Game {

BulletBombMarking::BulletBombMarking()
    : mState(MarkingBombState::cFinished),
      mTimer(0),
      mPosition(0.0f, 0.0f, 0.0f),
      mVelocity(0.0f, 0.0f, 0.0f),
      mTeamId(0),
      mOwnerPlayerId(0),
      mMarkingSphereRadius(cPulseRadius),
      mMarkingEffectRadius(cPulseRadius),
      mDetonationPos(0.0f, 0.0f, 0.0f),
      mMarkingFlags(0) {
    std::memset(mPadding1, 0, sizeof(mPadding1));
    std::memset(mPadding2, 0, sizeof(mPadding2));
    std::memset(mPadding3, 0, sizeof(mPadding3));
}

BulletBombMarking::~BulletBombMarking() {
}

void BulletBombMarking::init() {
    GambitActor::init();
    mState = MarkingBombState::cFinished;
    mTimer = 0;
    mMarkingFlags = 0;
    mMarkingSphereRadius = cPulseRadius;
    mMarkingEffectRadius = cPulseRadius;
}

void BulletBombMarking::throwBomb(const sead::Vector3f& startPos, const sead::Vector3f& initVel, u32 team, u32 ownerPlayerId) {
    mPosition = startPos;
    mVelocity = initVel;
    mTeamId = team;
    mOwnerPlayerId = ownerPlayerId;
    mState = MarkingBombState::cInAirAirborne;
    mTimer = 0;
    mMarkingFlags = 0;
}

/**
 * BulletBombMarkingCore__vfunc_7 @ 0x02210a5c
 * Triggers pulse particle effect and sets bit 0 on *(this + 0xF4).
 */
void BulletBombMarking::vfunc_7() {
    mMarkingFlags |= 1;
}

/**
 * BulletBombMarkingCore__vfunc_11 @ 0x02210a94
 * Detonation routine: establishes pulse origin at (0xE8, 0xEC, 0xF0)
 * and applies marking aura.
 */
void BulletBombMarking::vfunc_11() {
    mDetonationPos = mPosition;
    vfunc_7();
    mState = MarkingBombState::cSensorPulse;
    mTimer = 0;
}

/**
 * BulletBombMarkingCore__vfunc_88 @ 0x02210af0
 * Evaluates whether target at targetPos is within marking sphere radius (0xC0).
 * If target team != bomb team (0x2C), returns 1.0f (marked hit).
 */
f64 BulletBombMarking::vfunc_88(u32 param2, const u32* targetTeam, const sead::Vector3f* targetPos) {
    (void)param2;
    if ((mMarkingFlags & 1) == 0 || !targetPos) {
        return 0.0;
    }

    f32 dx = mDetonationPos.x - targetPos->x;
    f32 dy = mDetonationPos.y - targetPos->y;
    f32 dz = mDetonationPos.z - targetPos->z;
    f32 distSq = dx * dx + dy * dy + dz * dz;

    f32 radiusSq = mMarkingEffectRadius * mMarkingEffectRadius;
    if (distSq < radiusSq) {
        if (targetTeam && *targetTeam != mTeamId) {
            return 1.0; // Marked enemy!
        }
    }
    return 0.0;
}

bool BulletBombMarking::checkMarkEnemy(const sead::Vector3f& enemyPos, u32 enemyTeam) const {
    if (mState != MarkingBombState::cSensorPulse) {
        return false;
    }
    f32 dx = enemyPos.x - mPosition.x;
    f32 dy = enemyPos.y - mPosition.y;
    f32 dz = enemyPos.z - mPosition.z;
    f32 distSq = dx * dx + dy * dy + dz * dz;
    return (distSq <= cPulseRadius * cPulseRadius) && (enemyTeam != mTeamId);
}

void BulletBombMarking::update() {
    GambitActor::update();

    if (mState == MarkingBombState::cInAirAirborne) {
        mPosition.x += mVelocity.x;
        mPosition.y += mVelocity.y;
        mPosition.z += mVelocity.z;
        mVelocity.y -= cGravity;

        // Ground / surface impact: explode immediately on impact
        if (mPosition.y <= 0.0f) {
            mPosition.y = 0.0f;
            vfunc_11();
        }
    } else if (mState == MarkingBombState::cSensorPulse) {
        mTimer++;
        if (mTimer >= 30) { // Pulse wave lasts 0.5 sec
            mState = MarkingBombState::cFinished;
            mMarkingFlags &= ~1;
        }
    }
}

void BulletBombMarking::draw() {
    GambitActor::draw();
}

} // namespace Game
