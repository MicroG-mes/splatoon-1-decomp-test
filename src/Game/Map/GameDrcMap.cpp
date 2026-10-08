#include "Game/Map/GameDrcMap.h"
#include <cmath>
#include <algorithm>

namespace Game {

GameDrcMap::GameDrcMap()
    : mCurrentStage(nullptr)
    , mLocalTeamId(0)
    , mIsBravoInverted(false)
    , mCameraMode(DrcMapCameraMode::cNeutralCamera)
    , mTouchState(DrcMapTouchState::cTouchOff)
    , mTargetWorldPos(0.0f, 0.0f, 0.0f)
    , mCenterScreenX(427.0f) // 854 / 2
    , mCenterScreenY(240.0f) // 480 / 2
    , mZoomScale(5.0f)
{
}

GameDrcMap::~GameDrcMap() {
}

void GameDrcMap::init(const MapParam* stageParam, u32 localPlayerTeamId) {
    mCurrentStage = stageParam;
    mLocalTeamId = localPlayerTeamId;
    mCameraMode = DrcMapCameraMode::cNeutralCamera;
    mTouchState = DrcMapTouchState::cTouchOff;
    mTargetWorldPos.set(0.0f, 0.0f, 0.0f);

    if (mCurrentStage) {
        // If local team is Bravo (1) and stage defines Bravo Inversion, invert by 180 deg
        mIsBravoInverted = (mLocalTeamId == 1 && mCurrentStage->mapCameraBravoInversionType != 0);

        f32 stageWidth = std::max(10.0f, mCurrentStage->boundsMaxX - mCurrentStage->boundsMinX);
        f32 stageHeight = std::max(10.0f, mCurrentStage->boundsMaxZ - mCurrentStage->boundsMinZ);

        // Fit stage cleanly onto 854x480 DRC display with margin
        f32 scaleX = (854.0f * 0.75f) / stageWidth;
        f32 scaleY = (480.0f * 0.75f) / stageHeight;
        mZoomScale = std::min(scaleX, scaleY) * mCurrentStage->mapCameraScale;
    }
}

void GameDrcMap::setCameraMode(DrcMapCameraMode mode) {
    mCameraMode = mode;
    mTouchState = DrcMapTouchState::cTouchOff;
}

void GameDrcMap::update() {
    // DRC map periodic tick (fade timers, reticle pulse animation)
}

void GameDrcMap::worldToScreen(const sead::Vector3f& worldPos, f32& outScreenX, f32& outScreenY) const {
    if (!mCurrentStage) {
        outScreenX = mCenterScreenX;
        outScreenY = mCenterScreenY;
        return;
    }

    f32 relX = worldPos.x - mCurrentStage->objectiveCenter.x;
    f32 relZ = worldPos.z - mCurrentStage->objectiveCenter.z;

    if (mIsBravoInverted) {
        relX = -relX;
        relZ = -relZ;
    }

    outScreenX = mCenterScreenX + relX * mZoomScale;
    outScreenY = mCenterScreenY + relZ * mZoomScale;
}

void GameDrcMap::screenToWorld(f32 screenX, f32 screenY, sead::Vector3f& outWorldPos) const {
    if (!mCurrentStage) {
        outWorldPos.set(0.0f, 0.0f, 0.0f);
        return;
    }

    f32 relX = (screenX - mCenterScreenX) / mZoomScale;
    f32 relZ = (screenY - mCenterScreenY) / mZoomScale;

    if (mIsBravoInverted) {
        relX = -relX;
        relZ = -relZ;
    }

    outWorldPos.x = mCurrentStage->objectiveCenter.x + relX;
    outWorldPos.y = mCurrentStage->objectiveCenter.y;
    outWorldPos.z = mCurrentStage->objectiveCenter.z + relZ;
}

void GameDrcMap::handleTouch(const DrcTouchPoint& touch) {
    if (!mCurrentStage) return;

    if (!touch.isTouched) {
        if (mTouchState == DrcMapTouchState::cTouchOn || mTouchState == DrcMapTouchState::cTouchOnSame) {
            // Touch released -> Decide action (Super Jump or launch Inkstrike!)
            mTouchState = DrcMapTouchState::cTouchDecided;
        } else if (mTouchState != DrcMapTouchState::cTouchDecided) {
            mTouchState = DrcMapTouchState::cTouchOff;
        }
        return;
    }

    // Touch is active
    sead::Vector3f touchedPos;
    screenToWorld(touch.screenX, touch.screenY, touchedPos);

    // Validate bounds
    bool withinBounds = (touchedPos.x >= mCurrentStage->boundsMinX - 10.0f &&
                         touchedPos.x <= mCurrentStage->boundsMaxX + 10.0f &&
                         touchedPos.z >= mCurrentStage->boundsMinZ - 10.0f &&
                         touchedPos.z <= mCurrentStage->boundsMaxZ + 10.0f);

    if (!withinBounds) {
        mTouchState = DrcMapTouchState::cTouchInvalid;
        return;
    }

    if (mTouchState == DrcMapTouchState::cTouchOff || mTouchState == DrcMapTouchState::cTouchInvalid) {
        mTouchState = DrcMapTouchState::cTouchOn;
        mTargetWorldPos = touchedPos;
    } else if (mTouchState == DrcMapTouchState::cTouchOn) {
        // Continuous touch
        mTouchState = DrcMapTouchState::cTouchOnSame;
        mTargetWorldPos = touchedPos;
    } else if (mTouchState == DrcMapTouchState::cTouchOnSame) {
        mTargetWorldPos = touchedPos;
    }
}

} // namespace Game
