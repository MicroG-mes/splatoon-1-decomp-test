#include "Game/Enemy/EnemyHohei.h"
#include <cstring>
#include <cmath>

namespace Game {

EnemyHohei::EnemyHohei()
    : mState(OctotrooperState::cIdlePatrol),
      mStateTimer(0),
      mYaw(0.0f),
      mMaxHp(40.0f),
      mCurrentHp(40.0f),
      mParamStructPtr(nullptr),
      mSoundComponent(nullptr),
      mWorldPos(0.0f, 0.0f, 0.0f),
      mTrajectoryComponent(nullptr),
      mInkMuzzleEmitter(nullptr),
      mStateMachineComponent(nullptr) {
    std::memset(mPadding1, 0, sizeof(mPadding1));
    std::memset(mPadding2, 0, sizeof(mPadding2));
    std::memset(mPadding3, 0, sizeof(mPadding3));
    std::memset(mPadding4, 0, sizeof(mPadding4));
    std::memset(mPadding5, 0, sizeof(mPadding5));
}

EnemyHohei::~EnemyHohei() {
}

void EnemyHohei::init() {
    GambitActor::init();
    vfunc_3();
    vfunc_5();
}

/**
 * Enm_Hohei__vfunc_3 @ 0x02322260
 * Resource load.
 */
void EnemyHohei::vfunc_3() {
    mWorldPos.set(0.0f, 0.0f, 0.0f);
}

/**
 * Enm_Hohei__vfunc_5 @ 0x02322798
 * Reset / spawn.
 */
void EnemyHohei::vfunc_5() {
    mState = OctotrooperState::cIdlePatrol;
    mCurrentHp = mMaxHp;
}

/**
 * Enm_Hohei__vfunc_7 @ 0x023230cc
 * Main AI tick.
 */
void EnemyHohei::vfunc_7() {
}

/**
 * Enm_Hohei__vfunc_47 @ 0x023256ec
 * Animation / model sync.
 */
void EnemyHohei::vfunc_47() {
}

/**
 * Enm_Hohei__vfunc_52 @ 0x02323c70
 * Target nearest player.
 */
void EnemyHohei::vfunc_52() {
}

void EnemyHohei::updateAi(const sead::Vector3f& playerPos) {
    mStateTimer++;

    f32 dx = playerPos.x - mWorldPos.x;
    f32 dz = playerPos.z - mWorldPos.z;
    f32 dist = std::sqrt(dx * dx + dz * dz);

    switch (mState) {
        case OctotrooperState::cIdlePatrol:
            if (dist < 15.0f) {
                mState = OctotrooperState::cAimCharge;
                mStateTimer = 0;
            }
            break;

        case OctotrooperState::cAimCharge:
            mYaw = std::atan2(dx, dz);
            if (mStateTimer >= 45) {
                mState = OctotrooperState::cShoot;
                mStateTimer = 0;
                fireInkBlob();
            }
            break;

        case OctotrooperState::cShoot:
            if (mStateTimer >= 30) {
                mState = OctotrooperState::cHopBack;
                mStateTimer = 0;
            }
            break;

        case OctotrooperState::cHopBack:
            if (mStateTimer >= 30) {
                mState = OctotrooperState::cIdlePatrol;
                mStateTimer = 0;
            }
            break;

        case OctotrooperState::cSplatted:
            break;
    }
}

void EnemyHohei::fireInkBlob() {
    // Projectile spawned via bullet system
}

void EnemyHohei::applyDamage(f32 damage) {
    mCurrentHp -= damage;
    if (mCurrentHp <= 0.0f) {
        mState = OctotrooperState::cSplatted;
    }
}

void EnemyHohei::update() {
    GambitActor::update();
    vfunc_7();
    vfunc_47();
}

void EnemyHohei::draw() {
    GambitActor::draw();
}

} // namespace Game
