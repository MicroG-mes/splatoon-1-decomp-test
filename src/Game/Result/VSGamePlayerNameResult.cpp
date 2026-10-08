#include "Game/Result/VSGamePlayerNameResult.h"
#include <cstring>

namespace Game {

VSGamePlayerNameResult::VSGamePlayerNameResult() {
    mStats.name[0] = '\0';
    mStats.level = 1;
    mStats.rank = 0;
    mStats.weaponId = 0;
    mStats.splats = 0;
    mStats.deaths = 0;
    mStats.turfInked = 0;
    mStats.isLocalPlayer = false;
}

VSGamePlayerNameResult::~VSGamePlayerNameResult() {
}

void VSGamePlayerNameResult::setPlayerInfo(const char* name, u32 level, s32 rank, u32 weaponId, u32 splats, u32 deaths, u32 turfInked, bool isLocal) {
    std::strncpy(mStats.name, name, sizeof(mStats.name) - 1);
    mStats.name[sizeof(mStats.name) - 1] = '\0';
    mStats.level = level;
    mStats.rank = rank;
    mStats.weaponId = weaponId;
    mStats.splats = splats;
    mStats.deaths = deaths;
    mStats.turfInked = turfInked;
    mStats.isLocalPlayer = isLocal;
}

f32 VSGamePlayerNameResult::getKDRatio() const {
    if (mStats.deaths == 0) {
        return static_cast<f32>(mStats.splats);
    }
    return static_cast<f32>(mStats.splats) / static_cast<f32>(mStats.deaths);
}

} // namespace Game
