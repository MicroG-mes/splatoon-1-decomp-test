#pragma once

#include "types.h"
#include <string>
#include <vector>

namespace Game {

struct MainWeaponAssetInfo {
    u32 id;
    std::string name;
    std::string type;
    std::string arcName;
    std::string modelName;
};

struct SubWeaponAssetInfo {
    u32 id;
    std::string name;
    std::string arcName;
};

struct SpecialWeaponAssetInfo {
    u32 id;
    std::string name;
    std::string arcName;
};

struct TankAssetInfo {
    u32 id;
    std::string name;
    std::string arcName;
    std::string modelName;
};

/**
 * WeaponParamCatalog
 * Authentic 134-item Nintendo retail weapon and tank asset linking engine.
 * Loads directly from:
 * - WeaponInfo_Main.byaml (94 main weapons)
 * - WeaponInfo_Sub.byaml (26 sub weapons)
 * - WeaponInfo_Special.byaml (8 special weapons)
 * - TankInfo.byaml (6 ink tanks)
 */
class WeaponParamCatalog {
public:
    WeaponParamCatalog();
    ~WeaponParamCatalog();

    bool loadAll(const char* mainByml, const char* subByml, const char* specialByml, const char* tankByml);

    size_t getMainCount() const { return mMains.size(); }
    size_t getSubCount() const { return mSubs.size(); }
    size_t getSpecialCount() const { return mSpecials.size(); }
    size_t getTankCount() const { return mTanks.size(); }
    size_t getTotalAssetCount() const { return mMains.size() + mSubs.size() + mSpecials.size() + mTanks.size(); }

    const MainWeaponAssetInfo* findMainByName(const char* name) const;
    const SubWeaponAssetInfo* findSubByName(const char* name) const;
    const SpecialWeaponAssetInfo* findSpecialByName(const char* name) const;
    const TankAssetInfo* findTankByName(const char* name) const;

private:
    std::vector<MainWeaponAssetInfo> mMains;
    std::vector<SubWeaponAssetInfo> mSubs;
    std::vector<SpecialWeaponAssetInfo> mSpecials;
    std::vector<TankAssetInfo> mTanks;
};

} // namespace Game
