#include "Game/Enemy/EnemyTakodozer.h"
#include "Game/Paint/PaintTextureMgr.h"

namespace Game {

EnemyTakodozer::EnemyTakodozer()
    : mState(OctodozerState::cChargeForward),
      mStateTimer(0),
      mPosition(0.0f, 0.0f, 0.0f),
      mMoveDir(1.0f),
      mSpeed(0.08f),
      mTentacleHp(60.0f) {
}

EnemyTakodozer::~EnemyTakodozer() {
}

void EnemyTakodozer::init() {
    GambitActor::init();
    mTentacleHp = 60.0f;
    mState = OctodozerState::cChargeForward;
}

void EnemyTakodozer::applyWeakpointDamage(f32 damage) {
    if (mState == OctodozerState::cDestroyed) {
        return;
    }

    mTentacleHp -= damage;
    if (mTentacleHp <= 0.0f) {
        mTentacleHp = 0.0f;
        mState = OctodozerState::cDestroyed;
        mStateTimer = 0;
    } else {
        mState = OctodozerState::cTentacleHit;
        mStateTimer = 0;
    }
}

void EnemyTakodozer::paintDozerTrail() {
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        paint->splatInk(mPosition, 2.8f, 1); // Wide enemy purple ink trail
    }
}

void EnemyTakodozer::updatePatrol(f32 pathLength) {
    if (mState == OctodozerState::cDestroyed) {
        return;
    }

    switch (mState) {
        case OctodozerState::cChargeForward:
            mPosition.z += mSpeed * mMoveDir;
            paintDozerTrail();

            if (mPosition.z > pathLength || mPosition.z < -pathLength) {
                mState = OctodozerState::cTurnAround;
                mStateTimer = 0;
            }
            break;

        case OctodozerState::cTurnAround:
            if (mStateTimer >= 45) { // 45 frames turn
                mMoveDir = -mMoveDir;
                mState = OctodozerState::cChargeForward;
            }
            break;

        case OctodozerState::cTentacleHit:
            if (mStateTimer >= 20) {
                mState = OctodozerState::cChargeForward;
            }
            break;

        default:
            break;
    }
}

void EnemyTakodozer::update() {
    mStateTimer++;
}

void EnemyTakodozer::draw() {
    if (mState != OctodozerState::cDestroyed) {
        GambitActor::draw();
    }
}

} // namespace Game
