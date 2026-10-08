#pragma once

#include "types.h"
#include <string>

namespace Game {

enum class KindOfShop : u32 {
    cShoes   = 0, // Shrimp Kicks (Crusty Sean)
    cClothes = 1, // Jelly Fresh (Jelonzo)
    cHead    = 2, // Cooler Heads (Annie & Moe)
    cWeapon  = 3  // Ammo Knights (Sheldon)
};

enum class BrandId : u32 {
    cSquidForce  = 0,
    cZekko       = 1,
    cKrakOn      = 2,
    cRockenberg  = 3,
    cTentatek    = 4,
    cSplashMob   = 5,
    cInkline     = 6,
    cTakoroka    = 7,
    cFirefin     = 8,
    cSkalop      = 9,
    cWarmwood    = 10,
    cForge       = 11,
    cToniKensa   = 12,
    cZink        = 13,
    cCuttlegear  = 14,
    cAmiibo      = 15
};

enum class AbilityId : u32 {
    DamageUp           = 0,
    DefenseUp          = 1,
    InkSaverMain       = 2,
    InkSaverSub        = 3,
    InkRecoveryUp      = 4,
    RunSpeedUp         = 5,
    SwimSpeedUp        = 6,
    SpecialChargeUp    = 7,
    SpecialDurationUp  = 8,
    SpecialSaver       = 9,
    QuickRespawn       = 10,
    QuickSuperJump     = 11,
    BombRangeUp        = 12,
    OpeningGambit      = 13,
    LastDitchEffort    = 14,
    Tenacity           = 15,
    Comeback           = 16,
    ColdBlooded        = 17,
    NinjaSquid         = 18,
    Haunt              = 19,
    InkResistanceUp    = 20,
    StealthJump        = 21,
    BombSniffer        = 22,
    None               = 0xFF
};

struct ShopItem {
    u32 id;
    KindOfShop kind;
    BrandId brand;
    u32 starCount;          // 1 to 3 stars
    AbilityId mainAbility;
    AbilityId subAbilities[3];
    u32 price;
    u32 unlockLevel;
    bool isOwned;

    // For weapons
    u32 subWeaponId;
    u32 specialWeaponId;
    f32 rangeStat;
    f32 damageStat;
    f32 fireRateStat;
};

} // namespace Game
