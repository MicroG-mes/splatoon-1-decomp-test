#pragma once

#include "types.h"
#include <string>
#include <vector>

namespace Game {

enum class AmiiboFigureType : u32 {
    cGirl  = 0,
    cBoy   = 1,
    cSquid = 2
};

struct AmiiboMissionEntry {
    std::string amiiboType;         // "Girl", "Boy", "Squid"
    u32 uiButtonId;                 // 0..19 (20 missions per figure)
    std::string challengeMapFile;   // e.g. "Fld_EasyHide00_Msn"
    std::string clearMapFile;       // e.g. "Fld_EasyHide00_Msn"
    std::string weapon;             // "Charge", "Roller", "Shot"
    f32 inkLimit;                   // -1.0 = none
    s32 timeLimit;                  // -1 = none
    bool kingSquid;                 // true if Kraken only
    bool noSuit;                    // true if armor disabled
    u32 moneyFirstClear;            // Initial clear reward cash
    u32 moneyRepeatClear;           // Repeat clear reward cash (typically 100)
    std::string prizeType;          // "Money", "Head", "Clothes", "Shoes", "Weapon", "MiniGame"
    std::string prizeName;          // e.g. "AMB000", "AMB001", "AMB002"
    std::string prizeMiniGame;      // "None", "SquidBall", "SquidRacer", etc.
};

/**
 * AmiiboChallengeMgr
 * Manages the authentic 60-mission single-player Amiibo Challenge subsystem
 * parsed from content/Static/AmiiboChallengeMapInfo.byaml.
 */
class AmiiboChallengeMgr {
public:
    AmiiboChallengeMgr();
    ~AmiiboChallengeMgr();

    bool loadFromByml(const char* bymlPath);

    size_t getTotalMissionCount() const { return mMissions.size(); }
    const AmiiboMissionEntry* getMission(size_t index) const;
    const AmiiboMissionEntry* findMission(const char* amiiboType, u32 uiButtonId) const;
    std::vector<const AmiiboMissionEntry*> getMissionsByFigure(const char* amiiboType) const;

    u32 calculateReward(const AmiiboMissionEntry* mission, bool isFirstClear) const {
        if (!mission) return 0;
        return isFirstClear ? mission->moneyFirstClear : mission->moneyRepeatClear;
    }

private:
    std::vector<AmiiboMissionEntry> mMissions;
};

} // namespace Game
