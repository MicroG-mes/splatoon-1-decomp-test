#include "Game/Mission/SunkenScrollCatalog.h"
#include "Game/Item/ItemAncientDocument.h"
#include <cstdio>
#include <cstring>
#include <algorithm>

namespace Game {

SunkenScrollCatalog::SunkenScrollCatalog() {
    init();
}

SunkenScrollCatalog::~SunkenScrollCatalog() {}

void SunkenScrollCatalog::registerScroll(const SunkenScrollEntry& entry) {
    mScrolls.push_back(entry);
}

bool SunkenScrollCatalog::init() {
    mScrolls.clear();
    mScrolls.reserve(cMaxScrolls);

    // Area 1: Scrolls 1 to 3 & Boss 1 Blueprint (Scroll 24)
    registerScroll({
        1, 1, 1, "Fld_Athletic00_Msn",
        "Creatures of the Surface",
        ScrollCategory::cCategory_AncientHistory, "Ancient History",
        "Since ancient times, many bizarre creatures have flourished on the surface. Here is an artist's rendition of a creature thought to have lived in antiquity.",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    registerScroll({
        2, 1, 2, "Fld_Labyrinth00_Msn",
        "Territorial Expansion",
        ScrollCategory::cCategory_AncientHistory, "Ancient History",
        "Though cephalopods walked upon land, their struggle for supremacy soon plunged the world into chaotic turf conflicts.",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    registerScroll({
        3, 1, 3, "Fld_Athletic01_Msn",
        "Cephalopod Morphological Evolution",
        ScrollCategory::cCategory_CreatureEcology, "Creature Ecology",
        "Cephalopods underwent unprecedented morphological adaptation, gaining high mobility, ink-propulsion sacs, and bipedal ambulation.",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    // Area 2: Scrolls 4 to 9 & Boss 2 Blueprint (Scroll 25)
    registerScroll({
        4, 2, 4, "Fld_Geyser00_Msn",
        "The Great Turf War: Battle of Arowana",
        ScrollCategory::cCategory_GreatTurfWar, "The Great Turf War",
        "A traditional folding screen depicting the Great Turf War of 100 years ago. Inkling youths and Octarians clash in fierce battle.",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    registerScroll({
        5, 2, 5, "Fld_Sponge00_Msn",
        "The Great Octoweapons",
        ScrollCategory::cCategory_OctarianSociety, "Octarian Society",
        "Octarians engineered gigantic mechanical warfare apparatuses known as Great Octoweapons, heavily armed and powered by captured Zapfish cores.",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    registerScroll({
        6, 2, 6, "Fld_Propeller00_Msn",
        "Cap'n Cuttlefish & The Squidbeak Splatoon",
        ScrollCategory::cCategory_GreatTurfWar, "The Great Turf War",
        "A vintage photograph from the Great Turf War. Cap'n Craig Cuttlefish stands proudly alongside his legendary comrades-in-arms and Judd.",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    registerScroll({
        7, 2, 7, "Fld_Spread00_Msn",
        "The Octarian Plug Incident",
        ScrollCategory::cCategory_GreatTurfWar, "The Great Turf War",
        "A fateful twist of destiny! An unplugged electrical cord disabled the Great Octoweapons on the eve of victory, handing victory to the Inklings.",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    registerScroll({
        8, 2, 8, "Fld_Octa00_Msn",
        "Elite Octolings",
        ScrollCategory::cCategory_OctarianSociety, "Octarian Society",
        "Octarian society features elite female shock troops known as Octolings. Highly intelligent and agile, they mirror Inkling combat capabilities.",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    registerScroll({
        9, 2, 9, "Fld_Ufo00_Msn",
        "Octarian Air Fleet (UFO)",
        ScrollCategory::cCategory_OctarianSociety, "Octarian Society",
        "Surveillance log documenting anomalous unidentified flying saucers roaming the skies, powered by advanced anti-gravity thrusters.",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    // Area 3: Scrolls 10 to 15 & Boss 3 Blueprint (Scroll 26)
    registerScroll({
        10, 3, 10, "Fld_Rail00_Msn",
        "Anatomy of the Modern Inkling",
        ScrollCategory::cCategory_CreatureEcology, "Creature Ecology",
        "Comprehensive biological analysis of the Inkling: squid beak, high-pressure ink sac, dynamic pigmented chromatophores, and extreme super-jump musculature.",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    registerScroll({
        11, 3, 11, "Fld_Invisible00_Msn",
        "Inkopolis Streetwear & Youth Fashion",
        ScrollCategory::cCategory_InkopolisCulture, "Inkopolis Culture",
        "Inkopolis youth revere fresh apparel and brand trends above all else. Gear ability synergies dominate social hierarchies.",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    registerScroll({
        12, 3, 12, "Fld_Cleaner00_Msn",
        "Subterranean Dome Sanctuaries",
        ScrollCategory::cCategory_OctarianSociety, "Octarian Society",
        "A blueprint cross-section of an Octarian underground dome. With internal energy sources collapsing, these habitats face imminent total blackout.",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    registerScroll({
        13, 3, 13, "Fld_Amida00_Msn",
        "Rising Sea Levels: Cataclysm of Antiquity",
        ScrollCategory::cCategory_HumanExtinction, "Human Extinction",
        "Ten thousand years ago, unabated rising sea levels inundated the continental landmasses, driving prehistoric terrestrial civilization to extinction.",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    registerScroll({
        14, 3, 14, "Fld_Octa01_Msn",
        "Judd the Immortal",
        ScrollCategory::cCategory_CreatureEcology, "Creature Ecology",
        "Before the great deluge, a loving professor sealed his beloved cat Judd inside a cryogenic life-support capsule with a 10,000-year timer.",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    registerScroll({
        15, 3, 15, "Fld_Ufo01_Msn",
        "Cryogenic Life-Support Pod Schematics",
        ScrollCategory::cCategory_HumanExtinction, "Human Extinction",
        "Architectural blueprints of Judd's cryogenic hibernation capsule, featuring perpetual life-sustaining nutrient flow.",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    // Area 4: Scrolls 16 to 21 & Boss 4 Blueprint (Scroll 27)
    registerScroll({
        16, 4, 16, "Fld_Propeller01_Msn",
        "The Ancient Human Fossil",
        ScrollCategory::cCategory_HumanExtinction, "Human Extinction",
        "A shocking excavation! The fossilized skeleton of an ancient human found clutching an electronic dual-screen entertainment console.",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    registerScroll({
        17, 4, 17, "Fld_Sniper00_Msn",
        "Super Sea Snail Biology",
        ScrollCategory::cCategory_CreatureEcology, "Creature Ecology",
        "Super Sea Snails harbor miraculous evolutionary pearls capable of unlocking untapped potentials in gear abilities.",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    registerScroll({
        18, 4, 18, "Fld_Crank00_Msn",
        "The Great Zapfish: Source of Electric Power",
        ScrollCategory::cCategory_CreatureEcology, "Creature Ecology",
        "The Great Zapfish is an ancient entity whose bio-electric yield supplies all electricity to Inkopolis Tower and the surrounding metropolis.",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    registerScroll({
        19, 4, 19, "Fld_Cleaner01_Msn",
        "Octarian Tentacle Fission",
        ScrollCategory::cCategory_OctarianSociety, "Octarian Society",
        "Remarkably, Octarians reproduce through asexual tentacle fission. Severed tentacles spontaneously develop into individual Octarian soldiers.",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    registerScroll({
        20, 4, 20, "Fld_Octa02_Msn",
        "DJ Octavio's Underground Radio",
        ScrollCategory::cCategory_OctarianSociety, "Octarian Society",
        "DJ Octavio uses heavy 8-beat basslines and wasabi-charged music broadcasts to maintain ideological grip over the Octarian legions.",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    registerScroll({
        21, 4, 21, "Fld_Ufo02_Msn",
        "Human Extinction Epitaph",
        ScrollCategory::cCategory_HumanExtinction, "Human Extinction",
        "A preserved transmission left behind by doomed humanity: 'Farewell, surface world. May whatever life arises next treat this planet more gently than we did.'",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    // Area 5: Scrolls 22 to 23 & Boss 5 Blueprint (Scroll 28)
    registerScroll({
        22, 5, 22, "Fld_Switch00_Msn",
        "Underground Dome Structural Warning",
        ScrollCategory::cCategory_OctarianSociety, "Octarian Society",
        "Structural integrity reports show ceiling collapses occurring across all Octarian residential domes due to failing power supplies.",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    registerScroll({
        23, 5, 23, "Fld_Sponge01_Msn",
        "Young Squid Sisters Photograph",
        ScrollCategory::cCategory_InkopolisCulture, "Inkopolis Culture",
        "A heartwarming photograph of Callie and Marie in their childhood, having just won the Calamari County Regional Folk Song Contest.",
        false, "", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    // Boss Blueprints (Scrolls 24 to 28)
    registerScroll({
        24, 1, 24, "Fld_BossStampKing_Bos_Msn",
        "Weapon Blueprint: Custom Splattershot Jr.",
        ScrollCategory::cCategory_WeaponBlueprint, "Weapon Blueprint",
        "Antique weapon blueprints deciphered by Sheldon at Ammo Knights to fabricate the Custom Splattershot Jr.",
        true, "Custom Splattershot Jr.", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    registerScroll({
        25, 2, 25, "Fld_BossBlowKing_Bos_Msn",
        "Weapon Blueprint: Kelp Splat Charger",
        ScrollCategory::cCategory_WeaponBlueprint, "Weapon Blueprint",
        "Specialized military charger blueprints decoded by Sheldon to forge the Kelp Splat Charger.",
        true, "Kelp Splat Charger", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    registerScroll({
        26, 3, 26, "Fld_BossBallKing_Bos_Msn",
        "Weapon Blueprint: Aerospray MG & RG",
        ScrollCategory::cCategory_WeaponBlueprint, "Weapon Blueprint",
        "Advanced aerodynamical nozzle schematics delivered to Ammo Knights to reconstruct the rapid-firing Aerospray MG and Aerospray RG.",
        true, "Aerospray MG", "Aerospray RG",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    registerScroll({
        27, 4, 27, "Fld_BossMawKing_Bos_Msn",
        "Weapon Blueprint: New Squiffer",
        ScrollCategory::cCategory_WeaponBlueprint, "Weapon Blueprint",
        "High-pressure classic squiffer schematics decoded by Sheldon to develop the New Squiffer.",
        true, "New Squiffer", "",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    registerScroll({
        28, 5, 28, "Fld_BossRailKing_Bos_Msn",
        "Weapon Blueprint: Dynamo Roller & Gold Dynamo",
        ScrollCategory::cCategory_WeaponBlueprint, "Weapon Blueprint",
        "Colossal heavy-duty power motor roller schematics retrieved from DJ Octavio's defeat, unlocking the devastating Dynamo Roller and Gold Dynamo Roller.",
        true, "Dynamo Roller", "Gold Dynamo Roller",
        "Obj_AncientDocument", "Obj_AncientDocumentDummy", false
    });

    return (mScrolls.size() == cMaxScrolls);
}

const SunkenScrollEntry* SunkenScrollCatalog::getScroll(u32 scrollId) const {
    if (scrollId == 0 || scrollId > mScrolls.size()) {
        return nullptr;
    }
    return &mScrolls[scrollId - 1];
}

const SunkenScrollEntry* SunkenScrollCatalog::getScrollByMission(u32 missionNo) const {
    for (const auto& s : mScrolls) {
        if (s.missionNo == missionNo) {
            return &s;
        }
    }
    return nullptr;
}

std::vector<const SunkenScrollEntry*> SunkenScrollCatalog::getScrollsByArea(u32 areaNo) const {
    std::vector<const SunkenScrollEntry*> result;
    for (const auto& s : mScrolls) {
        if (s.areaNo == areaNo) {
            result.push_back(&s);
        }
    }
    return result;
}

std::vector<const SunkenScrollEntry*> SunkenScrollCatalog::getScrollsByCategory(ScrollCategory category) const {
    std::vector<const SunkenScrollEntry*> result;
    for (const auto& s : mScrolls) {
        if (s.category == category) {
            result.push_back(&s);
        }
    }
    return result;
}

std::vector<const SunkenScrollEntry*> SunkenScrollCatalog::getBlueprintScrolls() const {
    std::vector<const SunkenScrollEntry*> result;
    for (const auto& s : mScrolls) {
        if (s.isBlueprint) {
            result.push_back(&s);
        }
    }
    return result;
}

std::vector<const SunkenScrollEntry*> SunkenScrollCatalog::getLoreScrolls() const {
    std::vector<const SunkenScrollEntry*> result;
    for (const auto& s : mScrolls) {
        if (!s.isBlueprint) {
            result.push_back(&s);
        }
    }
    return result;
}

bool SunkenScrollCatalog::isCollected(u32 scrollId) const {
    const auto* s = getScroll(scrollId);
    return s ? s->collected : false;
}

bool SunkenScrollCatalog::setCollected(u32 scrollId, bool collected) {
    if (scrollId == 0 || scrollId > mScrolls.size()) {
        return false;
    }
    mScrolls[scrollId - 1].collected = collected;
    return true;
}

size_t SunkenScrollCatalog::getCollectedCount() const {
    size_t count = 0;
    for (const auto& s : mScrolls) {
        if (s.collected) count++;
    }
    return count;
}

f32 SunkenScrollCatalog::getCollectionPercentage() const {
    if (mScrolls.empty()) return 0.0f;
    return (static_cast<f32>(getCollectedCount()) / static_cast<f32>(mScrolls.size())) * 100.0f;
}

bool SunkenScrollCatalog::isWeaponUnlockedByScroll(const char* weaponName) const {
    if (!weaponName) return false;
    for (const auto& s : mScrolls) {
        if (s.isBlueprint && s.collected) {
            if (s.unlockedWeapon == weaponName || s.unlockedWeaponSub == weaponName) {
                return true;
            }
        }
    }
    return false;
}

bool SunkenScrollCatalog::linkToItem(ItemAncientDocument* item, u32 scrollId) {
    if (!item || scrollId == 0 || scrollId > cMaxScrolls) return false;
    if (item->isCollected()) {
        return setCollected(scrollId, true);
    }
    return false;
}

bool SunkenScrollCatalog::verifyModelAssets() const {
    FILE* f1 = fopen("content/Model/Obj_AncientDocument.szs", "rb");
    if (!f1) return false;
    fclose(f1);

    FILE* f2 = fopen("content/Model/Obj_AncientDocumentDummy.szs", "rb");
    if (!f2) return false;
    fclose(f2);

    return true;
}

} // namespace Game
