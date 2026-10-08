#include "Game/Bullet/Bomb_Chase.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cmath>

namespace Game {

Bomb_Chase::Bomb_Chase()
    : mPosition(0.0f, 0.0f, 0.0f),
      mYawAngle(0.0f),
      mTeamId(0),
      mOwnerPlayerId(0),
      mState(ChaseBombState::cFinished),
      mTimer(0),
      mHomingTargetPos(0.0f, 0.0f, 0.0f),
      mHasTarget(false) {
}

Bomb_Chase::~Bomb_Chase() {
}

void Bomb_Chase::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mYawAngle = 0.0f;
    mTeamId = 0;
    mOwnerPlayerId = 0;
    mState = ChaseBombState::cFinished;
    mTimer = 0;
    mHomingTargetPos.set(0.0f, 0.0f, 0.0f);
    mHasTarget = false;
}

void Bomb_Chase::launch(const sead::Vector3f& startPos, f32 yawAngle, u32 teamId, u32 ownerPlayerId) {
    mPosition = startPos;
    mYawAngle = yawAngle;
    mTeamId = teamId;
    mOwnerPlayerId = ownerPlayerId;
    mState = ChaseBombState::cCruisingTrail;
    mTimer = 0;
    mHasTarget = false;
}

void Bomb_Chase::checkEnemyHoming(const sead::Vector3f& enemyPos, u32 enemyTeam) {
    if (mState != ChaseBombState::cCruisingTrail && mState != ChaseBombState::cHomingTarget) {
        return;
    }

    if (enemyTeam == mTeamId) {
        return;
    }

    f32 dx = enemyPos.x - mPosition.x;
    f32 dz = enemyPos.z - mPosition.z;
    f32 distSq = dx * dx + dz * dz;

    if (distSq < (cDetectionRange * cDetectionRange)) {
        mHomingTargetPos = enemyPos;
        mHasTarget = true;
        mState = ChaseBombState::cHomingTarget;
    }
}

void Bomb_Chase::triggerDetonation() {
    mState = ChaseBombState::cDetonating;
    mTimer = 0;

    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        paint->splatInk(mPosition, cBlastRadius, mTeamId);
    }
}

void Bomb_Chase::update() {
    switch (mState) {
        case ChaseBombState::cLaunching:
            mTimer++;
            if (mTimer > 5) {
                mState = ChaseBombState::cCruisingTrail;
            }
            break;

        case ChaseBombState::cCruisingTrail: {
            mTimer++;
            mPosition.x += std::sin(mYawAngle) * cCruiseSpeed;
            mPosition.z += std::cos(mYawAngle) * cCruiseSpeed;

            PaintTextureMgr* paint = PaintTextureMgr::instance();
            if (paint && (mTimer % 3 == 0)) {
                paint->splatInk(mPosition, 1.2f, mTeamId);
            }

            if (mTimer >= cMaxLifetimeFrames) {
                triggerDetonation();
            }
            break;
        }

        case ChaseBombState::cHomingTarget: {
            mTimer++;
            f32 dx = mHomingTargetPos.x - mPosition.x;
            f32 dz = mHomingTargetPos.z - mPosition.z;
            f32 targetYaw = std::atan2(dx, dz);

            // Interpolate yaw towards target
            f32 diff = targetYaw - mYawAngle;
            while (diff > 3.14159265f) diff -= 6.2831853f;
            while (diff < -3.14159265f) diff += 6.2831853f;
            mYawAngle += diff * 0.15f;

            mPosition.x += std::sin(mYawAngle) * cHomingSpeed;
            mPosition.z += std::cos(mYawAngle) * cHomingSpeed;

            PaintTextureMgr* paint = PaintTextureMgr::instance();
            if (paint && (mTimer % 3 == 0)) {
                paint->splatInk(mPosition, 1.4f, mTeamId);
            }

            f32 distRemainingSq = dx * dx + dz * dz;
            if (distRemainingSq < 1.0f || mTimer >= cMaxLifetimeFrames) {
                triggerDetonation();
            }
            break;
        }

        case ChaseBombState::cDetonating:
            mState = ChaseBombState::cFinished;
            break;

        case ChaseBombState::cFinished:
        default:
            break;
    }
}

void Bomb_Chase::draw() {
    // Model rendering performed by ModelSceneMgr
}

} // namespace Game
