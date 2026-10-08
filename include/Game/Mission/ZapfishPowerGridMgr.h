#pragma once

#include "types.h"
#include <string>
#include <vector>

namespace Game {

class Obj_Goal;

struct ZapfishInfo {
    u32 id;                     // 1 to 28
    bool isGreatZapfish;        // True for id == 28 (Great Zapfish)
    u32 areaNo;                 // 1 to 5
    u32 missionNo;              // 1 to 28
    std::string name;           // "Mini-Zapfish #N" or "The Great Zapfish"
    std::string stageMapName;   // Associated stage map
    bool rescued;               // Rescue status
    f32 powerOutputMegaWatts;   // 100 MW for mini, 10,000 MW for Great Zapfish
};

struct AreaPowerStatus {
    u32 areaNo;
    u32 totalZapfish;
    u32 rescuedZapfish;
    f32 currentPowerMegaWatts;
    f32 maxPowerMegaWatts;
    f32 powerRatio;             // 0.0 to 1.0
    bool isFullyPowered;
};

/**
 * ZapfishPowerGridMgr
 * Octo Valley & Inkopolis Tower electric power grid restoration subsystem.
 * Manages 27 Mini-Zapfish and 1 Great Zapfish, conduit power flow, and city illumination.
 */
class ZapfishPowerGridMgr {
public:
    static constexpr u32 cTotalMiniZapfish = 27;
    static constexpr u32 cTotalGreatZapfish = 1;
    static constexpr u32 cTotalZapfish = 28;
    static constexpr u32 cAreaCount = 5;

    ZapfishPowerGridMgr();
    ~ZapfishPowerGridMgr();

    bool init();

    size_t getTotalZapfishCount() const { return mZapfishList.size(); }
    const ZapfishInfo* getZapfish(u32 id) const;
    const ZapfishInfo* getZapfishByMission(u32 missionNo) const;

    // Rescue progression
    bool rescueZapfish(u32 id);
    bool isRescued(u32 id) const;
    size_t getRescuedMiniZapfishCount() const;
    bool isGreatZapfishRescued() const;

    // Connects with Goal actor on stage clear
    bool onGoalCollected(const Obj_Goal& goal);

    // Power grid metrics
    AreaPowerStatus getAreaPowerStatus(u32 areaNo) const;
    f32 getTotalGridPowerMegaWatts() const;
    f32 getInkopolisPowerPercentage() const;
    f32 getTowerIlluminationIntensity() const;
    bool isCityBlackout() const;

    // Verifies 3D retail Zapfish and Goal model assets
    bool verifyModelAssets() const;

private:
    std::vector<ZapfishInfo> mZapfishList;
};

} // namespace Game
