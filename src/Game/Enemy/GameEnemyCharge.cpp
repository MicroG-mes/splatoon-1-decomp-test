#include "Game/Enemy/GameEnemyCharge.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cmath>
#include <algorithm>

namespace Game {

GameEnemyCharge::GameEnemyCharge()
    : mPosition(0.0f, 0.0f, 0.0f),
      mAimTarget(0.0f, 0.0f, 0.0f),
      mShootDir(0.0f, 0.0f, 1.0f),
      mHealth(80.0f),
      mYawAngle(0.0f),
      mIsAlive(true),
      mHasLineOfSight(false),
      mState(OctosniperState::cIdle),
      mStateTimer(0),
      mChargeTimer(0) {
}

GameEnemyCharge::~GameEnemyCharge() = default;

void GameEnemyCharge::init() {
    GambitActor::init();
    vfunc_3();
}

/**
 * Enm_Charge__vfunc_1 @ 0x022d3168
 * Reset & actor cleanup.
 */
void GameEnemyCharge::vfunc_1() {
    mIsAlive = false;
    mState = OctosniperState::cIdle;
}

/**
 * Enm_Charge__vfunc_3 @ 0x022cc774
 * Sniping platform & sight line initialization.
 */
void GameEnemyCharge::vfunc_3() {
    mHealth = 80.0f;
    mIsAlive = true;
    mState = OctosniperState::cIdle;
    mStateTimer = 0;
    mChargeTimer = 0;
}

/**
 * Enm_Charge__vfunc_5 @ 0x022cd640
 * Reset aim coordinates.
 */
void GameEnemyCharge::vfunc_5() {
    mAimTarget = mPosition;
    mStateTimer = 0;
}

/**
 * Enm_Charge__vfunc_7 @ 0x022cfd50
 * Aiming tick, charge state machine, and sniper firing trigger.
 */
void GameEnemyCharge::vfunc_7() {
    if (!mIsAlive) return;

    mStateTimer++;

    switch (mState) {
        case OctosniperState::cIdle:
            if (mHasLineOfSight) {
                mState = OctosniperState::cLockOn;
                mStateTimer = 0;
            }
            break;

        case OctosniperState::cLockOn:
            if (mStateTimer >= 20) {
                // Transition to State 0x04: cCharging (laser guide line locked)
                mState = OctosniperState::cCharging;
                mStateTimer = 0;
                mChargeTimer = cChargeDuration;
            }
            break;

        case OctosniperState::cCharging:
            mChargeTimer--;
            if (mChargeTimer <= 0) {
                // Transition to State 0x07: cFiring
                mState = OctosniperState::cFiring;
                fireSniperShot();
            }
            break;

        case OctosniperState::cFiring:
            mState = OctosniperState::cCooldown;
            mStateTimer = 0;
            break;

        case OctosniperState::cCooldown:
            if (mStateTimer >= cCooldownDuration) {
                mState = OctosniperState::cIdle;
                mStateTimer = 0;
            }
            break;

        case OctosniperState::cStunned:
            if (mStateTimer >= 60) {
                mState = OctosniperState::cIdle;
                mStateTimer = 0;
            }
            break;

        default:
            break;
    }
}

/**
 * Enm_Charge__vfunc_47 @ 0x022d31a4
 * Draw laser sight guide beam.
 */
void GameEnemyCharge::vfunc_47() {
}

/**
 * Enm_Charge__vfunc_52 @ 0x022cff7c
 * Damage reaction & tentacle hit test.
 * Frontal hits against the ink barrier are heavily shielded; rear hits to tentacle deal full damage.
 */
void GameEnemyCharge::vfunc_52(f32 damage, bool isRearHit) {
    if (!mIsAlive) return;

    if (isRearHit) {
        mHealth -= damage;
    } else {
        mHealth -= damage * 0.15f; // Frontal riot armor deflection
    }

    if (mHealth <= 0.0f) {
        mHealth = 0.0f;
        mIsAlive = false;
        vfunc_1();

        PaintTextureMgr* paint = PaintTextureMgr::instance();
        if (paint) {
            paint->splatInk(mPosition, 2.5f, 0); // Burst on splat
        }
    } else {
        mState = OctosniperState::cStunned;
        mStateTimer = 0;
    }
}

void GameEnemyCharge::fireSniperShot() {
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        // High-velocity piercing ink line along mShootDir
        for (f32 step = 3.0f; step <= cMaxSniperRange; step += 3.0f) {
            sead::Vector3f trailPos(
                mPosition.x + mShootDir.x * step,
                mPosition.y + 0.5f,
                mPosition.z + mShootDir.z * step
            );
            paint->splatInk(trailPos, 0.9f, 1); // Enemy team ink
        }
    }
}

void GameEnemyCharge::setupBunker(const sead::Vector3f& pos, f32 yawAngle) {
    mPosition = pos;
    mYawAngle = yawAngle;
    mShootDir.set(std::sin(yawAngle), 0.0f, std::cos(yawAngle));
    vfunc_3();
}

void GameEnemyCharge::updateAimAtPlayer(const sead::Vector3f& playerPos) {
    f32 dx = playerPos.x - mPosition.x;
    f32 dy = playerPos.y - mPosition.y;
    f32 dz = playerPos.z - mPosition.z;
    f32 dist = std::sqrt(dx * dx + dy * dy + dz * dz);

    if (dist <= cMaxSniperRange && dist > 1.0f) {
        mHasLineOfSight = true;
        mAimTarget = playerPos;
        mShootDir.set(dx / dist, dy / dist, dz / dist);
    } else {
        mHasLineOfSight = false;
    }
}

void GameEnemyCharge::update() {
    GambitActor::update();
    vfunc_7();
}

void GameEnemyCharge::draw() {
    GambitActor::draw();
    if (isLaserActive()) {
        vfunc_47();
    }
}

} // namespace Game
