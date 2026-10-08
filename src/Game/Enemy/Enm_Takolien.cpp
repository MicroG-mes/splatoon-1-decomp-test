#include "Game/Enemy/Enm_Takolien.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cmath>

namespace Game {

Enm_Takolien::Enm_Takolien()
    : mPosition(0.0f, 0.0f, 0.0f),
      mMoveDirection(0.0f, 0.0f, 1.0f),
      mHp(cMaxHp),
      mForm(TakolienForm::cHumanoid),
      mState(TakolienState::cPatrol),
      mStateTimer(0),
      mBurstShotCounter(0),
      mTargetAcquisitionFrames(0),
      mOctoDodgePos(0.0f, 0.0f, 0.0f),
      mOctoFormFlags(0) {
}

Enm_Takolien::~Enm_Takolien() {
}

// 0x023B73AC: Decompiled vfunc_52 (Enter Humanoid Form)
void Enm_Takolien::vfunc_52() {
    mForm = TakolienForm::cHumanoid;
    mOctoFormFlags &= ~1;
}

// 0x023BC858: Decompiled vfunc_53 (Enter Octopus Form)
void Enm_Takolien::vfunc_53() {
    mForm = TakolienForm::cOctopus;
    mOctoFormFlags |= 1;
    mOctoDodgePos = mPosition;
}

// 0x023B71EC: Decompiled vfunc_7 (AI update tick & target frame counter)
void Enm_Takolien::vfunc_7() {
    if (mState == TakolienState::cDefeated) return;

    mTargetAcquisitionFrames++;
    if (mForm == TakolienForm::cHumanoid) {
        // Humanoid updates aim and weapon
    } else {
        // Octopus form swims in purple ink
        mPosition.x += mMoveDirection.x * cOctoSwimSpeed;
        mPosition.z += mMoveDirection.z * cOctoSwimSpeed;
    }
}

void Enm_Takolien::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mMoveDirection.set(0.0f, 0.0f, 1.0f);
    mHp = cMaxHp;
    mForm = TakolienForm::cHumanoid;
    mState = TakolienState::cPatrol;
    mStateTimer = 0;
    mBurstShotCounter = 0;
    mTargetAcquisitionFrames = 0;
    mOctoDodgePos.set(0.0f, 0.0f, 0.0f);
    mOctoFormFlags = 0;
}

void Enm_Takolien::applyDamage(f32 damage, const sead::Vector3f& hitDir) {
    if (mState == TakolienState::cDefeated) {
        return;
    }

    mHp -= damage;
    if (mHp <= 0.0f) {
        mState = TakolienState::cDefeated;
        mStateTimer = 0;

        PaintTextureMgr* paint = PaintTextureMgr::instance();
        if (paint) {
            paint->splatInk(mPosition, 3.0f, 0); // Player ink burst upon defeat
        }
    } else {
        // High-level AI response: quick dodge swim when taking damage
        mState = TakolienState::cSwimDodge;
        mForm = TakolienForm::cOctopus;
        mMoveDirection.set(-hitDir.x, 0.0f, -hitDir.z);
        mStateTimer = 0;
    }
}

void Enm_Takolien::fireWeaponBurst(const sead::Vector3f& targetPos) {
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        paint->splatInk(targetPos, 1.4f, 1); // Octarian enemy ink
    }
}

void Enm_Takolien::throwSplatBomb(const sead::Vector3f& targetPos) {
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        paint->splatInk(targetPos, 3.5f, 1);
    }
}

void Enm_Takolien::updateAi(const sead::Vector3f& playerPos, bool isPlayerVisible) {
    if (mState == TakolienState::cDefeated) {
        return;
    }

    f32 dx = playerPos.x - mPosition.x;
    f32 dz = playerPos.z - mPosition.z;
    f32 dist = std::sqrt(dx * dx + dz * dz);

    switch (mState) {
        case TakolienState::cPatrol:
            mForm = TakolienForm::cHumanoid;
            mPosition.x += mMoveDirection.x * cHumanRunSpeed;
            mPosition.z += mMoveDirection.z * cHumanRunSpeed;

            if (isPlayerVisible && dist < 16.0f) {
                mState = TakolienState::cEngageShooting;
                mStateTimer = 0;
                mBurstShotCounter = 0;
            }
            break;

        case TakolienState::cEngageShooting:
            mForm = TakolienForm::cHumanoid;
            // Strafe perpendicular to player line-of-sight
            if (dist > 0.01f) {
                f32 perpX = -dz / dist;
                f32 perpZ =  dx / dist;
                mPosition.x += perpX * (cHumanRunSpeed * 0.7f);
                mPosition.z += perpZ * (cHumanRunSpeed * 0.7f);
            }

            // Rapid 3-burst shooting
            if (mStateTimer % 8 == 0 && mBurstShotCounter < 6) {
                fireWeaponBurst(playerPos);
                mBurstShotCounter++;
            }

            if (mStateTimer >= 60) {
                if (dist < 8.0f) {
                    mState = TakolienState::cThrowBomb;
                    mStateTimer = 0;
                } else {
                    mState = TakolienState::cSwimDodge;
                    mForm = TakolienForm::cOctopus;
                    mStateTimer = 0;
                }
            }
            break;

        case TakolienState::cThrowBomb:
            mForm = TakolienForm::cHumanoid;
            if (mStateTimer >= 20) {
                throwSplatBomb(playerPos);
                mState = TakolienState::cSwimDodge;
                mForm = TakolienForm::cOctopus;
                mStateTimer = 0;
            }
            break;

        case TakolienState::cSwimDodge:
            mForm = TakolienForm::cOctopus;
            // Swim quickly to cover
            mPosition.x += mMoveDirection.x * cOctoSwimSpeed;
            mPosition.z += mMoveDirection.z * cOctoSwimSpeed;

            if (mStateTimer >= 45) {
                mState = TakolienState::cEngageShooting;
                mForm = TakolienForm::cHumanoid;
                mStateTimer = 0;
                mBurstShotCounter = 0;
            }
            break;

        case TakolienState::cHitStagger:
        case TakolienState::cDefeated:
        default:
            break;
    }
}

void Enm_Takolien::update() {
    mStateTimer++;
    vfunc_7();
}

void Enm_Takolien::draw() {
    if (mState != TakolienState::cDefeated) {
        GambitActor::draw();
    }
}

} // namespace Game
