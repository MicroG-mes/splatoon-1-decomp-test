#include "Game/Weapon/WeaponCatalog.h"
#include "Game/Map/BymlParser.h"
#include <fstream>
#include <algorithm>

namespace Game {

WeaponCatalog::WeaponCatalog() {}

WeaponCatalog::~WeaponCatalog() {}

bool WeaponCatalog::loadFromByml(const char* bymlPath) {
    if (!bymlPath) return false;
    std::ifstream file(bymlPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<u8> buffer(static_cast<size_t>(fileSize));
    if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
        return false;
    }

    return loadFromBuffer(buffer.data(), buffer.size());
}

bool WeaponCatalog::loadFromBuffer(const u8* data, size_t size) {
    if (!data || size == 0) return false;

    BymlParser parser;
    if (!parser.parse(data, size) || !parser.getRoot() || !parser.getRoot()->isArray()) {
        return false;
    }

    mWeapons.clear();
    const auto* rootArray = parser.getRoot();
    size_t count = rootArray->getArraySize();

    for (size_t i = 0; i < count; ++i) {
        const auto* item = rootArray->getElement(i);
        if (!item || !item->isDictionary()) continue;

        WeaponKitInfo info;
        info.id = static_cast<u32>(item->getInt("Id", 0));
        info.name = item->getString("Name", "");
        info.mainWeapon = item->getString("Main", "");
        info.subWeapon = item->getString("Sub", "");
        info.specialWeapon = item->getString("Special", "");
        info.price = static_cast<u32>(item->getInt("Price", 0));
        info.unlockRank = static_cast<u32>(item->getInt("Rank", 1));
        info.catalogueOrder = static_cast<u32>(item->getInt("CatalogueOrder", 0));
        info.rangeStat = static_cast<u32>(item->getInt("ParamValue0", 0));
        info.powerStat = static_cast<u32>(item->getInt("ParamValue1", 0));
        info.fireRateStat = static_cast<u32>(item->getInt("ParamValue2", 0));
        info.lockCondition = item->getString("Lock", "None");

        mWeapons.push_back(info);
    }

    // Sort by CatalogueOrder
    std::sort(mWeapons.begin(), mWeapons.end(), [](const WeaponKitInfo& a, const WeaponKitInfo& b) {
        return a.catalogueOrder < b.catalogueOrder;
    });

    return !mWeapons.empty();
}

const WeaponKitInfo* WeaponCatalog::getWeaponByIndex(size_t index) const {
    if (index < mWeapons.size()) {
        return &mWeapons[index];
    }
    return nullptr;
}

const WeaponKitInfo* WeaponCatalog::getWeaponById(u32 id) const {
    for (const auto& w : mWeapons) {
        if (w.id == id) return &w;
    }
    return nullptr;
}

const WeaponKitInfo* WeaponCatalog::findWeaponByName(const char* name) const {
    if (!name) return nullptr;
    for (const auto& w : mWeapons) {
        if (w.name == name) return &w;
    }
    return nullptr;
}

std::vector<const WeaponKitInfo*> WeaponCatalog::getUnlockedWeapons(u32 playerRank) const {
    std::vector<const WeaponKitInfo*> unlocked;
    for (const auto& w : mWeapons) {
        if (w.unlockRank <= playerRank) {
            unlocked.push_back(&w);
        }
    }
    return unlocked;
}

} // namespace Game
