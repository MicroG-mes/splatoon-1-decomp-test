#include "Game/Rule/GachiArea.h"

namespace Game {

GachiArea::GachiArea()
    : mMinBounds(0.0f, 0.0f, 0.0f),
      mMaxBounds(0.0f, 0.0f, 0.0f),
      mControlState(ZoneControlState::cNeutral),
      mAlphaCoverage(0.0f),
      mBravoCoverage(0.0f),
      mAlphaCounter(cInitialCounter),
      mBravoCounter(cInitialCounter),
      mAlphaPenalty(0),
      mBravoPenalty(0),
      mTimerTick(0),
      mIsOvertime(false) {
}

GachiArea::~GachiArea() {
}

void GachiArea::init() {
    GambitActor::init();
    mControlState = ZoneControlState::cNeutral;
    mAlphaCoverage = 0.0f;
    mBravoCoverage = 0.0f;
    mAlphaCounter = cInitialCounter;
    mBravoCounter = cInitialCounter;
    mAlphaPenalty = 0;
    mBravoPenalty = 0;
    mTimerTick = 0;
    mIsOvertime = false;
}

void GachiArea::setZoneBounds(const sead::Vector3f& minBounds, const sead::Vector3f& maxBounds) {
    mMinBounds = minBounds;
    mMaxBounds = maxBounds;
}

void GachiArea::updatePaintCoverage(f32 alphaPercent, f32 bravoPercent) {
    mAlphaCoverage = alphaPercent;
    mBravoCoverage = bravoPercent;

    ZoneControlState oldState = mControlState;

    if (alphaPercent >= cCaptureThreshold) {
        mControlState = ZoneControlState::cControlledP1;
    } else if (bravoPercent >= cCaptureThreshold) {
        mControlState = ZoneControlState::cControlledP2;
    } else if (alphaPercent > 0.4f && bravoPercent > 0.4f) {
        mControlState = ZoneControlState::cContested;
    } else {
        mControlState = ZoneControlState::cNeutral;
    }

    // Apply penalty counter when a controlling team loses the zone
    if (oldState == ZoneControlState::cControlledP1 && mControlState != ZoneControlState::cControlledP1) {
        mAlphaPenalty = (cInitialCounter - mAlphaCounter) / 2;
    } else if (oldState == ZoneControlState::cControlledP2 && mControlState != ZoneControlState::cControlledP2) {
        mBravoPenalty = (cInitialCounter - mBravoCounter) / 2;
    }
}

void GachiArea::decrementTimer() {
    mTimerTick++;
    if (mTimerTick >= 60) { // Every 1 second (60 frames) = 1 point tick
        mTimerTick = 0;

        if (mControlState == ZoneControlState::cControlledP1) {
            if (mAlphaPenalty > 0) {
                mAlphaPenalty--;
            } else if (mAlphaCounter > 0) {
                mAlphaCounter--;
            }
        } else if (mControlState == ZoneControlState::cControlledP2) {
            if (mBravoPenalty > 0) {
                mBravoPenalty--;
            } else if (mBravoCounter > 0) {
                mBravoCounter--;
            }
        }
    }
}

void GachiArea::update() {
    if (!isGameOver()) {
        decrementTimer();
    }
}

void GachiArea::draw() {
    GambitActor::draw();
}

} // namespace Game
