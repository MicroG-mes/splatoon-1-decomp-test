#include "Game/Hud/HudMgr.h"

namespace Game {

HudMgr::HudMgr()
    : mInkLevel(1.0f),
      mSubWeaponCost(0.7f),
      mIsLowInk(false),
      mSpecialPoints(0.0f),
      mSpecialMaxPoints(180.0f),
      mSpecialRatio(0.0f),
      mIsSpecialReady(false),
      mDamageVignetteAlpha(0.0f),
      mFlashAnimTimer(0) {
    mReticle.isTargetLocked = false;
    mReticle.targetDistance = 0.0f;
    mReticle.isEffectiveRange = false;
}

HudMgr::~HudMgr() {
}

void HudMgr::init() {
    GambitActor::init();
    mInkLevel = 1.0f;
    mSpecialRatio = 0.0f;
    mDamageVignetteAlpha = 0.0f;
}

void HudMgr::updateReticle(bool targetLocked, f32 distance, bool effectiveRange) {
    mReticle.isTargetLocked = targetLocked;
    mReticle.targetDistance = distance;
    mReticle.isEffectiveRange = effectiveRange;
}

void HudMgr::updateInkTank(f32 inkLevel, f32 subWeaponCost) {
    mInkLevel = inkLevel;
    if (mInkLevel < 0.0f) mInkLevel = 0.0f;
    if (mInkLevel > 1.0f) mInkLevel = 1.0f;

    mSubWeaponCost = subWeaponCost;
    mIsLowInk = (mInkLevel < mSubWeaponCost);
}

void HudMgr::updateSpecialGauge(f32 currentPoints, f32 maxPoints) {
    mSpecialPoints = currentPoints;
    mSpecialMaxPoints = maxPoints > 0.0f ? maxPoints : 180.0f;

    mSpecialRatio = mSpecialPoints / mSpecialMaxPoints;
    if (mSpecialRatio >= 1.0f) {
        mSpecialRatio = 1.0f;
        mIsSpecialReady = true;
    } else {
        mIsSpecialReady = false;
    }
}

void HudMgr::triggerDamageVignette(f32 damageAmount) {
    mDamageVignetteAlpha += damageAmount * 0.01f;
    if (mDamageVignetteAlpha > 1.0f) {
        mDamageVignetteAlpha = 1.0f;
    }
}

void HudMgr::update() {
    mFlashAnimTimer++;

    // Fade out damage vignette
    if (mDamageVignetteAlpha > 0.0f) {
        mDamageVignetteAlpha -= 0.015f;
        if (mDamageVignetteAlpha < 0.0f) {
            mDamageVignetteAlpha = 0.0f;
        }
    }
}

void HudMgr::draw() {
    GambitActor::draw();
}

} // namespace Game
