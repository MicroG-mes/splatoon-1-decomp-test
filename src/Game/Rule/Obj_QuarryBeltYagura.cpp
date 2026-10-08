#include "Game/Rule/Obj_QuarryBeltYagura.h"
#include <cstring>
#include <cmath>

namespace Game {

Obj_QuarryBeltYagura::Obj_QuarryBeltYagura()
    : mState(TowerControlState::cNeutral),
      mStateTimer(0),
      mRailProgress(0.0f),
      mBestScoreAlpha(100.0f),
      mBestScoreBravo(100.0f),
      mRidersAlpha(0),
      mRidersBravo(0),
      mIdleFrames(0),
      mRailTangent(0.0f, 0.0f, 1.0f),
      mSplineNode0(0.0f, 0.0f, 0.0f),
      mSplineNode1(0.0f, 0.0f, 0.0f),
      mSplineNode2(0.0f, 0.0f, 0.0f),
      mVelocity(0.0f, 0.0f, 0.0f),
      mTargetSpeed(0.0f),
      mSmoothing(1.0f),
      mSoundHandle(0) {
    std::memset(mSplineContext, 0, sizeof(mSplineContext));
    std::memset(mPaddingNodes, 0, sizeof(mPaddingNodes));
    std::memset(mPaddingVel, 0, sizeof(mPaddingVel));
}

Obj_QuarryBeltYagura::~Obj_QuarryBeltYagura() {
}

void Obj_QuarryBeltYagura::init() {
    GambitActor::init();
    mState = TowerControlState::cNeutral;
    mRailProgress = 0.0f;
    mBestScoreAlpha = 100.0f;
    mBestScoreBravo = 100.0f;
    mIdleFrames = 0;
    mTargetSpeed = 0.0f;
    mVelocity.set(0.0f, 0.0f, 0.0f);
}

void Obj_QuarryBeltYagura::updateRiders(u32 alphaRiderCount, u32 bravoRiderCount) {
    mRidersAlpha = alphaRiderCount;
    mRidersBravo = bravoRiderCount;

    if (mRidersAlpha > 0 && mRidersBravo > 0) {
        mState = TowerControlState::cContested;
        mTargetSpeed = 0.0f;
        mIdleFrames = 0;
    } else if (mRidersAlpha > 0) {
        mState = TowerControlState::cMovingAlpha;
        mTargetSpeed = 0.0015f * (1.0f + (mRidersAlpha - 1) * 0.15f);
        mIdleFrames = 0;
    } else if (mRidersBravo > 0) {
        mState = TowerControlState::cMovingBravo;
        mTargetSpeed = -0.0015f * (1.0f + (mRidersBravo - 1) * 0.15f);
        mIdleFrames = 0;
    } else {
        mState = TowerControlState::cNeutral;
        mTargetSpeed = 0.0f;
    }
}

/**
 * FUN_0243f3bc - Espresso PPC velocity projection along spline rail
 * lfs f13, 0xfb4(r3)  (mTargetSpeed)
 * lfs f0,  0xf20(r3)  (mRailTangent.x)
 * lfs f8,  0xf28(r3)  (mRailTangent.z)
 * fmuls f12, f13, f0
 * stores into 0xfac (mVelocity)
 */
void Obj_QuarryBeltYagura::stepMovementPPC() {
    mVelocity.x = mTargetSpeed * mRailTangent.x;
    mVelocity.z = mTargetSpeed * mRailTangent.z;
    mVelocity.y = 0.0f;
}

/**
 * Obj_QuarryBeltYagura__vfunc_6 @ 0x0243f558
 * Evaluates spline nodes, updates velocities, paints central pillar and sound handles.
 */
void Obj_QuarryBeltYagura::vfunc_6() {
    stepMovementPPC();

    // Advance rail progress
    mRailProgress += mTargetSpeed;

    if (mState == TowerControlState::cMovingAlpha) {
        f32 score = (1.0f - mRailProgress) * 100.0f;
        if (score < mBestScoreAlpha) {
            mBestScoreAlpha = score;
        }
        if (mRailProgress >= 1.0f) {
            mRailProgress = 1.0f;
            mBestScoreAlpha = 0.0f;
            mState = TowerControlState::cKnockoutAlpha;
        }
    } else if (mState == TowerControlState::cMovingBravo) {
        f32 score = (1.0f + mRailProgress) * 100.0f;
        if (score < mBestScoreBravo) {
            mBestScoreBravo = score;
        }
        if (mRailProgress <= -1.0f) {
            mRailProgress = -1.0f;
            mBestScoreBravo = 0.0f;
            mState = TowerControlState::cKnockoutBravo;
        }
    } else if (mState == TowerControlState::cNeutral) {
        mIdleFrames++;
        if (mIdleFrames > 300) {
            const f32 retreatSpeed = 0.00075f;
            if (mRailProgress > 0.0f) {
                mRailProgress -= retreatSpeed;
                if (mRailProgress < 0.0f) mRailProgress = 0.0f;
            } else if (mRailProgress < 0.0f) {
                mRailProgress += retreatSpeed;
                if (mRailProgress > 0.0f) mRailProgress = 0.0f;
            }
        }
    }

    // Sync spline nodes
    mSplineNode1 = mVelocity;
}

void Obj_QuarryBeltYagura::update() {
    mStateTimer++;
    vfunc_6();
}

void Obj_QuarryBeltYagura::draw() {
    GambitActor::draw();
}

} // namespace Game
