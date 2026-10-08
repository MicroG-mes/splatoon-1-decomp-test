#include "Game/Paint/IrregularWallSplash.h"
#include <cmath>

namespace Game {

IrregularWallSplash::IrregularWallSplash()
    : mHitPos(0.0f, 0.0f, 0.0f),
      mNormal(0.0f, 1.0f, 0.0f),
      mRadius(1.0f),
      mDripLength(0.0f),
      mTeamId(0),
      mIsComplete(false),
      mTimer(0) {
}

IrregularWallSplash::~IrregularWallSplash() {
}

void IrregularWallSplash::init() {
    GambitActor::init();
    mHitPos.set(0.0f, 0.0f, 0.0f);
    mNormal.set(0.0f, 1.0f, 0.0f);
    mRadius = 1.0f;
    mDripLength = 0.0f;
    mTeamId = 0;
    mIsComplete = false;
    mTimer = 0;
}

void IrregularWallSplash::applySplash(const sead::Vector3f& hitPos, const sead::Vector3f& normal, f32 radius, u32 teamId) {
    mHitPos = hitPos;
    mNormal = normal;
    mRadius = radius;
    mTeamId = teamId;
    mDripLength = 0.0f;
    mIsComplete = false;
    mTimer = 0;
}

void IrregularWallSplash::update() {
    if (mIsComplete) {
        return;
    }

    mTimer++;

    // Only surfaces that are sufficiently vertical (normal.y near 0) develop gravity drip deformation
    f32 verticalSlope = 1.0f - std::abs(mNormal.y);
    if (verticalSlope > 0.4f) {
        mDripLength += cDripSpeed * verticalSlope;
        if (mDripLength >= cMaxDripLength) {
            mDripLength = cMaxDripLength;
            mIsComplete = true;
        }
    } else {
        mIsComplete = true;
    }
}

void IrregularWallSplash::draw() {
    // Dynamic projector decal rendering handled by PaintTextureMgr
}

} // namespace Game
