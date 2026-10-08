#pragma once

#include "types.h"

namespace Game {

struct InklingCustomization {
    u32 gender;       // 0 = Girl, 1 = Boy
    u32 skinColorId;  // 0 to 6
    u32 eyeColorId;   // 0 to 6
    u32 headGearId;   // Head equipment ID
    u32 clothesId;    // Shirt/Jacket equipment ID
    u32 shoesId;      // Shoes equipment ID
    u32 weaponSetId;  // Main weapon loadout ID
};

struct PlayerStats {
    u32 level;          // Level 1 to 50
    u32 exp;            // Experience points
    u32 money;          // Coins / Cash
    u32 superSeaSnails; // Shell currency
    s32 udemaeRank;     // 0 = C-, ..., 9 = S, 10 = S+
    s32 udemaePoints;   // 0 to 99 points in current rank
};

class SaveDataMgr {
public:
    SaveDataMgr();
    virtual ~SaveDataMgr();

    static SaveDataMgr* instance() { return sInstance; }

    bool load();
    bool save();
    bool verifyIntegrity() const;

    const InklingCustomization& getCustomization() const { return mCustomization; }
    void setCustomization(const InklingCustomization& custom) { mCustomization = custom; }

    const PlayerStats& getStats() const { return mStats; }
    void addMoney(u32 amount) { mStats.money += amount; }
    void addExp(u32 amount);

    static SaveDataMgr* sInstance;

protected:
    InklingCustomization mCustomization;
    PlayerStats mStats;
    bool mIsLoaded;
    bool mIsCorrupted;
    undefined mReserved[0x40];
};

} // namespace Game
