#include "Game/Enemy/Enm_SuperShotMan.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cmath>

namespace Game {

Enm_SuperShotMan::Enm_SuperShotMan()
    : mPosition(0.0f, 0.0f, 0.0f),
      mAimTarget(0.0f, 0.0f, 0.0f),
      mHp(cMaxHp),
      mState(OctosniperState::cScanningSight),
      mStateTimer(0) {
}

Enm_SuperShotMan::~Enm_SuperShotMan() {
}

void Enm_SuperShotMan::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mAimTarget.set(0.0f, 0.0f, 0.0f);
    mHp = cMaxHp;
    mState = OctosniperState::cScanningSight;
    mStateTimer = 0;
}

void Enm_SuperShotMan::applyDamage(f32 damage) {
    if (mState == OctosniperState::cDefeated || mState == OctosniperState::cDuckCover) {
        return; // Invulnerable behind bunker cover
    }

    mHp -= damage;
    if (mHp <= 0.0f) {
        mState = OctosniperState::cDefeated;
        mStateTimer = 0;

        PaintTextureMgr* paint = PaintTextureMgr::instance();
        if (paint) {
            paint->splatInk(mPosition, 3.5f, 0); // Player ink burst on death
        }
    } else {
        mState = OctosniperState::cHitStagger;
        mStateTimer = 0;
    }
}

void Enm_SuperShotMan::dischargeSniperBullet() {
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        // Splats continuous ink trail along laser aiming line
        f32 dx = mAimTarget.x - mPosition.x;
        f32 dz = mAimTarget.z - mPosition.z;
        f32 dist = std::sqrt(dx * dx + dz * dz);
        if (dist > 0.1f) {
            f32 dirX = dx / dist;
            f32 dirZ = dz / dist;
            for (f32 step = 2.0f; step <= dist; step += 3.0f) {
                sead::Vector3f path(mPosition.x + dirX * step, mPosition.y, mPosition.z + dirZ * step);
                paint->splatInk(path, 1.2f, 1); // Octarian enemy ink
            }
        }
        paint->splatInk(mAimTarget, 2.5f, 1);
    }
}

void Enm_SuperShotMan::updateAi(const sead::Vector3f& playerPos, bool hasLineOfSight) {
    if (mState == OctosniperState::cDefeated) {
        return;
    }

    f32 dx = playerPos.x - mPosition.x;
    f32 dz = playerPos.z - mPosition.z;
    f32 dist = std::sqrt(dx * dx + dz * dz);

    switch (mState) {
        case OctosniperState::cScanningSight:
            if (hasLineOfSight && dist <= cMaxLaserDistance) {
                mAimTarget = playerPos;
                mState = OctosniperState::cChargingShot;
                mStateTimer = 0;
            }
            break;

        case OctosniperState::cChargingShot:
            // Smoothly track laser aiming target towards player
            mAimTarget.x += (playerPos.x - mAimTarget.x) * 0.12f;
            mAimTarget.y += (playerPos.y - mAimTarget.y) * 0.12f;
            mAimTarget.z += (playerPos.z - mAimTarget.z) * 0.12f;

            if (mStateTimer >= cChargeDuration) {
                mState = OctosniperState::cFireSnipe;
                mStateTimer = 0;
            }
            break;

        case OctosniperState::cFireSnipe:
            dischargeSniperBullet();
            mState = OctosniperState::cDuckCover;
            mStateTimer = 0;
            break;

        case OctosniperState::cDuckCover:
            // Crouches safely behind bunker
            if (mStateTimer >= cCoverDuration) {
                mState = OctosniperState::cScanningSight;
                mStateTimer = 0;
            }
            break;

        case OctosniperState::cHitStagger:
            if (mStateTimer >= 30) {
                mState = OctosniperState::cDuckCover;
                mStateTimer = 0;
            }
            break;

        case OctosniperState::cDefeated:
        default:
            break;
    }
}

void Enm_SuperShotMan::update() {
    mStateTimer++;
}

void Enm_SuperShotMan::draw() {
    if (mState != OctosniperState::cDefeated) {
        GambitActor::draw();
    }
}

} // namespace Game
