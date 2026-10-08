#include "Game/Weapon/WeaponParamCatalog.h"
#include "Game/Map/BymlParser.h"
#include <fstream>

namespace Game {

WeaponParamCatalog::WeaponParamCatalog() {}

WeaponParamCatalog::~WeaponParamCatalog() {}

static bool loadBymlBuffer(const char* path, std::vector<u8>& outBuffer) {
    if (!path) return false;
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    outBuffer.resize(static_cast<size_t>(fileSize));
    return file.read(reinterpret_cast<char*>(outBuffer.data()), fileSize) ? true : false;
}

bool WeaponParamCatalog::loadAll(const char* mainByml, const char* subByml, const char* specialByml, const char* tankByml) {
    bool okMain = false, okSub = false, okSpecial = false, okTank = false;

    // 1. Main Weapons
    std::vector<u8> bufMain;
    if (loadBymlBuffer(mainByml, bufMain)) {
        BymlParser parser;
        if (parser.parse(bufMain.data(), bufMain.size()) && parser.getRoot() && parser.getRoot()->isArray()) {
            mMains.clear();
            const auto* root = parser.getRoot();
            size_t count = root->getArraySize();
            for (size_t i = 0; i < count; ++i) {
                const auto* elem = root->getElement(i);
                if (!elem || !elem->isDictionary()) continue;

                MainWeaponAssetInfo info;
                info.id = static_cast<u32>(elem->getInt("Id", 0));
                info.name = elem->getString("Name", "");
                info.type = elem->getString("Type", "");
                info.arcName = elem->getString("ArcName", "");
                info.modelName = elem->getString("ModelName", "");
                mMains.push_back(info);
            }
            okMain = !mMains.empty();
        }
    }

    // 2. Sub Weapons
    std::vector<u8> bufSub;
    if (loadBymlBuffer(subByml, bufSub)) {
        BymlParser parser;
        if (parser.parse(bufSub.data(), bufSub.size()) && parser.getRoot() && parser.getRoot()->isArray()) {
            mSubs.clear();
            const auto* root = parser.getRoot();
            size_t count = root->getArraySize();
            for (size_t i = 0; i < count; ++i) {
                const auto* elem = root->getElement(i);
                if (!elem || !elem->isDictionary()) continue;

                SubWeaponAssetInfo info;
                info.id = static_cast<u32>(elem->getInt("Id", 0));
                info.name = elem->getString("Name", "");
                info.arcName = elem->getString("ArcName", "");
                mSubs.push_back(info);
            }
            okSub = !mSubs.empty();
        }
    }

    // 3. Special Weapons
    std::vector<u8> bufSpecial;
    if (loadBymlBuffer(specialByml, bufSpecial)) {
        BymlParser parser;
        if (parser.parse(bufSpecial.data(), bufSpecial.size()) && parser.getRoot() && parser.getRoot()->isArray()) {
            mSpecials.clear();
            const auto* root = parser.getRoot();
            size_t count = root->getArraySize();
            for (size_t i = 0; i < count; ++i) {
                const auto* elem = root->getElement(i);
                if (!elem || !elem->isDictionary()) continue;

                SpecialWeaponAssetInfo info;
                info.id = static_cast<u32>(elem->getInt("Id", 0));
                info.name = elem->getString("Name", "");
                info.arcName = elem->getString("ArcName", "");
                mSpecials.push_back(info);
            }
            okSpecial = !mSpecials.empty();
        }
    }

    // 4. Ink Tanks
    std::vector<u8> bufTank;
    if (loadBymlBuffer(tankByml, bufTank)) {
        BymlParser parser;
        if (parser.parse(bufTank.data(), bufTank.size()) && parser.getRoot() && parser.getRoot()->isArray()) {
            mTanks.clear();
            const auto* root = parser.getRoot();
            size_t count = root->getArraySize();
            for (size_t i = 0; i < count; ++i) {
                const auto* elem = root->getElement(i);
                if (!elem || !elem->isDictionary()) continue;

                TankAssetInfo info;
                info.id = static_cast<u32>(elem->getInt("Id", 0));
                info.name = elem->getString("Name", "");
                info.arcName = elem->getString("ArcName", "");
                info.modelName = elem->getString("ModelName", "");
                mTanks.push_back(info);
            }
            okTank = !mTanks.empty();
        }
    }

    return okMain && okSub && okSpecial && okTank;
}

const MainWeaponAssetInfo* WeaponParamCatalog::findMainByName(const char* name) const {
    if (!name) return nullptr;
    for (const auto& w : mMains) {
        if (w.name == name || w.modelName == name) return &w;
    }
    return nullptr;
}

const SubWeaponAssetInfo* WeaponParamCatalog::findSubByName(const char* name) const {
    if (!name) return nullptr;
    for (const auto& w : mSubs) {
        if (w.name == name || w.arcName == name) return &w;
    }
    return nullptr;
}

const SpecialWeaponAssetInfo* WeaponParamCatalog::findSpecialByName(const char* name) const {
    if (!name) return nullptr;
    for (const auto& w : mSpecials) {
        if (w.name == name || w.arcName == name) return &w;
    }
    return nullptr;
}

const TankAssetInfo* WeaponParamCatalog::findTankByName(const char* name) const {
    if (!name) return nullptr;
    for (const auto& t : mTanks) {
        if (t.name == name || t.arcName == name || t.modelName == name) return &t;
    }
    return nullptr;
}

} // namespace Game
