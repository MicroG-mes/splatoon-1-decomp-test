#pragma once

#include "types.h"
#include <vector>
#include <string>

namespace Game {

struct PlayerRankInfo {
    u32 rank;
    u32 nextRankExp;
};

class PlayerRankMgr {
public:
    PlayerRankMgr();
    ~PlayerRankMgr();

    bool load(const char* byamlPath);
    void clear();

    bool isLoaded() const { return mIsLoaded; }
    size_t getRankCount() const { return mRanks.size(); }
    const PlayerRankInfo* getRankInfo(u32 rank) const;
    u32 getNextRankExp(u32 rank) const;

    // Calculate current rank and remaining EXP from total earned EXP
    void calculateRankFromTotalExp(u32 totalExp, u32& outRank, u32& outCurrentRankExp, u32& outNeededExp) const;

    static PlayerRankMgr* instance();

private:
    bool mIsLoaded = false;
    std::vector<PlayerRankInfo> mRanks;
    static PlayerRankMgr* sInstance;
};

} // namespace Game
