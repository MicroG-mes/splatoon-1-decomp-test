#include "Game/Map/MapInfoCatalog.h"
#include "Game/Map/BymlParser.h"
#include <fstream>
#include <cstdio>

namespace Game {

MapInfoCatalog::MapInfoCatalog() {}

MapInfoCatalog::~MapInfoCatalog() {}

bool MapInfoCatalog::loadFromByml(const char* mapInfoBymlPath) {
    if (!mapInfoBymlPath) return false;

    std::ifstream file(mapInfoBymlPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<u8> buffer(static_cast<size_t>(fileSize));
    if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
        return false;
    }

    BymlParser parser;
    if (!parser.parse(buffer.data(), buffer.size()) || !parser.getRoot() || !parser.getRoot()->isArray()) {
        return false;
    }

    mEntries.clear();
    const auto* root = parser.getRoot();
    size_t count = root->getArraySize();

    for (size_t i = 0; i < count; ++i) {
        const auto* elem = root->getElement(i);
        if (!elem || !elem->isDictionary()) continue;

        MapInfoEntry entry;
        entry.id = elem->getInt("Id", -1);
        entry.mapFileName = elem->getString("MapFileName", "");
        entry.mapCameraRotPitchDeg = elem->getFloat("MapCameraRotPitchDeg", 0.0f);
        entry.mapCameraRotYawDeg = elem->getFloat("MapCameraRotYawDeg", 0.0f);
        entry.mapCameraScale = elem->getFloat("MapCameraScale", 1.0f);
        entry.mapCameraBravoInversionType = elem->getInt("MapCameraBravoInversionType", 0);
        entry.msnStageNo = elem->getInt("MsnStageNo", -1);
        entry.teamColorMsn = elem->getString("TeamColor_Msn", "");
        entry.sndSceneEnv = elem->getString("SndSceneEnv", "");
        entry.rotateVR = elem->getFloat("RotateVR", 0.0f);
        entry.yOffsetVR = elem->getFloat("YoffsetVR", 0.0f);
        entry.abnormalYPos = elem->getFloat("AbnormalYPos", -100.0f);
        entry.baseSceneEnvSetName = elem->getString("BaseSceneEnvSetName", "");
        entry.sceneEnvSetName = elem->getString("SceneEnvSetName", "");
        entry.brightness = elem->getString("Brightness", "");
        entry.envHour = elem->getString("EnvHour", "");

        std::string transStr = elem->getString("MapCameraTrans", "");
        float tx = 0.0f, ty = 0.0f, tz = 0.0f;
        if (!transStr.empty()) {
            if (sscanf(transStr.c_str(), "%f , %f , %f", &tx, &ty, &tz) == 3 ||
                sscanf(transStr.c_str(), "%f, %f, %f", &tx, &ty, &tz) == 3 ||
                sscanf(transStr.c_str(), "%f,%f,%f", &tx, &ty, &tz) == 3) {
                entry.mapCameraTrans.set(tx, ty, tz);
            }
        }

        mEntries.push_back(entry);
    }

    return !mEntries.empty();
}

const MapInfoEntry* MapInfoCatalog::getEntry(size_t index) const {
    if (index < mEntries.size()) {
        return &mEntries[index];
    }
    return nullptr;
}

const MapInfoEntry* MapInfoCatalog::findEntryById(s32 id) const {
    for (const auto& e : mEntries) {
        if (e.id == id) return &e;
    }
    return nullptr;
}

const MapInfoEntry* MapInfoCatalog::findEntryByName(const char* mapFileName) const {
    if (!mapFileName) return nullptr;
    for (const auto& e : mEntries) {
        if (e.mapFileName == mapFileName) return &e;
    }
    return nullptr;
}

const MapInfoEntry* MapInfoCatalog::findEntryByMissionNo(s32 missionStageNo) const {
    for (const auto& e : mEntries) {
        if (e.msnStageNo == missionStageNo) return &e;
    }
    return nullptr;
}

std::vector<const MapInfoEntry*> MapInfoCatalog::getEntriesByCategory(const char* categorySuffix) const {
    std::vector<const MapInfoEntry*> result;
    if (!categorySuffix) return result;
    std::string suffix = categorySuffix;
    for (const auto& e : mEntries) {
        if (e.mapFileName.length() >= suffix.length() &&
            e.mapFileName.compare(e.mapFileName.length() - suffix.length(), suffix.length(), suffix) == 0) {
            result.push_back(&e);
        }
    }
    return result;
}

std::vector<const MapInfoEntry*> MapInfoCatalog::getVersusEntries() const {
    return getEntriesByCategory("Vss");
}

std::vector<const MapInfoEntry*> MapInfoCatalog::getMissionEntries() const {
    return getEntriesByCategory("Msn");
}

std::vector<const MapInfoEntry*> MapInfoCatalog::getDuelEntries() const {
    return getEntriesByCategory("Dul");
}

} // namespace Game
