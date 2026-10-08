#include "Game/Enemy/EnemyCleaner.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cmath>

namespace Game {

EnemyCleaner::EnemyCleaner()
    : mPosition(0.0f, 0.0f, 0.0f),
      mMoveDirection(0.0f, 0.0f, 1.0f),
      mHp(cMaxHp),
      mState(CleanerState::cPatrolVacuum),
      mStateTimer(0) {
}

EnemyCleaner::~EnemyCleaner() {
}

void EnemyCleaner::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mMoveDirection.set(0.0f, 0.0f, 1.0f);
    mHp = cMaxHp;
    mState = CleanerState::cPatrolVacuum;
    mStateTimer = 0;
}

void EnemyCleaner::applyDamage(f32 damage, bool isRearHit) {
    if (mState == CleanerState::cDefeated) {
        return;
    }

    if (!isRearHit && mState != CleanerState::cSpinStunned) {
        // Front/top metal brushes deflect ink completely
        return;
    }

    mHp -= damage;
    if (mHp <= 0.0f) {
        mState = CleanerState::cDefeated;
        mStateTimer = 0;

        PaintTextureMgr* paint = PaintTextureMgr::instance();
        if (paint) {
            paint->splatInk(mPosition, 3.5f, 0); // Burst of player ink upon destruction
        }
    } else {
        mState = CleanerState::cSpinStunned;
        mStateTimer = 0;
    }
}

void EnemyCleaner::vacuumInkFloor() {
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        // Erase ink underneath vacuum suction brushes (team -1 / clear)
        paint->splatInk(mPosition, cVacuumRadius, 1); // Rewrites with enemy/neutral ground
    }
}

void EnemyCleaner::updateAi(const sead::Vector3f& playerInkNearbyPos, bool hasPlayerInkNearby) {
    if (mState == CleanerState::cDefeated) {
        return;
    }

    switch (mState) {
        case CleanerState::cPatrolVacuum:
            if (hasPlayerInkNearby) {
                // Steer towards detected player ink
                f32 dx = playerInkNearbyPos.x - mPosition.x;
                f32 dz = playerInkNearbyPos.z - mPosition.z;
                f32 len = std::sqrt(dx * dx + dz * dz);
                if (len > 0.01f) {
                    mMoveDirection.set(dx / len, 0.0f, dz / len);
                }
                mState = CleanerState::cCleaningSpree;
                mStateTimer = 0;
            } else {
                // Fixed linear patrol
                mPosition.x += mMoveDirection.x * cMoveSpeed;
                mPosition.z += mMoveDirection.z * cMoveSpeed;
                if (mStateTimer >= 180) {
                    mMoveDirection.set(-mMoveDirection.x, 0.0f, -mMoveDirection.z);
                    mStateTimer = 0;
                }
            }
            vacuumInkFloor();
            break;

        case CleanerState::cCleaningSpree:
            mPosition.x += mMoveDirection.x * (cMoveSpeed * 1.4f);
            mPosition.z += mMoveDirection.z * (cMoveSpeed * 1.4f);
            vacuumInkFloor();

            if (mStateTimer >= 120 || !hasPlayerInkNearby) {
                mState = CleanerState::cPatrolVacuum;
                mStateTimer = 0;
            }
            break;

        case CleanerState::cSpinStunned:
            if (mStateTimer >= 60) {
                mState = CleanerState::cPatrolVacuum;
                mStateTimer = 0;
            }
            break;

        case CleanerState::cDefeated:
        default:
            break;
    }
}

void EnemyCleaner::update() {
    mStateTimer++;
}

void EnemyCleaner::draw() {
    if (mState != CleanerState::cDefeated) {
        GambitActor::draw();
    }
}

} // namespace Game
