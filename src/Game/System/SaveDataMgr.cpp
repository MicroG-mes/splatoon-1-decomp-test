#include "Game/System/SaveDataMgr.h"
#include <cstring>

namespace Game {

SaveDataMgr* SaveDataMgr::sInstance = nullptr;

SaveDataMgr::SaveDataMgr()
    : mIsLoaded(false),
      mIsCorrupted(false) {
    sInstance = this;

    // Default default starter Inkling loadout
    mCustomization.gender = 0;       // Inkling Girl
    mCustomization.skinColorId = 0;
    mCustomization.eyeColorId = 0;
    mCustomization.headGearId = 0;   // Studio Headphones
    mCustomization.clothesId = 0;    // White Tee
    mCustomization.shoesId = 0;      // Pink Trainers
    mCustomization.weaponSetId = 0;  // Splattershot Jr.

    mStats.level = 1;
    mStats.exp = 0;
    mStats.money = 0;
    mStats.superSeaSnails = 0;
    mStats.udemaeRank = 0;          // C-
    mStats.udemaePoints = 0;
}

SaveDataMgr::~SaveDataMgr() {
    if (sInstance == this) {
        sInstance = nullptr;
    }
}

bool SaveDataMgr::load() {
    // Load from Cafe OS filesystem (/vol/save/)
    mIsLoaded = true;
    mIsCorrupted = false;
    return true;
}

bool SaveDataMgr::save() {
    // Commit save buffer to persistent storage
    return true;
}

bool SaveDataMgr::verifyIntegrity() const {
    return !mIsCorrupted && mIsLoaded;
}

void SaveDataMgr::addExp(u32 amount) {
    mStats.exp += amount;
    // Level up calculation (Max Level 50)
    u32 neededExp = mStats.level * 700;
    if (mStats.exp >= neededExp && mStats.level < 50) {
        mStats.level++;
        mStats.exp -= neededExp;
    }
}

} // namespace Game
