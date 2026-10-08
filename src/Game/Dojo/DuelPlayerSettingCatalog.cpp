#include "Game/Dojo/DuelPlayerSettingCatalog.h"
#include "Game/Map/BymlParser.h"
#include <fstream>

namespace Game {

DuelPlayerSettingCatalog* DuelPlayerSettingCatalog::sInstance = nullptr;

DuelPlayerSettingCatalog::DuelPlayerSettingCatalog() {
    sInstance = this;
}

DuelPlayerSettingCatalog::~DuelPlayerSettingCatalog() {
    if (sInstance == this) {
        sInstance = nullptr;
    }
}

DuelPlayerSettingCatalog* DuelPlayerSettingCatalog::instance() {
    return sInstance;
}

void DuelPlayerSettingCatalog::clear() {
    mIsLoaded = false;
    mPresets.clear();
}

bool DuelPlayerSettingCatalog::load(const char* byamlPath) {
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
    mPresets.reserve(count);

    for (size_t i = 0; i < count; ++i) {
        const BymlNode* elem = root->getElement(i);
        if (!elem || !elem->isDictionary()) continue;

        DuelPlayerPreset preset;
        preset.id = static_cast<u32>(elem->getInt("Id"));
        preset.weaponSet = elem->getString("WeaponSet");
        preset.head = elem->getString("Head");
        preset.clothes = elem->getString("Clothes");
        preset.shoes = elem->getString("Shoes");

        mPresets.push_back(preset);
    }

    mIsLoaded = !mPresets.empty();
    return mIsLoaded;
}

const DuelPlayerPreset* DuelPlayerSettingCatalog::getPreset(size_t index) const {
    if (index < mPresets.size()) {
        return &mPresets[index];
    }
    return nullptr;
}

const DuelPlayerPreset* DuelPlayerSettingCatalog::getPresetById(u32 id) const {
    for (const auto& p : mPresets) {
        if (p.id == id) {
            return &p;
        }
    }
    return nullptr;
}

} // namespace Game
