#include "Game/Camera/CameraCollision.h"
#include <algorithm>
#include <cmath>

namespace Game {

CameraCollision::CameraCollision()
    : mMinDistance(0.8f)
    , mMaxDistance(5.2f)
    , mSphereRadius(0.35f)
    , mCurrentDistance(5.2f)
    , mIsOccluded(false)
{
}

CameraCollision::~CameraCollision() {
}

void CameraCollision::init(f32 minDistance, f32 maxDistance, f32 sphereRadius) {
    mMinDistance = minDistance;
    mMaxDistance = maxDistance;
    mSphereRadius = sphereRadius;
    mCurrentDistance = maxDistance;
    mIsOccluded = false;
}

sead::Vector3f CameraCollision::resolveCameraPosition(
    const sead::Vector3f& targetEye,
    const sead::Vector3f& desiredCamPos,
    const KclFile& stageKcl,
    f32 deltaTime
) {
    sead::Vector3f toCam = desiredCamPos - targetEye;
    f32 fullDist = toCam.length();
    if (fullDist < 0.001f) {
        return desiredCamPos;
    }

    sead::Vector3f rayDir = toCam * (1.0f / fullDist);
    f32 targetDist = fullDist;

    // Raycast from player target toward camera
    KclHitResult hit;
    if (stageKcl.raycast(targetEye, rayDir, fullDist + mSphereRadius, hit)) {
        mIsOccluded = true;
        f32 hitDist = hit.distance - mSphereRadius;
        targetDist = (std::max)(mMinDistance, (std::min)(hitDist, fullDist));
    } else {
        mIsOccluded = false;
        targetDist = (std::min)(fullDist, mMaxDistance);
    }

    // Smooth distance adjustment: instant snap in when occluded, gradual glide back out
    if (targetDist < mCurrentDistance) {
        // Zoom in quickly to prevent wall penetration
        mCurrentDistance = targetDist;
    } else {
        // Smooth zoom out
        f32 zoomOutRate = 6.0f; // units per second
        mCurrentDistance = (std::min)(targetDist, mCurrentDistance + zoomOutRate * deltaTime);
    }

    return targetEye + rayDir * mCurrentDistance;
}

} // namespace Game
