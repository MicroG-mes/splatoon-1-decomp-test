#include "Game/Mission/ZapfishPowerGridMgr.h"
#include "Game/MapObj/Obj_Goal.h"
#include <cstdio>
#include <algorithm>

namespace Game {

ZapfishPowerGridMgr::ZapfishPowerGridMgr() {
    init();
}

ZapfishPowerGridMgr::~ZapfishPowerGridMgr() {}

bool ZapfishPowerGridMgr::init() {
    mZapfishList.clear();
    mZapfishList.reserve(cTotalZapfish);

    // Area 1: Stages 1 to 3
    mZapfishList.push_back({ 1, false, 1, 1, "Mini-Zapfish #1", "Fld_Athletic00_Msn", false, 100.0f });
    mZapfishList.push_back({ 2, false, 1, 2, "Mini-Zapfish #2", "Fld_Labyrinth00_Msn", false, 100.0f });
    mZapfishList.push_back({ 3, false, 1, 3, "Mini-Zapfish #3", "Fld_Athletic01_Msn", false, 100.0f });

    // Area 2: Stages 4 to 9
    mZapfishList.push_back({ 4, false, 2, 4, "Mini-Zapfish #4", "Fld_Geyser00_Msn", false, 100.0f });
    mZapfishList.push_back({ 5, false, 2, 5, "Mini-Zapfish #5", "Fld_Sponge00_Msn", false, 100.0f });
    mZapfishList.push_back({ 6, false, 2, 6, "Mini-Zapfish #6", "Fld_Propeller00_Msn", false, 100.0f });
    mZapfishList.push_back({ 7, false, 2, 7, "Mini-Zapfish #7", "Fld_Spread00_Msn", false, 100.0f });
    mZapfishList.push_back({ 8, false, 2, 8, "Mini-Zapfish #8", "Fld_Octa00_Msn", false, 100.0f });
    mZapfishList.push_back({ 9, false, 2, 9, "Mini-Zapfish #9", "Fld_Ufo00_Msn", false, 100.0f });

    // Area 3: Stages 10 to 15
    mZapfishList.push_back({ 10, false, 3, 10, "Mini-Zapfish #10", "Fld_Rail00_Msn", false, 100.0f });
    mZapfishList.push_back({ 11, false, 3, 11, "Mini-Zapfish #11", "Fld_Invisible00_Msn", false, 100.0f });
    mZapfishList.push_back({ 12, false, 3, 12, "Mini-Zapfish #12", "Fld_Cleaner00_Msn", false, 100.0f });
    mZapfishList.push_back({ 13, false, 3, 13, "Mini-Zapfish #13", "Fld_Amida00_Msn", false, 100.0f });
    mZapfishList.push_back({ 14, false, 3, 14, "Mini-Zapfish #14", "Fld_Octa01_Msn", false, 100.0f });
    mZapfishList.push_back({ 15, false, 3, 15, "Mini-Zapfish #15", "Fld_Ufo01_Msn", false, 100.0f });

    // Area 4: Stages 16 to 21
    mZapfishList.push_back({ 16, false, 4, 16, "Mini-Zapfish #16", "Fld_Propeller01_Msn", false, 100.0f });
    mZapfishList.push_back({ 17, false, 4, 17, "Mini-Zapfish #17", "Fld_Sniper00_Msn", false, 100.0f });
    mZapfishList.push_back({ 18, false, 4, 18, "Mini-Zapfish #18", "Fld_Crank00_Msn", false, 100.0f });
    mZapfishList.push_back({ 19, false, 4, 19, "Mini-Zapfish #19", "Fld_Cleaner01_Msn", false, 100.0f });
    mZapfishList.push_back({ 20, false, 4, 20, "Mini-Zapfish #20", "Fld_Octa02_Msn", false, 100.0f });
    mZapfishList.push_back({ 21, false, 4, 21, "Mini-Zapfish #21", "Fld_Ufo02_Msn", false, 100.0f });

    // Area 5: Stages 22 to 27
    mZapfishList.push_back({ 22, false, 5, 22, "Mini-Zapfish #22", "Fld_Switch00_Msn", false, 100.0f });
    mZapfishList.push_back({ 23, false, 5, 23, "Mini-Zapfish #23", "Fld_Sponge01_Msn", false, 100.0f });
    mZapfishList.push_back({ 24, false, 5, 24, "Mini-Zapfish #24", "Fld_BossStampKing_Bos_Msn", false, 100.0f });
    mZapfishList.push_back({ 25, false, 5, 25, "Mini-Zapfish #25", "Fld_BossBlowKing_Bos_Msn", false, 100.0f });
    mZapfishList.push_back({ 26, false, 5, 26, "Mini-Zapfish #26", "Fld_BossBallKing_Bos_Msn", false, 100.0f });
    mZapfishList.push_back({ 27, false, 5, 27, "Mini-Zapfish #27", "Fld_BossMawKing_Bos_Msn", false, 100.0f });

    // The Great Zapfish: Final Boss (DJ Octavio)
    mZapfishList.push_back({ 28, true, 5, 28, "The Great Zapfish", "Fld_BossRailKing_Bos_Msn", false, 10000.0f });

    return (mZapfishList.size() == cTotalZapfish);
}

const ZapfishInfo* ZapfishPowerGridMgr::getZapfish(u32 id) const {
    if (id == 0 || id > mZapfishList.size()) {
        return nullptr;
    }
    return &mZapfishList[id - 1];
}

const ZapfishInfo* ZapfishPowerGridMgr::getZapfishByMission(u32 missionNo) const {
    for (const auto& z : mZapfishList) {
        if (z.missionNo == missionNo) {
            return &z;
        }
    }
    return nullptr;
}

bool ZapfishPowerGridMgr::rescueZapfish(u32 id) {
    if (id == 0 || id > mZapfishList.size()) {
        return false;
    }
    mZapfishList[id - 1].rescued = true;
    return true;
}

bool ZapfishPowerGridMgr::isRescued(u32 id) const {
    const auto* z = getZapfish(id);
    return z ? z->rescued : false;
}

size_t ZapfishPowerGridMgr::getRescuedMiniZapfishCount() const {
    size_t count = 0;
    for (const auto& z : mZapfishList) {
        if (!z.isGreatZapfish && z.rescued) {
            count++;
        }
    }
    return count;
}

bool ZapfishPowerGridMgr::isGreatZapfishRescued() const {
    const auto* gz = getZapfish(28);
    return gz ? gz->rescued : false;
}

bool ZapfishPowerGridMgr::onGoalCollected(const Obj_Goal& goal) {
    if (!goal.isCollected()) return false;
    return rescueZapfish(goal.getStageNo());
}

AreaPowerStatus ZapfishPowerGridMgr::getAreaPowerStatus(u32 areaNo) const {
    AreaPowerStatus status;
    status.areaNo = areaNo;
    status.totalZapfish = 0;
    status.rescuedZapfish = 0;
    status.currentPowerMegaWatts = 0.0f;
    status.maxPowerMegaWatts = 0.0f;
    status.powerRatio = 0.0f;
    status.isFullyPowered = false;

    for (const auto& z : mZapfishList) {
        if (!z.isGreatZapfish && z.areaNo == areaNo) {
            status.totalZapfish++;
            status.maxPowerMegaWatts += z.powerOutputMegaWatts;
            if (z.rescued) {
                status.rescuedZapfish++;
                status.currentPowerMegaWatts += z.powerOutputMegaWatts;
            }
        }
    }

    if (status.maxPowerMegaWatts > 0.0f) {
        status.powerRatio = status.currentPowerMegaWatts / status.maxPowerMegaWatts;
    }
    status.isFullyPowered = (status.totalZapfish > 0 && status.rescuedZapfish == status.totalZapfish);

    return status;
}

f32 ZapfishPowerGridMgr::getTotalGridPowerMegaWatts() const {
    f32 total = 0.0f;
    for (const auto& z : mZapfishList) {
        if (z.rescued) {
            total += z.powerOutputMegaWatts;
        }
    }
    return total;
}

f32 ZapfishPowerGridMgr::getInkopolisPowerPercentage() const {
    return isGreatZapfishRescued() ? 100.0f : 0.0f;
}

f32 ZapfishPowerGridMgr::getTowerIlluminationIntensity() const {
    return isGreatZapfishRescued() ? 1.0f : 0.05f;
}

bool ZapfishPowerGridMgr::isCityBlackout() const {
    return !isGreatZapfishRescued();
}

bool ZapfishPowerGridMgr::verifyModelAssets() const {
    const char* paths[] = {
        "content/Model/Obj_BigNamazu.szs",
        "content/Model/Obj_Namazu.szs",
        "content/Model/Obj_NamazuDummy.szs",
        "content/Model/Obj_Goal.szs"
    };

    for (const char* p : paths) {
        FILE* fp = fopen(p, "rb");
        if (!fp) return false;
        fclose(fp);
    }

    return true;
}

} // namespace Game
