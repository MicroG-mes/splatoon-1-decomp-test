#include "Game/PlazaEnv/Obj_PlazaTrain.h"

namespace Game {

Obj_PlazaTrain::Obj_PlazaTrain()
    : mState(TrainState::cWaitingOffscreen),
      mCycleTimer(0),
      mCrossTimer(0),
      mPosition(-80.0f, 22.0f, -60.0f),
      mSpeed(0.45f),
      mTrackSplineProgress(0.0f) {
}

Obj_PlazaTrain::~Obj_PlazaTrain() {
}

void Obj_PlazaTrain::init() {
    GambitActor::init();
    mState = TrainState::cWaitingOffscreen;
    mCycleTimer = 0;
}

void Obj_PlazaTrain::triggerPass() {
    mState = TrainState::cApproaching;
    mTrackSplineProgress = 0.0f;
    mCrossTimer = 0;
}

void Obj_PlazaTrain::update() {
    mCycleTimer++;

    // Train passes automatically every 7200 frames (2 minutes)
    if (mState == TrainState::cWaitingOffscreen) {
        if (mCycleTimer >= 7200) {
            triggerPass();
            mCycleTimer = 0;
        }
    } else {
        mCrossTimer++;
        mTrackSplineProgress += (mSpeed * 0.003f);

        // Spline interpolation across plaza sky
        mPosition.x = -80.0f + mTrackSplineProgress * 160.0f;
        mPosition.z = -60.0f + mTrackSplineProgress * 120.0f;

        if (mTrackSplineProgress < 0.25f) {
            mState = TrainState::cApproaching;
        } else if (mTrackSplineProgress < 0.75f) {
            mState = TrainState::cCrossingPlaza;
        } else if (mTrackSplineProgress < 1.0f) {
            mState = TrainState::cDeparting;
        } else {
            mState = TrainState::cWaitingOffscreen;
            mTrackSplineProgress = 0.0f;
            mPosition = sead::Vector3f(-80.0f, 22.0f, -60.0f);
        }
    }
}

void Obj_PlazaTrain::draw() {
    if (mState != TrainState::cWaitingOffscreen) {
        GambitActor::draw();
    }
}

} // namespace Game
