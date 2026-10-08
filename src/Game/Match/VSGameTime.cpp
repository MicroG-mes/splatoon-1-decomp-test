#include "Game/Match/VSGameTime.h"

namespace Game {

VSGameTime::VSGameTime()
    : mTotalFrames(180 * 60),
      mRemainingFrames(180 * 60),
      mElapsedFrames(0),
      mIsOvertime(false),
      mIsPaused(false) {
}

VSGameTime::~VSGameTime() {
}

void VSGameTime::init(u32 durationSeconds) {
    mTotalFrames = static_cast<s32>(durationSeconds * 60);
    mRemainingFrames = mTotalFrames;
    mElapsedFrames = 0;
    mIsOvertime = false;
    mIsPaused = false;
}

void VSGameTime::triggerOvertime() {
    mIsOvertime = true;
}

void VSGameTime::endOvertime() {
    mIsOvertime = false;
    mRemainingFrames = 0;
}

void VSGameTime::update() {
    if (mIsPaused) {
        return;
    }

    mElapsedFrames++;

    if (mRemainingFrames > 0) {
        mRemainingFrames--;
    }
}

} // namespace Game
