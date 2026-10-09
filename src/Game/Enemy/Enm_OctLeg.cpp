#include "Game/Enemy/Enm_OctLeg.h"
#include <cmath>
#include <algorithm>

namespace Game {

Enm_OctLeg::Enm_OctLeg()
    : mPosition(0.0f, 0.0f, 0.0f)
    , mState(OctLegState::cState_Idle)
    , mHealth(cBaseLife)
    , mShockWaveRadiusProgress(0.0f)
    , mStateTimer(0)
    , mShotTimer(0)
    , mHasArmor(true)
    , mShockWaveActive(false)
{
    std::fill(mReserved, mReserved + sizeof(mReserved), 0);
    for (size_t i = 0; i < 4; ++i) {
        mSegmentPositions[i].set(0.0f, static_cast<f32>(i * 20.0f), 0.0f);
    }
}

Enm_OctLeg::~Enm_OctLeg() {
}

void Enm_OctLeg::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mState = OctLegState::cState_Idle;
    mHealth = cBaseLife;
    mShockWaveRadiusProgress = 0.0f;
    mStateTimer = 0;
    mShotTimer = 0;
    mHasArmor = true;
    mShockWaveActive = false;

    for (size_t i = 0; i < 4; ++i) {
        mSegmentPositions[i].set(0.0f, static_cast<f32>(i * 20.0f), 0.0f);
    }
}

void Enm_OctLeg::vfunc_3() {
    // 0x023657dc: Model & resource loading (Enm_OctLeg.szs, Enm_Break00/01/02)
}

void Enm_OctLeg::vfunc_5() {
    // 0x02365aa0: Parameter initialization from Enm_OctLeg.params
    mHealth = cBaseLife;
    mHasArmor = true;
}

void Enm_OctLeg::vfunc_7() {
    // 0x023666e4: Segmented tentacle motion update & shock wave
    update();
}

void Enm_OctLeg::vfunc_11() {
    // 0x0236672c: BulletEnemyBubbleShotOctLeg projectile emitter
    tryShootBubble();
}

void Enm_OctLeg::vfunc_47() {
    // 0x0236aee0: EnemyParametersMgr parameter sync
}

void Enm_OctLeg::vfunc_52() {
    // 0x02368a54: Damage handling & frontal armor deflection
}

void Enm_OctLeg::vfunc_60() {
    // 0x02368d7c: Death splat & broken armor piece burst
}

void Enm_OctLeg::appear(const sead::Vector3f& pos) {
    mPosition = pos;
    mState = OctLegState::cState_Appear;
    mStateTimer = 0;
    mShotTimer = cShotWaitFrame;
    mHealth = cBaseLife;
    mHasArmor = true;
    mShockWaveActive = false;
    mShockWaveRadiusProgress = 0.0f;
}

bool Enm_OctLeg::checkSight(const sead::Vector3f& targetPos) const {
    f32 dx = targetPos.x - mPosition.x;
    f32 dy = targetPos.y - mPosition.y;
    f32 dz = targetPos.z - mPosition.z;
    f32 distSq = dx * dx + dy * dy + dz * dz;

    if (distSq > cEyesightRadius * cEyesightRadius) {
        return false;
    }

    // Check vertical eyesight threshold (mEyesight_Height: 200.0)
    if (std::abs(dy) > 200.0f) {
        return false;
    }

    return true;
}

void Enm_OctLeg::triggerSlam() {
    mState = OctLegState::cState_Slam;
    mStateTimer = 0;
    mShockWaveActive = true;
    mShockWaveRadiusProgress = 0.0f;
}

bool Enm_OctLeg::tryShootBubble() {
    if (mShotTimer <= 0 && mState != OctLegState::cState_Die && mState != OctLegState::cState_Dazed) {
        mShotTimer = cShotWaitFrame;
        return true;
    }
    return false;
}

bool Enm_OctLeg::receiveDamage(f32 damage, const sead::Vector3f& hitPos, const sead::Vector3f& hitDir, bool& outShieldDeflected) {
    outShieldDeflected = false;

    if (mState == OctLegState::cState_Die) {
        return false;
    }

    // Frontal shield check: if hit arrives from front and armor is intact
    // mShieldOffsetZ: 8.0, mShieldRadius: 10.0
    f32 relZ = hitPos.z - mPosition.z;
    f32 relX = hitPos.x - mPosition.x;
    f32 horizDist = std::sqrt(relX * relX + (relZ - cShieldOffsetZ) * (relZ - cShieldOffsetZ));

    if (mHasArmor && hitDir.z < 0.0f && horizDist <= cShieldRadius) {
        // Frontal shield deflection!
        outShieldDeflected = true;
        return false;
    }

    // Vulnerable hit taken on rear flesh or armor broken
    mHealth -= damage;

    if (mHasArmor && mHealth <= cArmorBreakThresholdHp) {
        // Break armor layers (sheds Enm_Break00/01/02)
        mHasArmor = false;
        mState = OctLegState::cState_ArmorBreak;
        mStateTimer = 0;
    } else {
        mState = OctLegState::cState_Damage;
        mStateTimer = 0;
    }

    if (mHealth <= 0.0f) {
        mHealth = 0.0f;
        mState = OctLegState::cState_Die;
        mStateTimer = 0;
        mShockWaveActive = false;
    }

    return true;
}

void Enm_OctLeg::update() {
    mStateTimer++;

    if (mShotTimer > 0) {
        mShotTimer--;
    }

    // Update shockwave propagation
    if (mShockWaveActive) {
        mShockWaveRadiusProgress += 8.0f;
        if (mShockWaveRadiusProgress >= cShockWaveRadius) {
            mShockWaveActive = false;
            mShockWaveRadiusProgress = cShockWaveRadius;
        }
    }

    // Segmented tentacle motion (body4..body7 harmonic whip)
    f32 wavePhase = mStateTimer * 0.08f;
    for (size_t i = 0; i < 4; ++i) {
        f32 segHeight = 20.0f + static_cast<f32>(i) * 20.0f;
        f32 sway = std::sin(wavePhase + static_cast<f32>(i) * 0.6f) * (static_cast<f32>(i + 1) * 2.5f);
        mSegmentPositions[i].set(mPosition.x + sway, mPosition.y + segHeight, mPosition.z);
    }

    switch (mState) {
        case OctLegState::cState_Appear: {
            if (mStateTimer >= 30) {
                mState = OctLegState::cState_Idle;
                mStateTimer = 0;
            }
            break;
        }

        case OctLegState::cState_Slam: {
            if (mStateTimer >= 45) {
                mState = OctLegState::cState_Idle;
                mStateTimer = 0;
            }
            break;
        }

        case OctLegState::cState_Damage: {
            if (mStateTimer >= 15) {
                mState = OctLegState::cState_Idle;
                mStateTimer = 0;
            }
            break;
        }

        case OctLegState::cState_ArmorBreak: {
            if (mStateTimer >= 30) {
                mState = OctLegState::cState_Dazed;
                mStateTimer = 0;
            }
            break;
        }

        case OctLegState::cState_Dazed: {
            // Uncurled vulnerability window (mPostShotChanceFrame: 180 frames)
            if (mStateTimer >= 60) {
                mState = OctLegState::cState_Idle;
                mStateTimer = 0;
            }
            break;
        }

        case OctLegState::cState_Die: {
            // Sinks and dissolves into 20.0m purple ink splat
            break;
        }

        default:
            break;
    }
}

void Enm_OctLeg::draw() {
    // Model rendered via ModelSceneMgr
}

} // namespace Game
