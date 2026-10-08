#pragma once

#include "types.h"
#include "sead/math/seadVector.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace Game {

struct StageActorObj {
    std::string id;
    std::string unitConfigName;
    std::string layerConfigName;
    sead::Vector3f translate = {0.0f, 0.0f, 0.0f};
    sead::Vector3f rotate = {0.0f, 0.0f, 0.0f};
    sead::Vector3f scale = {1.0f, 1.0f, 1.0f};
    s32 team = 0;
    s32 switch0 = 0;
    s32 switch1 = 0;
    s32 param0 = -99;
};

struct StageRailPoint {
    sead::Vector3f translate = {0.0f, 0.0f, 0.0f};
    sead::Vector3f control0 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f control1 = {0.0f, 0.0f, 0.0f};
};

struct StageRailObj {
    std::string id;
    std::string railType; // "Linear", "Bezier"
    bool isClosed = false;
    s32 priority = 0;
    std::vector<StageRailPoint> points;
};

class MissionStageMapParser {
public:
    MissionStageMapParser();
    ~MissionStageMapParser();

    bool loadFromSzs(const char* szsPath);
    bool load(const u8* bymlData, size_t bymlSize);
    void clear();

    bool isLoaded() const { return mIsLoaded; }
    size_t getActorCount() const { return mActors.size(); }
    size_t getRailCount() const { return mRails.size(); }

    const StageActorObj* getActor(size_t index) const;
    const StageActorObj* findActorById(const std::string& id) const;
    std::vector<const StageActorObj*> findActorsByConfig(const std::string& configName) const;

    const StageRailObj* getRail(size_t index) const;

    bool hasSunkenScroll() const;
    bool hasGoalZapfish() const;
    bool hasRespawnPoint() const;
    bool hasBoss() const;

private:
    bool mIsLoaded = false;
    std::vector<StageActorObj> mActors;
    std::vector<StageRailObj> mRails;
    std::unordered_map<std::string, size_t> mActorIdMap;
};

} // namespace Game
