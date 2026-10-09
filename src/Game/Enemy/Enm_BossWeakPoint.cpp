#include "Game/Enemy/Enm_BossWeakPoint.h"
#include <cmath>
#include <algorithm>

namespace Game {

Enm_BossWeakPoint::Enm_BossWeakPoint()
    : mPosition(0.0f, 0.0f, 0.0f)
    , mState(BossWeakPointState::cState_Disappear)
    , mHealth(cMaxHealth)
    , mPulseScale(1.0f)
    , mPulseVelocity(0.0f)
    , mStateTimer(0)
    , mHitRecoveryTimer(0)
    , mSplatsTriggered(0)
{
    std::fill(mReserved, mReserved + sizeof(mReserved), 0);
}

Enm_BossWeakPoint::~Enm_BossWeakPoint() {
}

void Enm_BossWeakPoint::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mState = BossWeakPointState::cState_Disappear;
    mHealth = cMaxHealth;
    mPulseScale = 1.0f;
    mPulseVelocity = 0.0f;
    mStateTimer = 0;
    mHitRecoveryTimer = 0;
    mSplatsTriggered = 0;
}

void Enm_BossWeakPoint::vfunc_3() {
    // 0x022c82b4: Model/Enm_BossWeakPoint.szs loading & bone root binding
}

void Enm_BossWeakPoint::vfunc_5() {
    // 0x022de850: Parameter initialization from Enm_BossWeakPoint_AnmItp.params
    mHealth = cMaxHealth;
    mPulseScale = 1.0f;
}

void Enm_BossWeakPoint::vfunc_7() {
    // 0x02332a20: State machine update & pulsating scale reaction
    update();
}

void Enm_BossWeakPoint::vfunc_14() {
    // 0x0239ed08: Damage callback & hit reaction
}

void Enm_BossWeakPoint::vfunc_47() {
    // 0x0236aee0: EnemyParametersMgr parameter sync
}

void Enm_BossWeakPoint::appear(const sead::Vector3f& spawnPos) {
    mPosition = spawnPos;
    mState = BossWeakPointState::cState_Appear;
    mStateTimer = 0;
    mPulseScale = 0.1f;
    mPulseVelocity = 0.0f;
    mHealth = cMaxHealth;
}

void Enm_BossWeakPoint::disappear() {
    mState = BossWeakPointState::cState_Disappear;
    mStateTimer = 0;
}

bool Enm_BossWeakPoint::hitWithInk(f32 damage, const sead::Vector3f& hitDir) {
    if (!isVulnerable() || mHealth <= 0.0f) {
        return false;
    }

    mHealth -= damage;
    mState = BossWeakPointState::cState_Damage;
    mHitRecoveryTimer = 12;

    // Harmonic pulsate impulse
    mPulseVelocity = 0.15f;

    if (mHealth <= 0.0f) {
        mHealth = 0.0f;
        mState = BossWeakPointState::cState_Die;
        mStateTimer = 0;
        mSplatsTriggered++;
    }

    return true;
}

void Enm_BossWeakPoint::update() {
    mStateTimer++;

    // Elastic spring back to scale 1.0f
    f32 displacement = mPulseScale - 1.0f;
    f32 springForce = -0.25f * displacement;
    f32 dampingForce = -0.15f * mPulseVelocity;
    mPulseVelocity += (springForce + dampingForce);
    mPulseScale += mPulseVelocity;
    if (mPulseScale > cMaxPulseScale) mPulseScale = cMaxPulseScale;
    if (mPulseScale < 0.1f) mPulseScale = 0.1f;

    switch (mState) {
        case BossWeakPointState::cState_Appear: {
            // Emerges from boss torso
            if (mStateTimer >= 15) {
                mState = BossWeakPointState::cState_Wait;
                mStateTimer = 0;
                mPulseScale = 1.0f;
            }
            break;
        }

        case BossWeakPointState::cState_Wait: {
            // Oscillates gently in wait state
            f32 bobOffset = std::sin(mStateTimer * 0.1f) * 0.03f;
            mPulseScale = 1.0f + bobOffset;
            break;
        }

        case BossWeakPointState::cState_Damage: {
            if (mHitRecoveryTimer > 0) {
                mHitRecoveryTimer--;
            } else {
                mState = BossWeakPointState::cState_Wait;
            }
            break;
        }

        case BossWeakPointState::cState_Disappear: {
            mPulseScale *= 0.85f;
            break;
        }

        case BossWeakPointState::cState_Die: {
            // Erupting death sequence
            mPulseScale += 0.05f;
            break;
        }

        default:
            break;
    }
}

void Enm_BossWeakPoint::draw() {
    // Model rendered via ModelSceneMgr
}

} // namespace Game
