#pragma once

#include "types.h"
#include <string>
#include <vector>

namespace Game {

struct DuelPlayerPreset {
    u32 id = 0;
    std::string weaponSet;
    std::string head;
    std::string clothes;
    std::string shoes;
};

class DuelPlayerSettingCatalog {
public:
    DuelPlayerSettingCatalog();
    ~DuelPlayerSettingCatalog();

    bool load(const char* byamlPath);
    void clear();

    bool isLoaded() const { return mIsLoaded; }
    size_t getPresetCount() const { return mPresets.size(); }
    const DuelPlayerPreset* getPreset(size_t index) const;
    const DuelPlayerPreset* getPresetById(u32 id) const;

    static DuelPlayerSettingCatalog* instance();

private:
    bool mIsLoaded = false;
    std::vector<DuelPlayerPreset> mPresets;
    static DuelPlayerSettingCatalog* sInstance;
};

} // namespace Game
