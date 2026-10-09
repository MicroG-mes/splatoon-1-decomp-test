#include "Game/Enemy/Enm_Ball.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cmath>
#include <algorithm>

namespace Game {

Enm_Ball::Enm_Ball()
    : mPosition(0.0f, 0.0f, 0.0f),
      mMoveDir(0.0f, 0.0f, 1.0f),
      mHp(cMaxHp),
      mMaxHp(cMaxHp),
      mCurrentSpeed(cRollSpeed),
      mSpeedNormal(cSpeedNormal),
      mSpeedPL(cSpeedPlayerInkNormal),
      mDeflectedDamage(0.0f),
      mState(OctoballState::cWait),
      mVariant(OctoballVariant::cNormal),
      mStateTimer(0),
      mChanceTimer(0),
      mEscapeTimer(0),
      mDroppedEggs(0),
      mMuteki(false) {
}

Enm_Ball::~Enm_Ball() {
}

void Enm_Ball::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mMoveDir.set(0.0f, 0.0f, 1.0f);
    mHp = mMaxHp;
    mCurrentSpeed = cRollSpeed;
    mDeflectedDamage = 0.0f;
    mState = OctoballState::cWait;
    mStateTimer = 0;
    mChanceTimer = 0;
    mEscapeTimer = 0;
    mDroppedEggs = 0;
    mMuteki = false;
}

void Enm_Ball::setVariant(OctoballVariant variant) {
    mVariant = variant;
    switch (variant) {
        case OctoballVariant::cReal:
            mMaxHp = cLifeReal;
            mHp = cLifeReal;
            mSpeedNormal = cSpeedReal;
            mSpeedPL = cSpeedPlayerInkReal;
            break;
        case OctoballVariant::cFake:
            mMaxHp = cLifeFake;
            mHp = cLifeFake;
            mSpeedNormal = cSpeedFake;
            mSpeedPL = cSpeedPlayerInkFake;
            break;
        case OctoballVariant::cNormal:
        default:
            mMaxHp = cMaxHp;
            mHp = cMaxHp;
            mSpeedNormal = cSpeedNormal;
            mSpeedPL = cSpeedPlayerInkNormal;
            break;
    }
}

void Enm_Ball::vfunc_1() {
    // Teardown
}

void Enm_Ball::vfunc_3() {
    // Model & resource loading
}

void Enm_Ball::vfunc_5() {
    // Parameters initialization
}

void Enm_Ball::vfunc_7() {
    // Update tick
    mStateTimer++;
}

void Enm_Ball::vfunc_11() {
    // Collision handling
}

void Enm_Ball::vfunc_47() {
    // Roll rotation sync
}

void Enm_Ball::vfunc_52() {
    // Target tracking
}

void Enm_Ball::vfunc_60() {
    // State machine dispatcher
}

bool Enm_Ball::applyDamage(f32 damage) {
    if (mState == OctoballState::cDie) {
        return false;
    }

    // BarrierGuard invulnerability during roll
    if (mMuteki) {
        mDeflectedDamage += damage;
        return false;
    }

    mHp -= damage;
    if (mHp <= 0.0f) {
        mHp = 0.0f;
        mState = OctoballState::cDie;
        mMuteki = false;
        mDroppedEggs = cDroppedPowerEggs;
        mStateTimer = 0;

        PaintTextureMgr* paint = PaintTextureMgr::instance();
        if (paint) {
            paint->splatInk(mPosition, cDiePaintRadius * 0.1f, 0); // Player ink burst upon defeat
        }
    } else {
        if (mVariant == OctoballVariant::cReal) {
            mEscapeTimer = cEscapeTime;
        }
        mState = OctoballState::cDazedUncurl;
        mChanceTimer = cChanceSinkFrame;
        mStateTimer = 0;
    }
    return true;
}

void Enm_Ball::stepRollingMovement() {
    f32 speed = mCurrentSpeed;
    mPosition.x += mMoveDir.x * speed;
    mPosition.z += mMoveDir.z * speed;

    // Ink the floor with purple ink along rolling trajectory
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint && (mStateTimer % 3 == 0)) {
        paint->splatInk(mPosition, cTrackPaintRadius * 0.1f, 1);
    }
}

void Enm_Ball::updateAi(const sead::Vector3f& playerPos, bool isRollingOnPlayerInk) {
    if (mState == OctoballState::cDie) {
        return;
    }

    vfunc_7();

    f32 dx = playerPos.x - mPosition.x;
    f32 dz = playerPos.z - mPosition.z;
    f32 dist = std::sqrt(dx * dx + dz * dz);

    switch (mState) {
        case OctoballState::cWait:
            mMuteki = false;
            if (dist < cEyesightRadius) {
                if (dist > 0.01f) {
                    mMoveDir.set(dx / dist, 0.0f, dz / dist);
                }
                mState = OctoballState::cNotice;
                mStateTimer = 0;
            }
            break;

        case OctoballState::cNotice:
            mMuteki = false;
            if (mStateTimer >= 20) {
                mState = OctoballState::cMove;
                mMuteki = true; // BarrierGuard activates!
                mStateTimer = 0;
            }
            break;

        case OctoballState::cMove:
            // Sinking into player ink removes BarrierGuard and enters Chance
            if (isRollingOnPlayerInk) {
                mState = OctoballState::cDazedUncurl;
                mChanceTimer = cChanceSinkFrame;
                mMuteki = false;
                mCurrentSpeed = mSpeedPL;
                mStateTimer = 0;
                break;
            }

            mMuteki = true;
            mCurrentSpeed = mSpeedNormal;
            if (mEscapeTimer > 0) {
                mCurrentSpeed *= cEscapeSpeedRate;
                mEscapeTimer--;
            }

            stepRollingMovement();

            if (mStateTimer >= 180) { // Roll for 3 seconds then rest
                mState = OctoballState::cWait;
                mMuteki = false;
                mStateTimer = 0;
            }
            break;

        case OctoballState::cDazedUncurl:
            mMuteki = false;
            mCurrentSpeed = mSpeedPL;
            if (mChanceTimer > 0) {
                mChanceTimer--;
            }
            if (mStateTimer >= 60 || mChanceTimer <= 0) {
                mState = OctoballState::cWait;
                mStateTimer = 0;
            }
            break;

        case OctoballState::cFall:
            mMuteki = false;
            break;

        case OctoballState::cDie:
        default:
            mMuteki = false;
            break;
    }
}

void Enm_Ball::update() {
    vfunc_7();
}

void Enm_Ball::draw() {
    if (mState != OctoballState::cDie) {
        GambitActor::draw();
    }
}

} // namespace Game
