#include "Game/Weapon/Obj_Barrier.h"

namespace Game {

Obj_Barrier::Obj_Barrier()
    : mState(BarrierState::cInactive),
      mTotalDurationFrames(270),
      mRemainingFrames(0),
      mKnockbackVelocity(0.0f, 0.0f, 0.0f),
      mBubblePulsePhase(0.0f) {
}

Obj_Barrier::~Obj_Barrier() {
}

void Obj_Barrier::init() {
    GambitActor::init();
    mState = BarrierState::cInactive;
    mRemainingFrames = 0;
}

void Obj_Barrier::activate(f32 durationSeconds) {
    mTotalDurationFrames = static_cast<s32>(durationSeconds * 60.0f);
    mRemainingFrames = mTotalDurationFrames;
    mKnockbackVelocity = sead::Vector3f(0.0f, 0.0f, 0.0f);
    mBubblePulsePhase = 0.0f;
    mState = BarrierState::cActive;
}

void Obj_Barrier::applyKnockback(const sead::Vector3f& knockbackForce) {
    if (mState == BarrierState::cActive) {
        mKnockbackVelocity.x += knockbackForce.x;
        mKnockbackVelocity.y += knockbackForce.y;
        mKnockbackVelocity.z += knockbackForce.z;
    }
}

bool Obj_Barrier::checkTeammateShare(const sead::Vector3f& playerPos, const sead::Vector3f& teammatePos, f32 shareRadius) {
    if (mState != BarrierState::cActive) {
        return false;
    }

    f32 dx = playerPos.x - teammatePos.x;
    f32 dy = playerPos.y - teammatePos.y;
    f32 dz = playerPos.z - teammatePos.z;
    f32 distSq = dx * dx + dy * dy + dz * dz;

    return (distSq <= shareRadius * shareRadius);
}

f32 Obj_Barrier::getRemainingTimeRatio() const {
    if (mTotalDurationFrames == 0) return 0.0f;
    return static_cast<f32>(mRemainingFrames) / static_cast<f32>(mTotalDurationFrames);
}

void Obj_Barrier::update() {
    if (mState == BarrierState::cActive) {
        mBubblePulsePhase += 0.1f;

        // Dampen knockback velocity
        mKnockbackVelocity.x *= 0.85f;
        mKnockbackVelocity.y *= 0.85f;
        mKnockbackVelocity.z *= 0.85f;

        if (mRemainingFrames > 0) {
            mRemainingFrames--;
            if (mRemainingFrames <= 30) {
                mState = BarrierState::cFading;
            }
        }
    } else if (mState == BarrierState::cFading) {
        if (mRemainingFrames > 0) {
            mRemainingFrames--;
        } else {
            mState = BarrierState::cInactive;
        }
    }
}

void Obj_Barrier::draw() {
    GambitActor::draw();
}

} // namespace Game
