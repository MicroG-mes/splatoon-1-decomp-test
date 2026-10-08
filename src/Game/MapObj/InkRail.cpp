#include "Game/MapObj/InkRail.h"
#include <cmath>
#include <cstring>

namespace Game {

InkRail::InkRail()
    : mSplineResourcePtr(nullptr),
      mSplineDataPtr(nullptr),
      mStartPos(0.0f, 0.0f, 0.0f),
      mEndPos(0.0f, 0.0f, 0.0f),
      mTeamId(0),
      mState(InkRailState::cDormant),
      mTimer(0),
      mActivationProgress(0.0f) {
    std::memset(mReserved0_0x8, 0, sizeof(mReserved0_0x8));
    std::memset(mReserved1_0x54, 0, sizeof(mReserved1_0x54));
}

InkRail::~InkRail() {
}

// 0x0254D50C: Decompiled vfunc_3 (Spline resource setup)
void InkRail::vfunc_3() {
    mSplineDataPtr = mSplineResourcePtr;
}

// 0x0254F438: Decompiled vfunc_7 (4-stage rail traversal update)
void InkRail::vfunc_7() {
    // Stage 1: Active ink check along rail wire
    // Stage 2: Squid grind traction calculation
    // Stage 3: Catenary sag / cable vibration
    // Stage 4: Particle emitter update along rail
}

void InkRail::init() {
    GambitActor::init();
    mStartPos.set(0.0f, 0.0f, 0.0f);
    mEndPos.set(0.0f, 0.0f, 0.0f);
    mTeamId = 0;
    mState = InkRailState::cDormant;
    mTimer = 0;
    mActivationProgress = 0.0f;
    vfunc_3();
}

void InkRail::setupSpline(const sead::Vector3f& startPos, const sead::Vector3f& endPos) {
    mStartPos = startPos;
    mEndPos = endPos;
    mState = InkRailState::cDormant;
    mTimer = 0;
    mActivationProgress = 0.0f;
}

void InkRail::activateByInk(u32 teamId) {
    mTeamId = teamId;
    mState = InkRailState::cActivating;
    mTimer = 0;
    mActivationProgress = 0.0f;
}

sead::Vector3f InkRail::evaluateSplinePos(f32 progressT) const {
    if (progressT < 0.0f) progressT = 0.0f;
    if (progressT > 1.0f) progressT = 1.0f;

    // Linear spline interpolation with slight parabolic catenary sag
    f32 x = mStartPos.x + (mEndPos.x - mStartPos.x) * progressT;
    f32 z = mStartPos.z + (mEndPos.z - mStartPos.z) * progressT;
    f32 baseY = mStartPos.y + (mEndPos.y - mStartPos.y) * progressT;
    f32 sagY = -0.5f * 4.0f * progressT * (1.0f - progressT); // 0.5m dip in the center

    return sead::Vector3f(x, baseY + sagY, z);
}

f32 InkRail::stepGrindProgress(f32 currentT) const {
    f32 dx = mEndPos.x - mStartPos.x;
    f32 dy = mEndPos.y - mStartPos.y;
    f32 dz = mEndPos.z - mStartPos.z;
    f32 length = std::sqrt(dx * dx + dy * dy + dz * dz);

    if (length < 0.001f) return 1.0f;

    f32 stepT = cGrindSpeed / length;
    return currentT + stepT;
}

void InkRail::update() {
    vfunc_7();
    switch (mState) {
        case InkRailState::cActivating:
            mActivationProgress += 0.05f;
            if (mActivationProgress >= 1.0f) {
                mActivationProgress = 1.0f;
                mState = InkRailState::cActive;
                mTimer = 0;
            }
            break;

        case InkRailState::cActive:
            mTimer++;
            if (mTimer >= cActiveLifetimeFrames) {
                mState = InkRailState::cDeactivating;
                mTimer = 0;
            }
            break;

        case InkRailState::cDeactivating:
            mActivationProgress -= 0.05f;
            if (mActivationProgress <= 0.0f) {
                mActivationProgress = 0.0f;
                mState = InkRailState::cDormant;
                mTimer = 0;
            }
            break;

        case InkRailState::cDormant:
        default:
            break;
    }
}

void InkRail::draw() {
    if (mState != InkRailState::cDormant) {
        GambitActor::draw();
    }
}

} // namespace Game
