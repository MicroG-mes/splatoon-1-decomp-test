#pragma once

#include "types.h"
#include <string>
#include <vector>

namespace Game {

struct UdemaeGradeEntry {
    std::string name;
    s32 basePoint;
    u32 winBonusCash;
    u32 knockOutBonusCash;
};

struct PlayerRankThreshold {
    u32 rank;
    u32 nextRankExp;
};

/**
 * UdemaeGradeMgr
 * Reverse engineered from Splatoon 1 retail binary & Mush/UdemaeGrade.byaml / PlayerRank.byaml.
 * Manages player level progression (Lv 1 - 50) and Ranked Battle grade system (C- to S+).
 */
class UdemaeGradeMgr {
public:
    UdemaeGradeMgr();
    ~UdemaeGradeMgr();

    bool loadFromByml(const char* gradeBymlPath, const char* rankBymlPath);

    // Player level progression
    u32 getPlayerLevel() const { return mPlayerLevel; }
    u32 getCurrentExp() const { return mCurrentExp; }
    u32 getNextLevelExp() const;
    bool addExp(u32 expGained); // Returns true if leveled up

    // Ranked Grade progression
    const std::string& getGradeName() const;
    s32 getGradePoints() const { return mGradePoints; }
    s32 getGradeIndex() const { return mGradeIndex; }
    
    // Process match outcome in ranked mode
    // deltaPoints typically +10 to +14 on win, -10 to -14 on loss
    void applyMatchOutcome(bool won, s32 pointsDelta, bool isKnockout, u32& outCashAwarded);

    size_t getLoadedGradeCount() const { return mGrades.size(); }
    size_t getLoadedRankThresholdCount() const { return mRanks.size(); }

private:
    std::vector<UdemaeGradeEntry> mGrades;
    std::vector<PlayerRankThreshold> mRanks;

    u32 mPlayerLevel; // 1 to 50
    u32 mCurrentExp;

    s32 mGradeIndex;  // 0 = C-, 1 = C, ..., 8 = A+, 9 = S, 10 = S+
    s32 mGradePoints; // 0 to 100
};

} // namespace Game
