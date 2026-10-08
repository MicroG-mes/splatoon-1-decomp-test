#include "Game/MapObj/Obj_Sponge.h"

namespace Game {

Obj_Sponge::Obj_Sponge()
    : mPosition(0.0f, 0.0f, 0.0f),
      mCurrentScale(cMinScale),
      mTargetScale(cMinScale),
      mOwnerTeam(-1),
      mState(SpongeState::cIdle) {
}

Obj_Sponge::~Obj_Sponge() {
}

void Obj_Sponge::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mCurrentScale = cMinScale;
    mTargetScale = cMinScale;
    mOwnerTeam = -1;
    mState = SpongeState::cIdle;
}

void Obj_Sponge::hitByInk(s32 inkTeam, f32 inkVolume) {
    if (mOwnerTeam == -1) {
        // Unclaimed sponge absorbs ink and takes on team color
        mOwnerTeam = inkTeam;
        mTargetScale += cGrowthStep * (inkVolume > 0.0f ? inkVolume : 1.0f);
        if (mTargetScale > cMaxScale) {
            mTargetScale = cMaxScale;
        }
        mState = SpongeState::cExpanding;
    } else if (mOwnerTeam == inkTeam) {
        // Friendly ink expands sponge up to max scale
        mTargetScale += cGrowthStep * (inkVolume > 0.0f ? inkVolume : 1.0f);
        if (mTargetScale > cMaxScale) {
            mTargetScale = cMaxScale;
        }
        mState = SpongeState::cExpanding;
    } else {
        // Enemy ink shrinks sponge down to min scale
        mTargetScale -= cShrinkStep * (inkVolume > 0.0f ? inkVolume : 1.0f);
        if (mTargetScale <= cMinScale) {
            mTargetScale = cMinScale;
            mOwnerTeam = -1; // Reset to neutral when completely deflated
        }
        mState = SpongeState::cShrinking;
    }
}

void Obj_Sponge::update() {
    // Smooth interpolation towards target scale
    f32 diff = mTargetScale - mCurrentScale;
    if (diff > 0.005f) {
        mCurrentScale += diff * 0.15f;
    } else if (diff < -0.005f) {
        mCurrentScale += diff * 0.15f;
    } else {
        mCurrentScale = mTargetScale;
        mState = SpongeState::cIdle;
    }
}

void Obj_Sponge::draw() {
    GambitActor::draw();
}

} // namespace Game
