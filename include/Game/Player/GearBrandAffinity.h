#pragma once

#include "types.h"
#include <string>
#include <vector>

namespace Game {

enum class GearBrand : u32 {
    cSquidForce  = 0,
    cZekko       = 1,
    cKrakOn      = 2,
    cRockenberg  = 3,
    cTentatek    = 4,
    cFirefin     = 5,
    cSkalop      = 6,
    cSplashMob   = 7,
    cForge       = 8,
    cTakoroka    = 9,
    cZink        = 10,
    cCuttlegear  = 11,
    cAmiibo      = 12,
    cKOG         = 13
};

enum class SubAbilityType : u32 {
    cDamageUp            = 0,
    cDefenseUp           = 1,
    cInkSaverMain        = 2,
    cInkSaverSub         = 3,
    cInkRecoveryUp       = 4,
    cRunSpeedUp          = 5,
    cSwimSpeedUp         = 6,
    cSpecialChargeUp     = 7,
    cSpecialDurationUp   = 8,
    cSpecialSaver        = 9,
    cQuickRespawn        = 10,
    cQuickSuperJump      = 11,
    cBombSniffer         = 12, // Sub count = 13
    cCount               = 13
};

struct BrandAffinityInfo {
    GearBrand brand;
    const char* brandName;
    SubAbilityType favoredAbility;    // 5x weight (10 / 33 = ~30.3%)
    SubAbilityType unfavoredAbility;  // 0.5x weight (1 / 33 = ~3.0%)
    bool isNeutral;
};

struct GearItem {
    std::string name;
    GearBrand brand;
    u32 stars;            // 1 to 3 stars
    u32 unlockedSlots;    // 1 to 3 slots
    SubAbilityType mainAbility;
    SubAbilityType subAbilities[3];
};

class GearBrandAffinity {
public:
    static const BrandAffinityInfo* getBrandInfo(GearBrand brand);
    static f32 getAbilityProbability(GearBrand brand, SubAbilityType ability);

    // Roll a sub ability given a gear brand and a random seed [0, 1)
    static SubAbilityType rollSubAbility(GearBrand brand, f32 randomNormalized);
};

// Spyke (Downey) Inkopolis Plaza Service
class Npc_CustomShop_Spyke {
public:
    static constexpr u32 cSnailRerollCost = 1;
    static constexpr u32 cCashRerollCost = 30000;

    Npc_CustomShop_Spyke();
    ~Npc_CustomShop_Spyke();

    void init(u32 initialSnails = 3, u32 initialCash = 100000);

    // Add slot to gear (max 3)
    bool addGearSlot(GearItem& gear, bool useSnail);

    // Reroll all unlocked sub-abilities
    bool rerollGear(GearItem& gear, bool useSnail, u32 randomSeed);

    u32 getSuperSeaSnails() const { return mSuperSeaSnails; }
    u32 getCash() const { return mCash; }
    void addSnails(u32 count) { mSuperSeaSnails += count; }
    void addCash(u32 amount) { mCash += amount; }

private:
    u32 mSuperSeaSnails;
    u32 mCash;
};

} // namespace Game
