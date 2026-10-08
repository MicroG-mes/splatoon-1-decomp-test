#include "Game/Camera/CameraParamEngine.h"
#include <fstream>
#include <sstream>
#include <algorithm>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace Game {

CameraParamEngine::CameraParamEngine() {}
CameraParamEngine::~CameraParamEngine() {}

bool CameraParamEngine::parseCameraText(const std::string& text, CameraParams& outParams) {
    if (text.empty()) return false;

    std::istringstream stream(text);
    std::string token;

    while (stream >> token) {
        if (token == "{" || token == "}") continue;

        // Strip quotes if present
        std::string key = token;
        if (!key.empty() && key.front() == '"') key.erase(0, 1);
        if (!key.empty() && key.back() == '"') key.pop_back();

        if (key == "mRefType") {
            stream >> outParams.refType;
        } else if (key == "mDirType") {
            stream >> outParams.dirType;
        } else if (key == "mAt") {
            stream >> outParams.at.x >> outParams.at.y >> outParams.at.z;
        } else if (key == "mPos") {
            stream >> outParams.pos.x >> outParams.pos.y >> outParams.pos.z;
        } else if (key == "mUp") {
            stream >> outParams.up.x >> outParams.up.y >> outParams.up.z;
        } else if (key == "mNear") {
            stream >> outParams.nearPlane;
        } else if (key == "mFar") {
            stream >> outParams.farPlane;
        } else if (key == "mFovy") {
            stream >> outParams.fovy;
        } else if (key == "mAspect") {
            stream >> outParams.aspect;
        } else if (key == "mIsUseLimit") {
            stream >> outParams.isUseLimit;
        } else if (key == "mLimit") {
            stream >> outParams.limit;
        } else if (key == "mIsOverwritePushInterpolation") {
            stream >> outParams.isOverwritePushInterpolation;
        } else if (key == "mIsOverwritePopInterpolation") {
            stream >> outParams.isOverwritePopInterpolation;
        } else if (key == "mInterpolateFrameMax") {
            stream >> outParams.interpolateFrameMax;
        } else if (key == "mInterpolateCurve") {
            outParams.interpolateCurve.clear();
            std::string curveToken;
            while (stream >> curveToken && curveToken != "}") {
                try {
                    outParams.interpolateCurve.push_back(std::stof(curveToken));
                } catch (...) {}
            }
        }
    }

    return true;
}

bool CameraParamEngine::loadCameraFile(const std::string& filePath, CameraParams& outParams) {
    std::ifstream file(filePath);
    if (!file.is_open()) return false;

    std::stringstream buffer;
    buffer << file.rdbuf();
    return parseCameraText(buffer.str(), outParams);
}

bool CameraParamEngine::loadDirectory(const std::string& dirPath) {
#ifdef _WIN32
    std::string searchPattern = dirPath + "/*.camera.params";
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPattern.c_str(), &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return false;
    }

    do {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            std::string filename = findData.cFileName;
            std::string fullPath = dirPath + "/" + filename;
            CameraParams cam;
            if (loadCameraFile(fullPath, cam)) {
                const std::string suffix = ".camera.params";
                std::string baseName = filename.substr(0, filename.length() - suffix.length());
                mCameras[baseName] = cam;
                mCameras[filename] = cam;
            }
        }
    } while (FindNextFileA(hFind, &findData));

    FindClose(hFind);
    return !mCameras.empty();
#else
    return false;
#endif
}

const CameraParams* CameraParamEngine::getCamera(const std::string& name) const {
    auto it = mCameras.find(name);
    if (it != mCameras.end()) {
        return &it->second;
    }
    return nullptr;
}

std::vector<std::string> CameraParamEngine::getCameraNames() const {
    std::vector<std::string> names;
    for (const auto& pair : mCameras) {
        names.push_back(pair.first);
    }
    return names;
}

} // namespace Game
