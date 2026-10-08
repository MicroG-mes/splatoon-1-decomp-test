#pragma once

#include "types.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace Game {

struct TankInfoEntry {
    u32 id = 0;
    u32 catalogueOrder = 0;
    std::string name;
    std::string arcName;
    std::string modelName;
    std::string material;
};

class TankInfoCatalog {
public:
    TankInfoCatalog();
    ~TankInfoCatalog();

    bool load(const char* byamlPath);
    void clear();

    bool isLoaded() const { return mIsLoaded; }
    size_t getTankCount() const { return mTanks.size(); }
    const TankInfoEntry* getTankById(u32 id) const;
    const TankInfoEntry* getTankByName(const std::string& name) const;
    const TankInfoEntry* getTankByIndex(size_t index) const;

    static TankInfoCatalog* instance();

private:
    bool mIsLoaded = false;
    std::vector<TankInfoEntry> mTanks;
    std::unordered_map<u32, size_t> mIdMap;
    std::unordered_map<std::string, size_t> mNameMap;
    static TankInfoCatalog* sInstance;
};

} // namespace Game
