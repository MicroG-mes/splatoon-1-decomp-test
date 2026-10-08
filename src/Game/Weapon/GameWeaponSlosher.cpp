#include "Game/Weapon/GameWeaponSlosher.h"
#include "Game/Paint/PaintTextureMgr.h"

namespace Game {

GameWeaponSlosher::GameWeaponSlosher()
    : mState(SlosherState::cIdle),
      mStateTimer(0),
      mDirectDamage(70.0f),
      mSplashDamage(40.0f),
      mArcHeight(4.0f),
      mRange(18.0f) {
}

GameWeaponSlosher::~GameWeaponSlosher() {
}

void GameWeaponSlosher::init() {
    GambitActor::init();
    mState = SlosherState::cIdle;
}

void GameWeaponSlosher::triggerSlosh() {
    if (mState == SlosherState::cIdle) {
        mState = SlosherState::cSwingThrow;
        mStateTimer = 0;
        spawnInkVolley();
    }
}

void GameWeaponSlosher::spawnInkVolley() {
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        // Splatters 3 cascading blobs landing in an arc
        paint->splatInk(sead::Vector3f(0.0f, 0.0f, 6.0f), 1.6f, 0);
        paint->splatInk(sead::Vector3f(0.0f, 0.0f, 12.0f), 2.0f, 0);
        paint->splatInk(sead::Vector3f(0.0f, 0.0f, 18.0f), 2.4f, 0);
    }
}

void GameWeaponSlosher::update() {
    mStateTimer++;

    switch (mState) {
        case SlosherState::cSwingThrow:
            if (mStateTimer >= 10) {
                mState = SlosherState::cRecovery;
                mStateTimer = 0;
            }
            break;

        case SlosherState::cRecovery:
            if (mStateTimer >= 18) { // 18 frames slosher cadence
                mState = SlosherState::cIdle;
            }
            break;

        default:
            break;
    }
}

void GameWeaponSlosher::draw() {
    GambitActor::draw();
}

} // namespace Game
