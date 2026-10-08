#pragma once

#include "types.h"
#include "sead/math/seadVector.h"
#include "Game/Collision/KclFile.h"

namespace Game {

class CameraCollision {
public:
    CameraCollision();
    ~CameraCollision();

    void init(f32 minDistance = 0.8f, f32 maxDistance = 5.2f, f32 sphereRadius = 0.35f);

    // Resolves camera position against stage KCL collision geometry
    sead::Vector3f resolveCameraPosition(
        const sead::Vector3f& targetEye,
        const sead::Vector3f& desiredCamPos,
        const KclFile& stageKcl,
        f32 deltaTime = 0.01667f
    );

    f32 getCurrentDistance() const { return mCurrentDistance; }
    bool isOccluded() const { return mIsOccluded; }

private:
    f32 mMinDistance;
    f32 mMaxDistance;
    f32 mSphereRadius;
    f32 mCurrentDistance;
    bool mIsOccluded;
};

} // namespace Game
