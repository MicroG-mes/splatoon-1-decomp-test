#include "Game/MapObj/Obj_PaintLiftSlide.h"
#include <cmath>
#include <algorithm>

namespace Game {

Obj_PaintLiftSlide::Obj_PaintLiftSlide()
    : mPosition(0.0f, 0.0f, 0.0f)
    , mRailStart(0.0f, 0.0f, 0.0f)
    , mRailEnd(0.0f, 0.0f, 0.0f)
    , mState(SlideLiftState::cState_Moving)
    , mProgress(0.0f)
    , mDirection(1.0f)
    , mSpeed(cDefaultSpeed)
    , mEndWaitTimer(0)
{
    std::fill(mReserved, mReserved + sizeof(mReserved), 0);
}

Obj_PaintLiftSlide::~Obj_PaintLiftSlide() {
}

void Obj_PaintLiftSlide::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mRailStart.set(0.0f, 0.0f, 0.0f);
    mRailEnd.set(0.0f, 0.0f, 0.0f);
    mState = SlideLiftState::cState_Moving;
    mProgress = 0.0f;
    mDirection = 1.0f;
    mSpeed = cDefaultSpeed;
    mEndWaitTimer = 0;
}

void Obj_PaintLiftSlide::vfunc_3() {
    // 0x02591e2c: Model loading (Lft_WireNettingPlate00.szs, 1,552 vertices)
}

void Obj_PaintLiftSlide::vfunc_5() {
    // 0x02591e84: Waypoint interpolation initialization
}

void Obj_PaintLiftSlide::vfunc_7() {
    // 0x025927c0: Translation update along rail spline
    update();
}

void Obj_PaintLiftSlide::vfunc_11() {
    // 0x025932ac: Terminal reversal
    mDirection = -mDirection;
    mState = SlideLiftState::cState_Moving;
}

void Obj_PaintLiftSlide::setRailPoints(const sead::Vector3f& start, const sead::Vector3f& end) {
    mRailStart = start;
    mRailEnd = end;
    mPosition = start;
    mProgress = 0.0f;
    mDirection = 1.0f;
}

bool Obj_PaintLiftSlide::checkPassenger(const sead::Vector3f& playerPos, bool isSquid) const {
    if (isSquid) {
        // Squid form slips straight through the wire grating gaps!
        return false;
    }

    // Humanoid form can stand on the solid wire netting mesh
    f32 dx = std::abs(playerPos.x - mPosition.x);
    f32 dz = std::abs(playerPos.z - mPosition.z);
    f32 dy = playerPos.y - mPosition.y;

    return (dx <= cPlateWidth * 0.5f) &&
           (dz <= cPlateLength * 0.5f) &&
           (dy >= 0.0f && dy <= 1.5f);
}

void Obj_PaintLiftSlide::update() {
    if (mState == SlideLiftState::cState_Moving) {
        mProgress += mDirection * mSpeed;

        if (mProgress >= 1.0f) {
            mProgress = 1.0f;
            mState = SlideLiftState::cState_WaitEnd;
            mEndWaitTimer = 20; // 20 frames pause at rail end
        } else if (mProgress <= 0.0f) {
            mProgress = 0.0f;
            mState = SlideLiftState::cState_WaitEnd;
            mEndWaitTimer = 20;
        }

        // Interpolate linear coordinates
        mPosition.x = mRailStart.x + (mRailEnd.x - mRailStart.x) * mProgress;
        mPosition.y = mRailStart.y + (mRailEnd.y - mRailStart.y) * mProgress;
        mPosition.z = mRailStart.z + (mRailEnd.z - mRailStart.z) * mProgress;

    } else if (mState == SlideLiftState::cState_WaitEnd) {
        if (--mEndWaitTimer <= 0) {
            vfunc_11(); // Flip direction and resume transit
        }
    }
}

void Obj_PaintLiftSlide::draw() {
    // Model rendered via ModelSceneMgr
}

} // namespace Game
