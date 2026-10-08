#include "Game/Enemy/EnemyStampKing.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cmath>

namespace Game {

EnemyStampKing::EnemyStampKing()
    : mState(StampKingState::cHoverStalk),
      mPhase(StampKingPhase::cPhase1),
      mStateTimer(0),
      mPosition(0.0f, 0.0f, 0.0f),
      mTargetPlayerPos(0.0f, 0.0f, 0.0f),
      mYawAngle(0.0f),
      mTentacleHp(cTentacleMaxHp),
      mSideArmorLeft(false),
      mSideArmorRight(false) {
}

EnemyStampKing::~EnemyStampKing() {
}

void EnemyStampKing::init() {
    GambitActor::init();
    mState = StampKingState::cHoverStalk;
    mPhase = StampKingPhase::cPhase1;
    mStateTimer = 0;
    mPosition.set(0.0f, 0.0f, 0.0f);
    mTargetPlayerPos.set(0.0f, 0.0f, 0.0f);
    mYawAngle = 0.0f;
    mTentacleHp = cTentacleMaxHp;
    mSideArmorLeft = false;
    mSideArmorRight = false;
}

void EnemyStampKing::triggerFaceSlamImpact() {
    // Slams onto the arena floor, spraying shockwave ink
    mPosition.y = 0.0f;
    mState = StampKingState::cStuckVulnerable;
    mStateTimer = 0;

    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        paint->splatInk(mPosition, cSlamShockwaveRadius, 1); // Enemy purple ink
    }
}

void EnemyStampKing::applyTentacleDamage(f32 damage) {
    if (mState != StampKingState::cStuckVulnerable) {
        return; // Only vulnerable while face is stuck flat on ground
    }

    mTentacleHp -= damage;
    if (mTentacleHp <= 0.0f) {
        if (mPhase == StampKingPhase::cPhase1) {
            mPhase = StampKingPhase::cPhase2;
            mTentacleHp = cTentacleMaxHp;
            mSideArmorLeft = true; // Armor belt on 1 side
            mState = StampKingState::cRecoverRise;
            mStateTimer = 0;
        } else if (mPhase == StampKingPhase::cPhase2) {
            mPhase = StampKingPhase::cPhase3;
            mTentacleHp = cTentacleMaxHp;
            mSideArmorLeft = true;
            mSideArmorRight = true; // Metal grates on both sides
            mState = StampKingState::cRecoverRise;
            mStateTimer = 0;
        } else {
            mState = StampKingState::cDefeated;
            mStateTimer = 0;
        }
    }
}

void EnemyStampKing::updateBossAi(const sead::Vector3f& playerPos) {
    if (mState == StampKingState::cDefeated) {
        return;
    }

    mStateTimer++;

    switch (mState) {
        case StampKingState::cHoverStalk: {
            // Hover around arena turning toward player
            f32 dx = playerPos.x - mPosition.x;
            f32 dz = playerPos.z - mPosition.z;
            mYawAngle = std::atan2(dx, dz);

            f32 stalkSpeed = (mPhase == StampKingPhase::cPhase1) ? 0.04f : 0.08f;
            mPosition.x += dx * stalkSpeed;
            mPosition.z += dz * stalkSpeed;
            mPosition.y = 0.0f;

            if (mStateTimer >= 120) { // 2.0s stalk before rearing back
                mTargetPlayerPos = playerPos;
                mState = StampKingState::cTelegraphRise;
                mStateTimer = 0;
            }
            break;
        }

        case StampKingState::cTelegraphRise:
            // Rears back on its heels tilting face upward
            mPosition.y += 0.08f;
            if (mStateTimer >= 45) { // Warning pitch telegraph
                mState = StampKingState::cFaceSlam;
                mStateTimer = 0;
            }
            break;

        case StampKingState::cFaceSlam:
            // High-speed downward crash slam onto player position
            mPosition.x += (mTargetPlayerPos.x - mPosition.x) * 0.25f;
            mPosition.z += (mTargetPlayerPos.z - mPosition.z) * 0.25f;
            mPosition.y -= 0.35f;

            if (mPosition.y <= 0.0f) {
                triggerFaceSlamImpact();
            }
            break;

        case StampKingState::cStuckVulnerable: {
            // Face plant stuck on arena floor. Player can ink sides and climb to tentacle!
            mPosition.y = 0.0f;
            s32 stunDuration = (mPhase == StampKingPhase::cPhase1) ? 240 : (mPhase == StampKingPhase::cPhase2 ? 180 : 150);
            if (mStateTimer >= stunDuration) {
                mState = StampKingState::cRecoverRise;
                mStateTimer = 0;
            }
            break;
        }

        case StampKingState::cRecoverRise:
            // Pushes off ground and stands back upright
            mPosition.y += 0.15f;
            if (mStateTimer >= 60) {
                mPosition.y = 0.0f;
                mState = StampKingState::cHoverStalk;
                mStateTimer = 0;
            }
            break;

        case StampKingState::cDefeated:
        default:
            break;
    }
}

void EnemyStampKing::update() {
    GambitActor::update();
}

void EnemyStampKing::draw() {
    if (mState != StampKingState::cDefeated) {
        GambitActor::draw();
    }
}

} // namespace Game
