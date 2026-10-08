#include "Game/Weapon/PlayerWeaponTornado.h"
#include "Game/Paint/PaintTextureMgr.h"

namespace Game {

PlayerWeaponTornado::PlayerWeaponTornado()
    : mState(TornadoState::cSelectingTarget),
      mStateTimer(0),
      mTargetPosition(0.0f, 0.0f, 0.0f),
      mMissilePos(0.0f, 50.0f, 0.0f),
      mTeam(0),
      mVortexRadius(7.5f),
      mDamagePerFrame(25.0f) {
}

PlayerWeaponTornado::~PlayerWeaponTornado() {
}

void PlayerWeaponTornado::init() {
    GambitActor::init();
    mState = TornadoState::cSelectingTarget;
    mStateTimer = 0;
}

void PlayerWeaponTornado::startAiming() {
    mState = TornadoState::cSelectingTarget;
    mStateTimer = 0;
}

void PlayerWeaponTornado::confirmTarget(const sead::Vector3f& targetPos, u32 team) {
    mTargetPosition = targetPos;
    mMissilePos = sead::Vector3f(targetPos.x, 80.0f, targetPos.z);
    mTeam = team;
    mState = TornadoState::cLaunching;
    mStateTimer = 0;
}

void PlayerWeaponTornado::triggerVortexExplosion() {
    mState = TornadoState::cVortexActive;
    mStateTimer = 0;

    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        paint->splatInk(mTargetPosition, mVortexRadius, mTeam);
    }
}

void PlayerWeaponTornado::update() {
    mStateTimer++;

    switch (mState) {
        case TornadoState::cLaunching:
            // 45 frames launch animation
            if (mStateTimer >= 45) {
                mState = TornadoState::cDescending;
                mStateTimer = 0;
            }
            break;

        case TornadoState::cDescending:
            // Missile falls rapidly towards ground target
            mMissilePos.y -= 2.5f;
            if (mMissilePos.y <= mTargetPosition.y) {
                mMissilePos.y = mTargetPosition.y;
                triggerVortexExplosion();
            }
            break;

        case TornadoState::cVortexActive: {
            // Sustained ink pillar vortex for 120 frames (2.0s)
            PaintTextureMgr* paint = PaintTextureMgr::instance();
            if (paint && (mStateTimer % 10 == 0)) {
                paint->splatInk(mTargetPosition, mVortexRadius, mTeam);
            }

            if (mStateTimer >= 120) {
                mState = TornadoState::cFinished;
            }
            break;
        }

        default:
            break;
    }
}

void PlayerWeaponTornado::draw() {
    GambitActor::draw();
}

} // namespace Game
