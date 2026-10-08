#pragma once

#include "types.h"
#include <string>
#include <vector>

namespace Game {

class ItemAncientDocument;

enum class ScrollCategory : u32 {
    cCategory_CreatureEcology = 0,
    cCategory_GreatTurfWar,
    cCategory_AncientHistory,
    cCategory_HumanExtinction,
    cCategory_OctarianSociety,
    cCategory_InkopolisCulture,
    cCategory_WeaponBlueprint
};

struct SunkenScrollEntry {
    u32 scrollId;                   // 1 to 28
    u32 areaNo;                     // 1 to 5
    u32 missionNo;                  // 1 to 27 (or 28 for Final Boss)
    std::string stageMapName;       // Stage map file name (e.g. Fld_Athletic00_Msn, Fld_BossRailKing_Bos_Msn)
    std::string title;              // Historical title of the Sunken Scroll
    ScrollCategory category;        // Scroll thematic category
    std::string categoryName;       // Readable category name
    std::string loreText;           // Authentic translated narrative text & lore
    bool isBlueprint;               // True if scroll is a weapon blueprint awarded from bosses
    std::string unlockedWeapon;     // Weapon name unlocked at Ammo Knights
    std::string unlockedWeaponSub;  // Secondary variant unlocked
    std::string modelName;          // "Obj_AncientDocument"
    std::string dummyModelName;     // "Obj_AncientDocumentDummy"
    bool collected;                 // Collected status in player campaign save
};

/**
 * SunkenScrollCatalog
 * Authentic Single Player Sunken Scroll Lore & Archive Database (28 Scrolls).
 * Catalogs all ancient documents, lore entries, stage associations, and Sheldon weapon blueprints.
 */
class SunkenScrollCatalog {
public:
    static constexpr u32 cMaxScrolls = 28;
    static constexpr u32 cBlueprintScrolls = 5;
    static constexpr u32 cLoreScrolls = 23;

    SunkenScrollCatalog();
    ~SunkenScrollCatalog();

    bool init();

    size_t getTotalScrollCount() const { return mScrolls.size(); }
    const SunkenScrollEntry* getScroll(u32 scrollId) const;
    const SunkenScrollEntry* getScrollByMission(u32 missionNo) const;

    std::vector<const SunkenScrollEntry*> getScrollsByArea(u32 areaNo) const;
    std::vector<const SunkenScrollEntry*> getScrollsByCategory(ScrollCategory category) const;
    std::vector<const SunkenScrollEntry*> getBlueprintScrolls() const;
    std::vector<const SunkenScrollEntry*> getLoreScrolls() const;

    // Collection management
    bool isCollected(u32 scrollId) const;
    bool setCollected(u32 scrollId, bool collected = true);
    size_t getCollectedCount() const;
    f32 getCollectionPercentage() const;

    // Checks whether a weapon blueprint has been recovered for a given weapon
    bool isWeaponUnlockedByScroll(const char* weaponName) const;

    // Links with an active ItemAncientDocument actor
    bool linkToItem(ItemAncientDocument* item, u32 scrollId);

    // Verifies 3D model assets on disk
    bool verifyModelAssets() const;

private:
    std::vector<SunkenScrollEntry> mScrolls;
    void registerScroll(const SunkenScrollEntry& entry);
};

} // namespace Game
