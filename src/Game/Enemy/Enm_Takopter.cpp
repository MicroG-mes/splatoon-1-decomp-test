#include "Game/Enemy/Enm_Takopter.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cmath>

namespace Game {

Enm_Takopter::Enm_Takopter()
    : mPosition(0.0f, cHoverAltitude, 0.0f),
      mVelocity(0.0f, 0.0f, 0.0f),
      mHp(cMaxHp),
      mPropellerSpeed(1.0f),
      mBobPhase(0.0f),
      mState(TakopterState::cPatrolHover),
      mStateTimer(0),
      mShotCooldown(0) {
}

Enm_Takopter::~Enm_Takopter() {
}

void Enm_Takopter::init() {
    GambitActor::init();
    mPosition.set(0.0f, cHoverAltitude, 0.0f);
    mVelocity.set(0.0f, 0.0f, 0.0f);
    mHp = cMaxHp;
    mPropellerSpeed = 1.0f;
    mBobPhase = 0.0f;
    mState = TakopterState::cPatrolHover;
    mStateTimer = 0;
    mShotCooldown = 0;
}

void Enm_Takopter::applyDamage(f32 damage, const sead::Vector3f& knockbackDir) {
    if (mState == TakopterState::cDefeated) {
        return;
    }

    mHp -= damage;
    mVelocity.x += knockbackDir.x * 0.15f;
    mVelocity.z += knockbackDir.z * 0.15f;

    if (mHp <= 0.0f) {
        mState = TakopterState::cDefeated;
        mStateTimer = 0;

        PaintTextureMgr* paint = PaintTextureMgr::instance();
        if (paint) {
            paint->splatInk(mPosition, 2.5f, 0); // Player ink burst upon death
        }
    } else {
        mState = TakopterState::cHitStagger;
        mStateTimer = 0;
    }
}

void Enm_Takopter::fireInkShot(const sead::Vector3f& targetPos) {
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        paint->splatInk(targetPos, 1.2f, 1); // Enemy ink splatter
    }
}

void Enm_Takopter::updateAi(const sead::Vector3f& playerPos) {
    if (mState == TakopterState::cDefeated) {
        return;
    }

    f32 dx = playerPos.x - mPosition.x;
    f32 dz = playerPos.z - mPosition.z;
    f32 dist = std::sqrt(dx * dx + dz * dz);

    switch (mState) {
        case TakopterState::cPatrolHover:
            // Hover bobbing
            mBobPhase += 0.06f;
            mPosition.y = cHoverAltitude + std::sin(mBobPhase) * 0.25f;

            if (dist < 12.0f) {
                mState = TakopterState::cNoticePlayer;
                mStateTimer = 0;
            }
            break;

        case TakopterState::cNoticePlayer:
            // Rotate facing player and prepare shot
            if (mStateTimer >= 30) {
                mState = TakopterState::cShootInk;
                mStateTimer = 0;
            }
            break;

        case TakopterState::cShootInk:
            if (mStateTimer >= 20) {
                fireInkShot(playerPos);
                mState = TakopterState::cPatrolHover;
                mStateTimer = 0;
            }
            break;

        case TakopterState::cHitStagger:
            mPropellerSpeed = 0.4f; // Propeller wobbles
            if (mStateTimer >= 25) {
                mPropellerSpeed = 1.0f;
                mState = TakopterState::cPatrolHover;
                mStateTimer = 0;
            }
            break;

        case TakopterState::cDefeated:
        default:
            break;
    }
}

void Enm_Takopter::update() {
    mStateTimer++;

    // Drag velocity
    mPosition.x += mVelocity.x;
    mPosition.z += mVelocity.z;
    mVelocity.x *= 0.90f;
    mVelocity.z *= 0.90f;
}

void Enm_Takopter::draw() {
    if (mState != TakopterState::cDefeated) {
        GambitActor::draw();
    }
}

} // namespace Game
