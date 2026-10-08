#include "Game/MapObj/Obj_Sponge.h"
#include "Game/System/AglParameter.h"
#include <cmath>
#include <algorithm>

namespace Game {

bool SpongeParams::load(const char* paramsPath) {
    scaleDamageForMax = 2.40000010f;
    scaleBombCoreDamageK = 2.00000000f;
    paintingLiftDamage = 1.00000000f;
    enemyNoReactFrame = 12;

    if (paramsPath) {
        AglParameterObj obj;
        if (obj.loadFromFile(paramsPath)) {
            scaleDamageForMax = obj.getFloat("mScaleDamageForMax", scaleDamageForMax);
            scaleBombCoreDamageK = obj.getFloat("mScaleBombCoreDamageK", scaleBombCoreDamageK);
            paintingLiftDamage = obj.getFloat("mPaintingLiftDamage", paintingLiftDamage);
            enemyNoReactFrame = obj.getInt("mEnemyNoReactFrame", enemyNoReactFrame);
        }
    }

    return true;
}

Obj_Sponge::Obj_Sponge()
    : mState(SpongeState::cState_Neutral)
    , mTeamId(0)
    , mCurrentScale(cBaseScale)
    , mTargetScale(cBaseScale)
    , mBreathingPhase(0.0f)
    , mCooldownTimer(0) {
    mPosition.set(0.0f, 0.0f, 0.0f);
    mParams.load("content/Static/Obj_Sponge.params");
}

Obj_Sponge::~Obj_Sponge() {}

void Obj_Sponge::init() {
    init(mPosition, 0);
}

void Obj_Sponge::init(const sead::Vector3f& pos, u32 initialTeam) {
    mPosition = pos;
    mTeamId = initialTeam;
    mState = SpongeState::cState_Neutral;
    mCurrentScale = cBaseScale;
    mTargetScale = cBaseScale;
    mBreathingPhase = 0.0f;
    mCooldownTimer = 0;
    mParams.load("content/Static/Obj_Sponge.params");
}

bool Obj_Sponge::applyFriendlyInk(f32 inkAmount) {
    mTargetScale = std::min(mTargetScale + inkAmount * 0.10f, mParams.scaleDamageForMax);
    mState = SpongeState::cState_Expanding;
    return true;
}

bool Obj_Sponge::applyFriendlyBomb(f32 bombAmount) {
    f32 effective = bombAmount * 0.10f * mParams.scaleBombCoreDamageK;
    mTargetScale = std::min(mTargetScale + effective, mParams.scaleDamageForMax);
    mState = SpongeState::cState_Expanding;
    return true;
}

bool Obj_Sponge::applyEnemyInk(f32 inkAmount) {
    mTargetScale = std::max(mTargetScale - inkAmount * 0.15f, cMinScale);
    mState = SpongeState::cState_Contracting;
    return true;
}

void Obj_Sponge::update() {
    // Smooth scale interpolation toward target
    f32 diff = mTargetScale - mCurrentScale;
    mCurrentScale += diff * 0.20f;

    if (std::abs(diff) < 0.05f) {
        mCurrentScale = mTargetScale;
        if (mCurrentScale >= (mParams.scaleDamageForMax - 0.05f)) {
            mState = SpongeState::cState_MaxExpanded;
        } else if (mCurrentScale <= (cMinScale + 0.05f)) {
            mState = SpongeState::cState_MinContracted;
        } else {
            mState = SpongeState::cState_Neutral;
        }
    }

    // Idle rhythmic breathing pulsation when settled
    mBreathingPhase += 0.05f;
    if (mBreathingPhase > 6.2831853f) {
        mBreathingPhase -= 6.2831853f;
    }
}

bool Obj_Sponge::checkPlayerStanding(const sead::Vector3f& playerPos, f32& outGroundY) const {
    f32 dx = playerPos.x - mPosition.x;
    f32 dz = playerPos.z - mPosition.z;
    f32 distH = std::sqrt(dx * dx + dz * dz);

    f32 rad = getCurrentRadius();
    f32 topY = mPosition.y + getCurrentHeight();

    if (distH <= rad) {
        // Check if player's feet are within vertical proximity of the top surface
        if (playerPos.y >= (topY - 0.8f) && playerPos.y <= (topY + 1.2f)) {
            outGroundY = topY;
            return true;
        }
    }

    return false;
}

} // namespace Game
