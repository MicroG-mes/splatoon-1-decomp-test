#include "Game/MapObj/Obj_SwitchPaint.h"
#include <algorithm>

namespace Game {

Obj_SwitchPaint::Obj_SwitchPaint()
    : mPosition(0.0f, 0.0f, 0.0f),
      mState(SwitchPaintState::cOff),
      mAccumulatedInk(0.0f),
      mActivationThreshold(cDefaultActivationThreshold),
      mAutoResetFrames(0),
      mResetTimer(0),
      mOwningTeam(0),
      mSignalSent(false),
      mIsVsMode(false) {
}

Obj_SwitchPaint::~Obj_SwitchPaint() {
}

void Obj_SwitchPaint::init() {
    GambitActor::init();
    mState = SwitchPaintState::cOff;
    mAccumulatedInk = 0.0f;
    mResetTimer = 0;
    mSignalSent = false;
    mOwningTeam = 0;
}

void Obj_SwitchPaint::vfunc_3() {
    // Model & resource load (Obj_SwitchPaint.szs)
}

void Obj_SwitchPaint::vfunc_5() {
    // Initialization & parameter link setup
}

void Obj_SwitchPaint::vfunc_7() {
    // Main update tick
}

void Obj_SwitchPaint::vfunc_11() {
    // Ink paint sensor update
}

void Obj_SwitchPaint::vfunc_47() {
    // Light & animation sync
}

void Obj_SwitchPaint::vfunc_55() {
    // Signal broadcast event to connected lifts and doors
    mSignalSent = true;
}

bool Obj_SwitchPaint::paintInk(f32 inkAmount, u32 teamId) {
    if (mState == SwitchPaintState::cOn) {
        if (!mIsVsMode) {
            if (mAutoResetFrames > 0) {
                mResetTimer = mAutoResetFrames; // Refresh active timer
            }
            return false;
        }

        // VS mode: opponent ink neutralizes switch
        if (teamId != mOwningTeam) {
            mAccumulatedInk -= inkAmount;
            if (mAccumulatedInk <= 0.0f) {
                reset();
                return true;
            }
        } else if (mAutoResetFrames > 0) {
            mResetTimer = mAutoResetFrames;
        }
        return false;
    }

    // Switch is currently cOff
    mOwningTeam = teamId;
    mAccumulatedInk += inkAmount;
    if (mAccumulatedInk >= mActivationThreshold) {
        mState = SwitchPaintState::cOn;
        mSignalSent = true;
        vfunc_55();
        if (mAutoResetFrames > 0) {
            mResetTimer = mAutoResetFrames;
        }
        return true;
    }

    return false;
}

void Obj_SwitchPaint::reset() {
    mState = SwitchPaintState::cOff;
    mAccumulatedInk = 0.0f;
    mResetTimer = 0;
    mSignalSent = false;
}

void Obj_SwitchPaint::update() {
    vfunc_7();

    if (mState == SwitchPaintState::cOn && mAutoResetFrames > 0) {
        if (mResetTimer > 0) {
            mResetTimer--;
            if (mResetTimer <= 0) {
                reset();
            }
        }
    }
}

void Obj_SwitchPaint::draw() {
    GambitActor::draw();
}

} // namespace Game
