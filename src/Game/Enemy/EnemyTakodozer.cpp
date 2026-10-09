#include "Game/Enemy/EnemyTakodozer.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cmath>
#include <algorithm>

namespace Game {

EnemyTakodozer::EnemyTakodozer()
    : mState(OctodozerState::cPatrol),
      mStateTimer(0),
      mLostTargetTimer(0),
      mPosition(0.0f, 0.0f, 0.0f),
      mMoveDirVec(0.0f, 0.0f, 1.0f),
      mMoveDir(1.0f),
      mCurrentSpeed(cPatrolSpeed),
      mTargetSpeed(cPatrolSpeed),
      mTentacleHp(cDefaultLife),
      mMaxHp(cDefaultLife),
      mEyeScale(1.0f),
      mDeflectedDamage(0.0f),
      mDroppedEggs(0),
      mBoneScrap(0),
      mBoneNotAnimRoot(0),
      mBoneFootLF(0),
      mBoneFootLB(0),
      mBoneFootRF(0),
      mBoneFootRB(0),
      mBoneEyeL(0),
      mBoneEyeR(0) {
    vfunc_5();
}

EnemyTakodozer::~EnemyTakodozer() {
}

void EnemyTakodozer::init() {
    GambitActor::init();
    mTentacleHp = mMaxHp;
    mCurrentSpeed = cPatrolSpeed;
    mTargetSpeed = cPatrolSpeed;
    mMoveDir = 1.0f;
    mMoveDirVec.set(0.0f, 0.0f, 1.0f);
    mEyeScale = 1.0f;
    mDeflectedDamage = 0.0f;
    mDroppedEggs = 0;
    mLostTargetTimer = 0;
    mStateTimer = 0;
    mState = OctodozerState::cPatrol;
    vfunc_5();
}

void EnemyTakodozer::vfunc_3() {
    // Model and resource initialization (Enm_Takodozer.szs, Enm_Takodozer.params)
}

void EnemyTakodozer::vfunc_5() {
    // Look up bone indices from skeleton matching PowerPC 0x023AEFC8
    mBoneScrap        = 1; // Scrap: rear tentacle driver weak point
    mBoneNotAnimRoot  = 2; // not_anim_root
    mBoneFootLF       = 3; // foot_L_F
    mBoneFootLB       = 4; // foot_L_B
    mBoneFootRF       = 5; // foot_R_F
    mBoneFootRB       = 6; // foot_R_B
    mBoneEyeL         = 7; // eye_L
    mBoneEyeR         = 8; // eye_R
}

void EnemyTakodozer::vfunc_7() {
    // Update tick matching PowerPC 0x023B0238
    if (mLostTargetTimer > 0) {
        mLostTargetTimer--;
    }
    mStateTimer++;
}

void EnemyTakodozer::vfunc_14() {
    // Collision evaluation
}

void EnemyTakodozer::vfunc_47() {
    // Animation and skeleton sync
}

void EnemyTakodozer::vfunc_52() {
    // Eye directing and player tracking
}

void EnemyTakodozer::vfunc_54() {
    // Sound component events
}

void EnemyTakodozer::vfunc_60() {
    // State machine dispatcher
}

bool EnemyTakodozer::isFrontShieldHit(const sead::Vector3f& hitDir) const {
    // Front plow deflects shots coming toward the front (+Z or current forward heading)
    f32 dot = hitDir.x * mMoveDirVec.x + hitDir.z * mMoveDirVec.z;
    return dot < 0.0f; // Incoming projectile facing opposite to bulldozer direction
}

bool EnemyTakodozer::applyDamage(f32 damage, const sead::Vector3f& hitDir, bool isRearHit) {
    if (mState == OctodozerState::cDie) {
        return false;
    }

    if (!isRearHit && isFrontShieldHit(hitDir)) {
        // Frontal shield plow deflects damage completely!
        mDeflectedDamage += damage;
        return false;
    }

    // Rear driver cockpit hit
    applyWeakpointDamage(damage);
    return true;
}

void EnemyTakodozer::applyWeakpointDamage(f32 damage) {
    if (mState == OctodozerState::cDie) {
        return;
    }

    mTentacleHp -= damage;
    if (mTentacleHp <= 0.0f) {
        mTentacleHp = 0.0f;
        mState = OctodozerState::cDie;
        mStateTimer = 0;
        mDroppedEggs = cDroppedPowerEggsOnDie;

        PaintTextureMgr* paint = PaintTextureMgr::instance();
        if (paint) {
            paint->splatInk(mPosition, cDiePaintRadius, 1);
        }
    } else {
        // Stagger / alert
        mState = OctodozerState::cLost;
        mLostTargetTimer = 60;
        mStateTimer = 0;
    }
}

void EnemyTakodozer::paintDozerTrail() {
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        paint->splatInk(mPosition, cTrackPaintRadius * 0.1f, 1); // Enemy purple ink trail
    }
}

void EnemyTakodozer::turnAround() {
    mMoveDir = -mMoveDir;
    mMoveDirVec.set(0.0f, 0.0f, mMoveDir);
}

void EnemyTakodozer::updatePatrol(f32 pathLength) {
    if (mState == OctodozerState::cDie) {
        return;
    }

    vfunc_7();

    switch (mState) {
        case OctodozerState::cPatrol:
        case OctodozerState::cChargeForward:
            mCurrentSpeed = cPatrolSpeed;
            mPosition.z += mCurrentSpeed * mMoveDir;
            paintDozerTrail();

            if (mPosition.z > pathLength || mPosition.z < -pathLength) {
                turnAround();
                mState = OctodozerState::cTurnAround;
                mStateTimer = 0;
            }
            break;

        case OctodozerState::cTurnAround:
            if (mStateTimer >= 45) { // 45 frames turn completion
                mState = OctodozerState::cPatrol;
            }
            break;

        case OctodozerState::cLost:
            if (mLostTargetTimer <= 0) {
                mState = OctodozerState::cPatrol;
            }
            break;

        default:
            break;
    }
}

void EnemyTakodozer::updateAi(const sead::Vector3f& playerPos, f32 pathLength) {
    if (mState == OctodozerState::cDie) {
        return;
    }

    vfunc_7();

    f32 dx = playerPos.x - mPosition.x;
    f32 dz = playerPos.z - mPosition.z;
    f32 dist = std::sqrt(dx * dx + dz * dz);

    // Compute angle relative to movement direction
    f32 forwardZ = mMoveDirVec.z;
    bool inFront = (dz * forwardZ) > 0.0f;

    // Sight check: primary cone (500m, narrow angle) or secondary close range (50m, wide angle)
    bool detected = false;
    if (dist < cEyesightRadius && inFront) {
        // Forward cone
        detected = true;
    } else if (dist < cEyesightRadius2) {
        // Close range perimeter
        detected = true;
    }

    if (detected) {
        mState = OctodozerState::cChase;
        mEyeScale = cChaseEyeScale;
        mTargetSpeed = cChaseSpeed;
        mLostTargetTimer = cLostTargetCooldownFrame; // 300 frame cooldown
    } else if (mState == OctodozerState::cChase) {
        if (mLostTargetTimer <= 0) {
            mState = OctodozerState::cLost;
            mEyeScale = 1.0f;
            mTargetSpeed = cPatrolSpeed;
        }
    }

    // Accelerate toward target speed
    if (mCurrentSpeed < mTargetSpeed) {
        mCurrentSpeed = std::min(mCurrentSpeed + cAcceleration, mTargetSpeed);
    } else if (mCurrentSpeed > mTargetSpeed) {
        mCurrentSpeed = std::max(mCurrentSpeed - cAcceleration, mTargetSpeed);
    }

    // Move forward along heading
    mPosition.z += mCurrentSpeed * mMoveDir;
    paintDozerTrail();

    // Boundary bounce
    if (mPosition.z > pathLength || mPosition.z < -pathLength) {
        turnAround();
    }
}

void EnemyTakodozer::update() {
    vfunc_7();
}

void EnemyTakodozer::draw() {
    if (mState != OctodozerState::cDie) {
        GambitActor::draw();
    }
}

} // namespace Game
