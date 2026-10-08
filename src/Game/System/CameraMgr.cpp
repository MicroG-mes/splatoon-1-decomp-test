#include "Game/System/CameraMgr.h"
#include <cmath>

namespace Game {

CameraMgr::CameraMgr()
    : mMode(CameraMode::cFollowPlayer),
      mTargetPosition(0.0f, 0.0f, 0.0f),
      mEyePosition(0.0f, cHeightOffset, -cDefaultDistance),
      mLookAtPosition(0.0f, cHeightOffset, 0.0f),
      mYaw(0.0f),
      mPitch(0.0f),
      mBoomDistance(cDefaultDistance),
      mFieldOfView(60.0f) {
}

CameraMgr::~CameraMgr() {
}

void CameraMgr::init() {
    GambitActor::init();
    mMode = CameraMode::cFollowPlayer;
    mTargetPosition.set(0.0f, 0.0f, 0.0f);
    mEyePosition.set(0.0f, cHeightOffset, -cDefaultDistance);
    mLookAtPosition.set(0.0f, cHeightOffset, 0.0f);
    mYaw = 0.0f;
    mPitch = 0.0f;
    mBoomDistance = cDefaultDistance;
    mFieldOfView = 60.0f;
}

void CameraMgr::setTargetPosition(const sead::Vector3f& targetPos) {
    mTargetPosition = targetPos;
}

void CameraMgr::applyStickInput(f32 stickX, f32 stickY) {
    mYaw += stickX * 0.05f;
    mPitch += stickY * 0.035f;

    // Clamp pitch between -60 and +60 deg
    if (mPitch < cPitchMinRad) {
        mPitch = cPitchMinRad;
    } else if (mPitch > cPitchMaxRad) {
        mPitch = cPitchMaxRad;
    }
}

void CameraMgr::applyGyroInput(f32 gyroPitchDelta, f32 gyroYawDelta) {
    mPitch += gyroPitchDelta;
    mYaw += gyroYawDelta;

    if (mPitch < cPitchMinRad) {
        mPitch = cPitchMinRad;
    } else if (mPitch > cPitchMaxRad) {
        mPitch = cPitchMaxRad;
    }
}

void CameraMgr::resetBehindPlayer(f32 playerFacingYaw) {
    mYaw = playerFacingYaw;
    mPitch = 0.0f;
}

void CameraMgr::setCameraMode(CameraMode mode) {
    mMode = mode;
}

void CameraMgr::resolveTerrainOcclusion() {
    // If ground level is above eye position, bump eye upwards
    if (mEyePosition.y < 0.5f) {
        mEyePosition.y = 0.5f;
    }
}

void CameraMgr::updateCameraMatrices() {
    mLookAtPosition.x = mTargetPosition.x;
    mLookAtPosition.y = mTargetPosition.y + cHeightOffset;
    mLookAtPosition.z = mTargetPosition.z;

    // Calculate eye position based on spherical coordinates from look-at point
    f32 cosPitch = std::cos(mPitch);
    f32 sinPitch = std::sin(mPitch);
    f32 sinYaw = std::sin(mYaw);
    f32 cosYaw = std::cos(mYaw);

    mEyePosition.x = mLookAtPosition.x - (mBoomDistance * cosPitch * sinYaw);
    mEyePosition.y = mLookAtPosition.y + (mBoomDistance * sinPitch);
    mEyePosition.z = mLookAtPosition.z - (mBoomDistance * cosPitch * cosYaw);

    resolveTerrainOcclusion();
}

void CameraMgr::update() {
    updateCameraMatrices();
}

void CameraMgr::draw() {
    GambitActor::draw();
}

} // namespace Game
