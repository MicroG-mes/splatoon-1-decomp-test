#include "Game/Enemy/EnemyRailKing.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cmath>

namespace Game {

EnemyRailKing::EnemyRailKing()
    : mState(OctavioState::cHoverArena),
      mPhase(OctavioPhase::cPhase1),
      mStateTimer(0),
      mPosition(0.0f, 6.0f, 20.0f),
      mTargetPlayerPos(0.0f, 0.0f, 0.0f),
      mFistPos(0.0f, 6.0f, 15.0f),
      mFistVelocity(0.0f, 0.0f, 0.0f),
      mTentacleHp(cMaxHealth),
      mFistSwatRemaining(cFistSwatHp),
      mFistActive(false),
      mFistReflected(false) {
}

EnemyRailKing::~EnemyRailKing() {
}

void EnemyRailKing::init() {
    GambitActor::init();
    mState = OctavioState::cHoverArena;
    mPhase = OctavioPhase::cPhase1;
    mStateTimer = 0;
    mPosition.set(0.0f, 6.0f, 20.0f);
    mTargetPlayerPos.set(0.0f, 0.0f, 0.0f);
    mFistPos = mPosition;
    mFistVelocity.set(0.0f, 0.0f, 0.0f);
    mTentacleHp = cMaxHealth;
    mFistSwatRemaining = cFistSwatHp;
    mFistActive = false;
    mFistReflected = false;
}

void EnemyRailKing::applyDamageToFist(f32 damage) {
    if (!mFistActive || mFistReflected) {
        return;
    }

    mFistSwatRemaining -= damage;
    if (mFistSwatRemaining <= 0.0f) {
        // Swatted back! Fist flies back toward Octavio
        mFistReflected = true;
        mFistVelocity.x = -mFistVelocity.x * 1.5f;
        mFistVelocity.y = -mFistVelocity.y * 1.5f;
        mFistVelocity.z = -mFistVelocity.z * 1.5f;
        mState = OctavioState::cFistReflected;
    }
}

void EnemyRailKing::applyTentacleDamage(f32 damage) {
    if (mState != OctavioState::cStunnedExposed) {
        return; // Only vulnerable while stunned
    }

    mTentacleHp -= damage;
    if (mTentacleHp <= 0.0f) {
        advancePhase();
    }
}

void EnemyRailKing::advancePhase() {
    u32 current = static_cast<u32>(mPhase);
    if (current < 5) {
        mPhase = static_cast<OctavioPhase>(current + 1);
        mTentacleHp = cMaxHealth;
        mState = OctavioState::cPhaseTransition;
        mStateTimer = 0;
        mFistActive = false;
        mFistReflected = false;
    } else {
        mState = OctavioState::cDefeated;
        mStateTimer = 0;
    }
}

void EnemyRailKing::updateBossAi(const sead::Vector3f& playerPos) {
    if (mState == OctavioState::cDefeated) {
        return;
    }

    mStateTimer++;

    switch (mState) {
        case OctavioState::cHoverArena: {
            // Hover in 3D air swaying side-to-side
            f32 sway = std::sin(mStateTimer * 0.04f) * 4.0f;
            mPosition.x = sway;
            mPosition.y = 5.5f;

            if (mStateTimer >= 100) {
                // Launch rocket fist
                mTargetPlayerPos = playerPos;
                mFistPos = mPosition;
                mFistActive = true;
                mFistReflected = false;
                mFistSwatRemaining = cFistSwatHp;

                // Aim fist toward player
                f32 dx = playerPos.x - mPosition.x;
                f32 dy = playerPos.y - mPosition.y;
                f32 dz = playerPos.z - mPosition.z;
                f32 dist = std::sqrt(dx * dx + dy * dy + dz * dz);
                if (dist > 0.001f) {
                    f32 speed = 0.45f;
                    mFistVelocity.set(dx / dist * speed, dy / dist * speed, dz / dist * speed);
                }
                mState = OctavioState::cLaunchRocketPunch;
                mStateTimer = 0;
            }
            break;
        }

        case OctavioState::cLaunchRocketPunch: {
            // Fist travels toward player
            mFistPos.x += mFistVelocity.x;
            mFistPos.y += mFistVelocity.y;
            mFistPos.z += mFistVelocity.z;

            if (mFistPos.z <= 0.0f || mStateTimer >= 120) {
                // Fist missed or hit player -> retracts
                mFistActive = false;
                mState = OctavioState::cHoverArena;
                mStateTimer = 0;
            }
            break;
        }

        case OctavioState::cFistReflected: {
            // Fist flies backward into Octavio's mech
            mFistPos.x += mFistVelocity.x;
            mFistPos.y += mFistVelocity.y;
            mFistPos.z += mFistVelocity.z;

            f32 toBossDistSq = (mFistPos.x - mPosition.x) * (mFistPos.x - mPosition.x) +
                               (mFistPos.y - mPosition.y) * (mFistPos.y - mPosition.y) +
                               (mFistPos.z - mPosition.z) * (mFistPos.z - mPosition.z);

            if (toBossDistSq <= 4.0f || mFistPos.z >= mPosition.z) {
                // Direct hit on Octavio! Stuns the mech
                mFistActive = false;
                mState = OctavioState::cStunnedExposed;
                mPosition.y = 1.0f; // Mech drops low to ground
                mStateTimer = 0;
            }
            break;
        }

        case OctavioState::cStunnedExposed: {
            // Cockpit turntable exposed for tentacle attack
            if (mStateTimer >= 300) { // 5.0 seconds stun
                mPosition.y = 6.0f;
                mState = OctavioState::cHoverArena;
                mStateTimer = 0;
            }
            break;
        }

        case OctavioState::cPhaseTransition: {
            // Pushes back into air, initiates new phase patterns
            mPosition.y += 0.1f;
            if (mPosition.y >= 6.0f) {
                mPosition.y = 6.0f;
                mState = OctavioState::cHoverArena;
                mStateTimer = 0;
            }
            break;
        }

        case OctavioState::cKillerWailBarrage:
        case OctavioState::cOctoballBarrage:
        case OctavioState::cDefeated:
        default:
            break;
    }
}

void EnemyRailKing::update() {
    GambitActor::update();
}

void EnemyRailKing::draw() {
    if (mState != OctavioState::cDefeated) {
        GambitActor::draw();
    }
}

} // namespace Game
