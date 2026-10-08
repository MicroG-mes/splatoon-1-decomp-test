#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class CameraMode : u32 {
    cFollowPlayer   = 0,
    cSuperJumpTrack = 1,
    cKillCamOrbit   = 2,
    cCinematicIntro = 3,
    cShootingRange  = 4
};

class CameraMgr : public GambitActor {
public:
    static constexpr f32 cDefaultDistance = 5.2f;
    static constexpr f32 cMinDistance = 1.0f;
    static constexpr f32 cHeightOffset = 1.8f;
    static constexpr f32 cPitchMinRad = -1.047f; // -60 degrees
    static constexpr f32 cPitchMaxRad =  1.047f; // +60 degrees

    CameraMgr();
    virtual ~CameraMgr() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setTargetPosition(const sead::Vector3f& targetPos);
    void applyStickInput(f32 stickX, f32 stickY);
    void applyGyroInput(f32 gyroPitchDelta, f32 gyroYawDelta);
    void resetBehindPlayer(f32 playerFacingYaw);
    void setCameraMode(CameraMode mode);

    const sead::Vector3f& getEyePosition() const { return mEyePosition; }
    const sead::Vector3f& getLookAtPosition() const { return mLookAtPosition; }
    f32 getYaw() const { return mYaw; }
    f32 getPitch() const { return mPitch; }
    f32 getBoomDistance() const { return mBoomDistance; }
    CameraMode getMode() const { return mMode; }

protected:
    void updateCameraMatrices();
    void resolveTerrainOcclusion();

    CameraMode mMode;
    sead::Vector3f mTargetPosition;
    sead::Vector3f mEyePosition;
    sead::Vector3f mLookAtPosition;

    f32 mYaw;
    f32 mPitch;
    f32 mBoomDistance;
    f32 mFieldOfView;

    undefined mReserved[0x40];
};

} // namespace Game
