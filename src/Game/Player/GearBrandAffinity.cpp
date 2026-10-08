#include "Game/Player/GearBrandAffinity.h"
#include <algorithm>
#include <cmath>

namespace Game {

static const BrandAffinityInfo s_BrandTable[] = {
    { GearBrand::cSquidForce, "SquidForce", SubAbilityType::cDamageUp,          SubAbilityType::cInkSaverSub,         false },
    { GearBrand::cZekko,      "Zekko",      SubAbilityType::cSpecialSaver,       SubAbilityType::cSpecialChargeUp,     false },
    { GearBrand::cKrakOn,     "Krak-On",    SubAbilityType::cSwimSpeedUp,        SubAbilityType::cDefenseUp,           false },
    { GearBrand::cRockenberg, "Rockenberg", SubAbilityType::cRunSpeedUp,         SubAbilityType::cSwimSpeedUp,         false },
    { GearBrand::cTentatek,   "Tentatek",   SubAbilityType::cInkRecoveryUp,      SubAbilityType::cQuickSuperJump,      false },
    { GearBrand::cFirefin,    "Firefin",    SubAbilityType::cInkSaverSub,        SubAbilityType::cInkRecoveryUp,       false },
    { GearBrand::cSkalop,     "Skalop",     SubAbilityType::cQuickRespawn,       SubAbilityType::cSpecialSaver,        false },
    { GearBrand::cSplashMob,  "Splash Mob", SubAbilityType::cInkSaverMain,       SubAbilityType::cRunSpeedUp,          false },
    { GearBrand::cForge,      "Forge",      SubAbilityType::cSpecialDurationUp,  SubAbilityType::cInkSaverSub,         false },
    { GearBrand::cTakoroka,   "Takoroka",   SubAbilityType::cSpecialChargeUp,    SubAbilityType::cSpecialDurationUp,   false },
    { GearBrand::cZink,       "Zink",       SubAbilityType::cQuickSuperJump,     SubAbilityType::cQuickRespawn,        false },
    { GearBrand::cCuttlegear, "Cuttlegear", SubAbilityType::cDamageUp,          SubAbilityType::cDamageUp,            true  },
    { GearBrand::cAmiibo,     "Amiibo",     SubAbilityType::cDamageUp,          SubAbilityType::cDamageUp,            true  },
    { GearBrand::cKOG,        "KOG",        SubAbilityType::cDamageUp,          SubAbilityType::cDamageUp,            true  }
};

const BrandAffinityInfo* GearBrandAffinity::getBrandInfo(GearBrand brand) {
    u32 idx = static_cast<u32>(brand);
    if (idx < sizeof(s_BrandTable) / sizeof(s_BrandTable[0])) {
        return &s_BrandTable[idx];
    }
    return &s_BrandTable[0];
}

f32 GearBrandAffinity::getAbilityProbability(GearBrand brand, SubAbilityType ability) {
    const auto* info = getBrandInfo(brand);
    if (info->isNeutral) {
        return 1.0f / 13.0f; // Equal 1/13 chance (~7.69%)
    }

    if (ability == info->favoredAbility) {
        return 10.0f / 33.0f; // 5x favored weight (~30.30%)
    } else if (ability == info->unfavoredAbility) {
        return 1.0f / 33.0f;  // 0.5x unfavored weight (~3.03%)
    } else {
        return 2.0f / 33.0f;  // Normal neutral weight (~6.06%)
    }
}

SubAbilityType GearBrandAffinity::rollSubAbility(GearBrand brand, f32 randomNormalized) {
    f32 r = (std::max)(0.0f, (std::min)(0.999999f, randomNormalized));
    f32 cumulative = 0.0f;

    for (u32 i = 0; i < 13; ++i) {
        SubAbilityType ab = static_cast<SubAbilityType>(i);
        cumulative += getAbilityProbability(brand, ab);
        if (r < cumulative) {
            return ab;
        }
    }
    return SubAbilityType::cDamageUp;
}

// -----------------------------------------------------------------------------
// Npc_CustomShop_Spyke
// -----------------------------------------------------------------------------

Npc_CustomShop_Spyke::Npc_CustomShop_Spyke()
    : mSuperSeaSnails(3)
    , mCash(100000)
{
}

Npc_CustomShop_Spyke::~Npc_CustomShop_Spyke() {
}

void Npc_CustomShop_Spyke::init(u32 initialSnails, u32 initialCash) {
    mSuperSeaSnails = initialSnails;
    mCash = initialCash;
}

bool Npc_CustomShop_Spyke::addGearSlot(GearItem& gear, bool useSnail) {
    if (gear.unlockedSlots >= 3) return false;

    if (useSnail) {
        if (mSuperSeaSnails < cSnailRerollCost) return false;
        mSuperSeaSnails -= cSnailRerollCost;
    } else {
        if (mCash < cCashRerollCost) return false;
        mCash -= cCashRerollCost;
    }

    // Unlock new slot and roll its initial ability
    gear.unlockedSlots++;
    f32 dummyR = static_cast<f32>((gear.unlockedSlots * 12345) % 1000) / 1000.0f;
    gear.subAbilities[gear.unlockedSlots - 1] = GearBrandAffinity::rollSubAbility(gear.brand, dummyR);
    return true;
}

bool Npc_CustomShop_Spyke::rerollGear(GearItem& gear, bool useSnail, u32 randomSeed) {
    if (gear.unlockedSlots < 3) return false;

    if (useSnail) {
        if (mSuperSeaSnails < cSnailRerollCost) return false;
        mSuperSeaSnails -= cSnailRerollCost;
    } else {
        if (mCash < cCashRerollCost) return false;
        mCash -= cCashRerollCost;
    }

    // Reroll all 3 sub ability slots with seed
    u32 seed = randomSeed;
    for (u32 i = 0; i < 3; ++i) {
        seed = seed * 1664525u + 1013904223u; // Linear Congruential Generator
        f32 r = static_cast<f32>(seed & 0xFFFF) / 65536.0f;
        gear.subAbilities[i] = GearBrandAffinity::rollSubAbility(gear.brand, r);
    }

    return true;
}

} // namespace Game
