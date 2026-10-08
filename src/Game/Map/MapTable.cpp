#include "Game/Map/MapTable.h"
#include "Game/Map/StageDef.h"
#include "Game/Map/BymlParser.h"
#include <cstring>
#include <cstdio>

namespace Game {

MapTable* MapTable::sInstance = nullptr;

MapTable* MapTable::getInstance() {
    if (!sInstance) {
        sInstance = new MapTable();
        sInstance->initDefaultTables();
    }
    return sInstance;
}

MapTable::MapTable() {
}

MapTable::~MapTable() {
    if (sInstance == this) {
        sInstance = nullptr;
    }
}

void MapTable::initDefaultTables() {
    mStages.clear();
    u32 total = StageDef::getStageCount();
    const StageInfo* stages = StageDef::getAllStages();

    mStages.reserve(total);
    for (u32 i = 0; i < total; ++i) {
        MapParam p;
        std::memset(&p, 0, sizeof(MapParam));

        p.mapId = static_cast<s32>(stages[i].id);
        p.mapIndex = i;
        p.category = static_cast<u32>(stages[i].category);
        p.mapFileName = stages[i].codeName;
        p.displayName = stages[i].displayName;

        p.mapCameraTrans = stages[i].objectiveCenter;
        p.mapCameraTrans.y += 45.0f; // Overhead camera height
        p.mapCameraRotPitchDeg = -90.0f; // Directly looking down
        p.mapCameraRotYawDeg = 0.0f;
        p.mapCameraScale = 1.0f;
        p.mapCameraBravoInversionType = 1; // 1 = 180-deg rotation for Bravo team

        p.boundsMinX = stages[i].boundsMin.x;
        p.boundsMinZ = stages[i].boundsMin.z;
        p.boundsMaxX = stages[i].boundsMax.x;
        p.boundsMaxZ = stages[i].boundsMax.z;

        p.spawnAlpha = stages[i].spawnAlpha;
        p.spawnBravo = stages[i].spawnBravo;
        p.objectiveCenter = stages[i].objectiveCenter;

        mStages.push_back(p);
    }
}

// Authentic decompilation of 0x0201CB54: MapTable::findStageById
const MapParam* MapTable::findStageById(s32 stageId) const {
    for (const auto& stage : mStages) {
        if (stage.mapId == stageId) {
            return &stage;
        }
    }
    return nullptr;
}

// Authentic decompilation of 0x0201CC64: MapTable::getStageByIndex
const MapParam* MapTable::getStageByIndex(u32 index) const {
    if (index < mStages.size()) {
        return &mStages[index];
    }
    return nullptr;
}

// Authentic decompilation of 0x0201CCB4: MapTable::findStageByName
const MapParam* MapTable::findStageByName(const char* mapFileName) const {
    if (!mapFileName) return nullptr;

    for (const auto& stage : mStages) {
        if (stage.mapFileName && std::strcmp(stage.mapFileName, mapFileName) == 0) {
            return &stage;
        }
    }
    return nullptr;
}

bool MapTable::loadFromByml(const u8* data, size_t size) {
    BymlParser parser;
    if (!parser.parse(data, size)) {
        return false;
    }

    const BymlNode* root = parser.getRoot();
    if (!root || !root->isArray()) {
        return false;
    }

    size_t count = root->getArraySize();
    printf("[+] MapTable: Parsing %zu stages from Mush/MapInfo.byaml\n", count);

    for (size_t i = 0; i < count; ++i) {
        const BymlNode* entry = root->getElement(i);
        if (!entry || !entry->isDictionary()) continue;

        std::string fileName = entry->getString("MapFileName", "");
        if (fileName.empty()) continue;

        // Find or create map record
        MapParam* target = nullptr;
        for (auto& s : mStages) {
            if (s.mapFileName && fileName == s.mapFileName) {
                target = &s;
                break;
            }
        }

        if (target) {
            target->mapCameraRotPitchDeg = entry->getFloat("MapCameraRotPitchDeg", target->mapCameraRotPitchDeg);
            target->mapCameraRotYawDeg = entry->getFloat("MapCameraRotYawDeg", target->mapCameraRotYawDeg);
            target->mapCameraScale = entry->getFloat("MapCameraScale", target->mapCameraScale);
            target->mapCameraBravoInversionType = static_cast<u32>(entry->getInt("MapCameraBravoInversionType", target->mapCameraBravoInversionType));

            const BymlNode* transNode = entry->getChild("MapCameraTrans");
            if (transNode && transNode->isArray() && transNode->getArraySize() >= 3) {
                target->mapCameraTrans.x = transNode->getElement(0)->getFloat();
                target->mapCameraTrans.y = transNode->getElement(1)->getFloat();
                target->mapCameraTrans.z = transNode->getElement(2)->getFloat();
            }
        }
    }

    return true;
}

} // namespace Game
