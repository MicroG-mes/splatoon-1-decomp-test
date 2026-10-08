#pragma once

#include "types.h"

namespace Game {

class VSGameTime {
public:
    VSGameTime();
    virtual ~VSGameTime();

    void init(u32 durationSeconds);
    void update();

    void triggerOvertime();
    void endOvertime();

    bool isTimeUp() const { return mRemainingFrames <= 0 && !mIsOvertime; }
    bool isOvertime() const { return mIsOvertime; }
    bool isLastOneMinute() const { return mRemainingFrames <= 60 * 60; }

    s32 getRemainingSeconds() const { return mRemainingFrames > 0 ? (mRemainingFrames / 60) : 0; }
    s32 getRemainingFrames() const { return mRemainingFrames; }
    u32 getElapsedTimeFrames() const { return mElapsedFrames; }

protected:
    s32 mTotalFrames;
    s32 mRemainingFrames;
    u32 mElapsedFrames;
    bool mIsOvertime;
    bool mIsPaused;

    undefined mReserved[0x20];
};

} // namespace Game
