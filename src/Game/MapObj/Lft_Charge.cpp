#include "Game/MapObj/Lft_Charge.h"
#include <cmath>
#include <algorithm>

namespace Game {

Lft_Charge::Lft_Charge()
    : mBasePosition(0.0f, 0.0f, 0.0f)
    , mCurrentHeight(0.0f)
    , mTargetElevation(cMaxElevationHeight)
    , mState(ChargeLiftState::cBottom)
    , mDwellTimer(0)
{
    std::fill(mReserved, mReserved + sizeof(mReserved), 0);
}

Lft_Charge::~Lft_Charge() {
}

void Lft_Charge::init() {
    GambitActor::init();
    mBasePosition.set(0.0f, 0.0f, 0.0f);
    mCurrentHeight = 0.0f;
    mTargetElevation = cMaxElevationHeight;
    mState = ChargeLiftState::cBottom;
    mDwellTimer = 0;
}

void Lft_Charge::vfunc_3() {
    // 0x100cbcf4: Model loading (Lft_Charge.szs, 3,064 vertices)
}

void Lft_Charge::vfunc_5() {
    // Parameter loading
    mCurrentHeight = 0.0f;
    mState = ChargeLiftState::cBottom;
    mDwellTimer = 0;
}

void Lft_Charge::vfunc_7() {
    // Hydraulic motion tick
    update();
}

void Lft_Charge::vfunc_11() {
    // Trigger elevate / lower
    triggerAscend();
}

void Lft_Charge::spawn(const sead::Vector3f& basePos, f32 maxElevation) {
    mBasePosition = basePos;
    mCurrentHeight = 0.0f;
    mTargetElevation = (maxElevation > 0.0f) ? maxElevation : cMaxElevationHeight;
    mState = ChargeLiftState::cBottom;
    mDwellTimer = 0;
}

void Lft_Charge::triggerAscend() {
    if (mState == ChargeLiftState::cBottom || mState == ChargeLiftState::cDescending) {
        mState = ChargeLiftState::cAscending;
    }
}

void Lft_Charge::triggerDescend() {
    if (mState == ChargeLiftState::cTop || mState == ChargeLiftState::cAscending) {
        mState = ChargeLiftState::cDescending;
    }
}

sead::Vector3f Lft_Charge::getCurrentWorldPosition() const {
    return sead::Vector3f(mBasePosition.x, mBasePosition.y + mCurrentHeight, mBasePosition.z);
}

void Lft_Charge::update() {
    switch (mState) {
        case ChargeLiftState::cBottom:
            break;

        case ChargeLiftState::cAscending:
            mCurrentHeight += cAscentSpeed;
            if (mCurrentHeight >= mTargetElevation) {
                mCurrentHeight = mTargetElevation;
                mState = ChargeLiftState::cTop;
                mDwellTimer = cTopDwellDuration;
            }
            break;

        case ChargeLiftState::cTop:
            if (--mDwellTimer <= 0) {
                mState = ChargeLiftState::cDescending;
            }
            break;

        case ChargeLiftState::cDescending:
            mCurrentHeight -= cDescentSpeed;
            if (mCurrentHeight <= 0.0f) {
                mCurrentHeight = 0.0f;
                mState = ChargeLiftState::cBottom;
                mDwellTimer = 0;
            }
            break;
    }
}

void Lft_Charge::draw() {
    // Rendered via ModelSceneMgr
}

} // namespace Game
