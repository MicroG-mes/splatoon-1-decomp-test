#pragma once

#include "types.h"
#include "sead/math/seadVector.h"
#include "sead/math/seadMatrix.h"
#include <string>
#include <vector>
#include <map>

namespace Game {

/**
 * CameraParams
 * Authentic camera parameters parsed from Nintendo .camera.params files.
 * Controls cinematic cuts, shop views, stage overviews, and player intro angles.
 */
struct CameraParams {
    s32 refType = 0;
    s32 dirType = 0;
    sead::Vector3f at = sead::Vector3f(0.0f, 0.0f, 0.0f);
    sead::Vector3f pos = sead::Vector3f(0.0f, 0.0f, 10.0f);
    sead::Vector3f up = sead::Vector3f(0.0f, 1.0f, 0.0f);
    f32 nearPlane = 0.1f;
    f32 farPlane = 10000.0f;
    f32 fovy = 45.0f;
    f32 aspect = 1.77777779f; // default 16:9
    s32 isUseLimit = 0;
    s32 limit = 60;
    s32 isOverwritePushInterpolation = 0;
    s32 isOverwritePopInterpolation = 0;
    s32 interpolateFrameMax = 60;
    std::vector<f32> interpolateCurve;

    sead::Vector3f getForward() const {
        return (at - pos).normalized();
    }

    f32 getDistance() const {
        return (at - pos).length();
    }

    sead::Matrix44f buildViewMatrix() const {
        sead::Matrix44f view;
        view.buildLookAtDX11(pos, at, up);
        return view;
    }

    sead::Matrix44f buildProjMatrix() const {
        sead::Matrix44f proj;
        f32 fovyRad = fovy * (3.1415926535f / 180.0f);
        proj.buildPerspectiveDX11(fovyRad, aspect, nearPlane, farPlane);
        return proj;
    }
};

/**
 * CameraParamEngine
 * Engine for loading and accessing authentic cinematic and stage camera presets.
 */
class CameraParamEngine {
public:
    CameraParamEngine();
    ~CameraParamEngine();

    static bool parseCameraText(const std::string& text, CameraParams& outParams);
    bool loadCameraFile(const std::string& filePath, CameraParams& outParams);
    bool loadDirectory(const std::string& dirPath);

    size_t getLoadedCount() const { return mCameras.size(); }
    const CameraParams* getCamera(const std::string& name) const;
    std::vector<std::string> getCameraNames() const;

private:
    std::map<std::string, CameraParams> mCameras;
};

} // namespace Game
