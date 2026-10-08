#include "Game/Weapon/PlayerWeaponBigShot.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cmath>

namespace Game {

PlayerWeaponBigShot::PlayerWeaponBigShot()
    : mState(BigShotState::cFinished),
      mDurationTimer(0),
      mStateTimer(0),
      mAmmoRemaining(0),
      mTeamId(0),
      mLastShotPos(0.0f, 0.0f, 0.0f),
      mLastShotYaw(0.0f) {
}

PlayerWeaponBigShot::~PlayerWeaponBigShot() {
}

void PlayerWeaponBigShot::init() {
    GambitActor::init();
    mState = BigShotState::cFinished;
    mDurationTimer = 0;
    mStateTimer = 0;
    mAmmoRemaining = 0;
    mTeamId = 0;
    mLastShotPos.set(0.0f, 0.0f, 0.0f);
    mLastShotYaw = 0.0f;
}

void PlayerWeaponBigShot::startSpecial(u32 teamId, s32 durationFrames) {
    mTeamId = teamId;
    mDurationTimer = durationFrames;
    mAmmoRemaining = cMaxAmmo;
    mState = BigShotState::cReady;
    mStateTimer = 0;
}

bool PlayerWeaponBigShot::fireShot(const sead::Vector3f& muzzlePos, f32 yawAngle) {
    if (!canFire()) {
        return false;
    }

    mLastShotPos = muzzlePos;
    mLastShotYaw = yawAngle;
    mAmmoRemaining--;
    mState = BigShotState::cFireWindup;
    mStateTimer = 0;

    return true;
}

void PlayerWeaponBigShot::update() {
    if (mState != BigShotState::cFinished) {
        mDurationTimer--;
        if (mDurationTimer <= 0) {
            mState = BigShotState::cFinished;
            return;
        }
    }

    mStateTimer++;

    switch (mState) {
        case BigShotState::cFireWindup:
            if (mStateTimer >= 8) {
                mState = BigShotState::cLaunchVortex;
                mStateTimer = 0;

                // Splat ink ground trail where vortex spawns
                PaintTextureMgr* paint = PaintTextureMgr::instance();
                if (paint) {
                    paint->splatInk(mLastShotPos, cVortexRadius, mTeamId);
                    // Project ink wake forward
                    f32 dirX = std::sin(mLastShotYaw);
                    f32 dirZ = std::cos(mLastShotYaw);
                    for (f32 d = 4.0f; d <= 24.0f; d += 6.0f) {
                        sead::Vector3f trailPos(
                            mLastShotPos.x + dirX * d,
                            mLastShotPos.y,
                            mLastShotPos.z + dirZ * d
                        );
                        paint->splatInk(trailPos, cVortexRadius * 0.9f, mTeamId);
                    }
                }
            }
            break;

        case BigShotState::cLaunchVortex:
            if (mStateTimer >= 10) {
                mState = BigShotState::cRecoil;
                mStateTimer = 0;
            }
            break;

        case BigShotState::cRecoil:
            if (mStateTimer >= 20) {
                mState = BigShotState::cReady;
                mStateTimer = 0;
            }
            break;

        case BigShotState::cReady:
        case BigShotState::cFinished:
        default:
            break;
    }
}

void PlayerWeaponBigShot::draw() {
    if (mState != BigShotState::cFinished) {
        GambitActor::draw();
    }
}

} // namespace Game
