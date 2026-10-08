#pragma once

#include "types.h"
#include <string>
#include <vector>

namespace Game {

enum class DuelDropItemType : u32 {
    cBarrier   = 0,
    cDevil     = 1,
    cMarking   = 2,
    cPowerUp   = 3,
    cSprinkler = 4,
    cSuperJump = 5,
    cSuperShot = 6,
    cTornade   = 7,
    cNone      = 8
};

struct DuelItemProbEntry {
    s32 minPointDiff;
    s32 maxPointDiff;
    u32 barrier;
    u32 devil;
    u32 marking;
    u32 powerUp;
    u32 sprinkler;
    u32 superJump;
    u32 superShot;
    u32 tornade;
};

struct DuelPresetLoadout {
    u32 id;
    std::string weaponSet;
    std::string head;
    std::string clothes;
    std::string shoes;
};

/**
 * DuelItemTableMgr
 * Authentic Battle Dojo (Balloon Battle) dynamic D100 item drop probability engine
 * and 8-preset player loadouts from DuelItemTable.byaml and DuelPlayerSetting.byaml.
 */
class DuelItemTableMgr {
public:
    DuelItemTableMgr();
    ~DuelItemTableMgr();

    bool loadFromByml(const char* itemTableByml, const char* playerSettingByml);

    const DuelItemProbEntry* getEntryForPointDiff(s32 pointDiff) const;
    DuelDropItemType rollDropItem(s32 pointDiff, u32 roll100) const;

    size_t getProbabilityEntryCount() const { return mProbEntries.size(); }
    size_t getPresetLoadoutCount() const { return mPresetLoadouts.size(); }

    const DuelPresetLoadout* getPresetLoadout(u32 id) const;

private:
    std::vector<DuelItemProbEntry> mProbEntries;
    std::vector<DuelPresetLoadout> mPresetLoadouts;
};

} // namespace Game
