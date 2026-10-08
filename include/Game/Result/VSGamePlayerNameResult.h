#pragma once

#include "types.h"

namespace Game {

struct PlayerMatchStats {
    char name[32];
    u32 level;
    s32 rank;
    u32 weaponId;
    u32 splats;
    u32 deaths;
    u32 turfInked;
    bool isLocalPlayer;
};

class VSGamePlayerNameResult {
public:
    VSGamePlayerNameResult();
    virtual ~VSGamePlayerNameResult();

    void setPlayerInfo(const char* name, u32 level, s32 rank, u32 weaponId, u32 splats, u32 deaths, u32 turfInked, bool isLocal);

    const PlayerMatchStats& getStats() const { return mStats; }
    f32 getKDRatio() const;

protected:
    PlayerMatchStats mStats;
    undefined mReserved[0x20];
};

} // namespace Game
