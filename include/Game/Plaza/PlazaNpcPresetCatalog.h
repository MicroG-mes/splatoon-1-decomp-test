#pragma once

#include "types.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace Game {

struct PlazaNpcPresetEntry {
    u32 index = 0;
    std::string name;
    u32 gender = 0;
    u32 rank = 0;
    u32 weaponId = 0;
    u32 headGearId = 0;
    u32 clothesGearId = 0;
    u32 shoesGearId = 0;
};

class PlazaNpcPresetCatalog {
public:
    PlazaNpcPresetCatalog();
    ~PlazaNpcPresetCatalog();

    bool loadFromSzs(const char* szsPath);
    void clear();

    bool isLoaded() const { return mIsLoaded; }
    size_t getPresetCount() const { return mPresets.size(); }
    const PlazaNpcPresetEntry* getPreset(size_t index) const;
    const PlazaNpcPresetEntry* getPresetByName(const std::string& name) const;

    static PlazaNpcPresetCatalog* instance();

private:
    bool mIsLoaded = false;
    std::vector<PlazaNpcPresetEntry> mPresets;
    std::unordered_map<std::string, size_t> mNameMap;
    static std::unique_ptr<PlazaNpcPresetCatalog> sInstance;
};

} // namespace Game
