#include "Game/Enemy/Enm_RailKing.h"
#include <cstring>
#include <cmath>

namespace Game {

Enm_RailKing::Enm_RailKing()
    : mState(RailKingState::cHoverPatrol),
      mPhase(RailKingPhase::cPhase1),
      mStateTimer(0),
      mPosition(0.0f, 6.0f, 25.0f),
      mFistPosition(0.0f, 6.0f, 25.0f),
      mFistVelocity(0.0f, 0.0f, 0.0f),
      mFistDamageAccumulated(0.0f),
      mCockpitHp(100.0f),
      mThrustNode0(0.0f, 0.0f, 0.0f),
      mThrustNode1(0.0f, 0.0f, 0.0f),
      mSoundHandle(0),
      mLeftFistPtr(nullptr),
      mRightFistPtr(nullptr),
      mCockpitTurntablePtr(nullptr),
      mVisualizerPtr(nullptr),
      mKillerWailSpeaker1(nullptr),
      mKillerWailSpeaker2(nullptr),
      mOctocopterSpawner1(nullptr),
      mOctocopterSpawner2(nullptr),
      mOctocopterSpawner3(nullptr),
      mRainmakerLauncher(nullptr) {
    std::memset(mPaddingNodes, 0, sizeof(mPaddingNodes));
    std::memset(mPaddingSound, 0, sizeof(mPaddingSound));
    std::memset(mPaddingFists, 0, sizeof(mPaddingFists));
    std::memset(mPaddingCockpit, 0, sizeof(mPaddingCockpit));
    std::memset(mPaddingSpk, 0, sizeof(mPaddingSpk));
    std::memset(mPaddingSpawn2, 0, sizeof(mPaddingSpawn2));
    std::memset(mPaddingBomb, 0, sizeof(mPaddingBomb));
}

Enm_RailKing::~Enm_RailKing() {
}

void Enm_RailKing::init() {
    GambitActor::init();
    mPilotHouse.init();
    mState = RailKingState::cHoverPatrol;
    mPhase = RailKingPhase::cPhase1;
    mCockpitHp = 100.0f;
    mFistDamageAccumulated = 0.0f;
    mPosition.set(0.0f, 6.0f, 25.0f);
}

/**
 * Enm_RailKing__vfunc_7 @ 0x02375284
 * Tick for DJ Octavio's mecha: manages rocket fists (0x298, 0x29C),
 * turntable (0x2AC), visualizer (0x2B0), floating thrust nodes (0x218, 0x220).
 */
void Enm_RailKing::vfunc_7() {
    // Thrust nodes oscillation
    f32 hoverBob = std::sin(static_cast<f32>(mStateTimer) * 0.05f) * 0.02f;
    mPosition.y += hoverBob;
    mThrustNode0.y = mPosition.y;
    mThrustNode1.y = mPosition.y;

    mPilotHouse.update();
}

/**
 * Enm_RailKing__vfunc_11 @ 0x02375c84
 * Collision check for deflection / reflected punch back at Octavio.
 */
void Enm_RailKing::vfunc_11() {
    if (mState == RailKingState::cFistReflected) {
        f32 dx = mFistPosition.x - mPosition.x;
        f32 dy = mFistPosition.y - mPosition.y;
        f32 dz = mFistPosition.z - mPosition.z;
        f32 distSq = dx * dx + dy * dy + dz * dz;
        if (distSq < 9.0f) { // Within 3 meters of cockpit
            mState = RailKingState::cStunnedGrooving;
            mStateTimer = 0;
            applyDamageToCockpit(35.0f);
        }
    }
}

void Enm_RailKing::updateBossAi(const sead::Vector3f& playerPos) {
    mStateTimer++;

    switch (mState) {
        case RailKingState::cHoverPatrol:
            if (mStateTimer >= 120) {
                mState = RailKingState::cLaunchFist;
                mStateTimer = 0;
                mFistPosition = mPosition;
                f32 dx = playerPos.x - mPosition.x;
                f32 dy = playerPos.y - mPosition.y;
                f32 dz = playerPos.z - mPosition.z;
                f32 len = std::sqrt(dx * dx + dy * dy + dz * dz);
                if (len > 0.01f) {
                    f32 speed = 0.4f + static_cast<f32>(static_cast<u32>(mPhase) - 1) * 0.08f;
                    mFistVelocity.set((dx / len) * speed, (dy / len) * speed, (dz / len) * speed);
                }
            }
            break;

        case RailKingState::cLaunchFist:
            mFistPosition.x += mFistVelocity.x;
            mFistPosition.y += mFistVelocity.y;
            mFistPosition.z += mFistVelocity.z;
            if (mStateTimer >= 180) {
                mState = RailKingState::cHoverPatrol;
                mStateTimer = 0;
                mFistDamageAccumulated = 0.0f;
            }
            break;

        case RailKingState::cFistReflected:
            mFistPosition.x += mFistVelocity.x;
            mFistPosition.y += mFistVelocity.y;
            mFistPosition.z += mFistVelocity.z;
            vfunc_11();
            break;

        case RailKingState::cStunnedGrooving:
            if (mStateTimer >= 180) {
                mState = RailKingState::cHoverPatrol;
                mStateTimer = 0;
            }
            break;

        case RailKingState::cMissileBarrage:
        case RailKingState::cDefeated:
            break;
    }
}

void Enm_RailKing::applyDamageToFist(f32 damage) {
    if (mState != RailKingState::cLaunchFist) {
        return;
    }

    mFistDamageAccumulated += damage;
    if (mFistDamageAccumulated >= cDeflectionThreshold) {
        triggerFistReflect();
    }
}

void Enm_RailKing::triggerFistReflect() {
    mState = RailKingState::cFistReflected;
    mFistDamageAccumulated = 0.0f;
    mFistVelocity.set(-mFistVelocity.x * 1.5f, -mFistVelocity.y * 1.5f, -mFistVelocity.z * 1.5f);
}

void Enm_RailKing::applyDamageToCockpit(f32 damage) {
    mCockpitHp -= damage;
    if (mCockpitHp <= 0.0f) {
        advancePhase();
    }
}

void Enm_RailKing::advancePhase() {
    u32 next = static_cast<u32>(mPhase) + 1;
    if (next <= static_cast<u32>(RailKingPhase::cPhase5)) {
        mPhase = static_cast<RailKingPhase>(next);
        mCockpitHp = 100.0f;
        mState = RailKingState::cHoverPatrol;
        mStateTimer = 0;
    } else {
        mState = RailKingState::cDefeated;
    }
}

void Enm_RailKing::update() {
    GambitActor::update();
    vfunc_7();
}

void Enm_RailKing::draw() {
    GambitActor::draw();
    mPilotHouse.draw();
}

} // namespace Game
