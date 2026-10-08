#pragma once

#include "types.h"
#include <string>
#include <vector>

namespace Game {

struct WeaponKitInfo {
    u32 id;
    std::string name;
    std::string mainWeapon;
    std::string subWeapon;
    std::string specialWeapon;
    u32 price;
    u32 unlockRank;
    u32 catalogueOrder;
    u32 rangeStat;
    u32 powerStat;
    u32 fireRateStat;
    std::string lockCondition;
};

/**
 * WeaponCatalog (Mush/WeaponSet.byaml)
 * Manages the entire 71-weapon retail catalog of Splatoon 1.
 * Loads directly from authentic Nintendo BYML data.
 */
class WeaponCatalog {
public:
    WeaponCatalog();
    ~WeaponCatalog();

    bool loadFromByml(const char* bymlPath);
    bool loadFromBuffer(const u8* data, size_t size);

    size_t getWeaponCount() const { return mWeapons.size(); }
    const WeaponKitInfo* getWeaponByIndex(size_t index) const;
    const WeaponKitInfo* getWeaponById(u32 id) const;
    const WeaponKitInfo* findWeaponByName(const char* name) const;

    // Filters weapons unlocked at a given player level/rank
    std::vector<const WeaponKitInfo*> getUnlockedWeapons(u32 playerRank) const;

private:
    std::vector<WeaponKitInfo> mWeapons;
};

} // namespace Game
