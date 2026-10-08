#include "Game/Weapon/PlayerWeaponBigLaser.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cmath>

namespace Game {

PlayerWeaponBigLaser::PlayerWeaponBigLaser()
    : mPosition(0.0f, 0.0f, 0.0f),
      mYawAngle(0.0f),
      mTeamId(0),
      mState(BigLaserState::cFinished),
      mTimer(0) {
}

PlayerWeaponBigLaser::~PlayerWeaponBigLaser() {
}

void PlayerWeaponBigLaser::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mYawAngle = 0.0f;
    mTeamId = 0;
    mState = BigLaserState::cFinished;
    mTimer = 0;
}

void PlayerWeaponBigLaser::deploy(const sead::Vector3f& pos, f32 yawAngle, u32 teamId) {
    mPosition = pos;
    mYawAngle = yawAngle;
    mTeamId = teamId;
    mState = BigLaserState::cPlacingSpeaker;
    mTimer = 0;
}

bool PlayerWeaponBigLaser::checkHitTarget(const sead::Vector3f& targetPos) const {
    if (mState != BigLaserState::cSonicBlast) {
        return false;
    }

    // Direction vector of the laser beam
    f32 dirX = std::sin(mYawAngle);
    f32 dirZ = std::cos(mYawAngle);

    // Vector from laser base to target
    f32 toX = targetPos.x - mPosition.x;
    f32 toZ = targetPos.z - mPosition.z;

    // Projection along beam direction
    f32 projection = toX * dirX + toZ * dirZ;
    if (projection < 0.0f || projection > cBeamLength) {
        return false;
    }

    // Perpendicular distance squared
    f32 perpX = toX - projection * dirX;
    f32 perpZ = toZ - projection * dirZ;
    f32 distSq = perpX * perpX + perpZ * perpZ;

    // Check height cylinder
    f32 dy = std::abs(targetPos.y - mPosition.y);
    if (dy > cBeamRadius) {
        return false;
    }

    return distSq <= (cBeamRadius * cBeamRadius);
}

void PlayerWeaponBigLaser::update() {
    mTimer++;

    switch (mState) {
        case BigLaserState::cPlacingSpeaker:
            if (mTimer >= 15) {
                mState = BigLaserState::cWarningPitch;
                mTimer = 0;
            }
            break;

        case BigLaserState::cWarningPitch:
            if (mTimer >= cWarningDuration) {
                mState = BigLaserState::cSonicBlast;
                mTimer = 0;
            }
            break;

        case BigLaserState::cSonicBlast:
            // Continually splat ink along the ground underneath beam
            if (mTimer % 6 == 0) {
                PaintTextureMgr* paint = PaintTextureMgr::instance();
                if (paint) {
                    f32 dirX = std::sin(mYawAngle);
                    f32 dirZ = std::cos(mYawAngle);
                    for (f32 dist = 5.0f; dist <= cBeamLength; dist += 10.0f) {
                        sead::Vector3f inkPos(
                            mPosition.x + dirX * dist,
                            mPosition.y,
                            mPosition.z + dirZ * dist
                        );
                        paint->splatInk(inkPos, cBeamRadius * 0.8f, mTeamId);
                    }
                }
            }

            if (mTimer >= cBlastDuration) {
                mState = BigLaserState::cCooldown;
                mTimer = 0;
            }
            break;

        case BigLaserState::cCooldown:
            if (mTimer >= 30) {
                mState = BigLaserState::cFinished;
                mTimer = 0;
            }
            break;

        case BigLaserState::cFinished:
        default:
            break;
    }
}

void PlayerWeaponBigLaser::draw() {
    if (mState != BigLaserState::cFinished) {
        GambitActor::draw();
    }
}

} // namespace Game
