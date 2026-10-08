#pragma once

#include "types.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class StageCategory : u32 {
    cVersus     = 0, // 4v4 Turf War & Ranked Battle stages
    cHub        = 1, // Inkopolis Plaza, Tower Lobby, Shooting Range
    cShop       = 2, // Booyah Base shops
    cSplatfest  = 3, // Night festival stages
    cOctoValley = 4, // Hero Mode single-player missions
    cOctoBoss   = 5  // Hero Mode boss battle arenas
};

enum class StageId : u32 {
    // -------------------------------------------------------------
    // Multiplayer Competitive Stages (16 stages)
    // -------------------------------------------------------------
    cStage_Crank00_Vss       = 0,  // Urchin Underpass (Original & Splatfest)
    cStage_Warehouse00_Vss   = 1,  // Walleye Warehouse
    cStage_SeaPlant00_Vss    = 2,  // Saltspray Rig
    cStage_Amida00_Vss       = 3,  // Arowana Mall
    cStage_SkatePark00_Vss   = 4,  // Blackbelly Skatepark
    cStage_Maze00_Vss        = 5,  // Kelp Dome
    cStage_UpDown00_Vss      = 6,  // Moray Towers
    cStage_Hiagari00_Vss     = 7,  // Camp Triggerfish
    cStage_Tuzura00_Vss      = 8,  // Flounder Heights
    cStage_Koke00_Vss        = 9,  // Hammerhead Bridge
    cStage_Pivot00_Vss       = 10, // Museum d'Alfonsino
    cStage_Night00_Vss       = 11, // Mahi-Mahi Resort
    cStage_Quarry00_Vss      = 12, // Piranha Pit
    cStage_Ruins00_Vss       = 13, // Ancho-V Games
    cStage_Tunnel00_Vss      = 14, // Bluefin Depot
    cStage_Crank00_Dul       = 15, // Battle Dojo (Urchin Underpass 2P)

    // -------------------------------------------------------------
    // Hubs, Lobby, Testing & Story Sequences
    // -------------------------------------------------------------
    cStage_ShootingRange_Shr = 16, // Weapon Testing Area (Shooting Range)
    cStage_Plaza00_Plz       = 17, // Inkopolis Plaza
    cStage_PlazaLobby        = 18, // Deca Tower Battle Lobby
    cStage_MatchRoom         = 19, // Online Matchmaking Waiting Room
    cStage_Tutorial00_Ttr    = 20, // Tutorial Course
    cStage_StaffRoll00_Stf   = 21, // Credits Course

    // -------------------------------------------------------------
    // Booyah Base Shops
    // -------------------------------------------------------------
    cStage_Room_Weapons      = 22, // Ammo Knights (Sheldon)
    cStage_Room_Gear         = 23, // Cooler Heads (Annie & Moe)
    cStage_Room_Tshirts      = 24, // Jelly Fresh (Jelonzo)
    cStage_Room_Shoes        = 25, // Shrimp Kicks (Crusty Sean)

    // -------------------------------------------------------------
    // Splatfest Festival Variations
    // -------------------------------------------------------------
    cStage_PlazaEvent00      = 26, // Splatfest Plaza Festival Night
    cStage_PlazaEvent01      = 27, // Splatfest Concert Stage Alpha
    cStage_PlazaEvent02      = 28, // Splatfest Concert Stage Bravo

    // -------------------------------------------------------------
    // Octo Valley (Hero Mode) Hub & Boss Encounters
    // -------------------------------------------------------------
    cStage_World00_Wld       = 29, // Octo Valley Hub (Sectors 1-5)
    cStage_BossStamp         = 30, // The Mighty Octostamp Arena (Boss 1)
    cStage_BossBall          = 31, // The Dread Roll / Octowhirl Arena (Boss 2)
    cStage_BossCylinder      = 32, // The Ravenous Octonozzle Arena (Boss 3)
    cStage_BossMouth         = 33, // The Dread Skewer / Octomaw Arena (Boss 4)
    cStage_BossRailKing      = 34, // Enter the Octobot King / DJ Octavio Arena (Final Boss)

    // -------------------------------------------------------------
    // Octo Valley Missions & Octoling Infiltrations
    // -------------------------------------------------------------
    cStage_OctZero00_Msn     = 35, // Octo Valley Level 1 (First Contact)
    cStage_OctCrank00_Msn    = 36, // Octo Valley Level 2 (Underpass Infiltration)
    cStage_OctRuins00_Msn    = 37, // Octo Valley Level 3 (Ruins Infiltration)
    cStage_OctSkatePark00_Msn= 38, // Octo Valley Level 4 (Skatepark Infiltration)
    cStage_RvlMaze00_Msn     = 39, // Octoling Rival (Kelp Dome Battle)
    cStage_RvlSeaPlant00_Msn = 40, // Octoling Rival (Saltspray Rig Battle)
    cStage_RvlSkatePark00_Msn= 41, // Octoling Rival (Skatepark Battle)
    cStage_RvlRuins00_Msn    = 42, // Octoling Rival (Ancho-V Games Battle)

    cStage_Count             = 43
};

struct StageInfo {
    StageId         id;
    const char*     codeName;       // Internal Wii U ELF string (e.g. "Fld_Warehouse00_Vss")
    const char*     displayName;    // Official English name (e.g. "Walleye Warehouse")
    const char*     japaneseName;   // Original dev name (e.g. "Hakofugu Warehouse")
    StageCategory   category;
    sead::Vector3f  boundsMin;      // Stage boundary minimum (X, Y, Z)
    sead::Vector3f  boundsMax;      // Stage boundary maximum (X, Y, Z)
    sead::Vector3f  spawnAlpha;      // Team Alpha respawn base coordinate
    sead::Vector3f  spawnBravo;      // Team Bravo respawn base coordinate
    sead::Vector3f  objectiveCenter;// Central tower/zone/rainmaker coordinate
    u32             floorColor;     // Base floor visual color (RGB)
};

class StageDef {
public:
    static const StageInfo* getStageInfo(StageId id);
    static const StageInfo* getStageInfoByCodeName(const char* codeName);
    static const StageInfo* getStageInfoByName(const char* name);
    static u32 getStageCount();
    static const StageInfo* getAllStages();
};

} // namespace Game
