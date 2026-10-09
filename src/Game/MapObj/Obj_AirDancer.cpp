#include "Game/MapObj/Obj_AirDancer.h"
#include <cmath>
#include <algorithm>

namespace Game {

Obj_AirDancer::Obj_AirDancer()
    : mState(AirDancerState::cWaving),
      mBlowerActive(true),
      mInflationRatio(1.0f),
      mOscillationPhase(0.0f),
      mWobbleAngle(0.0f),
      mBlowerWindSpeed(12.5f),
      mTipOffset(0.0f, 6.5f, 0.0f),
      mHitRecoverTimer(0) {
}

Obj_AirDancer::~Obj_AirDancer() {
}

void Obj_AirDancer::init() {
    GambitActor::init();
    vfunc_3();
    vfunc_5();
}

void Obj_AirDancer::vfunc_3() {
    mModel = sead::BfresParser::createAirDancerModel("Obj_AirDancer");
}

void Obj_AirDancer::vfunc_5() {
    mState = mBlowerActive ? AirDancerState::cWaving : AirDancerState::cDeflated;
    mInflationRatio = mBlowerActive ? 1.0f : 0.0f;
    mOscillationPhase = 0.0f;
    mWobbleAngle = 0.0f;
    mTipOffset.set(0.0f, mInflationRatio * 6.5f, 0.0f);
    mHitRecoverTimer = 0;
}

void Obj_AirDancer::vfunc_7() {
    const f32 kDt = 1.0f / 60.0f;
    const f32 kWaveFreq = 2.4f; // rad/s pneumatic flutter oscillation

    if (mHitRecoverTimer > 0) {
        mHitRecoverTimer--;
        if (mHitRecoverTimer == 0 && mBlowerActive) {
            mState = AirDancerState::cBillowing;
        }
    }

    if (mBlowerActive) {
        if (mInflationRatio < 1.0f) {
            mInflationRatio = std::min(1.0f, mInflationRatio + 0.025f);
            if (mInflationRatio >= 1.0f) {
                mState = AirDancerState::cWaving;
            }
        }
    } else {
        mInflationRatio = std::max(0.0f, mInflationRatio - 0.04f);
        if (mInflationRatio <= 0.0f) {
            mState = AirDancerState::cDeflated;
        }
    }

    if (mState == AirDancerState::cWaving) {
        mOscillationPhase += kWaveFreq * kDt;
        if (mOscillationPhase > 6.2831853f) {
            mOscillationPhase -= 6.2831853f;
        }

        // Dual harmonic pneumatic sway
        f32 primarySway = std::sin(mOscillationPhase) * 25.0f;
        f32 secondaryFlutter = std::sin(mOscillationPhase * 2.3f) * 10.0f;
        mWobbleAngle = (primarySway + secondaryFlutter) * mInflationRatio;

        f32 rad = mWobbleAngle * 0.017453292f;
        f32 height = 6.5f * mInflationRatio * std::cos(rad);
        f32 lateral = 6.5f * mInflationRatio * std::sin(rad);
        mTipOffset.set(lateral, height, lateral * 0.35f);
    } else if (mState == AirDancerState::cDeflated) {
        mWobbleAngle = 0.0f;
        mTipOffset.set(0.0f, 0.2f, 0.0f);
    } else if (mState == AirDancerState::cLimpHit) {
        mWobbleAngle = 35.0f * mInflationRatio;
        mTipOffset.set(2.5f * mInflationRatio, 3.0f * mInflationRatio, 0.0f);
    }
}

void Obj_AirDancer::update() {
    vfunc_7();
}

void Obj_AirDancer::draw() {
}

void Obj_AirDancer::setBlowerActive(bool active) {
    mBlowerActive = active;
    if (!active) {
        mState = AirDancerState::cDeflated;
    } else if (mState == AirDancerState::cDeflated) {
        mState = AirDancerState::cStarting;
    }
}

void Obj_AirDancer::applyImpact(const sead::Vector3f& force) {
    mState = AirDancerState::cLimpHit;
    mHitRecoverTimer = 45; // 45 frames recovery
    mInflationRatio = std::max(0.3f, mInflationRatio - 0.4f);
}

} // namespace Game
