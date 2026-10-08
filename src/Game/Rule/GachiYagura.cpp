#include "Game/Rule/GachiYagura.h"

namespace Game {

GachiYagura::GachiYagura()
    : mPosition(0.0f, 1.5f, 0.0f),
      mState(YaguraState::cNeutralCenter),
      mTrackProgress(0.0f),
      mSpeedMultiplier(1.0f),
      mAlphaDistance(cInitialDistance),
      mBravoDistance(cInitialDistance),
      mAlphaBestDistance(cInitialDistance),
      mBravoBestDistance(cInitialDistance),
      mAlphaRiders(0),
      mBravoRiders(0),
      mAbandonTimer(0) {
}

GachiYagura::~GachiYagura() {
}

void GachiYagura::init() {
    GambitActor::init();
    mPosition.set(0.0f, 1.5f, 0.0f);
    mState = YaguraState::cNeutralCenter;
    mTrackProgress = 0.0f;
    mSpeedMultiplier = 1.0f;
    mAlphaDistance = cInitialDistance;
    mBravoDistance = cInitialDistance;
    mAlphaBestDistance = cInitialDistance;
    mBravoBestDistance = cInitialDistance;
    mAlphaRiders = 0;
    mBravoRiders = 0;
    mAbandonTimer = 0;
}

void GachiYagura::updateRiders(u32 alphaRiderCount, u32 bravoRiderCount) {
    mAlphaRiders = alphaRiderCount;
    mBravoRiders = bravoRiderCount;

    if (alphaRiderCount > 0 && bravoRiderCount > 0) {
        mState = YaguraState::cContested;
        mAbandonTimer = 0;
    } else if (alphaRiderCount > 0) {
        mState = YaguraState::cAdvancingToBravo;
        mSpeedMultiplier = 1.0f + (static_cast<f32>(alphaRiderCount - 1) * 0.15f);
        mAbandonTimer = 0;
    } else if (bravoRiderCount > 0) {
        mState = YaguraState::cAdvancingToAlpha;
        mSpeedMultiplier = 1.0f + (static_cast<f32>(bravoRiderCount - 1) * 0.15f);
        mAbandonTimer = 0;
    } else {
        if (mTrackProgress != 0.0f) {
            mAbandonTimer++;
            if (mAbandonTimer >= cAbandonRetreatDelay) {
                mState = YaguraState::cRetreating;
            }
        } else {
            mState = YaguraState::cNeutralCenter;
        }
    }
}

void GachiYagura::stepMovement() {
    f32 step = cBaseSpeed * 0.02f * mSpeedMultiplier;

    switch (mState) {
        case YaguraState::cAdvancingToBravo:
            mTrackProgress += step;
            if (mTrackProgress > 1.0f) {
                mTrackProgress = 1.0f;
            }
            mAlphaDistance = static_cast<s32>((1.0f - mTrackProgress) * static_cast<f32>(cInitialDistance));
            if (mAlphaDistance < mAlphaBestDistance) {
                mAlphaBestDistance = mAlphaDistance;
            }
            break;

        case YaguraState::cAdvancingToAlpha:
            mTrackProgress -= step;
            if (mTrackProgress < -1.0f) {
                mTrackProgress = -1.0f;
            }
            mBravoDistance = static_cast<s32>((1.0f + mTrackProgress) * static_cast<f32>(cInitialDistance));
            if (mBravoDistance < mBravoBestDistance) {
                mBravoBestDistance = mBravoDistance;
            }
            break;

        case YaguraState::cRetreating:
            // Slowly glide back towards neutral center point (0.0)
            if (mTrackProgress > 0.0f) {
                mTrackProgress -= cBaseSpeed * 0.015f;
                if (mTrackProgress <= 0.0f) {
                    mTrackProgress = 0.0f;
                    mState = YaguraState::cNeutralCenter;
                }
            } else if (mTrackProgress < 0.0f) {
                mTrackProgress += cBaseSpeed * 0.015f;
                if (mTrackProgress >= 0.0f) {
                    mTrackProgress = 0.0f;
                    mState = YaguraState::cNeutralCenter;
                }
            }
            break;

        case YaguraState::cContested:
        case YaguraState::cNeutralCenter:
        default:
            break;
    }

    // Update physical coordinates along stage track spline (Z axis simplification)
    mPosition.z = mTrackProgress * 50.0f;
}

void GachiYagura::update() {
    if (!isKnockout()) {
        stepMovement();
    }
}

void GachiYagura::draw() {
    GambitActor::draw();
}

} // namespace Game
