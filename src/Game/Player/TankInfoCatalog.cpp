#include "Game/Player/TankInfoCatalog.h"
#include "Game/Map/BymlParser.h"
#include <fstream>

namespace Game {

TankInfoCatalog* TankInfoCatalog::sInstance = nullptr;

TankInfoCatalog::TankInfoCatalog() {
    sInstance = this;
}

TankInfoCatalog::~TankInfoCatalog() {
    if (sInstance == this) {
        sInstance = nullptr;
    }
}

TankInfoCatalog* TankInfoCatalog::instance() {
    return sInstance;
}

void TankInfoCatalog::clear() {
    mIsLoaded = false;
    mTanks.clear();
    mIdMap.clear();
    mNameMap.clear();
}

bool TankInfoCatalog::load(const char* byamlPath) {
    if (!byamlPath) return false;

    std::ifstream file(byamlPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<u8> buffer(static_cast<size_t>(fileSize));
    if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
        return false;
    }

    BymlParser parser;
    if (!parser.parse(buffer.data(), buffer.size())) {
        return false;
    }

    const BymlNode* root = parser.getRoot();
    if (!root || !root->isArray()) {
        return false;
    }

    clear();
    size_t count = root->getArraySize();
    mTanks.reserve(count);

    for (size_t i = 0; i < count; ++i) {
        const BymlNode* elem = root->getElement(i);
        if (!elem || !elem->isDictionary()) continue;

        TankInfoEntry entry;
        entry.id = static_cast<u32>(elem->getInt("Id"));
        entry.catalogueOrder = static_cast<u32>(elem->getInt("CatalogueOrder"));
        entry.name = elem->getString("Name");
        entry.arcName = elem->getString("ArcName");
        entry.modelName = elem->getString("ModelName");
        entry.material = elem->getString("Material");

        size_t idx = mTanks.size();
        mTanks.push_back(entry);
        mIdMap[entry.id] = idx;
        mNameMap[entry.name] = idx;
    }

    mIsLoaded = !mTanks.empty();
    return mIsLoaded;
}

const TankInfoEntry* TankInfoCatalog::getTankById(u32 id) const {
    auto it = mIdMap.find(id);
    if (it != mIdMap.end()) {
        return &mTanks[it->second];
    }
    return nullptr;
}

const TankInfoEntry* TankInfoCatalog::getTankByName(const std::string& name) const {
    auto it = mNameMap.find(name);
    if (it != mNameMap.end()) {
        return &mTanks[it->second];
    }
    return nullptr;
}

const TankInfoEntry* TankInfoCatalog::getTankByIndex(size_t index) const {
    if (index < mTanks.size()) {
        return &mTanks[index];
    }
    return nullptr;
}

} // namespace Game
