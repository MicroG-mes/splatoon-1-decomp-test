#pragma once

#include "types.h"
#include "sead/math/seadVector.h"
#include <string>
#include <vector>

namespace Game {

struct MapInfoEntry {
    s32 id;
    std::string mapFileName;
    sead::Vector3f mapCameraTrans;
    f32 mapCameraRotPitchDeg;
    f32 mapCameraRotYawDeg;
    f32 mapCameraScale;
    s32 mapCameraBravoInversionType;
    s32 msnStageNo;
    std::string teamColorMsn;
    std::string sndSceneEnv;
    f32 rotateVR;
    f32 yOffsetVR;
    f32 abnormalYPos;
    std::string baseSceneEnvSetName;
    std::string sceneEnvSetName;
    std::string brightness;
    std::string envHour;
};

/**
 * MapInfoCatalog
 * Master Nintendo Retail 129-stage configuration and camera parameter engine.
 * Loads directly from content/Static/MapInfo.byaml.
 */
class MapInfoCatalog {
public:
    MapInfoCatalog();
    ~MapInfoCatalog();

    bool loadFromByml(const char* mapInfoBymlPath);

    size_t getTotalEntryCount() const { return mEntries.size(); }
    const MapInfoEntry* getEntry(size_t index) const;
    const MapInfoEntry* findEntryById(s32 id) const;
    const MapInfoEntry* findEntryByName(const char* mapFileName) const;
    const MapInfoEntry* findEntryByMissionNo(s32 missionStageNo) const;

    std::vector<const MapInfoEntry*> getEntriesByCategory(const char* categorySuffix) const;
    std::vector<const MapInfoEntry*> getVersusEntries() const;
    std::vector<const MapInfoEntry*> getMissionEntries() const;
    std::vector<const MapInfoEntry*> getDuelEntries() const;

private:
    std::vector<MapInfoEntry> mEntries;
};

} // namespace Game
