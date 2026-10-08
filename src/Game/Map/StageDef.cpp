#include "Game/Map/StageDef.h"
#include <cstring>
#include <cctype>

namespace Game {

static const StageInfo sStageTable[] = {
    // -------------------------------------------------------------
    // Multiplayer Competitive Stages (16 stages)
    // -------------------------------------------------------------
    {
        StageId::cStage_Crank00_Vss,
        "Fld_Crank00_Vss",
        "Urchin Underpass",
        "Dekarainbow",
        StageCategory::cVersus,
        sead::Vector3f(-45.0f, -5.0f, -25.0f),
        sead::Vector3f( 45.0f, 15.0f,  25.0f),
        sead::Vector3f(-38.0f,  1.0f,   0.0f),
        sead::Vector3f( 38.0f,  1.0f,   0.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF333842
    },
    {
        StageId::cStage_Warehouse00_Vss,
        "Fld_Warehouse00_Vss",
        "Walleye Warehouse",
        "Hakofugu Warehouse",
        StageCategory::cVersus,
        sead::Vector3f(-40.0f, -5.0f, -20.0f),
        sead::Vector3f( 40.0f, 15.0f,  20.0f),
        sead::Vector3f(-34.0f,  1.0f,   0.0f),
        sead::Vector3f( 34.0f,  1.0f,   0.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF2C3E50
    },
    {
        StageId::cStage_SeaPlant00_Vss,
        "Fld_SeaPlant00_Vss",
        "Saltspray Rig",
        "Shiokaze Rig",
        StageCategory::cVersus,
        sead::Vector3f(-30.0f, -5.0f, -45.0f),
        sead::Vector3f( 30.0f, 20.0f,  45.0f),
        sead::Vector3f(-18.0f,  1.0f, -38.0f),
        sead::Vector3f( 18.0f,  1.0f, -38.0f),
        sead::Vector3f(  0.0f,  4.0f,  15.0f),
        0xFF1A365D
    },
    {
        StageId::cStage_Amida00_Vss,
        "Fld_Amida00_Vss",
        "Arowana Mall",
        "Amida Mall",
        StageCategory::cVersus,
        sead::Vector3f(-50.0f, -5.0f, -18.0f),
        sead::Vector3f( 50.0f, 18.0f,  18.0f),
        sead::Vector3f(-42.0f,  2.0f,   0.0f),
        sead::Vector3f( 42.0f,  2.0f,   0.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF4A5568
    },
    {
        StageId::cStage_SkatePark00_Vss,
        "Fld_SkatePark00_Vss",
        "Blackbelly Skatepark",
        "B-Bus Skatepark",
        StageCategory::cVersus,
        sead::Vector3f(-35.0f, -5.0f, -35.0f),
        sead::Vector3f( 35.0f, 20.0f,  35.0f),
        sead::Vector3f(-28.0f,  1.0f,   0.0f),
        sead::Vector3f( 28.0f,  1.0f,   0.0f),
        sead::Vector3f(  0.0f,  5.0f,   0.0f), // Iconic central tower hill
        0xFF2D3748
    },
    {
        StageId::cStage_Maze00_Vss,
        "Fld_Maze00_Vss",
        "Kelp Dome",
        "Mozuku Farm",
        StageCategory::cVersus,
        sead::Vector3f(-38.0f, -5.0f, -38.0f),
        sead::Vector3f( 38.0f, 15.0f,  38.0f),
        sead::Vector3f(-32.0f,  1.0f,   0.0f),
        sead::Vector3f( 32.0f,  1.0f,   0.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF285E61
    },
    {
        StageId::cStage_UpDown00_Vss,
        "Fld_UpDown00_Vss",
        "Moray Towers",
        "Tachiuo Parking",
        StageCategory::cVersus,
        sead::Vector3f(-25.0f, -10.0f, -60.0f),
        sead::Vector3f( 25.0f,  45.0f,  60.0f),
        sead::Vector3f(  0.0f,  38.0f, -50.0f), // High rooftop spawn Alpha
        sead::Vector3f(  0.0f,  38.0f,  50.0f), // High rooftop spawn Bravo
        sead::Vector3f(  0.0f,   0.0f,   0.0f), // Valley floor
        0xFF1A202C
    },
    {
        StageId::cStage_Hiagari00_Vss,
        "Fld_Hiagari00_Vss",
        "Camp Triggerfish",
        "Hiagari Camp",
        StageCategory::cVersus,
        sead::Vector3f(-55.0f, -8.0f, -28.0f),
        sead::Vector3f( 55.0f, 15.0f,  28.0f),
        sead::Vector3f(-46.0f,  2.0f,   0.0f),
        sead::Vector3f( 46.0f,  2.0f,   0.0f),
        sead::Vector3f(  0.0f, -2.0f,   0.0f), // Lake flood gates
        0xFF276749
    },
    {
        StageId::cStage_Tuzura00_Vss,
        "Fld_Tuzura00_Vss",
        "Flounder Heights",
        "Hirame Gaoka",
        StageCategory::cVersus,
        sead::Vector3f(-40.0f, -5.0f, -30.0f),
        sead::Vector3f( 40.0f, 30.0f,  30.0f),
        sead::Vector3f(-32.0f,  1.0f,   0.0f),
        sead::Vector3f( 32.0f,  1.0f,   0.0f),
        sead::Vector3f(  0.0f, 15.0f,   0.0f), // Apartment rooftops
        0xFF744210
    },
    {
        StageId::cStage_Koke00_Vss,
        "Fld_Koke00_Vss",
        "Hammerhead Bridge",
        "Masaba Bridge",
        StageCategory::cVersus,
        sead::Vector3f(-60.0f, -5.0f, -15.0f),
        sead::Vector3f( 60.0f, 20.0f,  15.0f),
        sead::Vector3f(-52.0f,  2.0f,   0.0f),
        sead::Vector3f( 52.0f,  2.0f,   0.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF7B341E
    },
    {
        StageId::cStage_Pivot00_Vss,
        "Fld_Pivot00_Vss",
        "Museum d'Alfonsino",
        "Kinmedai Museum",
        StageCategory::cVersus,
        sead::Vector3f(-42.0f, -5.0f, -42.0f),
        sead::Vector3f( 42.0f, 18.0f,  42.0f),
        sead::Vector3f(-35.0f,  1.0f,   0.0f),
        sead::Vector3f( 35.0f,  1.0f,   0.0f),
        sead::Vector3f(  0.0f,  2.0f,   0.0f), // Central rotating propeller
        0xFF4C1D95
    },
    {
        StageId::cStage_Night00_Vss,
        "Fld_Night00_Vss",
        "Mahi-Mahi Resort",
        "Mahi-Mahi Resort",
        StageCategory::cVersus,
        sead::Vector3f(-36.0f, -6.0f, -36.0f),
        sead::Vector3f( 36.0f, 15.0f,  36.0f),
        sead::Vector3f(-28.0f,  1.0f,   0.0f),
        sead::Vector3f( 28.0f,  1.0f,   0.0f),
        sead::Vector3f(  0.0f, -1.0f,   0.0f), // Water drainage pool
        0xFF0F766E
    },
    {
        StageId::cStage_Quarry00_Vss,
        "Fld_Quarry00_Vss",
        "Piranha Pit",
        "Choshizame Quarry",
        StageCategory::cVersus,
        sead::Vector3f(-50.0f, -5.0f, -35.0f),
        sead::Vector3f( 50.0f, 15.0f,  35.0f),
        sead::Vector3f(-42.0f,  1.0f,   0.0f),
        sead::Vector3f( 42.0f,  1.0f,   0.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f), // Industrial belt hub
        0xFF854D0E
    },
    {
        StageId::cStage_Ruins00_Vss,
        "Fld_Ruins00_Vss",
        "Ancho-V Games",
        "Anchovy Office",
        StageCategory::cVersus,
        sead::Vector3f(-44.0f, -5.0f, -30.0f),
        sead::Vector3f( 44.0f, 22.0f,  30.0f),
        sead::Vector3f(-36.0f,  1.0f,   0.0f),
        sead::Vector3f( 36.0f,  1.0f,   0.0f),
        sead::Vector3f(  0.0f,  4.0f,   0.0f), // Propeller fan lifts
        0xFF1E293B
    },
    {
        StageId::cStage_Tunnel00_Vss,
        "Fld_Tunnel00_Vss",
        "Bluefin Depot",
        "Hokke Pier",
        StageCategory::cVersus,
        sead::Vector3f(-45.0f, -12.0f, -25.0f),
        sead::Vector3f( 45.0f,  15.0f,  25.0f),
        sead::Vector3f(-36.0f,   8.0f,   0.0f), // Elevated minecart bridge
        sead::Vector3f( 36.0f,   8.0f,   0.0f),
        sead::Vector3f(  0.0f,  -6.0f,   0.0f), // Low flooded valley
        0xFF334155
    },
    {
        StageId::cStage_Crank00_Dul,
        "Fld_Crank00_Dul",
        "Battle Dojo (Underpass)",
        "Dojo Underpass",
        StageCategory::cVersus,
        sead::Vector3f(-35.0f, -5.0f, -20.0f),
        sead::Vector3f( 35.0f, 15.0f,  20.0f),
        sead::Vector3f(-25.0f,  1.0f,   0.0f),
        sead::Vector3f( 25.0f,  1.0f,   0.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF3F3F46
    },

    // -------------------------------------------------------------
    // Hubs, Lobby, Testing & Story Sequences
    // -------------------------------------------------------------
    {
        StageId::cStage_ShootingRange_Shr,
        "Fld_ShootingRange_Shr",
        "Testing Area",
        "Shooting Range",
        StageCategory::cHub,
        sead::Vector3f(-15.0f, -2.0f, -30.0f),
        sead::Vector3f( 15.0f, 10.0f,  30.0f),
        sead::Vector3f(  0.0f,  0.0f, -24.0f),
        sead::Vector3f(  0.0f,  0.0f,  24.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF27272A
    },
    {
        StageId::cStage_Plaza00_Plz,
        "Fld_Plaza00_Plz",
        "Inkopolis Plaza",
        "Highcollar City",
        StageCategory::cHub,
        sead::Vector3f(-40.0f, -2.0f, -40.0f),
        sead::Vector3f( 40.0f, 35.0f,  40.0f),
        sead::Vector3f(  0.0f,  0.0f, -25.0f), // Train station exit
        sead::Vector3f(  0.0f,  0.0f,  25.0f), // Inkopolis Tower base
        sead::Vector3f(  0.0f,  0.0f,   0.0f), // Central crosswalk
        0xFF18181B
    },
    {
        StageId::cStage_PlazaLobby,
        "Fld_PlazaLobby",
        "Deca Tower Battle Lobby",
        "Inkopolis Tower Lobby",
        StageCategory::cHub,
        sead::Vector3f(-20.0f, -2.0f, -20.0f),
        sead::Vector3f( 20.0f, 15.0f,  20.0f),
        sead::Vector3f(  0.0f,  0.0f, -12.0f),
        sead::Vector3f(  0.0f,  0.0f,  12.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF09090B
    },
    {
        StageId::cStage_MatchRoom,
        "Fld_MatchRoom",
        "Battle Waiting Room",
        "Match Room",
        StageCategory::cHub,
        sead::Vector3f(-15.0f, -2.0f, -15.0f),
        sead::Vector3f( 15.0f, 10.0f,  15.0f),
        sead::Vector3f( -8.0f,  0.0f,   0.0f),
        sead::Vector3f(  8.0f,  0.0f,   0.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF1E1B4B
    },
    {
        StageId::cStage_Tutorial00_Ttr,
        "Fld_Tutorial00_Ttr",
        "Tutorial Intro Bridge",
        "Tutorial Course",
        StageCategory::cHub,
        sead::Vector3f(-10.0f, -5.0f, -60.0f),
        sead::Vector3f( 10.0f, 10.0f,  60.0f),
        sead::Vector3f(  0.0f,  0.0f, -50.0f),
        sead::Vector3f(  0.0f,  0.0f,  50.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF1F2937
    },
    {
        StageId::cStage_StaffRoll00_Stf,
        "Fld_StaffRoll00_Stf",
        "Credits Stage",
        "Staff Roll Arena",
        StageCategory::cHub,
        sead::Vector3f(-25.0f, -5.0f, -25.0f),
        sead::Vector3f( 25.0f, 15.0f,  25.0f),
        sead::Vector3f(  0.0f,  0.0f, -15.0f),
        sead::Vector3f(  0.0f,  0.0f,  15.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF312E81
    },

    // -------------------------------------------------------------
    // Booyah Base Shops
    // -------------------------------------------------------------
    {
        StageId::cStage_Room_Weapons,
        "Fld_Room_Weapons",
        "Ammo Knights",
        "Kanburi Weapons",
        StageCategory::cShop,
        sead::Vector3f(-12.0f, -2.0f, -12.0f),
        sead::Vector3f( 12.0f, 10.0f,  12.0f),
        sead::Vector3f(  0.0f,  0.0f,  -6.0f),
        sead::Vector3f(  0.0f,  0.0f,   6.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF451A03
    },
    {
        StageId::cStage_Room_Gear,
        "Fld_Room_Gear",
        "Cooler Heads",
        "Kura Heads",
        StageCategory::cShop,
        sead::Vector3f(-12.0f, -2.0f, -12.0f),
        sead::Vector3f( 12.0f, 10.0f,  12.0f),
        sead::Vector3f(  0.0f,  0.0f,  -6.0f),
        sead::Vector3f(  0.0f,  0.0f,   6.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF701A75
    },
    {
        StageId::cStage_Room_Tshirts,
        "Fld_Room_Tshirts",
        "Jelly Fresh",
        "Jelly Fresh",
        StageCategory::cShop,
        sead::Vector3f(-12.0f, -2.0f, -12.0f),
        sead::Vector3f( 12.0f, 10.0f,  12.0f),
        sead::Vector3f(  0.0f,  0.0f,  -6.0f),
        sead::Vector3f(  0.0f,  0.0f,   6.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF064E3B
    },
    {
        StageId::cStage_Room_Shoes,
        "Fld_Room_Shoes",
        "Shrimp Kicks",
        "Ebi Kicks",
        StageCategory::cShop,
        sead::Vector3f(-12.0f, -2.0f, -12.0f),
        sead::Vector3f( 12.0f, 10.0f,  12.0f),
        sead::Vector3f(  0.0f,  0.0f,  -6.0f),
        sead::Vector3f(  0.0f,  0.0f,   6.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF7C2D12
    },

    // -------------------------------------------------------------
    // Splatfest Festival Variations
    // -------------------------------------------------------------
    {
        StageId::cStage_PlazaEvent00,
        "Fld_PlazaEvent00",
        "Splatfest Festival Plaza",
        "Fest Plaza",
        StageCategory::cSplatfest,
        sead::Vector3f(-40.0f, -2.0f, -40.0f),
        sead::Vector3f( 40.0f, 35.0f,  40.0f),
        sead::Vector3f(  0.0f,  0.0f, -25.0f),
        sead::Vector3f(  0.0f,  0.0f,  25.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF1E1B4B // Glowing neon midnight
    },
    {
        StageId::cStage_PlazaEvent01,
        "Fld_PlazaEvent01",
        "Splatfest Stage Alpha (Callie)",
        "Fest Truck A",
        StageCategory::cSplatfest,
        sead::Vector3f(-15.0f, -2.0f, -15.0f),
        sead::Vector3f( 15.0f, 15.0f,  15.0f),
        sead::Vector3f( -8.0f,  0.0f,   0.0f),
        sead::Vector3f(  8.0f,  0.0f,   0.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF831843
    },
    {
        StageId::cStage_PlazaEvent02,
        "Fld_PlazaEvent02",
        "Splatfest Stage Bravo (Marie)",
        "Fest Truck B",
        StageCategory::cSplatfest,
        sead::Vector3f(-15.0f, -2.0f, -15.0f),
        sead::Vector3f( 15.0f, 15.0f,  15.0f),
        sead::Vector3f( -8.0f,  0.0f,   0.0f),
        sead::Vector3f(  8.0f,  0.0f,   0.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF14532D
    },

    // -------------------------------------------------------------
    // Octo Valley (Hero Mode) Hub & Boss Encounters
    // -------------------------------------------------------------
    {
        StageId::cStage_World00_Wld,
        "Fld_World00_Wld",
        "Octo Valley Hub Area",
        "World Hub",
        StageCategory::cOctoValley,
        sead::Vector3f(-50.0f, -10.0f, -50.0f),
        sead::Vector3f( 50.0f,  30.0f,  50.0f),
        sead::Vector3f(  0.0f,   0.0f, -40.0f),
        sead::Vector3f(  0.0f,   0.0f,  40.0f),
        sead::Vector3f(  0.0f,   0.0f,   0.0f),
        0xFF14532D
    },
    {
        StageId::cStage_BossStamp,
        "Fld_BossStamp",
        "The Mighty Octostamp Arena",
        "Boss Stamp",
        StageCategory::cOctoBoss,
        sead::Vector3f(-30.0f, -5.0f, -30.0f),
        sead::Vector3f( 30.0f, 25.0f,  30.0f),
        sead::Vector3f(  0.0f,  0.0f, -20.0f),
        sead::Vector3f(  0.0f,  0.0f,  20.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF450A0A
    },
    {
        StageId::cStage_BossBall,
        "Fld_BossBall",
        "The Dread Roll (Octowhirl) Arena",
        "Boss Ball King",
        StageCategory::cOctoBoss,
        sead::Vector3f(-35.0f, -5.0f, -35.0f),
        sead::Vector3f( 35.0f, 20.0f,  35.0f),
        sead::Vector3f(  0.0f,  0.0f, -25.0f),
        sead::Vector3f(  0.0f,  0.0f,  25.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF172554
    },
    {
        StageId::cStage_BossCylinder,
        "Fld_BossCylinder",
        "The Ravenous Octonozzle Arena",
        "Boss Cylinder",
        StageCategory::cOctoBoss,
        sead::Vector3f(-32.0f, -5.0f, -32.0f),
        sead::Vector3f( 32.0f, 35.0f,  32.0f),
        sead::Vector3f(  0.0f,  0.0f, -22.0f),
        sead::Vector3f(  0.0f,  0.0f,  22.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF451A03
    },
    {
        StageId::cStage_BossMouth,
        "Fld_BossMouth",
        "The Dread Skewer (Octomaw) Arena",
        "Boss Mouth King",
        StageCategory::cOctoBoss,
        sead::Vector3f(-35.0f, -8.0f, -35.0f),
        sead::Vector3f( 35.0f, 20.0f,  35.0f),
        sead::Vector3f(  0.0f,  0.0f, -24.0f),
        sead::Vector3f(  0.0f,  0.0f,  24.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF1E293B
    },
    {
        StageId::cStage_BossRailKing,
        "Fld_BossRailKing_Bos_Msn",
        "Enter the Octobot King (DJ Octavio)",
        "Boss Rail King Takowasa",
        StageCategory::cOctoBoss,
        sead::Vector3f(-45.0f, -10.0f, -45.0f),
        sead::Vector3f( 45.0f,  35.0f,  45.0f),
        sead::Vector3f(  0.0f,   0.0f, -30.0f),
        sead::Vector3f(  0.0f,   6.0f,  25.0f),
        sead::Vector3f(  0.0f,   0.0f,   0.0f),
        0xFF581C87 // Electric Wasabi purple arena
    },

    // -------------------------------------------------------------
    // Octo Valley Missions & Octoling Infiltrations
    // -------------------------------------------------------------
    {
        StageId::cStage_OctZero00_Msn,
        "Fld_OctZero00_Msn",
        "Hero Mission: Octotrooper Hideout",
        "Mission Zero",
        StageCategory::cOctoValley,
        sead::Vector3f(-30.0f, -5.0f, -60.0f),
        sead::Vector3f( 30.0f, 25.0f,  60.0f),
        sead::Vector3f(  0.0f,  0.0f, -50.0f),
        sead::Vector3f(  0.0f,  0.0f,  50.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF1E3A5F
    },
    {
        StageId::cStage_OctCrank00_Msn,
        "Fld_OctCrank00_Msn",
        "Hero Mission: Underpass Infiltration",
        "Mission Crank",
        StageCategory::cOctoValley,
        sead::Vector3f(-40.0f, -5.0f, -50.0f),
        sead::Vector3f( 40.0f, 20.0f,  50.0f),
        sead::Vector3f(  0.0f,  0.0f, -40.0f),
        sead::Vector3f(  0.0f,  0.0f,  40.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF2D3748
    },
    {
        StageId::cStage_OctRuins00_Msn,
        "Fld_OctRuins00_Msn",
        "Hero Mission: Ruins Infiltration",
        "Mission Ruins",
        StageCategory::cOctoValley,
        sead::Vector3f(-40.0f, -5.0f, -50.0f),
        sead::Vector3f( 40.0f, 25.0f,  50.0f),
        sead::Vector3f(  0.0f,  0.0f, -40.0f),
        sead::Vector3f(  0.0f,  0.0f,  40.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF1F2937
    },
    {
        StageId::cStage_OctSkatePark00_Msn,
        "Fld_OctSkatePark00_Msn",
        "Hero Mission: Skatepark Infiltration",
        "Mission SkatePark",
        StageCategory::cOctoValley,
        sead::Vector3f(-35.0f, -5.0f, -45.0f),
        sead::Vector3f( 35.0f, 20.0f,  45.0f),
        sead::Vector3f(  0.0f,  0.0f, -35.0f),
        sead::Vector3f(  0.0f,  0.0f,  35.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF334155
    },
    {
        StageId::cStage_RvlMaze00_Msn,
        "Fld_RvlMaze00_Msn",
        "Octoling Assault: Kelp Dome",
        "Rival Maze",
        StageCategory::cOctoValley,
        sead::Vector3f(-38.0f, -5.0f, -38.0f),
        sead::Vector3f( 38.0f, 15.0f,  38.0f),
        sead::Vector3f(-32.0f,  1.0f,   0.0f),
        sead::Vector3f( 32.0f,  1.0f,   0.0f),
        sead::Vector3f(  0.0f,  0.0f,   0.0f),
        0xFF285E61
    },
    {
        StageId::cStage_RvlSeaPlant00_Msn,
        "Fld_RvlSeaPlant00_Msn",
        "Octoling Assault: Saltspray Rig",
        "Rival SeaPlant",
        StageCategory::cOctoValley,
        sead::Vector3f(-30.0f, -5.0f, -45.0f),
        sead::Vector3f( 30.0f, 20.0f,  45.0f),
        sead::Vector3f(-18.0f,  1.0f, -38.0f),
        sead::Vector3f( 18.0f,  1.0f, -38.0f),
        sead::Vector3f(  0.0f,  4.0f,  15.0f),
        0xFF1A365D
    },
    {
        StageId::cStage_RvlSkatePark00_Msn,
        "Fld_RvlSkatePark00_Msn",
        "Octoling Assault: Blackbelly Skatepark",
        "Rival SkatePark",
        StageCategory::cOctoValley,
        sead::Vector3f(-35.0f, -5.0f, -35.0f),
        sead::Vector3f( 35.0f, 20.0f,  35.0f),
        sead::Vector3f(-28.0f,  1.0f,   0.0f),
        sead::Vector3f( 28.0f,  1.0f,   0.0f),
        sead::Vector3f(  0.0f,  5.0f,   0.0f),
        0xFF2D3748
    },
    {
        StageId::cStage_RvlRuins00_Msn,
        "Fld_RvlRuins00_Msn",
        "Octoling Assault: Ancho-V Games",
        "Rival Ruins",
        StageCategory::cOctoValley,
        sead::Vector3f(-44.0f, -5.0f, -30.0f),
        sead::Vector3f( 44.0f, 22.0f,  30.0f),
        sead::Vector3f(-36.0f,  1.0f,   0.0f),
        sead::Vector3f( 36.0f,  1.0f,   0.0f),
        sead::Vector3f(  0.0f,  4.0f,   0.0f),
        0xFF1E293B
    }
};

static constexpr u32 sStageCount = sizeof(sStageTable) / sizeof(sStageTable[0]);

const StageInfo* StageDef::getStageInfo(StageId id) {
    u32 idx = static_cast<u32>(id);
    if (idx < sStageCount) {
        return &sStageTable[idx];
    }
    return &sStageTable[0];
}

const StageInfo* StageDef::getStageInfoByCodeName(const char* codeName) {
    if (!codeName) return &sStageTable[0];
    for (u32 i = 0; i < sStageCount; ++i) {
        if (std::strcmp(sStageTable[i].codeName, codeName) == 0) {
            return &sStageTable[i];
        }
    }
    return nullptr;
}

const StageInfo* StageDef::getStageInfoByName(const char* name) {
    if (!name) return &sStageTable[0];
    for (u32 i = 0; i < sStageCount; ++i) {
        if (std::strstr(sStageTable[i].displayName, name) != nullptr ||
            std::strstr(sStageTable[i].codeName, name) != nullptr) {
            return &sStageTable[i];
        }
    }
    return nullptr;
}

u32 StageDef::getStageCount() {
    return sStageCount;
}

const StageInfo* StageDef::getAllStages() {
    return sStageTable;
}

} // namespace Game
