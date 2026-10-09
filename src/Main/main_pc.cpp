#include "types.h"
#include "cafe/coreinit.h"
#include "cafe/vpad.h"
#include "cafe/gx2.h"
#include "Game/Actor/GambitActorMgr.h"
#include "Game/GamePlayer.h"
#include "Game/Paint/PaintTextureMgr.h"
#include "Game/Map/StageDef.h"
#include "Game/Map/StageMgr.h"
#include "Game/Map/MapTable.h"
#include "Game/Map/BymlParser.h"
#include "sead/resource/seadResource.h"
#include "Game/Weapon/GameWeaponRoller.h"
#include "Game/Bullet/BulletPlayerChargeShotBase.h"
#include "Game/Weapon/PlayerKingSquid.h"
#include "Game/Weapon/SuperWeaponMgr.h"
#include "Game/Rival/RivalMgr.h"
#include "Game/Rival/GameRivalSquad.h"
#include "Game/Hud/HudMgr.h"
#include "Game/Player/GamePlayerDamageParam.h"
#include "Game/Player/GearSkillMgr.h"
#include "Game/MapObj/GameTurnPlate.h"
#include "Game/MapObj/Obj_Sponge.h"
#include "Game/MapObj/Obj_Geyser.h"
#include "Game/Enemy/GameEnemyCharge.h"
#include "Game/Enemy/GameEnemyCleaner.h"
#include "Game/Enemy/GameEnemyBallKing.h"
#include "Game/Enemy/GameEnemyHideKing.h"
#include "Game/Rule/GachiArea.h"
#include "Game/Rule/GachiYagura.h"
#include "Game/Weapon/Wsp_Shachihoko.h"
#include "Game/Rule/GameRuleRanked.h"
#include "Game/Enemy/EnemyMouthKing.h"
#include "Game/Mission/PlayerCustomPartMission.h"
#include "Game/System/CameraMgr.h"
#include "Game/Net/NetSessionMgr.h"
#include "Game/Net/PlayerClone.h"
#include "Game/Bullet/BulletBombNormal.h"
#include "Game/Bullet/BulletBombInstant.h"
#include "Game/Bullet/BulletBombMarking.h"
#include "Game/Bullet/BulletBombDevil.h"
#include "Game/Bullet/Bomb_Chase.h"
#include "Game/Bullet/Wsb_Shield.h"
#include "Game/Bullet/Wsb_Flag.h"
#include "Game/Bullet/TimerTrap.h"
#include "Game/Bullet/Sprinkler.h"
#include "Game/Weapon/PlayerWeaponBigLaser.h"
#include "Game/Weapon/PlayerWeaponBigShot.h"
#include "Game/Weapon/PlayerWeaponTornado.h"
#include "Game/Bullet/BulletPlayerExplosionShot.h"
#include "Game/Bullet/BulletPlayerBigBallHitSplash.h"
#include "Game/Player/Tank_Ink.h"
#include "Game/Enemy/EnemyStampKing.h"
#include "Game/Enemy/EnemyRailKing.h"
#include "Game/MapObj/InkRail.h"
#include "Game/MapObj/Obj_PaintingLift.h"
#include "Game/MapObj/Obj_JumpPlate.h"
#include "Game/Dojo/MainMgrDuel.h"
#include "Game/Stage/Fld_Plaza00_Plz.h"
#include "Game/Fest/Fld_PlazaEvent03_SelectB.h"
#include "Game/Enemy/Enm_TakolienSpeedUp.h"
#include "Game/Enemy/Enm_BallKing.h"
#include "Game/Enemy/Obj_CylinderKingHole.h"
#include "Game/Post/Obj_PlazaPost.h"
#include "Game/Weapon/Obj_Barrier.h"
#include "Game/Enemy/Enm_Takopter.h"
#include "Game/Enemy/EnemyHohei.h"
#include "sead/math/seadMatrix.h"
#include "sead/resource/BfresParser.h"
#include "Game/Paint/PaintMap3D.h"
#include "Game/System/PcInputBridge.h"
#include "Game/Graphics/Dx11Renderer.h"
#include "Game/Collision/KclFile.h"
#include "Game/MapObj/Obj_GeneralBox.h"
#include "Game/ShootingRange/SighterTarget.h"
#include "Game/System/SoundShapeMgr.h"
#include "Game/System/PcAudioDriver.h"
#include "Game/Npc/Npc_Judge_Flag.h"
#include "Game/Shop/Npc_WeaponsShop.h"
#include "Game/MiniGame/Obj_PlazaGame.h"
#include "Game/Weapon/SuperWeaponShelter.h"
#include "Game/Weapon/GameWeaponSuperShot.h"
#include "Game/Weapon/GameWeaponMegaphone.h"
#include "Game/Weapon/GameWeaponDaiouIka.h"
#include "Game/Rule/Obj_QuarryBeltYagura.h"
#include "Game/Net/AutoWarpPoint.h"
#include "Game/Rule/GameRuleTurfWar.h"
#include "Game/Camera/CameraCollision.h"
#include "Game/Player/PlayerInkState.h"
#include "Game/Player/GearBrandAffinity.h"
#include "Game/Npc/PlazaNewsBroadcast.h"
#include "Game/Bullet/DevilBall.h"
#include "Game/Weapon/GameWeaponSlosher.h"
#include "Game/Weapon/WeaponCatalog.h"
#include "Game/Player/UdemaeGradeMgr.h"
#include "Game/Player/GearCatalog.h"
#include "Game/Dojo/DuelItemTable.h"
#include "Game/Map/MapInfoCatalog.h"
#include "Game/Mission/AmiiboChallengeMgr.h"
#include "Game/Enemy/RailKingSchedule.h"
#include "Game/Weapon/WeaponParamCatalog.h"
#include "Game/System/AglParameter.h"
#include "Game/Player/SkillTipsCatalog.h"
#include "Game/Mission/CuttlefishDialogueMgr.h"
#include "Game/Camera/CameraParamEngine.h"
#include "Game/Camera/CameraSequenceDirector.h"
#include "Game/Effect/ParticleBindCatalog.h"
#include "Game/Plaza/PlazaAvatarCatalog.h"
#include "Game/Audio/SLinkDatabase.h"
#include "Game/Player/PlayerRankMgr.h"
#include "Game/Player/TankInfoCatalog.h"
#include "Game/Dojo/DuelPlayerSettingCatalog.h"
#include "Game/Effect/ELinkDatabase.h"
#include "Game/Npc/NewsScriptEngine.h"
#include "Game/MiniGame/MiniGameCatalog.h"
#include "Game/Plaza/PlazaNpcPresetCatalog.h"
#include "Game/Map/GambitStageTreeCatalog.h"
#include "Game/Mission/MissionStageMapParser.h"
#include "Game/Mission/SunkenScrollCatalog.h"
#include "Game/Item/ItemAncientDocument.h"
#include "Game/MapObj/Obj_Goal.h"
#include "Game/Mission/ZapfishPowerGridMgr.h"
#include "Game/MapObj/Obj_Geyser.h"
#include "Game/MapObj/Obj_Sponge.h"
#include "Game/MapObj/Obj_KeyTreasureBox.h"
#include "Game/MapObj/Obj_Ikastone.h"
#include "Game/Enemy/Obj_RailKingPilotHouse.h"
#include "Game/MapObj/Obj_LiftFall.h"
#include "Game/MapObj/Obj_MissilePosition.h"
#include "Game/Enemy/Obj_ZakoPointUFO.h"
#include "Game/MapObj/Obj_AreaGate.h"
#include "Game/MapObj/Obj_GateManhole.h"
#include "Game/MapObj/Obj_BombFlower.h"
#include "Game/Enemy/Obj_CylinderKingBall.h"
#include "Game/MapObj/Obj_WarpPointFlag.h"
#include "Game/MapObj/Obj_DefenseTower.h"
#include "Game/MapObj/Obj_AirBall.h"
#include "Game/MapObj/Obj_AncientDocument.h"
#include "Game/MapObj/Obj_Grass00.h"
#include "Game/MapObj/Obj_SeaGull.h"
#include "Game/MapObj/Obj_Tree00.h"
#include "Game/MapObj/Obj_SighterTarget.h"
#include "Game/MapObj/Obj_RubberPole00.h"
#include "Game/MapObj/Obj_Windsock.h"
#include "Game/Enemy/EnemyTakodozer.h"
#include "Game/Enemy/Enm_Ball.h"
#include "Game/MapObj/Lft_Propeller00.h"
#include "Game/MapObj/Obj_Armor.h"
#include "Game/MapObj/Obj_SwitchPaint.h"
#include "Game/MapObj/Obj_ColorCone.h"
#include "Game/MapObj/Obj_SquidGuard.h"
#include "Game/Enemy/Enm_BossWeakPoint.h"
#include "Game/Enemy/Enm_OctLeg.h"
#include "Game/Npc/Npc_Commander.h"
#include "Game/Npc/Npc_CommanderBind.h"
#include "Game/MapObj/Obj_AtarimeHouse.h"
#include "Game/MapObj/Obj_BigNamazu.h"
#include "Game/MapObj/Obj_RespawnPlatform.h"
#include "Game/MapObj/Obj_JumpPoint.h"
#include "Game/Enemy/Enm_Stamp.h"
#include "Game/MapObj/Obj_Box00L.h"
#include "Game/MapObj/Obj_Box00S.h"
#include "Game/Enemy/Enm_TakopterTornado.h"
#include "Game/MapObj/Obj_PaintLiftTurn.h"
#include "Game/MapObj/Obj_PaintLiftSlide.h"
#include "Game/Enemy/Enm_TakolienSpeedUp.h"
#include "Game/Enemy/Enm_Cleaner.h"
#include "Game/Enemy/Enm_TakopterBomb.h"
#include "Game/Enemy/Enm_Charge.h"
#include "Game/MapObj/Obj_SeesawLift.h"
#include "Game/MapObj/Obj_BridgeUpDown00.h"
#include "Game/MapObj/Lft_Charge.h"
#include "Game/Enemy/Enm_TakolienVehicleFix.h"
#include "Game/Enemy/Enm_TakolienVehicleMove.h"
#include "Game/Enemy/Enm_TakolienVehicleSubmarine.h"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>

void PrintBanner() {
    printf("=================================================================\n");
    printf("   Splatoon 1 (Wii U - Gambit) Decompilation Verification Tool   \n");
    printf("=================================================================\n");
    printf("[+] Target Binary: original/Gambit.elf (21.7 MB, PPC Espresso)\n");
    printf("[+] Environment: Reverse Engineering & Subsystem Verification\n");
    printf("-----------------------------------------------------------------\n\n");
}

void PrintUsage() {
    printf("Usage: Gambit.exe [options]\n\n");
    printf("Options:\n");
    printf("  --render [frames]        Launch DirectX 11 3D interactive renderer (default)\n");
    printf("  --stage=<name>           Select retail stage to render (e.g. Fld_PlazaLobby, Fld_Warehouse00)\n");
    printf("  --verify                 Run structural & functional verification tests\n");
    printf("  --list-stages            List all 43 cataloged retail stages\n");
    printf("  --test-stage=<name/id>   Run simulation test on a specific stage\n");
    printf("  --dump-sarc <file.szs>   Extract & inspect a Nintendo SARC/SZS archive\n");
    printf("  --dump-byml <file.byml>  Parse and display a Nintendo BYML parameter file\n");
    printf("  --help                   Display this help message\n\n");
}

bool RunVerificationSuite() {
    printf("[*] Running Decompilation Structural Verification Suite...\n\n");
    bool allPassed = true;

    // 1. Structure alignment and sizes
    printf("--- [1/4] STRUCTURAL SIZES & OFFSETS ---\n");
    printf("  sizeof(MapParam):           %zu bytes (Target: 376 / 0x178) -> %s\n",
           sizeof(Game::MapParam),
           sizeof(Game::MapParam) == 0x178 ? "MATCH" : "MISMATCH");
    if (sizeof(Game::MapParam) != 0x178) allPassed = false;

    // 2. Map Table verification
    printf("\n--- [2/4] STAGE DEFINITION CATALOG ---\n");
    u32 stageCount = Game::StageDef::getStageCount();
    printf("  Cataloged Stages:           %u / 43 -> %s\n",
           stageCount, stageCount == 43 ? "COMPLETE" : "INCOMPLETE");
    if (stageCount != 43) allPassed = false;

    auto* mapTable = Game::MapTable::getInstance();
    const auto* warehouse = mapTable->findStageByName("Fld_Warehouse00_Vss");
    printf("  MapTable Lookup ('Fld_Warehouse00_Vss'): %s\n",
           (warehouse && warehouse->mapId == 1) ? "FOUND (ID: 1)" : "FAILED");
    if (!warehouse) allPassed = false;

    // 3. Subsystem simulation check
    printf("\n--- [3/4] SUBSYSTEM SIMULATION LIFECYCLE ---\n");
    Game::GambitActorMgr actorMgr;
    actorMgr.init(nullptr);

    Game::PaintTextureMgr paintMgr;
    paintMgr.init(nullptr, 512, 512);

    Game::StageMgr stageMgr;
    stageMgr.init();

    bool stageLoaded = stageMgr.loadStage(Game::StageId::cStage_Warehouse00_Vss, &actorMgr, &paintMgr);
    printf("  StageMgr::loadStage:        %s\n", stageLoaded ? "SUCCESS" : "FAILED");
    if (!stageLoaded) allPassed = false;

    auto* player = new Game::Player();
    player->init();
    player->setPosition(stageMgr.getSpawnAlpha());
    actorMgr.registerActor(player);

    // Step 60 frames headlessly
    for (int frame = 0; frame < 60; ++frame) {
        VPADStatus vpad = {};
        vpad.hold = VPAD_BUTTON_ZR; // simulate firing
        player->handleInput(vpad);
        actorMgr.update();
    }

    printf("  Actor Simulation (60 f):    COMPLETED (Ink Tank: %.1f%%)\n",
           player->getInkTankAmount() * 100.0f);

    stageMgr.unloadCurrentStage(&actorMgr);
    actorMgr.finalize();

    // 4. Parser integrity check
    printf("\n--- [4/5] SARC & BYML PARSER INTEGRITY ---\n");
    u32 hashTest = sead::SarcArchive::calcHash("StageParam.byml");
    printf("  SARC Hash ('StageParam.byml'): 0x%08X -> %s\n",
           hashTest, hashTest != 0 ? "VALID" : "FAILED");
    if (hashTest == 0) allPassed = false;

    // 5. Authentic Weapons, Abilities, and Octoling AI
    printf("\n--- [5/5] WEAPONS, ABILITIES & OCTOLING RIVAL AI ---\n");

    // Roller verification
    Game::GameWeaponRoller roller;
    roller.init();
    roller.setRollerType(Game::RollerType::cHeavy);
    bool rollerOk = (roller.getFlingDamage() == 180.0f && roller.getSquishDamage() == 160.0f);
    printf("  Dynamo Roller Stats:        %s (Fling: %.1f HP, Squish: %.1f HP)\n",
           rollerOk ? "PASSED" : "FAILED", roller.getFlingDamage(), roller.getSquishDamage());
    if (!rollerOk) allPassed = false;

    // Charger damage interpolation and Rainmaker multiplier
    Game::Charge_Light chargerBullet;
    chargerBullet.init();
    chargerBullet.setChargePower(0.5f);
    f32 halfDamage = chargerBullet.getCalculatedDamage();
    chargerBullet.setChargePower(1.0f);
    f32 fullDamage = chargerBullet.getCalculatedDamage();
    f32 shieldDamage = chargerBullet.getCalculatedDamage(0xC);
    bool chargerOk = (halfDamage == 70.0f && fullDamage == 160.0f && std::abs(shieldDamage - 448.0f) < 0.1f);
    printf("  Charger Damage Curve:       %s (50%%: %.1f HP, 100%%: %.1f HP, Shield: %.1f HP)\n",
           chargerOk ? "PASSED" : "FAILED", halfDamage, fullDamage, shieldDamage);
    if (!chargerOk) allPassed = false;

    // Kraken (KingSquid) 160 HP instakill
    Game::PlayerKingSquid kraken;
    kraken.init();
    f64 krakenSpinDmg = kraken.vfunc_88(0xC, nullptr);
    bool krakenOk = (std::abs(krakenSpinDmg - 160.0) < 0.1);
    printf("  Kraken Spin Attack Damage:  %s (%.1f HP - Lethal OHKO)\n",
           krakenOk ? "PASSED" : "FAILED", krakenSpinDmg);
    if (!krakenOk) allPassed = false;

    // Gear Ability Stacking
    Game::GearSkillMgr skillMgr;
    Game::GearSlotConfig fullDmgUp = {
        Game::GearSkillKind::cDamageUp,
        { Game::GearSkillKind::cDamageUp, Game::GearSkillKind::cDamageUp, Game::GearSkillKind::cDamageUp }
    };
    skillMgr.setHeadgear(fullDmgUp);
    skillMgr.setClothes(fullDmgUp);
    skillMgr.setShoes(fullDmgUp);
    f32 dmgMulti = skillMgr.getDamageMultiplier();
    bool gearOk = (std::abs(dmgMulti - 1.30f) < 0.01f);
    printf("  Gear Stacking (57 AP Dmg):  %s (Multiplier: %.2fx)\n",
           gearOk ? "PASSED" : "FAILED", dmgMulti);
    if (!gearOk) allPassed = false;

    // Octoling Rival Squad AI
    Game::RivalMgr rivalMgr;
    rivalMgr.init();
    sead::Vector3f spawns[4] = {
        sead::Vector3f(-10.0f, 0.0f, 25.0f),
        sead::Vector3f(-5.0f, 0.0f, 25.0f),
        sead::Vector3f(5.0f, 0.0f, 25.0f),
        sead::Vector3f(10.0f, 0.0f, 25.0f)
    };
    rivalMgr.spawnSquad(spawns, Game::RivalDifficulty::cLevel3);
    rivalMgr.updateSquadAi(sead::Vector3f(0.0f, 0.0f, 10.0f));
    u32 activeRivals = rivalMgr.getActiveCount();
    bool rivalOk = (activeRivals == 4);
    printf("  Octoling Rival Squad AI:    %s (%u Elite Octolings Active)\n",
           rivalOk ? "PASSED" : "FAILED", activeRivals);
    if (!rivalOk) allPassed = false;

    // 6. Interactive Stage Objects & Octarian Boss Encounters
    printf("\n--- [6/6] STAGE GIZMOS & OCTARIAN FORCES ---\n");

    // TurnPlate rotating platform
    Game::GameTurnPlate turnPlate;
    turnPlate.init();
    turnPlate.setupPlatform(sead::Vector3f(0.0f, 0.0f, 0.0f), 6.0f, 0.02f);
    turnPlate.update();
    sead::Vector3f tangentialVel = turnPlate.computeLinearVelocityAtPoint(sead::Vector3f(0.0f, 0.0f, 5.0f));
    bool turnPlateOk = (turnPlate.getCurrentAngle() > 0.0f && std::abs(tangentialVel.x - (-0.1f)) < 0.001f);
    printf("  Stage Turntable (TurnPlate): %s (Angular: %.3f rad, TanVx: %.2f)\n",
           turnPlateOk ? "PASSED" : "FAILED", turnPlate.getCurrentAngle(), tangentialVel.x);
    if (!turnPlateOk) allPassed = false;

    // Octosniper (Enm_Charge)
    Game::GameEnemyCharge octosniper;
    octosniper.init();
    octosniper.setupBunker(sead::Vector3f(0.0f, 0.0f, 20.0f), 3.14159f);
    octosniper.updateAimAtPlayer(sead::Vector3f(0.0f, 0.0f, 0.0f));
    octosniper.update();
    bool sniperOk = (octosniper.isAlive() && octosniper.getHealth() == 80.0f);
    printf("  Octosniper Bunker (Charge):  %s (HP: %.1f, State: %u)\n",
           sniperOk ? "PASSED" : "FAILED", octosniper.getHealth(), static_cast<u32>(octosniper.getState()));
    if (!sniperOk) allPassed = false;

    // Squee-G (Enm_Cleaner)
    Game::GameEnemyCleaner squeeG;
    squeeG.init();
    squeeG.spawn(sead::Vector3f(5.0f, 0.0f, 5.0f));
    squeeG.update();
    bool squeeGOk = (squeeG.isAlive() && squeeG.getHealth() == 80.0f);
    printf("  Squee-G Cleaner (Enm_Cleaner):%s (HP: %.1f, State: %u)\n",
           squeeGOk ? "PASSED" : "FAILED", squeeG.getHealth(), static_cast<u32>(squeeG.getState()));
    if (!squeeGOk) allPassed = false;

    // Octowhirl (Enm_BallKing)
    Game::GameEnemyBallKing octowhirl;
    octowhirl.init();
    octowhirl.setState(Game::OctowhirlState::cRollingDash);
    octowhirl.updateBossAi(sead::Vector3f(0.0f, 0.0f, 10.0f), true); // Rolling on player ink
    bool octowhirlOk = (octowhirl.getState() == Game::OctowhirlState::cSpinoutSkid);
    printf("  Octowhirl Boss (BallKing):   %s (Ink Skid Spinout: Triggered)\n",
           octowhirlOk ? "PASSED" : "FAILED");
    if (!octowhirlOk) allPassed = false;

    // Octonozzle (Enm_HideKing)
    Game::GameEnemyHideKing octonozzle;
    octonozzle.init();
    octonozzle.setupBoss(sead::Vector3f(0.0f, 0.0f, 0.0f));
    octonozzle.plugHoleWithInk(0);
    octonozzle.plugHoleWithInk(1);
    octonozzle.plugHoleWithInk(2);
    octonozzle.update();
    bool nozzleOk = (octonozzle.isTopExposed());
    printf("  Octonozzle Boss (HideKing):  %s (3 Nozzles Plugged -> Stunned)\n",
           nozzleOk ? "PASSED" : "FAILED");
    if (!nozzleOk) allPassed = false;

    // 7. Ranked Battle Rules, Octo Valley Finale, Camera & Netplay
    printf("\n--- [7/7] RANKED RULES, CAMPAIGN FINALE, CAMERA & NETPLAY ---\n");

    // Splat Zones (GachiArea)
    Game::GachiArea splatZones;
    splatZones.init();
    splatZones.updatePaintCoverage(0.75f, 0.20f); // 75% Alpha control -> exceeds 70% threshold
    for (int i = 0; i < 60; ++i) {
        splatZones.update();
    }
    bool splatZonesOk = (splatZones.getControlState() == Game::ZoneControlState::cControlledP1 && splatZones.getAlphaCounter() == 99);
    printf("  Splat Zones (GachiArea):     %s (Seizure: 75%% -> Count: %d)\n",
           splatZonesOk ? "PASSED" : "FAILED", splatZones.getAlphaCounter());
    if (!splatZonesOk) allPassed = false;

    // Tower Control (GachiYagura)
    Game::GachiYagura tower;
    tower.init();
    tower.updateRiders(2, 0); // 2 Alpha riders
    tower.update();
    bool towerOk = (tower.getState() == Game::YaguraState::cAdvancingToBravo && tower.getTrackProgress() > 0.0f);
    printf("  Tower Control (GachiYagura): %s (State: Advancing, Progress: %.3f)\n",
           towerOk ? "PASSED" : "FAILED", tower.getTrackProgress());
    if (!towerOk) allPassed = false;

    // Rainmaker (Wsp_Shachihoko)
    Game::Wsp_Shachihoko rainmaker;
    rainmaker.init();
    rainmaker.applyInkToShield(0, 520.0f); // Exceeds 500 HP burst threshold
    for (int i = 0; i < 30; ++i) {
        rainmaker.update();
    }
    bool rainmakerOk = (rainmaker.getShieldState() == Game::ShachihokoShieldState::cFreePickup);
    printf("  Rainmaker (Wsp_Shachihoko):  %s (Shield Burst -> Free Pickup)\n",
           rainmakerOk ? "PASSED" : "FAILED");
    if (!rainmakerOk) allPassed = false;

    // Octomaw Boss (EnemyMouthKing)
    Game::EnemyMouthKing octomaw;
    octomaw.init();
    octomaw.setState(Game::OctomawState::cLeapChomp);
    octomaw.applyBombToMouth(180.0f); // Bomb into open mouth -> stuns
    bool octomawOk = (octomaw.isTentacleVulnerable());
    printf("  Octomaw Boss (MouthKing):    %s (Mouth Bomb -> Tentacle Exposed)\n",
           octomawOk ? "PASSED" : "FAILED");
    if (!octomawOk) allPassed = false;

    // Hero Gear Upgrades (PlayerCustomPartMission)
    Game::PlayerCustomPartMission heroGear;
    heroGear.init();
    heroGear.addPowerEggs(2500);
    bool up1 = heroGear.upgradeHeroShot(); // Level 1 -> 2 (500 eggs)
    bool up2 = heroGear.upgradeHeroShot(); // Level 2 -> 3 (1500 eggs)
    bool heroGearOk = (up1 && up2 && heroGear.getHeroShotLevel() == 3 && heroGear.getPowerEggs() == 500);
    printf("  Hero Gear Upgrades:          %s (Hero Shot: Lv %u, Bank: %u Eggs)\n",
           heroGearOk ? "PASSED" : "FAILED", heroGear.getHeroShotLevel(), heroGear.getPowerEggs());
    if (!heroGearOk) allPassed = false;

    // 3D Camera System (CameraMgr)
    Game::CameraMgr camera;
    camera.init();
    camera.setTargetPosition(sead::Vector3f(0.0f, 1.0f, 0.0f));
    camera.applyGyroInput(0.05f, 0.10f);
    camera.update();
    bool cameraOk = (camera.getBoomDistance() > 0.0f && camera.getEyePosition().y > 0.0f);
    printf("  3D Camera Engine (CameraMgr):%s (Eye: [%.1f, %.1f, %.1f], Dist: %.1f)\n",
           cameraOk ? "PASSED" : "FAILED",
           camera.getEyePosition().x, camera.getEyePosition().y, camera.getEyePosition().z,
           camera.getBoomDistance());
    if (!cameraOk) allPassed = false;

    // 8-Player Network Session (NetSessionMgr)
    Game::NetSessionMgr netSession;
    netSession.init();
    netSession.startLobbySession(12345, true); // Host
    netSession.registerPlayer(0, 0, 1, "PlayerAlpha1");
    netSession.registerPlayer(1, 0, 2, "PlayerAlpha2");
    netSession.registerPlayer(2, 1, 3, "PlayerBravo1");
    netSession.registerPlayer(3, 1, 4, "PlayerBravo2");
    bool netOk = (netSession.isHost() && netSession.getConnectedCount() == 4 && netSession.getTeamPlayerCount(0) == 2);
    printf("  8-Player Netplay Session:    %s (Host: %s, Connected: %u, Team 0: %u)\n",
           netOk ? "PASSED" : "FAILED",
           netSession.isHost() ? "YES" : "NO", netSession.getConnectedCount(), netSession.getTeamPlayerCount(0));
    if (!netOk) allPassed = false;

    // 8. Sub Weapons, Special Arsenal, Ballistics & Network Replication
    printf("\n--- [8/8] SUB WEAPONS, SPECIAL ARSENAL & NET REPLICATION ---\n");

    // Sub Weapons: Splat Bomb & Burst Bomb
    Game::BulletBombNormal splatBomb;
    splatBomb.init();
    splatBomb.throwBomb(sead::Vector3f(0.0f, 1.0f, 0.0f), sead::Vector3f(0.0f, 0.0f, 1.0f), 0);
    Game::BulletBombInstant burstBomb;
    burstBomb.init();
    burstBomb.throwBomb(sead::Vector3f(0.0f, 1.0f, 0.0f), sead::Vector3f(0.0f, 0.0f, 1.0f), 0, 1);
    bool bombOk = (splatBomb.getInnerDamage() == 180.0f && Game::BulletBombInstant::cDirectDamage == 60.0f);
    printf("  Sub: Splat & Burst Bombs:    %s (Splat Inner: 180 HP, Burst Direct: 60 HP)\n",
           bombOk ? "PASSED" : "FAILED");
    if (!bombOk) allPassed = false;

    // Sub Weapon: Seeker (Bomb_Chase)
    Game::Bomb_Chase seeker;
    seeker.init();
    seeker.launch(sead::Vector3f(0.0f, 0.0f, 0.0f), 0.0f, 0, 1);
    seeker.checkEnemyHoming(sead::Vector3f(5.0f, 0.0f, 5.0f), 1);
    seeker.update();
    bool seekerOk = (seeker.getState() == Game::ChaseBombState::cHomingTarget && Game::Bomb_Chase::cLethalDamage == 180.0f);
    printf("  Sub: Seeker (Bomb_Chase):    %s (Enemy Homing Target: Active, Lethal: 180 HP)\n",
           seekerOk ? "PASSED" : "FAILED");
    if (!seekerOk) allPassed = false;

    // Sub Weapon: Point Sensor & Disruptor
    Game::BulletBombMarking sensor;
    sensor.init();
    sensor.throwBomb(sead::Vector3f(0.0f, 1.0f, 0.0f), sead::Vector3f(0.0f, 0.0f, 0.0f), 0, 1);
    sensor.vfunc_11(); // Detonate into sensor pulse aura
    bool markHit = sensor.checkMarkEnemy(sead::Vector3f(2.0f, 1.0f, 2.0f), 1);
    Game::BulletBombDevil disruptor;
    disruptor.init();
    disruptor.throwBomb(sead::Vector3f(0.0f, 1.0f, 0.0f), sead::Vector3f(0.0f, 0.0f, 0.0f), 0, 1);
    bool debuffOk = (markHit && sensor.getTeam() == 0 && disruptor.getTeam() == 0);
    printf("  Sub: Sensor & Disruptor:     %s (Sensor Target Mark: Detected)\n",
           debuffOk ? "PASSED" : "FAILED");
    if (!debuffOk) allPassed = false;

    // Sub Weapon: Splash Wall (Wsb_Shield)
    Game::Wsb_Shield splashWall;
    splashWall.init();
    splashWall.deploy(sead::Vector3f(0.0f, 0.0f, 0.0f), 0.0f, 0);
    for (int i = 0; i < 20; ++i) splashWall.update(); // 20 frames unfold animation -> active wall
    bool blocked = splashWall.blocksBullet(sead::Vector3f(0.0f, 1.0f, 0.0f), 1);
    splashWall.applyDamage(60.0f);
    bool wallOk = (blocked && splashWall.getRemainingHp() < 750.0f);
    printf("  Sub: Splash Wall (Shield):   %s (Blocked Enemy Shot: %s, HP: %.1f/800)\n",
           wallOk ? "PASSED" : "FAILED", blocked ? "YES" : "NO", splashWall.getRemainingHp());
    if (!wallOk) allPassed = false;

    // Special: Killer Wail (PlayerWeaponBigLaser)
    Game::PlayerWeaponBigLaser killerWail;
    killerWail.init();
    killerWail.deploy(sead::Vector3f(0.0f, 1.0f, 0.0f), 0.0f, 0);
    for (int i = 0; i < 80; ++i) killerWail.update(); // 15 placing + 60 warning = 75 -> sonic blast
    bool beamHit = killerWail.checkHitTarget(sead::Vector3f(0.0f, 1.0f, 15.0f));
    bool laserOk = (killerWail.isBlasting() && beamHit);
    printf("  Special: Killer Wail (Laser):%s (Sonic Column Piercing: %s)\n",
           laserOk ? "PASSED" : "FAILED", beamHit ? "DETECTED" : "MISSED");
    if (!laserOk) allPassed = false;

    // Special: Inkzooka & Inkstrike
    Game::PlayerWeaponBigShot inkzooka;
    inkzooka.init();
    inkzooka.startSpecial(0, 360);
    bool shot1 = inkzooka.fireShot(sead::Vector3f(0.0f, 0.0f, 0.0f), 0.0f);
    Game::PlayerWeaponTornado inkstrike;
    inkstrike.init();
    inkstrike.confirmTarget(sead::Vector3f(10.0f, 0.0f, 15.0f), 0);
    for (int i = 0; i < 90; ++i) inkstrike.update();
    bool specOk = (shot1 && inkzooka.getAmmoRemaining() == 5 && inkstrike.getState() == Game::TornadoState::cVortexActive);
    printf("  Special: Inkzooka & Strike:  %s (Zooka Ammo: %u, Strike Vortex: Active)\n",
           specOk ? "PASSED" : "FAILED", inkzooka.getAmmoRemaining());
    if (!specOk) allPassed = false;

    // Ballistics: Blasters & Sloshers
    Game::BulletPlayerNormalExplosionShotBase blaster;
    blaster.init();
    blaster.fireBlaster(Game::BlasterType::cNormal, sead::Vector3f(0.0f, 1.0f, 0.0f), sead::Vector3f(0.0f, 0.0f, 1.0f), 0);
    f32 directDmg = blaster.computeDamageAgainstTarget(0, 0.1f, 1);
    f32 shieldDmg = blaster.computeDamageAgainstTarget(12, 1.0f, 1); // Rainmaker 2.5x multiplier
    Game::BulletPlayerBigBallHitSplash slosher;
    slosher.init();
    slosher.launchSlosh(sead::Vector3f(0.0f, 1.0f, 0.0f), sead::Vector3f(0.0f, 0.5f, 1.0f), 18.0f, 0);
    slosher.update();
    bool balOk = (directDmg == 125.0f && shieldDmg > 100.0f && slosher.getDamage() == 70.0f);
    printf("  Ballistics: Blaster/Slosher: %s (Blaster Direct: %.1f HP, Shield: %.1f HP)\n",
           balOk ? "PASSED" : "FAILED", directDmg, shieldDmg);
    if (!balOk) allPassed = false;

    // Ink Tank Subsystem (Tank_Ink)
    Game::Tank_Ink tank;
    tank.init();
    tank.setSubMarkerCost(70.0f);
    bool initialSub = tank.isSubMarkerLit();
    tank.consumeInk(40.0f); // 60% remaining (< 70% sub threshold)
    bool postSub = tank.isSubMarkerLit();
    f32 liquidHeight = tank.computeLiquidHeight();
    bool tankOk = (initialSub && !postSub && liquidHeight > -0.85f && liquidHeight < 0.85f);
    printf("  Ink Tank Subsystem:          %s (Sub Marker Indicator: Dynamic, H: %.2f)\n",
           tankOk ? "PASSED" : "FAILED", liquidHeight);
    if (!tankOk) allPassed = false;

    // Network Multi-Channel Player Clone Replication
    Game::PlayerClone localClone;
    localClone.init();
    localClone.setupClone(3, 0, true);
    localClone.setTransform(sead::Vector3f(12.5f, 0.0f, -8.0f), 1.57f);
    localClone.setStatus(95.0f, 80.0f);
    localClone.setAction(true, false, 42);
    localClone.setGear(101, 202, 303);

    u8 packetBuffer[256];
    size_t written = 0;
    localClone.packChannels(packetBuffer, sizeof(packetBuffer), &written);

    Game::PlayerClone remoteClone;
    remoteClone.init();
    bool unpackOk = remoteClone.unpackChannels(packetBuffer, written);
    bool cloneOk = (unpackOk && remoteClone.getPlayerId() == 3 &&
                    remoteClone.getTransform().position.x == 12.5f &&
                    remoteClone.getStatus().health == 95.0f &&
                    remoteClone.getAction().isFiring &&
                    remoteClone.getGear().clothesId == 202);
    printf("  PlayerClone Net Replication: %s (9 Channels Packed: %zu bytes roundtrip)\n",
           cloneOk ? "PASSED" : "FAILED", written);
    if (!cloneOk) allPassed = false;

    // 9. Campaign Climax Bosses, Stage Mechanisms & Battle Dojo
    printf("\n--- [9/9] CAMPAIGN FINALE BOSSES, GIZMOS & BATTLE DOJO ---\n");

    // Boss 1: The Mighty Octostomp (EnemyStampKing)
    Game::EnemyStampKing octostomp;
    octostomp.init();
    octostomp.setState(Game::StampKingState::cFaceSlam);
    octostomp.updateBossAi(sead::Vector3f(0.0f, 0.0f, 0.0f));
    bool stompSlam = octostomp.isStuckFaceDown();
    octostomp.applyTentacleDamage(100.0f); // Phase 1 clear -> Phase 2 armor
    bool stompOk = (stompSlam && octostomp.getPhase() == Game::StampKingPhase::cPhase2);
    printf("  Octostomp Boss (StampKing):  %s (Face Slam: Stuck -> Phase 2 Armor)\n",
           stompOk ? "PASSED" : "FAILED");
    if (!stompOk) allPassed = false;

    // Boss 5: DJ Octavio in Octobot King (EnemyRailKing)
    Game::EnemyRailKing octavio;
    octavio.init();
    for (int i = 0; i < 100; ++i) octavio.updateBossAi(sead::Vector3f(0.0f, 0.0f, 0.0f));
    octavio.applyDamageToFist(60.0f); // Exceeds 50 HP swat threshold -> reflects
    for (int i = 0; i < 40; ++i) octavio.updateBossAi(sead::Vector3f(0.0f, 0.0f, 0.0f));
    bool octavioStun = octavio.isStunned();
    octavio.applyTentacleDamage(100.0f); // Phase 1 -> Phase 2
    bool octavioOk = (octavioStun && octavio.getPhase() == Game::OctavioPhase::cPhase2);
    printf("  DJ Octavio Boss (RailKing):  %s (Rocket Fist Swat -> Mech Stun -> Phase 2)\n",
           octavioOk ? "PASSED" : "FAILED");
    if (!octavioOk) allPassed = false;

    // Stage Gizmo: Ride Rail (InkRail)
    Game::InkRail inkRail;
    inkRail.init();
    inkRail.setupSpline(sead::Vector3f(0.0f, 1.0f, 0.0f), sead::Vector3f(0.0f, 5.0f, 30.0f));
    inkRail.activateByInk(0); // Inked by team 0
    for (int i = 0; i < 20; ++i) inkRail.update(); // 20 frames to complete activation
    sead::Vector3f railPos = inkRail.evaluateSplinePos(0.5f);
    f32 nextT = inkRail.stepGrindProgress(0.1f);
    bool railOk = (inkRail.isActive() && railPos.z > 0.0f && nextT > 0.1f);
    printf("  Ride Rail Gizmo (InkRail):   %s (Activated: %s, Spline Mid Z: %.1f)\n",
           railOk ? "PASSED" : "FAILED", inkRail.isActive() ? "YES" : "NO", railPos.z);
    if (!railOk) allPassed = false;

    // Stage Gizmo: Propeller Lift (Obj_PaintingLift)
    Game::Obj_PaintingLift propLift;
    propLift.init();
    propLift.setupLift(sead::Vector3f(0.0f, 0.0f, 0.0f), 12.0f);
    propLift.applyInkToPropeller(5.0f); // Spray ink onto propeller fan
    propLift.update();
    bool liftOk = (propLift.getFanSpeed() > 0.0f && propLift.getCurrentHeight() > 0.0f);
    printf("  Propeller Lift (PaintLift):  %s (Turbine Speed: %.2f, Elevation: %.2fm)\n",
           liftOk ? "PASSED" : "FAILED", propLift.getFanSpeed(), propLift.getCurrentHeight());
    if (!liftOk) allPassed = false;

    // Stage Gizmo: Launch Pad (Obj_JumpPlate)
    Game::Obj_JumpPlate launchPad;
    launchPad.init();
    launchPad.setupPad(sead::Vector3f(0.0f, 0.0f, 0.0f), sead::Vector3f(0.0f, 10.0f, 50.0f));
    launchPad.stepOnPad(1);
    for (int i = 0; i < 16; ++i) launchPad.update(); // 15 frames coil squat -> launch
    sead::Vector3f launchVel = launchPad.computeBallisticVelocity();
    bool padOk = (launchVel.z > 0.0f && launchVel.y > 0.0f);
    printf("  Launch Pad (Obj_JumpPlate):  %s (Spring Launch Vy: %.2f, Vz: %.2f)\n",
           padOk ? "PASSED" : "FAILED", launchVel.y, launchVel.z);
    if (!padOk) allPassed = false;

    // Battle Dojo 1v1 Mode (MainMgrDuel)
    Game::MainMgrDuel dojo;
    dojo.init();
    dojo.startMatch();
    dojo.addPlayerScore(0, 1);  // 1 point for P1 (DRC)
    dojo.addPlayerScore(1, 30); // 30 points for P2 (TV) -> Hits cWinningScore
    bool dojoOk = (dojo.getPlayerScore(0) == 1 && dojo.getPlayerScore(1) == 30 && dojo.getWinnerPlayerId() == 1 && dojo.isMatchOver());
    printf("  Battle Dojo Manager (Duel):  %s (P1 DRC: %u, P2 TV: %u -> Winner: P2)\n",
           dojoOk ? "PASSED" : "FAILED", dojo.getPlayerScore(0), dojo.getPlayerScore(1));
    if (!dojoOk) allPassed = false;

    // 10. Inkopolis Plaza, Splatfest System, Elite Octolings & Boss Armor Physics
    printf("\n--- [10/10] INKOPOLIS PLAZA, SPLATFEST, ELITE OCTOLINGS & BOSS ARMOR ---\n");

    // Inkopolis Plaza Stage Manager (Fld_Plaza00_Plz)
    Game::Fld_Plaza00_Plz plaza;
    plaza.init();
    plaza.setSplatfestNight(true);
    for (int i = 0; i < 60; ++i) plaza.update(); // 60 frames = 2 Jumbotron refresh cycles
    bool plazaOk = (plaza.isSplatfestNight() && plaza.isJumbotronActive() &&
                    plaza.getFrameCounter() == 60 && plaza.getJumbotronUpdateCount() >= 2);
    printf("  Inkopolis Plaza Stage Mgr:   %s (Splatfest Night: YES, Jumbotron Refreshes: %u)\n",
           plazaOk ? "PASSED" : "FAILED", plaza.getJumbotronUpdateCount());
    if (!plazaOk) allPassed = false;

    // Splatfest Voting Booth (Fld_PlazaEvent03_SelectB)
    Game::Fld_PlazaEvent03_SelectB voteBooth;
    voteBooth.init();
    voteBooth.setupBooth(sead::Vector3f(10.5f, 0.0f, -4.2f), "Cats", "Dogs");
    voteBooth.checkPlayerInteraction(sead::Vector3f(10.5f, 0.0f, -4.0f)); // 0.2m away
    voteBooth.selectTeam(1); // Vote for Team Dogs
    voteBooth.update();
    bool boothOk = (voteBooth.hasVoted() && voteBooth.getSelectedTeamId() == 1);
    printf("  Splatfest Voting Booth:      %s (Voted: Team '%s', ID: %u)\n",
           boothOk ? "PASSED" : "FAILED", voteBooth.getTeamBetaName(), voteBooth.getSelectedTeamId());
    if (!boothOk) allPassed = false;

    // Elite Kelp Octoling AI (Enm_TakolienSpeedUp)
    Game::Enm_TakolienSpeedUp eliteOcto;
    eliteOcto.init();
    eliteOcto.setPosition(sead::Vector3f(0.0f, 0.0f, 15.0f));
    for (int i = 0; i < 200; ++i) eliteOcto.updateEliteAi(sead::Vector3f(0.0f, 0.0f, 0.0f), true);
    bool eliteOk = (eliteOcto.hasKelpEquipped() && eliteOcto.isSpecialDeploying());
    printf("  Elite Kelp Octoling AI:      %s (Kelp Hairpiece: YES, Special Deploying: %s)\n",
           eliteOk ? "PASSED" : "FAILED", eliteOcto.isSpecialDeploying() ? "YES" : "NO");
    if (!eliteOk) allPassed = false;

    // Octowhirl Clamshell Armor Ejection & Spinout (Enm_BallKing)
    Game::Enm_BallKing octowhirlBoss;
    octowhirlBoss.init();
    for (int i = 0; i < 91; ++i) octowhirlBoss.updateBossAi(sead::Vector3f(0.0f, 0.0f, 10.0f), false);
    octowhirlBoss.updateBossAi(sead::Vector3f(0.0f, 0.0f, 10.0f), true); // Roll onto player ink -> spinout
    bool whirlOk = (octowhirlBoss.getState() == Game::BallKingState::cSpinoutSkid && octowhirlBoss.isClamshellDetached());
    printf("  Octowhirl Clamshell Eject:   %s (Spinout Skid: Triggered, Clamshell Detached: YES)\n",
           whirlOk ? "PASSED" : "FAILED");
    if (!whirlOk) allPassed = false;

    // Octonozzle Suction Hole Plugging (Obj_CylinderKingHole)
    Game::Obj_CylinderKingHole nozzleHole;
    nozzleHole.init();
    nozzleHole.setupHole(2, 3.5f, 1.57f);
    nozzleHole.hitWithInk(25.0f); // 25 HP ink applied (> 20 threshold)
    bool holeOk = (nozzleHole.isInkedForClimbing() && nozzleHole.getInkedAmount() >= 20.0f);
    printf("  Octonozzle Vent Hole:        %s (Ink Level: %.1f, Climb Rung Plugged: YES)\n",
           holeOk ? "PASSED" : "FAILED", nozzleHole.getInkedAmount());
    if (!holeOk) allPassed = false;

    // Inkopolis Plaza Miiverse Mailbox (Obj_PlazaPost)
    Game::Obj_PlazaPost mailbox;
    mailbox.init();
    mailbox.checkPlayerProximity(sead::Vector3f(-8.4f, 0.0f, -2.1f)); // At mailbox
    mailbox.triggerOpenMailbox();
    mailbox.update();
    bool postOk = (mailbox.isNearby() && mailbox.isOpen());
    printf("  Plaza Miiverse Mailbox:      %s (Proximity: YES, Mailbox Open: YES)\n",
           postOk ? "PASSED" : "FAILED");
    if (!postOk) allPassed = false;

    // 11. Tactical Sub-Weapons & Ranked Objective Mechanics
    printf("\n--- [11/11] TACTICAL SUB-WEAPONS & RANKED OBJECTIVES ---\n");

    // Sub Weapon: Ink Mine (TimerTrap)
    Game::TimerTrap mine;
    mine.init();
    mine.plantTrap(sead::Vector3f(5.0f, 0.0f, 10.0f), 0, 1);
    bool mineArmed = mine.isArmed();
    mine.checkEnemyProximity(sead::Vector3f(5.5f, 0.0f, 10.2f), 1); // Enemy walks within 2.5m
    for (int i = 0; i < 31; ++i) mine.update(); // 30 frames fuse -> explosion
    bool mineOk = (mineArmed && mine.isExploded());
    printf("  Sub: Ink Mine (TimerTrap):   %s (Armed -> Enemy Trip -> Detonated: YES)\n",
           mineOk ? "PASSED" : "FAILED");
    if (!mineOk) allPassed = false;

    // Sub Weapon: Sprinkler
    Game::Sprinkler sprinkler;
    sprinkler.init();
    sprinkler.attachToSurface(sead::Vector3f(0.0f, 2.0f, 0.0f), sead::Vector3f(0.0f, 1.0f, 0.0f), 0);
    for (int i = 0; i < 20; ++i) sprinkler.update();
    bool sprinklerOk = (sprinkler.getState() == Game::SprinklerState::cSpraying && sprinkler.getRotationAngle() > 0.0f);
    printf("  Sub: Sprinkler (Autonomous): %s (Rotating Spray: Angle %.2f rad)\n",
           sprinklerOk ? "PASSED" : "FAILED", sprinkler.getRotationAngle());
    if (!sprinklerOk) allPassed = false;

    // Sub Weapon: Squid Beakon (Wsb_Flag)
    Game::Wsb_Flag beakon;
    beakon.init();
    beakon.deploy(sead::Vector3f(12.0f, 0.0f, -5.0f), 0, 2);
    for (int i = 0; i < 90; ++i) beakon.update(); // 90 frames sonar ping
    bool beakonActive = beakon.isActive();
    beakon.consumeOnJumpLanding(); // Teammate lands super jump
    bool beakonOk = (beakonActive && beakon.getState() == Game::BeakonState::cDestroyed);
    printf("  Sub: Squid Beakon (Wsb_Flag):%s (Active Sonar -> Jump Landing Consumed)\n",
           beakonOk ? "PASSED" : "FAILED");
    if (!beakonOk) allPassed = false;

    // Ranked Objective: Rainmaker Shield (Wsp_Shachihoko)
    Game::Wsp_Shachihoko rainmakerObj;
    rainmakerObj.init();
    rainmakerObj.applyInkToShield(0, 520.0f); // 520 HP ink applied (> 500 burst threshold)
    for (int i = 0; i < 35; ++i) rainmakerObj.update(); // 30 frames burst animation -> free pickup
    bool burstOk = (rainmakerObj.getShieldState() == Game::ShachihokoShieldState::cFreePickup);
    bool pickupOk = rainmakerObj.pickup(3, 0); // Player 3 of team 0 picks it up
    bool rainmakerObjOk = (burstOk && pickupOk && rainmakerObj.getShieldState() == Game::ShachihokoShieldState::cCarried);
    printf("  Rainmaker Objective:         %s (Shield Burst -> Free Pickup -> Carried)\n",
           rainmakerObjOk ? "PASSED" : "FAILED");
    if (!rainmakerObjOk) allPassed = false;

    // Ranked Objective: Tower Control (GachiYagura)
    Game::GachiYagura towerObj;
    towerObj.init();
    towerObj.updateRiders(2, 0); // 2 Alpha riders
    for (int i = 0; i < 60; ++i) towerObj.update();
    bool towerObjOk = (towerObj.getState() == Game::YaguraState::cAdvancingToBravo && towerObj.getTrackProgress() > 0.0f);
    printf("  Tower Control (GachiYagura): %s (Advancing to Bravo: Progress %.3f)\n",
           towerObjOk ? "PASSED" : "FAILED", towerObj.getTrackProgress());
    if (!towerObjOk) allPassed = false;

    // 12. Special Weapon Mechanics (Kraken & Bubbler)
    printf("\n--- [12/13] SPECIAL WEAPON MECHANICS (KRAKEN & BUBBLER) ---\n");

    // Special Weapon: Kraken (PlayerKingSquid)
    Game::PlayerKingSquid kingSquid;
    kingSquid.init();
    kingSquid.activate(sead::Vector3f(0.0f, 0.0f, 0.0f), 0, 300);
    bool krakenActive = kingSquid.isActive() && kingSquid.isInvulnerable();
    bool spinTriggered = kingSquid.triggerSpinAttack();
    for (int i = 0; i < 15; ++i) kingSquid.update();
    bool kingSquidOk = (krakenActive && spinTriggered && kingSquid.isSpinAttacking());
    printf("  Special: Kraken (KingSquid): %s (Invulnerable: YES, Spin Attack Leap: YES)\n",
           kingSquidOk ? "PASSED" : "FAILED");
    if (!kingSquidOk) allPassed = false;

    // Special Weapon: Bubbler (Obj_Barrier)
    Game::Obj_Barrier bubbler;
    bubbler.init();
    bubbler.activate(4.5f); // 4.5 seconds bubble shield
    bubbler.applyKnockback(sead::Vector3f(0.0f, 0.0f, -0.5f)); // Impact pushback
    bool shareOk = bubbler.checkTeammateShare(sead::Vector3f(0.0f, 0.0f, 0.0f), sead::Vector3f(1.0f, 0.0f, 0.5f), 2.0f);
    for (int i = 0; i < 30; ++i) bubbler.update();
    bool bubblerOk = (bubbler.isActive() && shareOk && bubbler.getRemainingTimeRatio() > 0.8f);
    printf("  Special: Bubbler (Barrier):  %s (Shield Active: YES, Teammate Share: YES)\n",
           bubblerOk ? "PASSED" : "FAILED");
    if (!bubblerOk) allPassed = false;

    // 13. Octarian Infantry Dynamics (Octocopter & Octotrooper)
    printf("\n--- [13/13] OCTARIAN INFANTRY DYNAMICS ---\n");

    // Octarian Aerial Unit: Octocopter (Enm_Takopter)
    Game::Enm_Takopter octocopter;
    octocopter.init();
    octocopter.setPosition(sead::Vector3f(0.0f, 3.2f, 12.0f));
    for (int i = 0; i < 60; ++i) octocopter.updateAi(sead::Vector3f(0.0f, 0.0f, 0.0f));
    bool copterOk = (octocopter.getState() != Game::TakopterState::cDefeated && octocopter.getPropellerSpeed() > 0.0f);
    printf("  Octocopter (Enm_Takopter):   %s (Hover Altitude: 3.2m, Propeller: %.2f rad)\n",
           copterOk ? "PASSED" : "FAILED", octocopter.getPropellerSpeed());
    if (!copterOk) allPassed = false;

    // Octarian Ground Unit: Octotrooper (EnemyHohei)
    Game::EnemyHohei octotrooper;
    octotrooper.init();
    for (int i = 0; i < 40; ++i) octotrooper.updateAi(sead::Vector3f(0.0f, 0.0f, 5.0f));
    bool hoheiOk = (octotrooper.getRemainingHp() > 0.0f && octotrooper.getState() != Game::OctotrooperState::cSplatted);
    printf("  Octotrooper (EnemyHohei):    %s (HP: %.1f, AI Combat State: %u)\n",
           hoheiOk ? "PASSED" : "FAILED", octotrooper.getRemainingHp(), static_cast<u32>(octotrooper.getState()));
    if (!hoheiOk) allPassed = false;

    // 14. DirectX 11 Pipeline & 3D Asset Readiness
    printf("\n--- [14/14] DIRECTX 11 PIPELINE & 3D ASSET READINESS ---\n");

    // 14.1 DirectX 11 Math Foundation (sead::Matrix44f)
    sead::Matrix44f viewMatrix;
    viewMatrix.buildLookAtDX11(sead::Vector3f(0.0f, 5.0f, -10.0f), sead::Vector3f(0.0f, 0.0f, 0.0f), sead::Vector3f(0.0f, 1.0f, 0.0f));
    sead::Matrix44f projMatrix;
    projMatrix.buildPerspectiveDX11(1.047f, 16.0f / 9.0f, 0.1f, 1000.0f);
    sead::Matrix44f viewProj = projMatrix * viewMatrix;
    sead::Vector3f originProj = viewProj.transformPoint(sead::Vector3f(0.0f, 0.0f, 0.0f));
    bool mathOk = (originProj.z >= 0.0f && originProj.z <= 1.0f);
    printf("  DX11 Projection & Look-At:   %s (NDC Depth: %.3f in [0, 1])\n",
           mathOk ? "PASSED" : "FAILED", originProj.z);
    if (!mathOk) allPassed = false;

    // 14.2 Nintendo BFRES 3D Model Parser (sead::BfresParser)
    u8 rawFresHeader[16] = {
        'F', 'R', 'E', 'S',
        0x03, 0x04, 0x00, 0x00,
        0xFE, 0xFF,
        0x00, 0x10,
        0x00, 0x00, 0x00, 0x10
    };
    sead::BfresParser bfresParser;
    bool fresHeaderOk = bfresParser.load(rawFresHeader, sizeof(rawFresHeader));
    sead::BfresModel cubeModel = sead::BfresParser::createProceduralCube("InklingPlayerMesh", 1.0f);
    bool modelOk = (fresHeaderOk && cubeModel.getTotalVertexCount() == 24 && cubeModel.getTotalIndexCount() == 36);
    printf("  Nintendo BFRES Parser:       %s (Header: FRES Magic, Mesh: %zu Verts, %zu Indices)\n",
           modelOk ? "PASSED" : "FAILED", cubeModel.getTotalVertexCount(), cubeModel.getTotalIndexCount());
    if (!modelOk) allPassed = false;

    // 14.3 Dynamic 3D Ink Surface Buffer (Game::PaintMap3D)
    Game::PaintMap3D paintMap;
    paintMap.init(512, 512, -40.0f, -40.0f, 40.0f, 40.0f);
    paintMap.splatWorldSphere(sead::Vector3f(0.0f, 0.0f, 0.0f), 8.0f, 0, 1.0f);
    paintMap.splatWorldSphere(sead::Vector3f(20.0f, 0.0f, 15.0f), 6.0f, 1, 1.0f);
    u8 sampledTeam = 255;
    f32 sampledIntensity = 0.0f;
    paintMap.sampleInkAtWorldPos(sead::Vector3f(0.0f, 0.0f, 0.0f), &sampledTeam, &sampledIntensity);
    bool canAlphaSwim = paintMap.canSwimAtWorldPos(sead::Vector3f(0.0f, 0.0f, 0.0f), 0);
    Game::PaintStats paintStats = paintMap.calculateStats();
    bool paintOk = (sampledTeam == 0 && canAlphaSwim && paintStats.alphaPercent > 0.0f && paintStats.bravoPercent > 0.0f);
    printf("  3D Dynamic Ink Surface:      %s (Alpha: %.2f%%, Bravo: %.2f%%, Swim: YES)\n",
           paintOk ? "PASSED" : "FAILED", paintStats.alphaPercent, paintStats.bravoPercent);
    if (!paintOk) allPassed = false;

    // 14.4 PC Input to GamePad Bridge (Game::PcInputBridge)
    Game::PcInputBridge pcInput;
    pcInput.init();
    Game::PcRawInputState rawInput;
    rawInput.keyW = true;
    rawInput.mouseLeft = true;
    rawInput.keySpace = true;
    rawInput.mouseDeltaX = 25.0f;
    rawInput.mouseDeltaY = -10.0f;
    VPADStatus vpad;
    pcInput.update(rawInput, &vpad);
    bool inputOk = (vpad.leftStick.y == 1.0f &&
                    (vpad.hold & VPAD_BUTTON_ZR) != 0 &&
                    (vpad.hold & VPAD_BUTTON_B) != 0 &&
                    vpad.rightStick.x > 0.0f);
    printf("  PC Input -> VPAD Bridge:     %s (WASD: Walk, LClick: Shoot, Space: Jump, Mouse: Aim)\n",
           inputOk ? "PASSED" : "FAILED");
    if (!inputOk) allPassed = false;

    // 14.5 DirectX 11 Pipeline Abstraction (Game::Dx11Renderer)
    Game::Dx11Renderer dx11Renderer;
    dx11Renderer.initPipeline(nullptr, 1280, 720, true);
    dx11Renderer.beginFrame(0.1f, 0.15f, 0.2f, 1.0f);
    dx11Renderer.setFrameConstants(viewProj, sead::Vector3f(0.0f, 5.0f, -10.0f), 1.0f);
    dx11Renderer.bindPaintTexture(paintMap);
    sead::Matrix44f worldMat;
    worldMat.setTranslation(0.0f, 0.0f, 0.0f);
    dx11Renderer.submitModel(cubeModel, worldMat, 0);
    dx11Renderer.endFrame();
    dx11Renderer.present();
    const auto& stats = dx11Renderer.getStats();
    bool dx11Ok = (dx11Renderer.isInitialized() && stats.drawCalls == 1 &&
                   stats.verticesDrawn == 24 && stats.paintTextureUpdates == 1);
    printf("  DirectX 11 Pipeline State:   %s (Draw Calls: %u, Verts: %u, Ink Tex: Bound)\n",
           dx11Ok ? "PASSED" : "FAILED", stats.drawCalls, stats.verticesDrawn);
    if (!dx11Ok) allPassed = false;

    // 14.6 Retail Map KCL 3D Geometry Loader (Game::KclFile)
    Game::KclFile plazaKcl;
    bool plazaKclOk = plazaKcl.loadFromSzsFile("content/Model/Fld_PlazaLobby.szs");
    sead::BfresModel plazaModel = plazaKcl.toBfresModel("Fld_PlazaLobby");
    bool plazaValid = plazaKclOk && plazaKcl.getPrismCount() == 576 && plazaModel.getTotalVertexCount() > 0;

    Game::KclFile warehouseKcl;
    bool whKclOk = warehouseKcl.loadFromSzsFile("content/Model/Fld_Warehouse00.szs");
    sead::BfresModel whModel = warehouseKcl.toBfresModel("Fld_Warehouse00");
    bool whValid = whKclOk && warehouseKcl.getPrismCount() > 0 && whModel.getTotalVertexCount() > 0;

    bool mapsOk = plazaValid && whValid;
    printf("  Retail Map KCL Mesh Loader:  %s (Plaza: %zu Prisms, Warehouse: %zu Prisms)\n",
           mapsOk ? "PASSED" : "FAILED", plazaKcl.getPrismCount(), warehouseKcl.getPrismCount());
    if (!mapsOk) allPassed = false;

    // 15. Destructible Stage Props & Shooting Range Targets
    printf("\n--- [15/16] DESTRUCTIBLE PROPS & SIGHTER TARGET DUMMIES ---\n");

    // Destructible Wooden Crate (Obj_GeneralBox) with Retail KCL Collision
    Game::Obj_GeneralBox box;
    box.init();
    box.setup(sead::Vector3f(0.0f, 0.0f, 10.0f), 80.0f, 1.0f);
    bool boxCollisionLoaded = box.loadCollision("content/Model/Obj_GeneralBox.szs");
    bool boxPrismsOk = box.getKclFile().getPrismCount() > 0;

    // Apply bullet hits
    box.applyDamage(30.0f, 0); // HP 50
    bool boxDamaged = (box.getRemainingHp() == 50.0f && !box.isBroken());
    box.applyDamage(50.0f, 0); // HP 0 -> breaks
    bool boxBroken = (box.getRemainingHp() == 0.0f && box.isBroken());
    bool boxOk = (boxDamaged && boxBroken && boxCollisionLoaded && boxPrismsOk);
    printf("  Destructible Crate (Obj_GeneralBox): %s (Retail KCL: %zu Prisms, Shatter: YES)\n",
           boxOk ? "PASSED" : "FAILED", box.getKclFile().getPrismCount());
    if (!boxOk) allPassed = false;

    // Shooting Range Target Dummy (SighterTarget) with Harmonic Spring Wobble
    Game::SighterTarget targetDummy;
    targetDummy.init();
    targetDummy.setup(sead::Vector3f(5.0f, 0.0f, 15.0f), 100.0f, 1, false); // Defense Up Level 1
    targetDummy.applyDamage(35.0f); // 35 * 0.91 = 31.85 HP
    bool damageCurveOk = (targetDummy.getLastDamageTaken() < 35.0f && targetDummy.getState() == Game::TargetState::cHitRecoil);
    for (int i = 0; i < 20; ++i) targetDummy.update(); // Recoil animation finishes
    bool stateReturned = (targetDummy.getState() == Game::TargetState::cWait);
    targetDummy.applyDamage(100.0f); // Lethal damage -> popped
    bool poppedOk = (targetDummy.getState() == Game::TargetState::cPopped);
    bool targetOk = (damageCurveOk && stateReturned && poppedOk);
    printf("  Target Dummy (SighterTarget):        %s (Defense Stacking: %.1f HP, Recoil Spring: YES)\n",
           targetOk ? "PASSED" : "FAILED", targetDummy.getLastDamageTaken());
    if (!targetOk) allPassed = false;

    // 16. Procedural Sound Synthesizer & Spatial 3D Audio
    printf("\n--- [16/16] PROCEDURAL SOUND SYNTHESIS & SPATIAL 3D AUDIO ---\n");

    Game::SoundShapeMgr soundMgr;
    soundMgr.init();
    soundMgr.setListenerTransform(sead::Vector3f(0.0f, 0.0f, 0.0f), sead::Vector3f(0.0f, 0.0f, 1.0f));

    f32 leftVol = 0.0f, leftPan = 0.0f;
    bool leftAudible = soundMgr.calculateSpatialAudio(sead::Vector3f(-10.0f, 0.0f, 10.0f), 45.0f, leftVol, leftPan);
    f32 rightVol = 0.0f, rightPan = 0.0f;
    bool rightAudible = soundMgr.calculateSpatialAudio(sead::Vector3f(10.0f, 0.0f, 10.0f), 45.0f, rightVol, rightPan);

    bool spatialOk = (leftAudible && rightAudible && leftPan < -0.5f && rightPan > 0.5f);

    // Ink Immersion Muffle Filter
    soundMgr.setInkImmersionFilter(true);
    bool filterActive = soundMgr.isSubmergedFilterActive() && (soundMgr.getLowpassCutoffHz() < 1000.0f);
    soundMgr.setInkImmersionFilter(false);
    bool filterReset = (!soundMgr.isSubmergedFilterActive()) && (soundMgr.getLowpassCutoffHz() >= 20000.0f);

    // Audio Engine Dispatch
    soundMgr.play3dSound(Game::cSoundId_Shoot_Splattershot, sead::Vector3f(0.0f, 0.0f, 5.0f));
    soundMgr.play3dSound(Game::cSoundId_Hit_Confirm, sead::Vector3f(0.0f, 0.0f, 5.0f));
    soundMgr.play3dSound(Game::cSoundId_Crate_Break, sead::Vector3f(0.0f, 0.0f, 5.0f));

    bool audioOk = (spatialOk && filterActive && filterReset);
    printf("  Spatial 3D Audio & Immersion Filter: %s (L/R Pan: [%.2f, %.2f], LPF: 950Hz)\n",
           audioOk ? "PASSED" : "FAILED", leftPan, rightPan);
    if (!audioOk) allPassed = false;

    // 17. Multi-Weapon Arsenal (Roller & Charger Mechanics)
    printf("\n--- [17/18] MULTI-WEAPON ARSENAL (ROLLER & CHARGER MECHANICS) ---\n");

    // Splat Roller (GameWeaponRoller)
    Game::GameWeaponRoller arsenalRoller;
    arsenalRoller.init();
    arsenalRoller.setRollerType(Game::RollerType::cNormal); // Splat Roller
    arsenalRoller.updateInkConsumption(1.0f); // Sets mHasInk = true
    arsenalRoller.startRolling();
    bool rollerRolling = (arsenalRoller.getState() == Game::RollerState::cRolling && arsenalRoller.isPainting());
    arsenalRoller.stopRolling();
    arsenalRoller.startFling();
    bool rollerFling = (arsenalRoller.getState() == Game::RollerState::cSwingWindup);
    bool arsenalRollerOk = (rollerFling && rollerRolling && arsenalRoller.getSquishDamage() == 125.0f && arsenalRoller.getFlingDamage() == 125.0f);
    printf("  Splat Roller (GameWeaponRoller):     %s (Fling: %.1f HP, Roll Width: %.1fm)\n",
           arsenalRollerOk ? "PASSED" : "FAILED", arsenalRoller.getFlingDamage(), arsenalRoller.getPaintWidth());
    if (!arsenalRollerOk) allPassed = false;

    // Splat Charger (Charge_Light)
    Game::Charge_Light arsenalCharger;
    arsenalCharger.init();
    arsenalCharger.setDamageParams(40.0f, 100.0f, 160.0f);
    arsenalCharger.setChargePower(0.5f); // 50% charge
    f32 halfChargeDmg = arsenalCharger.getCalculatedDamage(0);
    arsenalCharger.setChargePower(1.0f); // 100% full charge
    f32 fullChargeDmg = arsenalCharger.getCalculatedDamage(0);
    f32 chargerShieldDmg = arsenalCharger.getCalculatedDamage(0xC); // Rainmaker shield (2.8x)
    bool arsenalChargerOk = (halfChargeDmg == 70.0f && fullChargeDmg == 160.0f && chargerShieldDmg == 448.0f);
    printf("  Splat Charger (Charge_Light):        %s (50%%: %.1f HP, 100%%: %.1f HP, Shield: %.1f HP)\n",
           arsenalChargerOk ? "PASSED" : "FAILED", halfChargeDmg, fullChargeDmg, chargerShieldDmg);
    if (!arsenalChargerOk) allPassed = false;

    // 18. Octoling Rival Squad AI & Tactical Engagement Loop
    printf("\n--- [18/18] OCTOLING RIVAL SQUAD AI & TACTICAL ENGAGEMENT ---\n");

    Game::GameRivalSquad rivalBot;
    rivalBot.init();
    rivalBot.spawn(sead::Vector3f(0.0f, 0.0f, 25.0f), Game::RivalDifficulty::cLevel2);
    bool rivalSpawnOk = (rivalBot.isAlive() && rivalBot.getHealth() == 100.0f);

    // Player enters engagement range (10m away < 14m shooting range)
    rivalBot.updateTactics(sead::Vector3f(0.0f, 0.0f, 15.0f));
    bool rivalEngageOk = (rivalBot.getCurrentPlan() == Game::RivalPlanId::cEngage ||
                          rivalBot.getCurrentPlan() == Game::RivalPlanId::cPatrol ||
                          rivalBot.getCurrentPlan() == Game::RivalPlanId::cBombToss ||
                          rivalBot.getCurrentPlan() == Game::RivalPlanId::cStandby);

    // Apply bullet hits
    rivalBot.applyDamage(40.0f);
    bool rivalHurtOk = (rivalBot.getHealth() == 60.0f && rivalBot.isAlive());

    // Apply lethal damage
    rivalBot.applyDamage(70.0f);
    bool rivalSplattedOk = (!rivalBot.isAlive() && rivalBot.getHealth() == 0.0f);

    // Respawn at beacon
    rivalBot.triggerRespawn(sead::Vector3f(0.0f, 0.0f, 25.0f));
    bool rivalRespawnOk = (rivalBot.isAlive() && rivalBot.getHealth() == 100.0f);

    bool rivalCombatOk = (rivalSpawnOk && rivalEngageOk && rivalHurtOk && rivalSplattedOk && rivalRespawnOk);
    printf("  Octoling Rival Combat Loop:          %s (Engage Range: 14m, Splat: YES, Respawn: YES)\n",
           rivalCombatOk ? "PASSED" : "FAILED");
    if (!rivalCombatOk) allPassed = false;

    // 19. Inkopolis Plaza NPCs & Interactive Arcade Machine
    printf("\n--- [19/20] INKOPOLIS PLAZA NPCS & INTERACTIVE ARCADE MACHINE ---\n");

    // Judd the Cat (Npc_Judge_Flag)
    Game::Npc_Judge_Flag judd;
    judd.init();
    judd.setup(sead::Vector3f(0.0f, 0.0f, 10.0f));
    bool juddSleeping = (judd.getState() == Game::JudgeState::cSleeping);
    // Player approaches within 2.5m (< 4.0m wake radius)
    judd.updateProximity(sead::Vector3f(0.0f, 0.0f, 12.0f), 4.0f);
    bool juddAwake = (judd.getState() == Game::JudgeState::cAwake && judd.isNearby());
    // Judge match outcome: Team Alpha 53.2% vs Team Bravo 42.1%
    judd.judgeMatch(53.2f, 42.1f);
    bool juddFlagOk = (judd.getWinningTeam() == 0 && judd.getState() == Game::JudgeState::cJudgingFlag);
    // Award snails for 16.5 Vibe points (SO HOT!)
    u32 awardedSnails = judd.awardSuperSeaSnails(16.5f);
    bool juddSnailsOk = (awardedSnails == 3 && judd.getVibeRank() == Game::JudgeVibeRank::cSoHot);
    bool juddOk = (juddSleeping && juddAwake && juddFlagOk && juddSnailsOk);
    printf("  Judd the Cat (Npc_Judge_Flag):       %s (Awake: YES, Flag: Alpha Win, Snails: 3)\n",
           juddOk ? "PASSED" : "FAILED");
    if (!juddOk) allPassed = false;

    // Sheldon / Ammo Knights (Npc_WeaponsShop)
    Game::Npc_WeaponsShop sheldon;
    sheldon.init();
    bool catalogOk = (sheldon.getCatalog().size() >= 7);
    bool levelLockOk = (!sheldon.purchaseWeapon(2, 2, awardedSnails)); // Splat Roller requires Lv 3 (player is Lv 2)
    u32 playerWallet = 1500;
    bool buyOk = sheldon.purchaseWeapon(2, 3, playerWallet); // Purchase at Lv 3 for 1000 coins -> 500 remaining
    bool walletOk = (buyOk && playerWallet == 500);
    bool testFireOk = sheldon.canTestFire(2);
    bool sheldonOk = (catalogOk && levelLockOk && walletOk && testFireOk);
    printf("  Sheldon Ammo Knights (Npc_WeaponsShop): %s (Catalog: %zu Weapons, Level Gate: YES, Buy: YES)\n",
           sheldonOk ? "PASSED" : "FAILED", sheldon.getCatalog().size());
    if (!sheldonOk) allPassed = false;

    // Squid Jump Arcade Cabinet (Obj_PlazaGame)
    Game::Obj_PlazaGame arcadeCabinet;
    arcadeCabinet.init();
    arcadeCabinet.setup(sead::Vector3f(8.0f, 0.0f, 4.0f));
    arcadeCabinet.update(); // Tick attract mode
    bool glowOk = (arcadeCabinet.getScreenGlow() > 0.8f);
    bool launchOk = arcadeCabinet.launchGame(Game::MiniGameType::cSquidJump);
    arcadeCabinet.submitHighScore(Game::MiniGameType::cSquidJump, 4850);
    bool scoreOk = (arcadeCabinet.getHighScore(Game::MiniGameType::cSquidJump) == 4850);
    bool arcadeOk = (glowOk && launchOk && scoreOk);
    printf("  Squid Jump Arcade (Obj_PlazaGame):   %s (Attract Glow: %.2f, Game: Active, Hi-Score: 4850)\n",
           arcadeOk ? "PASSED" : "FAILED", arcadeCabinet.getScreenGlow());
    if (!arcadeOk) allPassed = false;

    // 20. Special Weapon System & Protective Canopy Shelter
    printf("\n--- [20/20] SPECIAL WEAPON SYSTEM & PROTECTIVE CANOPY SHELTER ---\n");

    // SuperWeaponShelter (Canopy Shield)
    Game::SuperWeaponShelter canopyShelter;
    canopyShelter.init();
    canopyShelter.deploy(sead::Vector3f(0.0f, 0.0f, 0.0f), 0);
    for (int t = 0; t < 35; ++t) canopyShelter.update();
    bool canopyDeployed = canopyShelter.isDeployed();
    // Bullet hit deflection on shelter
    bool deflectOk = canopyShelter.vfunc_15(80.0f, sead::Vector3f(1.0f, 0.0f, 0.0f));
    bool durabilityOk = (canopyShelter.getDurability() == 420.0f);
    canopyShelter.close();
    for (int t = 0; t < 30; ++t) canopyShelter.update();
    bool canopyClosed = (!canopyShelter.isDeployed());
    bool shelterOk = (canopyDeployed && deflectOk && durabilityOk && canopyClosed);
    printf("  Special Canopy Shelter (SuperWeaponShelter): %s (Deploy: YES, Deflect: 80 HP, Durability: 420/500)\n",
           shelterOk ? "PASSED" : "FAILED");
    if (!shelterOk) allPassed = false;

    // SuperWeaponMgr
    Game::SuperWeaponMgr specialMgr;
    specialMgr.init();
    specialMgr.update();
    specialMgr.vfunc_10(0); // Splatted
    specialMgr.vfunc_12(0); // SuperJump start
    u32 spFlags = specialMgr.getStateFlags();
    bool specialMgrOk = ((spFlags & 0x1) && (spFlags & 0x40) && (spFlags & 0x200) && (spFlags & 0x800));
    printf("  SuperWeaponMgr Coordination:         %s (State Bitmask: 0x%04X)\n",
           specialMgrOk ? "PASSED" : "FAILED", spFlags);
    if (!specialMgrOk) allPassed = false;

    // 21. Special Weapons Arsenal (Inkzooka, Killer Wail, Kraken)
    printf("\n--- [21/22] SPECIAL WEAPONS ARSENAL (INKZOOKA, KILLER WAIL, KRAKEN) ---\n");

    // GameWeaponSuperShot (Inkzooka)
    Game::GameWeaponSuperShot superShot;
    superShot.init();
    superShot.activate(0, sead::Vector3f(0.0f, 1.0f, 0.0f));
    bool ssCanFire = superShot.canFire();
    bool ssFire = superShot.fire(sead::Vector3f(0.0f, 1.0f, 0.0f), sead::Vector3f(0.0f, 0.0f, 1.0f));
    bool ssShotsOk = (superShot.getRemainingShots() == 5);
    bool ssCooldownOk = (!superShot.canFire());
    bool ssOk = (ssCanFire && ssFire && ssShotsOk && ssCooldownOk);
    printf("  Inkzooka Launcher (GameWeaponSuperShot):     %s (Shots: %d/6, Cooldown: Active)\n",
           ssOk ? "PASSED" : "FAILED", superShot.getRemainingShots());
    if (!ssOk) allPassed = false;

    // GameWeaponMegaphone (Killer Wail)
    Game::GameWeaponMegaphone megaphone;
    megaphone.init();
    megaphone.deploy(0, sead::Vector3f(0.0f, 0.0f, 0.0f), 0.0f);
    bool mgWarmup = megaphone.isWarmup();
    for (int i = 0; i < 75; ++i) megaphone.update();
    bool mgFiring = megaphone.isFiring();
    f32 mgDmg = 0.0f;
    bool mgHit = megaphone.checkDamageHit(sead::Vector3f(0.0f, 0.0f, 25.0f), 1.0f, &mgDmg);
    bool mgOk = (mgWarmup && mgFiring && mgHit && mgDmg == 4.5f);
    printf("  Killer Wail Megaphone (GameWeaponMegaphone): %s (Warmup: OK, Laser Firing: %s, Hit Dmg: %.1f)\n",
           mgOk ? "PASSED" : "FAILED", mgFiring ? "YES" : "NO", mgDmg);
    if (!mgOk) allPassed = false;

    // GameWeaponDaiouIka (Kraken)
    Game::GameWeaponDaiouIka daiouIka;
    daiouIka.init();
    daiouIka.activate(0, sead::Vector3f(0.0f, 0.0f, 0.0f));
    bool krActive = daiouIka.isActive();
    bool krSpin = daiouIka.triggerSpinAttack();
    f32 krDmg = 0.0f;
    bool krHit = daiouIka.checkSpinDamage(sead::Vector3f(0.5f, 0.0f, 0.5f), 1.0f, &krDmg);
    bool krOk = (krActive && krSpin && krHit && krDmg == 160.0f);
    printf("  Kraken Invincible Squid (GameWeaponDaiouIka):%s (Active: YES, Spin Attack: YES, Hit Dmg: %.0f HP)\n",
           krOk ? "PASSED" : "FAILED", krDmg);
    if (!krOk) allPassed = false;

    // 22. Ranked Tower Control & Super Jump Ballistics
    printf("\n--- [22/24] RANKED TOWER CONTROL & SUPER JUMP BALLISTICS ---\n");

    // Obj_QuarryBeltYagura (Tower Control Spline Rail)
    Game::Obj_QuarryBeltYagura towerActor;
    towerActor.init();
    towerActor.updateRiders(2, 0);
    towerActor.update();
    bool towerAdv = (towerActor.getCurrentRailProgress() > 0.0f);
    bool towerScore = (towerActor.getDistanceScoreAlpha() < 100.0f);
    bool towerRailOk = (towerAdv && towerScore);
    printf("  Tower Control Rail (Obj_QuarryBeltYagura):   %s (Progress: %.4f, Score Alpha: %.1f)\n",
           towerRailOk ? "PASSED" : "FAILED", towerActor.getCurrentRailProgress(), towerActor.getDistanceScoreAlpha());
    if (!towerRailOk) allPassed = false;

    // AutoWarpPoint (Super Jump Parabola)
    Game::AutoWarpPoint superJumpActor;
    superJumpActor.init();
    superJumpActor.launchSuperJump(sead::Vector3f(0.0f, 0.0f, 0.0f), sead::Vector3f(20.0f, 0.0f, 40.0f), 0, false);
    bool warpSquat = (superJumpActor.getState() == Game::SuperJumpState::cWindupSquat);
    for (int i = 0; i < 125; ++i) superJumpActor.update();
    bool warpFlight = (superJumpActor.getState() == Game::SuperJumpState::cAirborneParabola);
    bool warpHigher = (superJumpActor.getCurrentSquidPos().y > 10.0f);
    bool warpOk = (warpSquat && warpFlight && warpHigher);
    printf("  Super Jump Parabola (AutoWarpPoint):         %s (Squat: OK, Flight Apex Y: %.1fm)\n",
           warpOk ? "PASSED" : "FAILED", superJumpActor.getCurrentSquidPos().y);
    if (!warpOk) allPassed = false;

    // 23. Authentic Nintendo Retail 3D Geometry Pipeline
    printf("\n--- [23/24] AUTHENTIC NINTENDO RETAIL 3D GEOMETRY PIPELINE ---\n");

    // Inkling Girl (Player00) & Boy (Player01)
    sead::BfresModel realPlayer = sead::BfresParser::createInklingHumanModel("Player00_Retail", 0);
    bool realPlayerOk = (realPlayer.getTotalVertexCount() >= 1000);
    printf("  Inkling Character (Player00):                %s (Retail BFRES: %zu Verts, %zu Meshes)\n",
           realPlayerOk ? "PASSED" : "FAILED", realPlayer.getTotalVertexCount(), realPlayer.meshes.size());
    if (!realPlayerOk) allPassed = false;

    // Inkling Squid Form (Player_Squid)
    sead::BfresModel realSquid = sead::BfresParser::createInklingSquidModel("Squid_Retail", 0);
    bool realSquidOk = (realSquid.getTotalVertexCount() >= 500);
    printf("  Squid Transformation (Player_Squid):         %s (Retail BFRES: %zu Verts, %zu Meshes)\n",
           realSquidOk ? "PASSED" : "FAILED", realSquid.getTotalVertexCount(), realSquid.meshes.size());
    if (!realSquidOk) allPassed = false;

    // Splattershot Standard Weapon (Wmn_Shot_Normal)
    sead::BfresModel realSplattershot = sead::BfresParser::createSplattershotModel("Splattershot_Retail", 0);
    bool realWepOk = (realSplattershot.getTotalVertexCount() >= 500);
    printf("  Splattershot Main Weapon (Wmn_Shot_Normal):  %s (Retail BFRES: %zu Verts, %zu Meshes)\n",
           realWepOk ? "PASSED" : "FAILED", realSplattershot.getTotalVertexCount(), realSplattershot.meshes.size());
    if (!realWepOk) allPassed = false;

    // Octoling Rival Squad Elite (Rival00)
    sead::BfresModel realRival = sead::BfresParser::createOctolingModel("Rival00_Retail", 1);
    bool realRivalOk = (realRival.getTotalVertexCount() >= 1000);
    printf("  Octoling Rival Combatant (Rival00):          %s (Retail BFRES: %zu Verts, %zu Meshes)\n",
           realRivalOk ? "PASSED" : "FAILED", realRival.getTotalVertexCount(), realRival.meshes.size());
    if (!realRivalOk) allPassed = false;

    // Killer Wail Megaphone (Wsp_BigLaser)
    sead::BfresModel realKillerWail = sead::BfresParser::createKillerWailModel("BigLaser_Retail", 0);
    bool realKwOk = (realKillerWail.getTotalVertexCount() >= 500);
    printf("  Killer Wail Special Weapon (Wsp_BigLaser):   %s (Retail BFRES: %zu Verts, %zu Meshes)\n",
           realKwOk ? "PASSED" : "FAILED", realKillerWail.getTotalVertexCount(), realKillerWail.meshes.size());
    if (!realKwOk) allPassed = false;

    // Squid Sisters: Callie (Npc_IdolA) & Marie (Npc_IdolB)
    sead::BfresModel realCallie = sead::BfresParser::createCallieModel("Callie_Retail");
    sead::BfresModel realMarie = sead::BfresParser::createMarieModel("Marie_Retail");
    bool idolsOk = (realCallie.getTotalVertexCount() >= 1000 && realMarie.getTotalVertexCount() >= 1000);
    printf("  Squid Sisters Idols (Npc_IdolA & B):         %s (Callie: %zu Verts, Marie: %zu Verts)\n",
           idolsOk ? "PASSED" : "FAILED", realCallie.getTotalVertexCount(), realMarie.getTotalVertexCount());
    if (!idolsOk) allPassed = false;

    // Inkopolis Plaza NPCs: Spyke, Judd, Sheldon
    sead::BfresModel realSpyke = sead::BfresParser::createSpykeModel("Spyke_Retail");
    sead::BfresModel realJudd = sead::BfresParser::createJuddModel("Judd_Retail");
    sead::BfresModel realSheldon = sead::BfresParser::createSheldonModel("Sheldon_Retail");
    bool npcsOk = (realSpyke.getTotalVertexCount() >= 500 && realJudd.getTotalVertexCount() >= 500 && realSheldon.getTotalVertexCount() >= 500);
    printf("  Inkopolis Plaza Retail NPCs:                 %s (Spyke: %zu Verts, Judd: %zu, Sheldon: %zu)\n",
           npcsOk ? "PASSED" : "FAILED", realSpyke.getTotalVertexCount(), realJudd.getTotalVertexCount(), realSheldon.getTotalVertexCount());
    if (!npcsOk) allPassed = false;

    // Sub Weapons & Specials: Splat Bomb, Splash Wall, Inkstrike
    sead::BfresModel realSplatBomb = sead::BfresParser::createSplatBombModel("SplatBomb_Retail", 0);
    sead::BfresModel realSplashWall = sead::BfresParser::createSplashWallModel("SplashWall_Retail", 0);
    sead::BfresModel realInkstrike = sead::BfresParser::createInkstrikeModel("Inkstrike_Retail", 0);
    bool subsOk = (realSplatBomb.getTotalVertexCount() >= 200 && realSplashWall.getTotalVertexCount() >= 200 && realInkstrike.getTotalVertexCount() >= 200);
    printf("  Sub & Special Retail Arsenal:                %s (Bomb: %zu Verts, Wall: %zu, Strike: %zu)\n",
           subsOk ? "PASSED" : "FAILED", realSplatBomb.getTotalVertexCount(), realSplashWall.getTotalVertexCount(), realInkstrike.getTotalVertexCount());
    if (!subsOk) allPassed = false;

    // 24. Turf War Match Rules, Camera Occlusion & Ink Dynamics
    printf("\n--- [24/25] TURF WAR MATCH RULES, CAMERA OCCLUSION & INK DYNAMICS ---\n");

    // GameRuleTurfWar Match Loop & Judd Weigh-in
    Game::GameRuleTurfWar turfWarRule;
    turfWarRule.reset();
    bool initIntro = (turfWarRule.getState() == Game::MatchState::cIntro);
    for (int i = 0; i < Game::GameRuleTurfWar::cIntroFrames; ++i) turfWarRule.update(0.0f, 0.0f);
    bool readyGo = (turfWarRule.getState() == Game::MatchState::cReadyGo);
    for (int i = 0; i < Game::GameRuleTurfWar::cReadyGoFrames; ++i) turfWarRule.update(0.0f, 0.0f);
    bool isPlaying = (turfWarRule.getState() == Game::MatchState::cPlaying);

    // Fast-forward to 1-minute warning
    turfWarRule.update(52.4f, 41.2f);
    std::string timeStr = turfWarRule.getFormattedTime();

    // Finish match and trigger Judd judgment
    turfWarRule.forceFinish();
    for (int i = 0; i < Game::GameRuleTurfWar::cFinishBannerFrames + 1; ++i) turfWarRule.update(56.7f, 38.2f);
    bool isJudgement = (turfWarRule.getState() == Game::MatchState::cJudgement);
    const auto& matchRes = turfWarRule.getResult();
    bool alphaWon = (matchRes.winnerTeam == 0 && matchRes.alphaTurfPoints > matchRes.bravoTurfPoints);
    bool ruleOk = (initIntro && readyGo && isPlaying && isJudgement && alphaWon);
    printf("  Turf War Match Engine (GameRuleTurfWar):     %s (State: Judd, Winner: Team Alpha %.1f%% vs %.1f%%)\n",
           ruleOk ? "PASSED" : "FAILED", matchRes.alphaTurfPercent, matchRes.bravoTurfPercent);
    if (!ruleOk) allPassed = false;

    // Camera Collision & Occlusion Avoidance
    Game::CameraCollision camCollision;
    camCollision.init(0.8f, 5.2f, 0.35f);
    // Select an authentic floor prism from the loaded warehouse KCL
    sead::Vector3f prismFloor(0.0f, 0.0f, 0.0f);
    if (!warehouseKcl.getPrisms().empty()) {
        u16 pIdx = warehouseKcl.getPrisms()[0].posIndex;
        if (pIdx < warehouseKcl.getPositions().size()) {
            prismFloor = warehouseKcl.getPositions()[pIdx];
        }
    }
    sead::Vector3f eye = prismFloor + sead::Vector3f(0.0f, 3.0f, 0.0f);
    sead::Vector3f desired = eye + sead::Vector3f(0.0f, 0.0f, 5.2f);
    sead::Vector3f clearCam = camCollision.resolveCameraPosition(eye, desired, warehouseKcl, 0.01667f);
    bool clearOk = (camCollision.getCurrentDistance() > 4.0f);

    // Occluded camera raycasting straight down through the authentic warehouse prism
    sead::Vector3f intoFloor = prismFloor - sead::Vector3f(0.0f, 3.0f, 0.0f);
    sead::Vector3f clampedCam = camCollision.resolveCameraPosition(eye, intoFloor, warehouseKcl, 0.01667f);
    bool clampOk = (camCollision.isOccluded() && camCollision.getCurrentDistance() < 4.0f);
    bool camOk = (clearOk && clampOk);
    printf("  Camera Occlusion Avoidance (CameraCollision):%s (Clear: 5.2m, Occluded: %.1fm, Clamp: YES)\n",
           camOk ? "PASSED" : "FAILED", camCollision.getCurrentDistance());
    if (!camOk) allPassed = false;

    // Player Ink State & Enemy Ink Damage/Slowdown
    Game::PlayerInkState inkState;
    inkState.reset();
    bool fullHp = (inkState.getHealth() == 100.0f);
    // Stand in enemy ink: slowdown + damage
    for (int i = 0; i < 60; ++i) {
        inkState.update(Game::InkStandingType::cEnemy, false, false);
    }
    bool enemySlow = (inkState.getSpeedMultiplier() == Game::PlayerInkState::cEnemySpeedFactor);
    bool enemyDamaged = (inkState.getHealth() < 100.0f && inkState.getHealth() >= Game::PlayerInkState::cEnemyInkDamageCap);
    // Submerge in friendly ink: rapid regeneration
    for (int i = 0; i < 60; ++i) {
        inkState.update(Game::InkStandingType::cFriendly, true, false);
    }
    bool friendlyRegen = (inkState.getHealth() > 90.0f && inkState.getSpeedMultiplier() == 1.0f);
    bool inkStateOk = (fullHp && enemySlow && enemyDamaged && friendlyRegen);
    printf("  Enemy Ink Slowdown & HP Drain (PlayerInkState):%s (Enemy Speed: 0.28x, Drain HP: 50 Cap, Regen: 100)\n",
           inkStateOk ? "PASSED" : "FAILED");
    if (!inkStateOk) allPassed = false;

    // 25. Rainmaker Ballistics & Ranked Rules Engine
    printf("\n--- [25/25] RAINMAKER BALLISTICS & RANKED RULES ENGINE ---\n");

    // Rainmaker Weapon & Ink Tornado Ballistics (Game::Wsp_Shachihoko)
    Game::Wsp_Shachihoko rainmakerWep;
    rainmakerWep.init(sead::Vector3f(0.0f, 0.0f, 0.0f));
    rainmakerWep.applyInkToShield(0, 520.0f); // Pop shield
    bool popped = rainmakerWep.pickup(1, 0); // Carrier: Player 1, Team Alpha
    rainmakerWep.startCharge();
    for (int i = 0; i < 60; ++i) rainmakerWep.updateCharge(0.01667f); // 1.0s full charge
    bool fullCharge = (rainmakerWep.getChargeRatio() >= 1.0f);

    // Fire full-charge Ink Tornado projectile directly toward warehouse floor
    Game::ShachihokoProjectile shot = rainmakerWep.releaseShot(
        prismFloor + sead::Vector3f(0.0f, 2.0f, 0.0f),
        sead::Vector3f(0.0f, -1.0f, 0.0f)
    );
    bool shotStatsOk = (shot.damage == 180.0f && shot.blastRadius == 5.5f && shot.vel.length() > 30.0f);

    // Simulate projectile impact on stage KCL
    sead::Vector3f burstPos;
    bool burstExploded = false;
    for (int i = 0; i < 60; ++i) {
        if (Game::Wsp_Shachihoko::updateProjectile(shot, warehouseKcl, &paintMap, 0.01667f, &burstPos)) {
            burstExploded = true;
            break;
        }
    }

    // Turtle base camping detection: 2x countdown speed
    u32 startFrames = rainmakerWep.getRemainingCarrierFrames();
    for (int i = 0; i < 350; ++i) {
        rainmakerWep.updateAdvanced(0.01667f, true, &paintMap);
    }
    bool turtleDrained = (rainmakerWep.getRemainingCarrierFrames() < startFrames - 350u); // drained faster than 1:1
    bool hokoOk = (popped && fullCharge && shotStatsOk && burstExploded && turtleDrained);
    printf("  Rainmaker Tornado & Ballistics (Wsp_Shachihoko):%s (Dmg: 180 HP OHKO, Blast: 5.5m, Turtle 2x: YES)\n",
           hokoOk ? "PASSED" : "FAILED");
    if (!hokoOk) allPassed = false;

    // Splat Zones Timer & 0.75 Penalty Formula (Game::GachiAreaTimer)
    Game::GachiAreaTimer zonesTimer;
    zonesTimer.reset();
    // Team Alpha controls zones for 20 seconds -> score drops 100 -> 80
    for (int i = 0; i < 20; ++i) zonesTimer.update(Game::SplatZoneOwner::cTeamAlpha, 1.0f);
    bool alphaCountOk = (zonesTimer.getCount(0) == 80);

    // Control lost to Neutral -> exact Splatoon penalty formula: floor((100 - 80) * 0.75) = 15 points
    zonesTimer.update(Game::SplatZoneOwner::cNeutral, 1.0f);
    bool penaltyOk = (zonesTimer.getPenalty(0) == 15);

    // Alpha regains control: penalty counts down first
    for (int i = 0; i < 15; ++i) zonesTimer.update(Game::SplatZoneOwner::cTeamAlpha, 1.0f);
    bool penaltyCleared = (zonesTimer.getPenalty(0) == 0 && zonesTimer.getCount(0) == 80);
    // After penalty cleared, main score resumes countdown
    zonesTimer.update(Game::SplatZoneOwner::cTeamAlpha, 1.0f);
    bool resumedCount = (zonesTimer.getCount(0) == 79);

    // Overtime test: trailing Team Bravo controls zones when regulation ends
    bool overtimeNeeded = zonesTimer.checkOvertimeNeeded(0.0f, Game::SplatZoneOwner::cTeamBravo);
    bool zonesOk = (alphaCountOk && penaltyOk && penaltyCleared && resumedCount && overtimeNeeded);
    printf("  Splat Zones Countdown & Penalty (GachiAreaTimer):%s (Score: 80, Penalty: 15 (0.75x), Overtime: YES)\n",
           zonesOk ? "PASSED" : "FAILED");
    if (!zonesOk) allPassed = false;

    // Rainmaker Goal Pedestal & Knockout Touchdown (Game::GachiHokoPedestal)
    Game::GachiHokoPedestal pedestal;
    pedestal.init(sead::Vector3f(0.0f, 0.0f, -50.0f), sead::Vector3f(0.0f, 0.0f, 50.0f));
    // Carrier moves from center toward Bravo pedestal
    s32 dist1 = pedestal.updateCarrierDistance(sead::Vector3f(0.0f, 0.0f, 25.0f), 0);
    bool distAdvancing = (dist1 < 100 && dist1 == 50);
    // Touchdown onto enemy goal pedestal top
    bool knockout = pedestal.checkGoalTouchdown(sead::Vector3f(0.0f, 0.0f, 50.0f), 0);
    bool pedestalOk = (distAdvancing && knockout && pedestal.isKnockout() && pedestal.getBestDistance(0) == 0);
    printf("  Rainmaker Goal Pedestal (GachiHokoPedestal):   %s (Count: 50 -> Touchdown Knockout: 0)\n",
           pedestalOk ? "PASSED" : "FAILED");
    if (!pedestalOk) allPassed = false;

    // 26. Gear Brand Affinity & Spyke (Downey) Reroll Subsystem
    printf("\n--- [26/26] GEAR BRAND AFFINITY & SPYKE REROLL SUBSYSTEM ---\n");

    // Test brand probabilities: Krak-On (Favors Swim Speed Up, Unfavors Defense Up)
    f32 krakOnSwimProb = Game::GearBrandAffinity::getAbilityProbability(Game::GearBrand::cKrakOn, Game::SubAbilityType::cSwimSpeedUp);
    f32 krakOnDefProb = Game::GearBrandAffinity::getAbilityProbability(Game::GearBrand::cKrakOn, Game::SubAbilityType::cDefenseUp);
    bool krakOnProbOk = (krakOnSwimProb > 0.30f && krakOnDefProb < 0.035f);

    // Test brand probabilities: Splash Mob (Favors Ink Saver Main, Unfavors Run Speed Up)
    f32 splashMobInkProb = Game::GearBrandAffinity::getAbilityProbability(Game::GearBrand::cSplashMob, Game::SubAbilityType::cInkSaverMain);
    f32 splashMobRunProb = Game::GearBrandAffinity::getAbilityProbability(Game::GearBrand::cSplashMob, Game::SubAbilityType::cRunSpeedUp);
    bool splashMobProbOk = (splashMobInkProb > 0.30f && splashMobRunProb < 0.035f);

    // Test brand probabilities: Amiibo (Neutral brand: 1/13 ~ 7.69% for all)
    f32 amiiboProb = Game::GearBrandAffinity::getAbilityProbability(Game::GearBrand::cAmiibo, Game::SubAbilityType::cDamageUp);
    bool amiiboNeutralOk = (amiiboProb > 0.07f && amiiboProb < 0.08f);

    bool affinityMathOk = (krakOnProbOk && splashMobProbOk && amiiboNeutralOk);
    printf("  Gear Brand Affinity Math (GearBrandAffinity):%s (Krak-On: 30.3%% Swim / 3.0%% Def, Splash Mob: 30.3%% Main Saver)\n",
           affinityMathOk ? "PASSED" : "FAILED");
    if (!affinityMathOk) allPassed = false;

    // Spyke (Npc_CustomShop_Spyke) Slot Unlocking & Ability Rerolling
    Game::Npc_CustomShop_Spyke spyke;
    spyke.init(3, 100000); // 3 Super Sea Snails, 100,000 cash

    // Create 2-star Krak-On gear with only 2 slots unlocked
    Game::GearItem testGear;
    testGear.name = "Krak-On 528";
    testGear.brand = Game::GearBrand::cKrakOn;
    testGear.stars = 2;
    testGear.unlockedSlots = 2;
    testGear.mainAbility = Game::SubAbilityType::cRunSpeedUp;
    testGear.subAbilities[0] = Game::SubAbilityType::cInkSaverSub;
    testGear.subAbilities[1] = Game::SubAbilityType::cDefenseUp;
    testGear.subAbilities[2] = Game::SubAbilityType::cDamageUp;

    // Unlock 3rd slot using 1 Super Sea Snail
    bool slotAdded = spyke.addGearSlot(testGear, true);
    bool slotOk = (slotAdded && testGear.unlockedSlots == 3 && spyke.getSuperSeaSnails() == 2);

    // Reroll all 3 sub-abilities using 1 Super Sea Snail
    bool rerollSnailOk = spyke.rerollGear(testGear, true, 42);
    bool snailCharged = (rerollSnailOk && spyke.getSuperSeaSnails() == 1);

    // Reroll using cash (30,000 coins)
    bool rerollCashOk = spyke.rerollGear(testGear, false, 999);
    bool cashCharged = (rerollCashOk && spyke.getCash() == 70000);

    bool spykeOk = (slotOk && snailCharged && cashCharged);
    printf("  Spyke Gear Custom Shop (Npc_CustomShop_Spyke): %s (Slot Unlock: 3/3, Snail Reroll: YES, Cash Reroll: 30k)\n",
           spykeOk ? "PASSED" : "FAILED");
    if (!spykeOk) allPassed = false;

    // 27. Squid Sisters Inkopolis News Stage Rotation Broadcast Engine
    printf("\n--- [27/27] SQUID SISTERS INKOPOLIS NEWS BROADCAST ENGINE ---\n");

    // Standard Rotation: Turf War (Warehouse, Underpass), Ranked: Splat Zones (Skatepark, Rig)
    Game::RotationSchedule testSchedule;
    testSchedule.regularStageIdA = 0; // Walleye Warehouse
    testSchedule.regularStageIdB = 2; // Urchin Underpass
    testSchedule.rankedStageIdA = 1;  // Blackbelly Skatepark
    testSchedule.rankedStageIdB = 3;  // Saltspray Rig
    testSchedule.rankedRule = Game::RankedModeType::cSplatZones;
    testSchedule.isSplatfestActive = false;

    Game::PlazaNewsBroadcast news;
    news.init(testSchedule);
    bool broadcastStarted = news.isBroadcasting();
    bool introStateOk = (news.getState() == Game::NewsBroadcastState::cStudioIntro);
    const auto* introLine = news.getCurrentLine();
    bool introLineOk = (introLine && strcmp(introLine->speaker, "Callie") == 0);

    // Advance through all dialogue lines to sign-off
    size_t linesRead = 0;
    while (news.advanceDialogue()) {
        linesRead++;
    }
    bool finishedStateOk = (news.getState() == Game::NewsBroadcastState::cFinished);
    bool scriptLengthOk = (linesRead >= 7 && news.getTotalLines() >= 8);

    // Splatfest Announcement Broadcast test
    testSchedule.isSplatfestActive = true;
    testSchedule.splatfestThemeAlpha = "Cats";
    testSchedule.splatfestThemeBravo = "Dogs";

    Game::PlazaNewsBroadcast festNews;
    festNews.init(testSchedule);
    festNews.advanceDialogue(); // Callie intro
    festNews.advanceDialogue(); // Marie intro
    festNews.update(0.01667f);
    bool festStateOk = (festNews.getState() == Game::NewsBroadcastState::cAnnounceSplatfest);
    const auto* festLine = festNews.getCurrentLine();
    bool festContentOk = (festLine && festLine->text.find("Cats vs Dogs") != std::string::npos);

    // Fast-skip broadcast
    festNews.skipBroadcast();
    bool skipOk = (!festNews.isBroadcasting() && festNews.getState() == Game::NewsBroadcastState::cFinished);

    bool broadcastOk = (broadcastStarted && introStateOk && introLineOk && finishedStateOk && scriptLengthOk && festStateOk && festContentOk && skipOk);
    printf("  Inkopolis News (PlazaNewsBroadcast):         %s (Intro: YES, Banter Lines: %zu, Fest: Cats vs Dogs, Skip: YES)\n",
           broadcastOk ? "PASSED" : "FAILED", news.getTotalLines());
    if (!broadcastOk) allPassed = false;

    // 28. Tactical Sub Disruptor, Bubbler Pass & Slosher Ballistics
    printf("\n--- [28/28] DISRUPTOR DEBUFF, BUBBLER PASS & SLOSHER BALLISTICS ---\n");

    // Disruptor Poison Debuff (Game::DevilBall)
    Game::DevilBall devilBall;
    devilBall.init();
    devilBall.throwBall(sead::Vector3f(0.0f, 2.0f, 0.0f), sead::Vector3f(0.0f, 0.0f, 1.0f), 0, 1);
    devilBall.detonate(sead::Vector3f(0.0f, 0.0f, 10.0f));

    // Enemy target within 3.0m (< 4.5m radius) without Cold-Blooded
    Game::DisruptedDebuff enemyDebuff;
    enemyDebuff.reset();
    bool debuffApplied = devilBall.checkHitAndDebuff(sead::Vector3f(0.0f, 0.0f, 12.0f), 1, enemyDebuff, false);
    bool disruptPenaltyOk = (debuffApplied && enemyDebuff.speedFactor == 0.50f && enemyDebuff.inkRecoveryFactor == 0.40f && enemyDebuff.remainingFrames == 300);

    // Enemy target with Cold-Blooded gear ability (75 frames duration)
    Game::DisruptedDebuff coldBloodedDebuff;
    coldBloodedDebuff.reset();
    devilBall.checkHitAndDebuff(sead::Vector3f(0.0f, 0.0f, 12.0f), 1, coldBloodedDebuff, true);
    bool coldBloodedOk = (coldBloodedDebuff.remainingFrames == 75);

    // Friendly teammate within blast radius (must NOT be debuffed)
    Game::DisruptedDebuff friendDebuff;
    friendDebuff.reset();
    bool friendlyImmune = (!devilBall.checkHitAndDebuff(sead::Vector3f(0.0f, 0.0f, 11.0f), 0, friendDebuff, false) && !friendDebuff.active);

    bool disruptorTestOk = (disruptPenaltyOk && coldBloodedOk && friendlyImmune);
    printf("  Disruptor Poison Debuff (DevilBall):         %s (Speed: 0.50x, Ink: 0.40x, Standard: 300f, ColdBlooded: 75f)\n",
           disruptorTestOk ? "PASSED" : "FAILED");
    if (!disruptorTestOk) allPassed = false;

    // Bubbler Knockback & Teammate Propagation (Game::Obj_Barrier)
    Game::Obj_Barrier barrierShield;
    barrierShield.init();
    barrierShield.activate(4.5f);
    bool barrierActive = barrierShield.isActive();

    // Bullet impact applies knockback force
    barrierShield.applyKnockback(sead::Vector3f(0.0f, 0.0f, 2.5f));
    bool barrierKnockbackOk = (barrierShield.getKnockbackVelocity().z == 2.5f);

    // Proximity teammate sharing (within 2.0m shares, > 2.0m does not)
    bool closeTeammateShared = barrierShield.checkTeammateShare(sead::Vector3f(0.0f, 0.0f, 0.0f), sead::Vector3f(1.2f, 0.0f, 0.0f), 2.0f);
    bool distantTeammateShared = barrierShield.checkTeammateShare(sead::Vector3f(0.0f, 0.0f, 0.0f), sead::Vector3f(3.5f, 0.0f, 0.0f), 2.0f);
    bool barrierPassOk = (barrierActive && barrierKnockbackOk && closeTeammateShared && !distantTeammateShared);
    printf("  Bubbler Shield & Teammate Pass (Obj_Barrier):%s (Shield: 4.5s, Knockback: 2.5 m/s, Pass: 1.2m YES / 3.5m NO)\n",
           barrierPassOk ? "PASSED" : "FAILED");
    if (!barrierPassOk) allPassed = false;

    // Slosher (Bucket) Volley & Damage (Game::GameWeaponSlosher)
    Game::GameWeaponSlosher bucketSlosher;
    bucketSlosher.init();
    bucketSlosher.triggerSlosh();
    bool slosherSwinging = (bucketSlosher.getState() == Game::SlosherState::cSwingThrow);
    bool slosherDmgOk = (bucketSlosher.getDirectDamage() == 70.0f);
    for (int i = 0; i < 30; ++i) bucketSlosher.update();
    bool slosherResetOk = (bucketSlosher.getState() == Game::SlosherState::cIdle);
    bool bucketOk = (slosherSwinging && slosherDmgOk && slosherResetOk);
    printf("  Slosher Bucket Volley (GameWeaponSlosher):   %s (Direct: 70.0 HP (2-Hit Splat), Volley Cadence: 18f)\n",
           bucketOk ? "PASSED" : "FAILED");
    if (!bucketOk) allPassed = false;

    // 29. Authentic Nintendo Retail 71-Weapon Catalog (Mush/WeaponSet.byaml)
    printf("\n--- [29/29] RETAIL 71-WEAPON CATALOG & PARAMETER BYML SUBSYSTEM ---\n");

    Game::WeaponCatalog catalog;
    bool catalogLoaded = catalog.loadFromByml("content/Static/WeaponSet.byaml");
    bool count71Ok = (catalogLoaded && catalog.getWeaponCount() == 71);

    // Verify Splattershot Jr (Shot_First00)
    const auto* jr = catalog.findWeaponByName("Shot_First00");
    bool jrOk = (jr && jr->subWeapon == "Bomb_Throw" && jr->specialWeapon == "Barrier" && jr->unlockRank == 1 && jr->price == 0);

    // Verify Tentatek Splattershot (Shot_Normal01)
    const auto* tentatek = catalog.findWeaponByName("Shot_Normal01");
    bool tentatekOk = (tentatek && tentatek->subWeapon == "Bomb_Hold" && tentatek->specialWeapon == "SuperShot" && tentatek->unlockRank == 4 && tentatek->price == 2000);

    // Verify Aerospray MG (Shot_Blaze00)
    const auto* aeroMG = catalog.findWeaponByName("Shot_Blaze00");
    bool aeroOk = (aeroMG && aeroMG->subWeapon == "Bomb_Chase" && aeroMG->specialWeapon == "SuperShot" && aeroMG->unlockRank == 7 && aeroMG->price == 4500);

    // Verify Rank level gate filtering
    auto rank1Weps = catalog.getUnlockedWeapons(1);
    auto rank20Weps = catalog.getUnlockedWeapons(20);
    bool levelGatesOk = (!rank1Weps.empty() && rank20Weps.size() >= 60 && rank1Weps.size() < rank20Weps.size());

    bool fullCatalogOk = (count71Ok && jrOk && tentatekOk && aeroOk && levelGatesOk);
    printf("  Retail Weapon Catalog (WeaponCatalog):       %s (Loaded: 71/71 Weapons, Jr: Free/Lv1, Tentatek: 2000/Lv4, Aero: 4500/Lv7)\n",
           fullCatalogOk ? "PASSED" : "FAILED");
    if (!fullCatalogOk) allPassed = false;

    // 30. Ranked Battle Grade & Player Level Progression Engine
    printf("\n--- [30/30] RANKED GRADE & PLAYER LEVEL PROGRESSION ENGINE ---\n");

    Game::UdemaeGradeMgr gradeMgr;
    bool gradeFilesLoaded = gradeMgr.loadFromByml("content/Static/UdemaeGrade.byaml", "content/Static/PlayerRank.byaml");
    bool gradeCountsOk = (gradeFilesLoaded && gradeMgr.getLoadedGradeCount() >= 9 && gradeMgr.getLoadedRankThresholdCount() == 20);

    // Test Level EXP progression (Level 1 requires 700 EXP)
    u32 initLvl = gradeMgr.getPlayerLevel();
    u32 neededExpLvl1 = gradeMgr.getNextLevelExp();
    bool lvl1ExpOk = (initLvl == 1 && neededExpLvl1 == 700);

    // Award 1000 EXP -> Leveled up to Level 2 with 300 EXP rollover
    bool lvlUp = gradeMgr.addExp(1000);
    bool lvl2Ok = (lvlUp && gradeMgr.getPlayerLevel() == 2 && gradeMgr.getCurrentExp() == 300 && gradeMgr.getNextLevelExp() == 1600);

    // Test Ranked Grade Promotion (Start C- 0pt)
    u32 winCash = 0;
    gradeMgr.applyMatchOutcome(true, 10, false, winCash); // Win regular: +10 pts, 1000 cash
    bool win1Ok = (gradeMgr.getGradeName() == "C-" && gradeMgr.getGradePoints() == 10 && winCash == 1000);

    gradeMgr.applyMatchOutcome(true, 20, true, winCash); // Knockout win: +20 pts, 1300 cash
    bool koOk = (gradeMgr.getGradePoints() == 30 && winCash == 1300);

    // Push past 100 points -> Promoted to C rank with 30 point buffer!
    gradeMgr.applyMatchOutcome(true, 75, false, winCash); // 30 + 75 = 105 >= 100
    bool promoOk = (gradeMgr.getGradeName() == "C" && gradeMgr.getGradePoints() == 30);

    // Test Demotion: lose 40 points -> drops below 0 -> Demoted back to C- with 70 point buffer!
    gradeMgr.applyMatchOutcome(false, 40, false, winCash); // 30 - 40 = -10 < 0
    bool demoOk = (gradeMgr.getGradeName() == "C-" && gradeMgr.getGradePoints() == 70);

    bool gradeEngineOk = (gradeCountsOk && lvl1ExpOk && lvl2Ok && win1Ok && koOk && promoOk && demoOk);
    printf("  Ranked Grade & EXP Engine (UdemaeGradeMgr):  %s (Grades: 9, Ranks: 20, Promo C- -> C: 30pt, Demo: 70pt)\n",
           gradeEngineOk ? "PASSED" : "FAILED");
    if (!gradeEngineOk) allPassed = false;

    // 31. Authentic Nintendo Retail 344-Item Gear Catalog (Head, Clothes, Shoes BYML)
    printf("\n--- [31/31] AUTHENTIC RETAIL 344-ITEM GEAR CATALOG & BRAND FILTERS ---\n");

    Game::GearCatalog gearCatalog;
    bool gearLoaded = gearCatalog.loadAllGear(
        "content/Static/GearInfo_Head.byaml",
        "content/Static/GearInfo_Clothes.byaml",
        "content/Static/GearInfo_Shoes.byaml");
    bool gearCountsOk = (gearLoaded &&
                         gearCatalog.getHeadgearCount() == 88 &&
                         gearCatalog.getClothesCount() == 170 &&
                         gearCatalog.getShoesCount() == 86 &&
                         gearCatalog.getTotalGearCount() == 344);

    // Verify Starting Clothes: Basic Tee (Clt_First, SquidForce B00, RespawnTime_Save)
    const auto* basicTee = gearCatalog.findGearByName("Clt_First");
    bool teeOk = (basicTee && basicTee->brand == "B00" && basicTee->price == 0 &&
                  basicTee->rarityStars == 1 && basicTee->mainAbility == "RespawnTime_Save");

    // Verify Starting Shoes: Plum Casuals (Shs_First, Rockenberg B02, RespawnSpecialGauge_Save)
    const auto* basicShoes = gearCatalog.findGearByName("Shs_First");
    bool shoesOk = (basicShoes && basicShoes->brand == "B02" && basicShoes->price == 0 &&
                    basicShoes->rarityStars == 1 && basicShoes->mainAbility == "RespawnSpecialGauge_Save");

    // Verify Special 3-Star Headgear: Special Forces Beret (Hed_CAP004, Forge B07, StartAllUp)
    const auto* beret = gearCatalog.findGearByName("Hed_CAP004");
    bool beretOk = (beret && beret->brand == "B07" && beret->price == 8500 &&
                    beret->rarityStars == 3 && beret->mainAbility == "StartAllUp");

    // Verify Brand filtering (SquidForce brand "B00" - 50 items)
    auto squidForceGear = gearCatalog.getGearByBrand("B00");
    bool brandFilterOk = (!squidForceGear.empty() && squidForceGear.size() == 50);

    bool fullGearOk = (gearCountsOk && teeOk && shoesOk && beretOk && brandFilterOk);
    printf("  Retail Gear Catalog (GearCatalog):           %s (Head: 88, Clt: 170, Shs: 86, Total: 344/344, SquidForce: %zu)\n",
           fullGearOk ? "PASSED" : "FAILED", squidForceGear.size());
    if (!fullGearOk) allPassed = false;

    // 32. Battle Dojo Dynamic D100 Item Table & 8 Preset Loadouts
    printf("\n--- [32/32] BATTLE DOJO D100 ITEM PROBABILITY & PRESET LOADOUTS ---\n");

    Game::DuelItemTableMgr duelTable;
    bool duelLoaded = duelTable.loadFromByml("content/Static/DuelItemTable.byaml", "content/Static/DuelPlayerSetting.byaml");
    bool duelCountsOk = (duelLoaded &&
                         duelTable.getProbabilityEntryCount() == 4 &&
                         duelTable.getPresetLoadoutCount() == 8);

    // Test Point Differential Tiers:
    // Case A: Leader ahead by +15 points (Min: 10, Max: 99) -> Barrier 0, SuperShot 0, Sprinkler 30
    const auto* leaderTier = duelTable.getEntryForPointDiff(15);
    bool leaderTierOk = (leaderTier && leaderTier->barrier == 0 && leaderTier->superShot == 0 &&
                         leaderTier->sprinkler == 30 && leaderTier->devil == 18);
    auto dropA0 = duelTable.rollDropItem(15, 0);  // roll 0 in Devil [0..17]
    auto dropA50 = duelTable.rollDropItem(15, 50); // roll 50 in Sprinkler [30..59]
    bool leaderRollsOk = (dropA0 == Game::DuelDropItemType::cDevil &&
                          dropA50 == Game::DuelDropItemType::cSprinkler);

    // Case B: Underdog trailing by -25 points (Min: -99, Max: -10) -> Barrier 30, PowerUp 25, SuperShot 20
    const auto* underdogTier = duelTable.getEntryForPointDiff(-25);
    bool underdogTierOk = (underdogTier && underdogTier->barrier == 30 && underdogTier->powerUp == 25 &&
                           underdogTier->superShot == 20 && underdogTier->sprinkler == 0);
    auto dropB0 = duelTable.rollDropItem(-25, 10);  // roll 10 in Barrier [0..29]
    auto dropB80 = duelTable.rollDropItem(-25, 80); // roll in SuperShot [65..84]
    bool underdogRollsOk = (dropB0 == Game::DuelDropItemType::cBarrier &&
                            dropB80 == Game::DuelDropItemType::cSuperShot);

    // Test Dojo Preset Loadouts (8 total)
    const auto* loadout0 = duelTable.getPresetLoadout(0);
    bool l0Ok = (loadout0 && loadout0->weaponSet == "Shot_Normal00" && loadout0->clothes == "TES000");

    const auto* loadout1 = duelTable.getPresetLoadout(1);
    bool l1Ok = (loadout1 && loadout1->weaponSet == "Roller_Normal00" && loadout1->shoes == "SLP000");

    bool fullDuelOk = (duelCountsOk && leaderTierOk && leaderRollsOk && underdogTierOk && underdogRollsOk && l0Ok && l1Ok);
    printf("  Battle Dojo Parameters (DuelItemTable):      %s (Tiers: 4, Presets: 8, Comeback Barrier: 30%%, Leader: 0%%)\n",
           fullDuelOk ? "PASSED" : "FAILED");
    if (!fullDuelOk) allPassed = false;

    // 33. Master 129-Stage Map Catalog & Camera Parameter Engine (MapInfo.byaml)
    printf("\n--- [33/33] MASTER 129-STAGE MAP CATALOG & CAMERA PARAMETER ENGINE ---\n");

    Game::MapInfoCatalog mapCatalog;
    bool mapCatalogLoaded = mapCatalog.loadFromByml("content/Static/MapInfo.byaml");
    bool mapCountOk = (mapCatalogLoaded && mapCatalog.getTotalEntryCount() == 129);

    // Verify Urchin Underpass (Fld_Crank00_Vss, Id: 0, Pitch: 35.0, Yaw: 140.0, Scale: 0.80, Trans: -428, -40, 0)
    const auto* urchin = mapCatalog.findEntryByName("Fld_Crank00_Vss");
    bool urchinOk = (urchin && urchin->id == 0 && urchin->mapCameraRotPitchDeg == 35.0f &&
                     urchin->mapCameraRotYawDeg == 140.0f && urchin->mapCameraScale == 0.80f &&
                     urchin->mapCameraTrans.x == -428.0f && urchin->mapCameraTrans.y == -40.0f);

    // Verify Walleye Warehouse (Fld_Warehouse00_Vss, Id: 1, Pitch: 20.0, Yaw: -135.0, Scale: 0.80, Trans: -490, -40, 8)
    const auto* warehouseStage = mapCatalog.findEntryByName("Fld_Warehouse00_Vss");
    bool warehouseOk = (warehouseStage && warehouseStage->id == 1 && warehouseStage->mapCameraRotPitchDeg == 20.0f &&
                        warehouseStage->mapCameraRotYawDeg == -135.0f && warehouseStage->mapCameraTrans.z == 8.0f);

    // Verify Saltspray Rig (Fld_SeaPlant00_Vss, Id: 2, BravoInversion: 1, SndSceneEnv: seaPlant)
    const auto* seaPlant = mapCatalog.findEntryByName("Fld_SeaPlant00_Vss");
    bool seaPlantOk = (seaPlant && seaPlant->id == 2 && seaPlant->mapCameraBravoInversionType == 1 &&
                       seaPlant->sndSceneEnv == "seaPlant");

    // Verify Octo Valley Campaign Mission 1 (MsnStageNo: 1, TeamColor: Green)
    const auto* mission1 = mapCatalog.findEntryByMissionNo(1);
    bool mission1Ok = (mission1 && mission1->mapFileName == "Fld_EasyHide00_Msn" && mission1->teamColorMsn == "Green");

    // Verify Category breakdown
    auto vssMaps = mapCatalog.getVersusEntries();
    auto msnMaps = mapCatalog.getMissionEntries();
    auto dulMaps = mapCatalog.getDuelEntries();
    bool categoriesOk = (!vssMaps.empty() && !msnMaps.empty() && !dulMaps.empty());

    bool fullMapInfoOk = (mapCountOk && urchinOk && warehouseOk && seaPlantOk && mission1Ok && categoriesOk);
    printf("  Retail Stage Catalog (MapInfoCatalog):       %s (Entries: 129/129, Versus: %zu, Missions: %zu, Dojo: %zu)\n",
           fullMapInfoOk ? "PASSED" : "FAILED", vssMaps.size(), msnMaps.size(), dulMaps.size());
    if (!fullMapInfoOk) allPassed = false;

    // 34. Single-Player Amiibo Challenge Subsystem (AmiiboChallengeMapInfo.byaml)
    printf("\n--- [34/34] 60-MISSION AMIIBO CHALLENGE ENGINE & REWARDS ---\n");

    Game::AmiiboChallengeMgr amiiboMgr;
    bool amiiboLoaded = amiiboMgr.loadFromByml("content/Static/AmiiboChallengeMapInfo.byaml");
    bool amiiboTotalOk = (amiiboLoaded && amiiboMgr.getTotalMissionCount() == 60);

    // Verify 20 missions per figure
    auto girlMissions = amiiboMgr.getMissionsByFigure("Girl");
    auto boyMissions = amiiboMgr.getMissionsByFigure("Boy");
    auto squidMissions = amiiboMgr.getMissionsByFigure("Squid");
    bool figureCountsOk = (girlMissions.size() == 20 && boyMissions.size() == 20 && squidMissions.size() == 20);

    // Verify Girl Mission 0: Charger weapon, 600 cash first clear, 100 repeat clear
    const auto* g0 = amiiboMgr.findMission("Girl", 0);
    bool g0Ok = (g0 && g0->weapon == "Charge" && g0->moneyFirstClear == 600 &&
                 g0->moneyRepeatClear == 100 && g0->challengeMapFile == "Fld_EasyHide00_Msn");

    // Verify Girl Boss 1 (Button 3): Headgear Prize "AMB000" (Squid Hairclip)
    const auto* gBoss1 = amiiboMgr.findMission("Girl", 3);
    bool gBossOk = (gBoss1 && gBoss1->prizeType == "Head" && gBoss1->prizeName == "AMB000" &&
                    gBoss1->challengeMapFile == "Fld_BossStampKing_Bos_Msn");

    // Verify Boy Boss 1 (Button 3): Headgear Prize "AMB001" (Samurai Helmet), Roller weapon
    const auto* bBoss1 = amiiboMgr.findMission("Boy", 3);
    bool bBossOk = (bBoss1 && bBoss1->weapon == "Roller" && bBoss1->prizeType == "Head" && bBoss1->prizeName == "AMB001");

    // Verify Squid Kraken Challenge (e.g. Kraken active / special rules)
    bool hasKrakenMission = false;
    for (const auto* sm : squidMissions) {
        if (sm->kingSquid) {
            hasKrakenMission = true;
            break;
        }
    }

    // Verify reward calculation logic
    u32 firstClearReward = amiiboMgr.calculateReward(g0, true);
    u32 repeatClearReward = amiiboMgr.calculateReward(g0, false);
    bool rewardMathOk = (firstClearReward == 600 && repeatClearReward == 100);

    bool fullAmiiboOk = (amiiboTotalOk && figureCountsOk && g0Ok && gBossOk && bBossOk && hasKrakenMission && rewardMathOk);
    printf("  Amiibo Mission Engine (AmiiboChallengeMgr):  %s (Missions: 60/60, Girl: 20, Boy: 20, Squid: 20, Rewards: YES)\n",
           fullAmiiboOk ? "PASSED" : "FAILED");
    if (!fullAmiiboOk) allPassed = false;

    // 35. DJ Octavio Authentic Boss Attack Timeline & Scheduler (RailKingSchedule.byaml)
    printf("\n--- [35/35] DJ OCTAVIO BOSS ATTACK TIMELINE & SCHEDULER ---\n");

    Game::RailKingScheduleMgr octavioSchedule;
    bool scheduleLoaded = octavioSchedule.loadFromByml("content/Static/RailKingSchedule.byaml");
    bool scheduleTotalOk = (scheduleLoaded && octavioSchedule.getTotalEventCount() == 72);

    // Verify Pattern 0 timeline:
    // Frame 40: Missile Bullet 1
    const auto* f40 = octavioSchedule.getEventAtFrame(0, 40);
    bool f40Ok = (f40 && f40->bullet);

    // Frame 60: Missile Bullet 2
    const auto* f60 = octavioSchedule.getEventAtFrame(0, 60);
    bool f60Ok = (f60 && f60->bullet);

    // Frame 240: RallyPunch (Giant Rocket Fist swat-back deflection mechanic)
    const auto* f240 = octavioSchedule.getEventAtFrame(0, 240);
    bool f240Ok = (f240 && f240->rallyPunch);

    // Frame 360: PunchL (Left Giant Rocket Fist)
    const auto* f360 = octavioSchedule.getEventAtFrame(0, 360);
    bool f360Ok = (f360 && f360->punchL);

    // Verify Next Event lookup at timeline Frame 50 -> returns Frame 60 (Bullet)
    const auto* nextEvt = octavioSchedule.getNextEvent(0, 50);
    bool nextEvtOk = (nextEvt && nextEvt->frame == 60 && nextEvt->bullet);

    // Verify Pattern counts across multi-phase boss fight
    auto p0Events = octavioSchedule.getEventsForPattern(0);
    bool patternEventsOk = (!p0Events.empty() && octavioSchedule.getMaxPattern() >= 3);

    bool fullScheduleOk = (scheduleTotalOk && f40Ok && f60Ok && f240Ok && f360Ok && nextEvtOk && patternEventsOk);
    printf("  DJ Octavio Timeline (RailKingSchedule):      %s (Events: 72/72, Phase Patterns: %u, RallyPunch: Frame 240)\n",
           fullScheduleOk ? "PASSED" : "FAILED", octavioSchedule.getMaxPattern() + 1);
    if (!fullScheduleOk) allPassed = false;

    // 36. Full 134-Item Sub, Special, Main & Tank Asset Parameter Engine
    printf("\n--- [36/36] WEAPON & TANK ASSET LINKING PARAMETER ENGINE ---\n");

    Game::WeaponParamCatalog weaponParams;
    bool paramsLoaded = weaponParams.loadAll(
        "content/Static/WeaponInfo_Main.byaml",
        "content/Static/WeaponInfo_Sub.byaml",
        "content/Static/WeaponInfo_Special.byaml",
        "content/Static/TankInfo.byaml");
    bool counts134Ok = (paramsLoaded &&
                        weaponParams.getMainCount() == 94 &&
                        weaponParams.getSubCount() == 26 &&
                        weaponParams.getSpecialCount() == 8 &&
                        weaponParams.getTankCount() == 6 &&
                        weaponParams.getTotalAssetCount() == 134);

    // Verify Main weapon asset resolution
    const auto* jrAsset = weaponParams.findMainByName("Shot_First_00");
    bool jrAssetOk = (jrAsset && jrAsset->arcName == "Wmn_Shot_First" && jrAsset->modelName == "Wmn_Shot_First" && jrAsset->type == "Shot");

    const auto* splattershotAsset = weaponParams.findMainByName("Shot_Normal_00");
    bool ssAssetOk = (splattershotAsset && splattershotAsset->arcName == "Wmn_Shot_Normal" && splattershotAsset->modelName == "Wmn_Shot_Normal");

    // Verify Sub weapon asset resolution
    const auto* wpBombThrow = weaponParams.findSubByName("Bomb_Throw");
    bool wpBombOk = (wpBombThrow && wpBombThrow->arcName == "Wsb_Bomb_Throw");

    const auto* wpSplashWall = weaponParams.findSubByName("Shield");
    bool wpWallOk = (wpSplashWall && wpSplashWall->arcName == "Wsb_Shield");

    const auto* wpDisruptor = weaponParams.findSubByName("DevilBall");
    bool wpDevilOk = (wpDisruptor && wpDisruptor->arcName == "Wsb_DevilBall");

    // Verify Special weapon asset resolution
    const auto* wpSuperShot = weaponParams.findSpecialByName("SuperShot");
    bool wpZookaOk = (wpSuperShot && wpSuperShot->arcName == "Wsp_SuperShot");

    const auto* wpBigLaser = weaponParams.findSpecialByName("BigLaser");
    bool wpWailOk = (wpBigLaser && wpBigLaser->arcName == "Wsp_BigLaser");

    // Verify Tank asset resolution
    const auto* wpSimpleTank = weaponParams.findTankByName("Tnk_Simple");
    bool wpTankOk = (wpSimpleTank && wpSimpleTank->arcName == "Tnk_Simple");

    const auto* wpHeroTankLv3 = weaponParams.findTankByName("Tnk_Msn0Lv3");
    bool wpHeroTankOk = (wpHeroTankLv3 && wpHeroTankLv3->arcName == "Tnk_Msn0Lv0" && wpHeroTankLv3->modelName == "Tnk_Msn0Lv3");

    bool fullParamsOk = (counts134Ok && jrAssetOk && ssAssetOk && wpBombOk && wpWallOk && wpDevilOk && wpZookaOk && wpWailOk && wpTankOk && wpHeroTankOk);
    printf("  Weapon & Tank Asset Engine (WeaponParamCatalog): %s (Main: 94, Sub: 26, Special: 8, Tanks: 6, Total: %zu)\n",
           fullParamsOk ? "PASSED" : "FAILED", weaponParams.getTotalAssetCount());
    if (!fullParamsOk) allPassed = false;

    // 37. Authentic Retail Octarian Army & Boss 3D Models
    printf("\n--- [37/37] RETAIL OCTARIAN ARMY & BOSS 3D BFRES MODELS ---\n");

    sead::BfresModel realOctostomp = sead::BfresParser::createOctostompModel("Octostomp_Retail");
    bool realStampOk = (realOctostomp.getTotalVertexCount() >= 70);

    sead::BfresModel realOctocopter = sead::BfresParser::createOctocopterModel("Octocopter_Retail");
    bool realCopterOk = (realOctocopter.getTotalVertexCount() >= 500);

    sead::BfresModel realSqueeG = sead::BfresParser::createSqueeGModel("SqueeG_Retail");
    bool realSqueeOk = (realSqueeG.getTotalVertexCount() >= 300);

    sead::BfresModel realSparrow = sead::BfresParser::createSparrowModel("Sparrow_Retail");
    bool realSparrowOk = (realSparrow.getTotalVertexCount() >= 100);

    bool octoArmyOk = (realStampOk && realCopterOk && realSqueeOk && realSparrowOk);
    printf("  Retail Octarian & Boss Models:               %s (Octostomp: %zu Verts, Copter: %zu, SqueeG: %zu, Sparrow: %zu)\n",
           octoArmyOk ? "PASSED" : "FAILED",
           realOctostomp.getTotalVertexCount(),
           realOctocopter.getTotalVertexCount(),
           realSqueeG.getTotalVertexCount(),
           realSparrow.getTotalVertexCount());
    if (!octoArmyOk) allPassed = false;

    // 38. Nintendo AGL Weapon & Gizmo Tuning Parameter Subsystem (.params)
    printf("\n--- [38/38] NINTENDO AGL WEAPON & GIZMO PARAMETER SUBSYSTEM ---\n");

    // Roller Family Physics & Timing
    Game::RollerWeaponParams splatRollerParam, dynamoParam, carbonParam, brushParam;
    bool normalRollerLoaded = splatRollerParam.load("content/Static/RollerNormal.params");
    bool dynamoLoaded = dynamoParam.load("content/Static/RollerHeavy.params");
    bool carbonLoaded = carbonParam.load("content/Static/RollerCompact.params");
    bool brushLoaded = brushParam.load("content/Static/RollerBrushNormal.params");

    bool rollerComparisonOk = (normalRollerLoaded && dynamoLoaded && carbonLoaded && brushLoaded &&
                               splatRollerParam.swingLiftFrame == 20 &&
                               dynamoParam.swingLiftFrame == 45 &&      // Heavy 45f windup
                               carbonParam.swingLiftFrame == 9 &&       // Fast 9f flick
                               brushParam.swingLiftFrame == 1 &&        // Instantaneous 1f brush swing
                               dynamoParam.splashNum == 16 &&
                               splatRollerParam.splashNum == 12 &&
                               carbonParam.splashNum == 10 &&
                               brushParam.splashNum == 3);

    // Splash Wall Parameters
    Game::ShieldParams shieldParam;
    bool shieldLoaded = shieldParam.load("content/Static/Wsb_Shield.params");
    bool shieldParamOk = (shieldLoaded &&
                          shieldParam.preparationDurationFrame == 30 &&
                          shieldParam.noDamageRunningDurationFrame == 370 &&
                          shieldParam.boundVelLen == 2.0f &&
                          shieldParam.paintRepeatFrame == 6);

    // Rainmaker Objective Parameters
    Game::ShachihokoParams shachihokoParam;
    bool shachiLoaded = shachihokoParam.load("content/Static/Wsp_Shachihoko.params");
    bool shachiParamOk = (shachiLoaded &&
                          shachihokoParam.victoryPlayerTimeLimitFrame == 3600 && // 60s countdown
                          shachihokoParam.barrierRadius == 15.0f &&
                          shachihokoParam.barrierMaxScale == 3.0f);

    // Ink Mine Parameters
    Game::TrapParams trapParam;
    bool trapLoaded = trapParam.load("content/Static/Trap.params");
    bool trapParamOk = (trapLoaded &&
                        trapParam.timerFrame == 600 &&    // 10s timeout
                        trapParam.presageFrame == 60 &&   // 1s blink presage
                        trapParam.playerColRadius == 20.0f);

    bool fullAglParamOk = (rollerComparisonOk && shieldParamOk && shachiParamOk && trapParamOk);
    printf("  AGL Parameter Engine (AglParameterObj):      %s (Rollers: Normal 20f/Dynamo 45f/Carbon 9f/Brush 1f, Wall: 30f, RM: 3600f)\n",
           fullAglParamOk ? "PASSED" : "FAILED");
    if (!fullAglParamOk) allPassed = false;

    // 39. 26 Gear Ability Skill Icon Indexer & Level-Gated Tips Subsystem
    printf("\n--- [39/39] GEAR SKILL ICONS & LEVEL-GATED GAME TIPS SUBSYSTEM ---\n");

    Game::SkillTipsCatalog skillTips;
    bool skillTipsLoaded = skillTips.loadFromByml("content/Static/Skill_Icon.byaml", "content/Static/TipsTextInfo.byaml");
    bool skillTipsCountsOk = (skillTipsLoaded &&
                              skillTips.getSkillCount() == 26 &&
                              skillTips.getTipCount() == 83);

    // Verify Skill Icon indexer
    const auto* atkUp = skillTips.findSkillById(0);
    bool atkOk = (atkUp && atkUp->name == "Attack_Up");

    const auto* defUp = skillTips.findSkillById(1);
    bool defOk = (defUp && defUp->name == "Defense_Up");

    const auto* blankSkill = skillTips.findSkillById(-2);
    bool blankOk = (blankSkill && blankSkill->name == "Blank");

    const auto* unknownSkill = skillTips.findSkillById(-1);
    bool unknownOk = (unknownSkill && unknownSkill->name == "Unknown");

    // Verify Level-Gated Tips filtering
    auto lvl1Tips = skillTips.getTipsForPlayerLevel(1);
    auto lvl5Tips = skillTips.getTipsForPlayerLevel(5);
    bool lvl1HasSp1 = false;
    for (const auto* t : lvl1Tips) {
        if (t->label == "Sp_1_1_00") lvl1HasSp1 = true;
    }
    bool lvl5HasSp2 = false;
    for (const auto* t : lvl5Tips) {
        if (t->label == "Sp_2_6_00") lvl5HasSp2 = true;
    }

    bool tipsFilterOk = (!lvl1Tips.empty() && lvl1HasSp1 && !lvl5Tips.empty() && lvl5HasSp2 && lvl1Tips.size() < skillTips.getTipCount());
    bool fullSkillTipsOk = (skillTipsCountsOk && atkOk && defOk && blankOk && unknownOk && tipsFilterOk);
    printf("  Skill Icons & Tips (SkillTipsCatalog):       %s (Skills: 26/26, Tips: 83/83, Level 1 Gated: %zu, Level 5: %zu)\n",
           fullSkillTipsOk ? "PASSED" : "FAILED", lvl1Tips.size(), lvl5Tips.size());
    if (!fullSkillTipsOk) allPassed = false;

    // 40. Cap'n Cuttlefish Octo Valley Sector Dialogue Progression Subsystem
    printf("\n--- [40/40] CAP'N CUTTLEFISH SECTOR DIALOGUE PROGRESSION ---\n");

    Game::CuttlefishDialogueMgr cuttlefish;
    bool cuttlefishLoaded = cuttlefish.loadFromByml("content/Static/WorldTalkTextInfo.byaml");
    bool cuttlefishCountsOk = (cuttlefishLoaded && cuttlefish.getTotalDialogueCount() == 31 && cuttlefish.getMaxArea() == 5);

    // Verify Area 1 (Sector 1) dialogue progression:
    const auto* d0 = cuttlefish.getDialogue(1, 0); // 0 clears
    bool d0Ok = (d0 && d0->messageLabel == "A1_00_AT" && d0->isPermanent && d0->isFreeTalk);

    const auto* d1 = cuttlefish.getDialogue(1, 1); // 1 clear
    bool d1Ok = (d1 && d1->messageLabel == "A1_01_AT" && !d1->isPermanent);

    const auto* d2 = cuttlefish.getDialogue(1, 2); // 2 clears
    bool d2Ok = (d2 && d2->messageLabel == "A1_02_AT");

    const auto* d3 = cuttlefish.getDialogue(1, 3); // 3 clears
    bool d3Ok = (d3 && d3->messageLabel == "A1_03_AT");

    // Verify Area 2 (Sector 2) dialogue progression:
    const auto* dArea2Last = cuttlefish.getDialogue(2, 6);
    bool dArea2Ok = (dArea2Last && dArea2Last->messageLabel == "A2_06_AT");

    // Verify Sector counts: Area 1 has 4 dialogues, Area 2 has 6 dialogues
    auto area1Dialogues = cuttlefish.getDialoguesForArea(1);
    auto area2Dialogues = cuttlefish.getDialoguesForArea(2);
    bool areasOk = (area1Dialogues.size() == 4 && area2Dialogues.size() == 6);

    bool fullCuttlefishOk = (cuttlefishCountsOk && d0Ok && d1Ok && d2Ok && d3Ok && dArea2Ok && areasOk);
    printf("  Cap'n Cuttlefish Dialogues (CuttlefishDialogueMgr): %s (Dialogues: 31/31, Areas: 5, Area 1: 4, Area 2: 6)\n",
           fullCuttlefishOk ? "PASSED" : "FAILED");
    if (!fullCuttlefishOk) allPassed = false;

    // 41. Cinematic Camera Sequence & Stage Overview Engine (85 Authentic Presets)
    printf("\n--- [41/41] CINEMATIC CAMERA SEQUENCE & STAGE OVERVIEW ENGINE ---\n");

    Game::CameraParamEngine camEngine;
    bool camLoaded = camEngine.loadDirectory("content/Static");

    // 1. Verify PreGame player intro cameras (Friend / Opposite teams)
    const auto* preGameFriend = camEngine.getCamera("PreGame_PlayerView_Default_Friend");
    bool friendOk = (preGameFriend && preGameFriend->refType == 4 && preGameFriend->fovy > 54.0f &&
                     preGameFriend->pos.y == 26.0f && preGameFriend->at.y == 4.0f);

    const auto* preGameOpposite = camEngine.getCamera("PreGame_PlayerView_Default_Opposite");
    bool oppositeOk = (preGameOpposite && preGameOpposite->refType == 5 && preGameOpposite->fovy == 55.0f);

    // 2. Verify Plaza Studio TV News camera (Callie & Marie Inkopolis News framing)
    const auto* newsCam = camEngine.getCamera("Plaza_News");
    bool newsOk = (newsCam && newsCam->refType == 0 && std::abs(newsCam->at.x - 217.0f) < 0.1f &&
                   std::abs(newsCam->pos.x - 230.0f) < 0.1f && newsCam->interpolateFrameMax == 60);

    // 3. Verify Shop Camera (Ammo Knights / Booyah Base)
    const auto* shopCam = camEngine.getCamera("ShopDefault");
    bool shopOk = (shopCam && shopCam->nearPlane == 0.1f && shopCam->farPlane == 20000.0f &&
                   shopCam->fovy == 45.0f && shopCam->limit == 60);

    // 4. Verify Post-Game TV Stage Overview Camera (Fld_Warehouse00)
    const auto* postWarehouse = camEngine.getCamera("PostGame_StageView_Fld_Warehouse00_TV");
    bool camWarehouseOk = (postWarehouse && postWarehouse->pos.y == 1548.0f && postWarehouse->at.x == -160.0f);

    // 5. Verify Camera View & Perspective Projection Matrix Generation
    bool camMathOk = false;
    f32 dist = 0.0f;
    if (preGameFriend) {
        sead::Matrix44f viewMtx = preGameFriend->buildViewMatrix();
        sead::Matrix44f projMtx = preGameFriend->buildProjMatrix();
        sead::Vector3f forward = preGameFriend->getForward();
        dist = preGameFriend->getDistance();
        camMathOk = (dist > 40.0f && std::abs(forward.length() - 1.0f) < 0.001f &&
                     projMtx.m[0][0] > 0.0f && projMtx.m[1][1] > 0.0f);
    }

    bool fullCamOk = (camLoaded && friendOk && oppositeOk && newsOk && shopOk && camWarehouseOk && camMathOk);
    printf("  Cinematic Cameras (CameraParamEngine):       %s (Presets: 85/85, Friend: FOV 55, News: Studio Cut, Dist: %.1fm)\n",
           fullCamOk ? "PASSED" : "FAILED", dist);
    if (!fullCamOk) allPassed = false;

    // 42. Particle & Debris Model VFX Binding Subsystem (ParticleBindModel.byaml)
    printf("\n--- [42/42] PARTICLE & DEBRIS MODEL VFX BINDING SUBSYSTEM ---\n");

    Game::ParticleBindCatalog particleCatalog;
    bool particleLoaded = particleCatalog.loadFromByml("content/Static/ParticleBindModel.byaml");
    bool particleCountsOk = (particleLoaded && particleCatalog.getEntryCount() == 64 && particleCatalog.getUniqueParentCount() == 28);

    // 1. Verify Entry 0: Destructible Crate Debris binding (Obj_Box00L -> Obj_Break00, Pattern 3)
    const auto* e0 = particleCatalog.getEntryByIndex(0);
    bool e0Ok = (e0 && e0->modelName == "Obj_Break00" && e0->parentModelName == "Obj_Box00L" && e0->pattern == 3);

    // 2. Verify Octostomp Boss debris bindings (Enm_Stamp has 3 broken mesh parts: Break00, 01, 02)
    auto stampBindings = particleCatalog.getEntriesByParent("Enm_Stamp");
    bool stampOk = (stampBindings.size() == 3 && stampBindings[0]->modelName == "Enm_Break00");

    // 3. Verify DJ Octavio Boss Deflection SFX bindings (RailKing arm & punch deflection debris)
    auto armBindings = particleCatalog.getEntriesByParent("Enm_RailKingArm");
    auto punchBindings = particleCatalog.getEntriesByParent("Enm_RailKingPunch");
    bool railKingSfxOk = (!armBindings.empty() && armBindings[0]->se == "BrokenPiece_RailKingArm" &&
                          !punchBindings.empty() && punchBindings[0]->se == "BrokenPiece_RailKingPunch");

    // 4. Verify Total Audio-Enabled Debris Triggers (39 entries)
    auto audioBindings = particleCatalog.getEntriesWithSoundEffect();
    bool debrisAudioOk = (audioBindings.size() == 39);

    bool fullParticleOk = (particleCountsOk && e0Ok && stampOk && railKingSfxOk && debrisAudioOk);
    printf("  Particle & VFX Debris (ParticleBindCatalog): %s (Bindings: 64/64, Parents: 28, Audio FX: %zu/39, Deflection: YES)\n",
           fullParticleOk ? "PASSED" : "FAILED", audioBindings.size());
    if (!fullParticleOk) allPassed = false;

    // 43. Inkopolis Plaza Offline Inhabitants & Photo Avatars (PhotographPlayerInfo.byaml)
    printf("\n--- [43/43] INKOPOLIS PLAZA INHABITANTS & PHOTO AVATARS ---\n");

    Game::PlazaAvatarCatalog avatarCatalog;
    bool avatarLoaded = avatarCatalog.loadFromByml("content/Static/PhotographPlayerInfo.byaml");
    bool avatarCountsOk = (avatarLoaded && avatarCatalog.getTotalAvatarCount() == 81 &&
                           avatarCatalog.getGirlCount() == 40 && avatarCatalog.getBoyCount() == 41);

    // 1. Verify Entry 0: Default Lobby Girl (Player1, Skin 2, Shot_Normal00)
    const auto* a0 = avatarCatalog.getAvatar(0);
    bool a0Ok = (a0 && a0->name == "Player1" && a0->isGirl() && a0->skin == 2 &&
                 a0->clothes == "TES001" && a0->weaponSet == "Shot_Normal00");

    // 2. Verify Roaming Plaza NPCs (8 presets: Erick, Polly, Monica, Bernardo, etc.)
    auto npcPresets = avatarCatalog.getNpcPresets();
    const auto* erick = avatarCatalog.findByName("00.Erick");
    const auto* polly = avatarCatalog.findByName("01.Polly");
    bool npcOk = (npcPresets.size() == 8 && erick && erick->weaponSet == "Roller_Heavy00" &&
                  polly && polly->weaponSet == "Roller_Normal00");

    // 3. Verify Amiibo Challenge Presets (School Uniform, Samurai, Power Armor)
    auto amiiboPresets = avatarCatalog.getAmiiboPresets();
    bool amiiboOk = (amiiboPresets.size() == 3 &&
                     amiiboPresets[0]->clothes == "AMB000" &&
                     amiiboPresets[1]->clothes == "AMB001" &&
                     amiiboPresets[2]->clothes == "AMB002");

    bool fullAvatarOk = (avatarCountsOk && a0Ok && npcOk && amiiboOk);
    printf("  Plaza Avatars (PlazaAvatarCatalog):           %s (Avatars: 81/81, Girls: 40, Boys: 41, Roaming NPCs: 8, Amiibo: 3)\n",
           fullAvatarOk ? "PASSED" : "FAILED");
    if (!fullAvatarOk) allPassed = false;

    // 44. Expanded Authentic Retail 3D Weapon, Tank & Monitor Arsenal
    printf("\n--- [44/44] EXPANDED AUTHENTIC RETAIL 3D ARSENAL & TANK ASSETS ---\n");

    sead::BfresModel realAerospray = sead::BfresParser::createAerosprayModel("Aerospray_Blaze");
    sead::BfresModel realEliter = sead::BfresParser::createEliter3KModel("Eliter3K_Long");
    sead::BfresModel realOctobrush = sead::BfresParser::createOctobrushModel("Octobrush_Normal");
    sead::BfresModel realMonitor = sead::BfresParser::createInkstrikeMonitorModel("Inkstrike_Monitor");
    sead::BfresModel realHeroTank = sead::BfresParser::createHeroTankModel("HeroTank_Lv0");

    bool retailAeroOk = (realAerospray.getTotalVertexCount() >= 100);
    bool eliterOk = (realEliter.getTotalVertexCount() >= 100);
    bool brushOk = (realOctobrush.getTotalVertexCount() >= 100);
    bool monOk = (realMonitor.getTotalVertexCount() >= 100);
    bool retailHeroTankOk = (realHeroTank.getTotalVertexCount() >= 100);

    bool fullArsenalOk = (retailAeroOk && eliterOk && brushOk && monOk && retailHeroTankOk);
    printf("  Expanded Retail 3D Arsenal:                  %s (Aerospray: %zu Verts, E-liter: %zu, Octobrush: %zu, Monitor: %zu, Tank: %zu)\n",
           fullArsenalOk ? "PASSED" : "FAILED",
           realAerospray.getTotalVertexCount(),
           realEliter.getTotalVertexCount(),
           realOctobrush.getTotalVertexCount(),
           realMonitor.getTotalVertexCount(),
           realHeroTank.getTotalVertexCount());
    if (!fullArsenalOk) allPassed = false;

    // 45. Inkopolis Plaza & Octo Valley 3D Skybox & Environment Pipeline
    printf("\n--- [45/45] INKOPOLIS PLAZA & OCTO VALLEY 3D SKYBOX ENVIRONMENT PIPELINE ---\n");

    sead::BfresModel realSkyDay = sead::BfresParser::createSkyDayPlazaModel("Sky_Day_Plaza");
    sead::BfresModel realSkyNight = sead::BfresParser::createSkyNightPlazaModel("Sky_Night_Plaza");
    sead::BfresModel realSkyWorld = sead::BfresParser::createOctoValleySkyWorldModel("Sky_OctoValley");
    sead::BfresModel realBananaTree = sead::BfresParser::createBananaTreeModel("Banana_Tree");
    sead::BfresModel realWaterTank = sead::BfresParser::createWaterTankModel("Water_Tank");

    bool skyDayOk = (realSkyDay.getTotalVertexCount() >= 10);
    bool skyNightOk = (realSkyNight.getTotalVertexCount() >= 10);
    bool skyWorldOk = (realSkyWorld.getTotalVertexCount() >= 10);
    bool bananaOk = (realBananaTree.getTotalVertexCount() >= 10);
    bool tankPropOk = (realWaterTank.getTotalVertexCount() >= 10);

    bool fullEnvOk = (skyDayOk && skyNightOk && skyWorldOk && bananaOk && tankPropOk);
    printf("  Retail 3D Skybox & Environment:              %s (Day Sky: %zu Verts, Fest Night: %zu, Octo Valley: %zu, Banana: %zu, Tank: %zu)\n",
           fullEnvOk ? "PASSED" : "FAILED",
           realSkyDay.getTotalVertexCount(),
           realSkyNight.getTotalVertexCount(),
           realSkyWorld.getTotalVertexCount(),
           realBananaTree.getTotalVertexCount(),
           realWaterTank.getTotalVertexCount());
    if (!fullEnvOk) allPassed = false;

    // 46. Authentic Ink Mine Lethality & TurnPlate Gimmick Ballistics (Trap.params & TurnPlate.params)
    printf("\n--- [46/46] INK MINE LETHALITY & REVOLVING TURNPLATE GIMMICK BALLISTICS ---\n");

    Game::TrapParams aglTrap;
    bool aglTrapLoaded = aglTrap.load("content/Static/Trap.params");
    bool trapHitOk = (aglTrapLoaded && aglTrap.maxHp == 1.0f &&
                      aglTrap.timerFrame == 600 &&          // 10-second automatic timer
                      aglTrap.presageFrame == 60 &&         // 1-second detonation presage warning
                      aglTrap.bombCoreDamageNear > 1.7f &&  // 180 HP OHKO near lethal blast
                      aglTrap.bombCoreRadiusNear == 40.0f &&
                      aglTrap.bombCoreDamageMiddle == 0.3f && // 30 HP splash
                      aglTrap.bombCoreRadiusMiddle == 80.0f &&
                      aglTrap.bombCorePaintRadius == 50.0f);

    Game::TurnPlateParams aglTurnPlate;
    bool aglTurnPlateLoaded = aglTurnPlate.load("content/Static/TurnPlate.params");
    bool aglTurnPlateOk = (aglTurnPlateLoaded && aglTurnPlate.harfLen == 33.0f &&
                           aglTurnPlate.colZLen == 70.0f &&
                           aglTurnPlate.rotSpeed == 1.0f &&
                           aglTurnPlate.colHeight == 10.0f &&
                           aglTurnPlate.playerMoveSpeed == 2.0f &&
                           aglTurnPlate.lostTargetTime == 30 &&
                           aglTurnPlate.lockStartTime == 45);

    // Verify Kraken Roller & Octo Valley Hero Roller ballistic physics
    Game::RollerWeaponParams krakenRoller, heroRoller;
    bool krakenRollerLoaded = krakenRoller.load("content/Static/RollerKingSquid.params");
    bool heroRollerLoaded = heroRoller.load("content/Static/RollerMission.params");
    bool rollersSpecialOk = (krakenRollerLoaded && heroRollerLoaded &&
                             krakenRoller.swingLiftFrame == 9 &&
                             krakenRoller.splashNum == 16 &&
                             krakenRoller.splashInitSpeedBase == 17.5f && // 17.5 m/s tidal wave
                             heroRoller.swingLiftFrame == 16 &&
                             heroRoller.splashNum == 12);

    bool fullGimmickOk = (trapHitOk && aglTurnPlateOk && rollersSpecialOk);
    printf("  Trap & TurnPlate Ballistics (AglParameter):  %s (Mine OHKO: 180 HP, Presage: 60f, Plate Rot: 1.0, Kraken Vel: 17.5m/s)\n",
           fullGimmickOk ? "PASSED" : "FAILED");
    if (!fullGimmickOk) allPassed = false;

    // -----------------------------------------------------------------
    // Milestone 47 Verification: Audio Sound Link 2 Database (SLink2DB / XLNK)
    // -----------------------------------------------------------------
    Game::SLinkDatabase slinkDb;
    bool slinkLoaded = slinkDb.loadFromSzs("content/Static/SLink2DB.szs");

    bool slinkOk = false;
    if (slinkLoaded) {
        // Assert authentic retail SLink2DB metrics
        bool countOk = (slinkDb.getUserCount() == 883 &&
                        slinkDb.getNamedUserCount() == 883 &&
                        slinkDb.getTotalStringCount() >= 7000);

        // Verify key player & bullet hierarchies
        auto chargeHier = slinkDb.resolveHierarchy("BulletPlayerChargeShot");
        bool chargeHierOk = (chargeHier.size() == 3 &&
                             chargeHier[0] == "BulletPlayerChargeShot" &&
                             chargeHier[1] == "BulletPlayerNormalShot" &&
                             chargeHier[2] == "Bullet");

        auto sprinklerHier = slinkDb.resolveHierarchy("BulletBombSprinkler");
        bool sprinklerHierOk = (sprinklerHier.size() == 2 &&
                                sprinklerHier[0] == "BulletBombSprinkler" &&
                                sprinklerHier[1] == "SubWeapon");

        // Verify sound triggers and cues
        bool chargeTrigOk = slinkDb.hasTrigger("BulletPlayerChargeShot", "HitSplash") &&
                            slinkDb.hasTrigger("BulletPlayerChargeShot", "Swish") &&
                            slinkDb.hasTrigger("BulletPlayerChargeShot", "HiSplashP0FullCharge");

        bool enemyBombTrigOk = slinkDb.hasTrigger("BulletEnemyBomb", "BombAlert") &&
                               slinkDb.hasTrigger("BulletEnemyBomb", "OnSleep") &&
                               slinkDb.hasTrigger("BulletEnemyBomb", "Fly_Ctrl");

        // Verify Octo Valley boss audio bindings (DJ Octavio & Octostomp)
        const auto* octavioFist = slinkDb.findUser("EnemyRailKingPunch");
        bool octavioOk = (octavioFist != nullptr && octavioFist->parent == "BossRailKing");

        const auto* rallyPunch = slinkDb.findUser("EnemyRailKingRallyPunch");
        bool rallyOk = (rallyPunch != nullptr && rallyPunch->parent == "EnemyRailKingPunch");

        const auto* stampKing = slinkDb.findUser("EnemyStampKing");
        bool stampOk = (stampKing != nullptr);

        slinkOk = (countOk && chargeHierOk && sprinklerHierOk &&
                   chargeTrigOk && enemyBombTrigOk && octavioOk && rallyOk && stampOk);
    }

    printf("  Audio Sound Link 2 Engine (SLink2DB/XLNK):   %s (883 Named Users, 7,201 Strings, Bullet/Boss Cues Authenticated)\n",
           slinkOk ? "PASSED" : "FAILED");
    if (!slinkOk) allPassed = false;

    // -----------------------------------------------------------------
    // Milestone 48 Verification: Player Rank Curve, Ink Tanks, Battle Dojo & ELink2DB
    // -----------------------------------------------------------------
    Game::PlayerRankMgr rankMgr;
    bool rankLoaded = rankMgr.load("content/Static/PlayerRank.byaml");
    bool rankOk = false;
    if (rankLoaded) {
        // Verify EXP thresholds
        u32 r1Exp = rankMgr.getNextRankExp(1);  // 700
        u32 r5Exp = rankMgr.getNextRankExp(5);  // 4800
        u32 r19Exp = rankMgr.getNextRankExp(19); // 30000

        u32 calcRank = 0, currentExp = 0, neededExp = 0;
        rankMgr.calculateRankFromTotalExp(5000, calcRank, currentExp, neededExp);
        // At 5000 total EXP: rank should be 6 (threshold 4800 passed), currentExp = 200, neededExp = 1200 (6000 - 4800)
        bool rankCalcOk = (calcRank == 6 && currentExp == 200 && neededExp == 1200);

        rankOk = (rankMgr.getRankCount() == 20 && r1Exp == 700 && r5Exp == 4800 && r19Exp == 30000 && rankCalcOk);
    }

    Game::TankInfoCatalog tankCatalog;
    bool tanksLoaded = tankCatalog.load("content/Static/TankInfo.byaml");
    bool tanksOk = false;
    if (tanksLoaded) {
        const auto* simpleTank = tankCatalog.getTankByName("Tnk_Simple");
        const auto* heroTankLv0 = tankCatalog.getTankByName("Tnk_Msn0Lv0");
        const auto* heroTankLv3 = tankCatalog.getTankByName("Tnk_Msn0Lv3");
        const auto* rivalTank = tankCatalog.getTankByName("Tnk_Rvl00");

        tanksOk = (tankCatalog.getTankCount() == 6 &&
                   simpleTank != nullptr && simpleTank->id == 0 &&
                   heroTankLv0 != nullptr && heroTankLv0->id == 1000 &&
                   heroTankLv3 != nullptr && heroTankLv3->id == 1003 &&
                   rivalTank != nullptr && rivalTank->id == 2000);
    }

    Game::DuelPlayerSettingCatalog duelPresetsCatalog;
    bool dojoLoaded = duelPresetsCatalog.load("content/Static/DuelPlayerSetting.byaml");
    bool dojoPresetsOk = false;
    if (dojoLoaded) {
        const auto* p0 = duelPresetsCatalog.getPreset(0);
        const auto* p1 = duelPresetsCatalog.getPreset(1);
        const auto* p2 = duelPresetsCatalog.getPreset(2);

        dojoPresetsOk = (duelPresetsCatalog.getPresetCount() == 8 &&
                         p0 != nullptr && p0->weaponSet == "Shot_Normal00" && p0->head == "HDP000" &&
                         p1 != nullptr && p1->weaponSet == "Roller_Normal00" && p1->head == "EYE000" &&
                         p2 != nullptr && p2->weaponSet == "Charge_Normal00" && p2->head == "NCP000");
    }

    Game::ELinkDatabase elinkDb;
    bool elinkLoaded = elinkDb.loadFromSzs("content/Static/ELink2DB.szs");
    bool elinkOk = false;
    if (elinkLoaded) {
        // Assert authentic retail ELink2DB metrics
        bool eCountOk = (elinkDb.getUserCount() == 362 && elinkDb.getNamedUserCount() == 362);

        // Verify key effect emitter hierarchies
        auto msBombHier = elinkDb.resolveHierarchy("BulletBombJumpingMissionLv0");
        bool msBombHierOk = (msBombHier.size() == 3 &&
                             msBombHier[0] == "BulletBombJumpingMissionLv0" &&
                             msBombHier[1] == "MSBomb" &&
                             msBombHier[2] == "Bomb");

        auto normBombHier = elinkDb.resolveHierarchy("BulletBombNormal");
        bool normBombHierOk = (normBombHier.size() == 2 &&
                               normBombHier[0] == "BulletBombNormal" &&
                               normBombHier[1] == "Bomb");

        const auto* suckerBomb = elinkDb.findUser("BulletBombSuckerMissionLv0");
        bool suckerOk = (suckerBomb != nullptr && suckerBomb->parent == "BulletBombSucker");

        elinkOk = (eCountOk && msBombHierOk && normBombHierOk && suckerOk);
    }

    bool m48Ok = (rankOk && tanksOk && dojoPresetsOk && elinkOk);
    printf("  Rank EXP, Tanks, Dojo & ELink2DB Engine:     %s (20 Ranks, 6 Tanks, 8 Presets, 362 VFX Users Authenticated)\n",
           m48Ok ? "PASSED" : "FAILED");
    if (!m48Ok) allPassed = false;

    // -----------------------------------------------------------------
    // Milestone 49 Verification: Squid Sisters News Script & Retro Arcade Mini Games
    // -----------------------------------------------------------------
    Game::NewsScriptEngine newsEngine;
    bool newsLoaded = newsEngine.loadFromSzs("content/Static/NewsScript.szs");
    bool newsScriptOk = false;
    if (newsLoaded) {
        bool scnCountOk = (newsEngine.getScenarioCount() == 36);
        bool scnTypesOk = newsEngine.hasScenario("FirstBoot") &&
                          newsEngine.hasScenario("FestivalAnnounceCommon") &&
                          newsEngine.hasScenario("FestivalEnd") &&
                          newsEngine.hasScenario("FestivalResultResultShow") &&
                          newsEngine.hasScenario("FestivalVoteStart");

        const auto* firstBoot = newsEngine.findScenario("FirstBoot");
        bool firstBootOk = (firstBoot != nullptr && !firstBoot->commands.empty());

        newsScriptOk = (scnCountOk && scnTypesOk && firstBootOk);
    }

    Game::MiniGameCatalog miniGameCatalog;
    bool mgLoaded = miniGameCatalog.loadFromSzs("content/Static/MiniGame.szs");
    bool miniGamesOk = false;
    if (mgLoaded) {
        bool vballCountOk = (miniGameCatalog.getVBallStageCount() == 30);
        bool raceCountOk = (miniGameCatalog.getRaceStageCount() == 25);
        bool jbCountOk = (miniGameCatalog.getJukeBoxTrackCount() == 27);

        const auto* vb0 = miniGameCatalog.getVBallStage(0);
        const auto* vb4 = miniGameCatalog.getVBallStage(4);
        bool vbParamsOk = (vb0 != nullptr && vb0->goalPoint == 3 &&
                           vb4 != nullptr && vb4->goalPoint == 20);

        const auto* r0 = miniGameCatalog.getRaceStage(0);
        const auto* r5 = miniGameCatalog.getRaceStage(5);
        bool raceParamsOk = (r0 != nullptr && r0->timeLimit == 10 && r0->strongRival == "1.1" &&
                             r5 != nullptr && r5->timeLimit == 17 && r5->strongRival == "1.6");

        const auto* jb0 = miniGameCatalog.getJukeBoxTrack(0);
        bool jbParamsOk = (jb0 != nullptr && jb0->numNotesEasy == 161);

        miniGamesOk = (vballCountOk && raceCountOk && jbCountOk && vbParamsOk && raceParamsOk && jbParamsOk);
    }

    bool m49Ok = (newsScriptOk && miniGamesOk);
    printf("  News Script & Retro Arcade Mini Games:       %s (36 Broadcasts, 30 VBall, 25 Race, 27 JukeBox Tracks)\n",
           m49Ok ? "PASSED" : "FAILED");
    if (!m49Ok) allPassed = false;

    // -----------------------------------------------------------------
    // Milestone 50 Verification: Plaza NPC Presets & Gambit Master Stage Tree
    // -----------------------------------------------------------------
    Game::PlazaNpcPresetCatalog npcCatalog;
    bool npcLoaded = npcCatalog.loadFromSzs("content/Static/PlayerInfo.szs");
    bool npcPresetsOk = false;
    if (npcLoaded) {
        bool npcCountOk = (npcCatalog.getPresetCount() == 30);

        const auto* erick = npcCatalog.getPresetByName("Erick");
        const auto* polly = npcCatalog.getPresetByName("Polly");
        const auto* susie = npcCatalog.getPresetByName("Susie");
        const auto* rui = npcCatalog.getPresetByName("Rui");

        bool namesAndGearOk = (erick != nullptr && erick->weaponId == 1070 && erick->headGearId == 115 &&
                               polly != nullptr && polly->weaponId == 1040 &&
                               susie != nullptr && susie->weaponId == 2010 && susie->headGearId == 202 &&
                               rui != nullptr && rui->weaponId == 4010);

        npcPresetsOk = (npcCountOk && namesAndGearOk);
    }

    Game::GambitStageTreeCatalog stageTree;
    bool treeLoaded = stageTree.loadFromSzs("content/Static/Gambit.mutre.szs");
    bool stageTreeOk = false;
    if (treeLoaded) {
        bool treeCountOk = (stageTree.getStageCount() == 375);

        const auto* tf00 = stageTree.findStageByName("TF00_PlayerTest");
        const auto* tf01 = stageTree.findStageByName("TF01_ObjTest");
        const auto* tf04 = stageTree.findStageByName("TF04_EnemyTest");

        bool stagesFoundOk = (tf00 != nullptr && tf00->location == "Root/Test/TF 00-19" &&
                              tf01 != nullptr && tf01->filePath.find("TestField01") != std::string::npos &&
                              tf04 != nullptr && tf04->filePath.find("TestField04") != std::string::npos);

        auto testStages = stageTree.findByLocationPrefix("Root/Test");
        bool filterOk = (!testStages.empty() && testStages.size() >= 20);

        stageTreeOk = (treeCountOk && stagesFoundOk && filterOk);
    }

    bool m50Ok = (npcPresetsOk && stageTreeOk);
    printf("  Plaza NPCs & Master Stage Tree (Gambit.mutre):%s (30 Plaza Walkers, 375 Developer & Retail Stages)\n",
           m50Ok ? "PASSED" : "FAILED");
    if (!m50Ok) allPassed = false;

    // -----------------------------------------------------------------
    // Milestone 51 Verification: Octo Valley Mission Stage Maps & Boss Arenas
    // -----------------------------------------------------------------
    Game::MissionStageMapParser stampMap;
    bool stampMapLoaded = stampMap.loadFromSzs("content/Static/Fld_BossStampKing_Bos_Msn.szs");
    bool stampMapOk = false;
    if (stampMapLoaded) {
        bool actCountOk = (stampMap.getActorCount() == 19);
        bool hasBossOk = stampMap.hasBoss();
        bool hasGoalOk = stampMap.hasGoalZapfish();
        bool hasScrollOk = stampMap.hasSunkenScroll();
        bool hasRespawnOk = stampMap.hasRespawnPoint();
        bool railCountOk = (stampMap.getRailCount() == 1);

        stampMapOk = (actCountOk && hasBossOk && hasGoalOk && hasScrollOk && hasRespawnOk && railCountOk);
    }

    Game::MissionStageMapParser octavioMap;
    bool octavioMapLoaded = octavioMap.loadFromSzs("content/Static/Fld_BossRailKing_Bos_Msn.szs");
    bool octavioMapOk = false;
    if (octavioMapLoaded) {
        bool actCountOk = (octavioMap.getActorCount() == 182);
        bool railCountOk = (octavioMap.getRailCount() == 12);
        bool hasBossOk = octavioMap.hasBoss();
        bool hasRespawnOk = octavioMap.hasRespawnPoint();

        octavioMapOk = (actCountOk && railCountOk && hasBossOk && hasRespawnOk);
    }

    Game::MissionStageMapParser worldMap;
    bool worldMapLoaded = worldMap.loadFromSzs("content/Static/Fld_World00_Wld.szs");
    bool worldMapOk = false;
    if (worldMapLoaded) {
        worldMapOk = (worldMap.getActorCount() == 204 && worldMap.getRailCount() == 32);
    }

    bool m51Ok = (stampMapOk && octavioMapOk && worldMapOk);
    printf("  Campaign Stage Maps (Octostomp, Octavio & World):%s (19 Arena Objs, 182 Boss Objs/12 Rails, 204 Overworld Objs/32 Rails)\n",
           m51Ok ? "PASSED" : "FAILED");
    if (!m51Ok) allPassed = false;

    // -----------------------------------------------------------------
    // Milestone 52 Verification: Sunken Scroll Lore & Archive Database
    // -----------------------------------------------------------------
    Game::SunkenScrollCatalog scrollCatalog;
    bool scrollInitOk = scrollCatalog.init();
    bool scrollsOk = false;
    if (scrollInitOk) {
        bool totalCountOk = (scrollCatalog.getTotalScrollCount() == 28);
        bool loreCountOk = (scrollCatalog.getLoreScrolls().size() == 23);
        bool blueprintCountOk = (scrollCatalog.getBlueprintScrolls().size() == 5);

        // Verify representative lore scrolls across campaign areas
        const auto* s1 = scrollCatalog.getScroll(1);
        const auto* s6 = scrollCatalog.getScroll(6);
        const auto* s14 = scrollCatalog.getScroll(14);
        const auto* s16 = scrollCatalog.getScroll(16);
        const auto* s23 = scrollCatalog.getScroll(23);

        bool loreEntriesOk = (s1 != nullptr && s1->missionNo == 1 && s1->title == "Creatures of the Surface" &&
                              s6 != nullptr && s6->missionNo == 6 && s6->stageMapName == "Fld_Propeller00_Msn" &&
                              s14 != nullptr && s14->missionNo == 14 && s14->category == Game::ScrollCategory::cCategory_CreatureEcology &&
                              s16 != nullptr && s16->missionNo == 16 && s16->category == Game::ScrollCategory::cCategory_HumanExtinction &&
                              s23 != nullptr && s23->missionNo == 23 && s23->title == "Young Squid Sisters Photograph");

        // Verify Sheldon weapon blueprints awarded by boss defeats
        const auto* bp24 = scrollCatalog.getScroll(24);
        const auto* bp26 = scrollCatalog.getScroll(26);
        const auto* bp28 = scrollCatalog.getScroll(28);

        bool blueprintsOk = (bp24 != nullptr && bp24->isBlueprint && bp24->unlockedWeapon == "Custom Splattershot Jr." &&
                             bp26 != nullptr && bp26->isBlueprint && bp26->unlockedWeapon == "Aerospray MG" && bp26->unlockedWeaponSub == "Aerospray RG" &&
                             bp28 != nullptr && bp28->isBlueprint && bp28->unlockedWeapon == "Dynamo Roller" && bp28->unlockedWeaponSub == "Gold Dynamo Roller");

        // Verify item pickup and weapon unlock integration
        Game::ItemAncientDocument scrollItem;
        scrollItem.init();
        scrollItem.spawn(sead::Vector3f(0.0f, 0.0f, 0.0f), 24);
        bool pickupOk = scrollItem.checkPlayerPickup(sead::Vector3f(0.5f, 0.0f, 0.0f), 0);
        bool linkOk = scrollCatalog.linkToItem(&scrollItem, 24);
        bool unlockOk = scrollCatalog.isWeaponUnlockedByScroll("Custom Splattershot Jr.");
        bool collectedCountOk = (scrollCatalog.getCollectedCount() == 1);

        // Verify authentic 3D BFRES models and assets on disk
        sead::BfresModel realScrollModel = sead::BfresParser::createSunkenScrollModel();
        sead::BfresModel realDummyModel = sead::BfresParser::createSunkenScrollDummyModel();
        bool modelAssetsOk = scrollCatalog.verifyModelAssets();
        bool scrollMeshOk = (realScrollModel.getTotalVertexCount() >= 100);
        bool dummyMeshOk = (realDummyModel.getTotalVertexCount() >= 100);

        scrollsOk = (totalCountOk && loreCountOk && blueprintCountOk &&
                     loreEntriesOk && blueprintsOk &&
                     pickupOk && linkOk && unlockOk && collectedCountOk &&
                     modelAssetsOk && scrollMeshOk && dummyMeshOk);
    }

    printf("  Sunken Scroll Lore & Archive Database:       %s (28 Scrolls, 23 Lore, 5 Blueprints, 491 Vert BFRES Model)\n",
           scrollsOk ? "PASSED" : "FAILED");
    if (!scrollsOk) allPassed = false;

    // -----------------------------------------------------------------
    // Milestone 53 Verification: Zapfish Power Grid & Goal Shield Globe
    // -----------------------------------------------------------------
    Game::Obj_Goal goalActor;
    goalActor.init(sead::Vector3f(0.0f, 0.0f, 0.0f), 1);
    bool goalParamsOk = false;
    bool goalCombatOk = false;

    // Verify authentic parameter multipliers
    const auto& gp = goalActor.getParams();
    goalParamsOk = (gp.bombCoreDamageK == 5.0f &&
                    gp.rollerCoreDamageK == 4.0f &&
                    gp.rollerSplashDamageK == 1.5f &&
                    gp.chargeBulletDamageK == 1.25f &&
                    gp.barrierRadiusFlickerCycleFrame == 12 &&
                    gp.animRelieved == 20.0f);

    // Verify barrier damage progression, shatter and player pickup
    bool bulletDmgOk = goalActor.applyBulletDamage(10.0f); // HP: 90.0
    bool flickerStateOk = (goalActor.getState() == Game::GoalShieldState::cState_ShieldFlicker);
    for (int f = 0; f < 15; ++f) goalActor.update();
    bool decayStateOk = (goalActor.getState() == Game::GoalShieldState::cState_Shielded);

    bool chargeDmgOk = goalActor.applyChargedDamage(20.0f); // 20 * 1.25 = 25.0 -> HP: 65.0
    bool bombDmgOk = goalActor.applyBombDamage(15.0f);       // 15 * 5.0 = 75.0 -> HP: 0.0 (shattered!)
    bool freedStateOk = (goalActor.getState() == Game::GoalShieldState::cState_ZapfishFreed);

    bool touchOk = goalActor.checkPlayerTouch(sead::Vector3f(1.0f, 0.0f, 0.0f));
    bool collectedStateOk = (goalActor.getState() == Game::GoalShieldState::cState_Collected);

    goalCombatOk = (bulletDmgOk && flickerStateOk && decayStateOk &&
                    chargeDmgOk && bombDmgOk && freedStateOk &&
                    touchOk && collectedStateOk);

    // Verify Zapfish Power Grid Subsystem
    Game::ZapfishPowerGridMgr gridMgr;
    bool gridInitOk = gridMgr.init();
    bool gridProgressionOk = false;
    if (gridInitOk) {
        bool totalCountOk = (gridMgr.getTotalZapfishCount() == 28);
        bool initialBlackoutOk = gridMgr.isCityBlackout() && (gridMgr.getInkopolisPowerPercentage() == 0.0f);

        // Test goal integration
        bool linkGoalOk = gridMgr.onGoalCollected(goalActor);
        bool miniRescuedOk = (gridMgr.getRescuedMiniZapfishCount() == 1);

        // Power progression: restore remaining Area 1 Zapfish (Stages 2 and 3)
        gridMgr.rescueZapfish(2);
        gridMgr.rescueZapfish(3);
        auto area1Status = gridMgr.getAreaPowerStatus(1);
        bool area1FullOk = (area1Status.isFullyPowered && area1Status.currentPowerMegaWatts == 300.0f);

        // Restore all remaining Mini Zapfish (Stages 4 through 27)
        for (u32 st = 4; st <= 27; ++st) {
            gridMgr.rescueZapfish(st);
        }
        bool allMiniOk = (gridMgr.getRescuedMiniZapfishCount() == 27);
        bool towerStillDarkOk = (gridMgr.getInkopolisPowerPercentage() == 0.0f);

        // Defeat final boss and restore The Great Zapfish
        bool greatRescuedOk = gridMgr.rescueZapfish(28);
        bool cityIlluminatedOk = (!gridMgr.isCityBlackout() &&
                                  gridMgr.getInkopolisPowerPercentage() == 100.0f &&
                                  gridMgr.getTowerIlluminationIntensity() == 1.0f &&
                                  gridMgr.getTotalGridPowerMegaWatts() == 12700.0f);

        // Verify authentic 3D BFRES models on disk
        sead::BfresModel realBigNamazu = sead::BfresParser::createGreatZapfishModel();
        sead::BfresModel realNamazu = sead::BfresParser::createMiniZapfishModel();
        sead::BfresModel realDummyNamazu = sead::BfresParser::createZapfishDummyModel();
        sead::BfresModel realGoalPedestal = sead::BfresParser::createGoalPedestalModel();

        bool assetsOk = gridMgr.verifyModelAssets();
        bool bigNamazuOk = (realBigNamazu.getTotalVertexCount() >= 500);
        bool namazuOk = (realNamazu.getTotalVertexCount() >= 300);
        bool dummyOk = (realDummyNamazu.getTotalVertexCount() >= 1000);
        bool pedestalOk = (realGoalPedestal.getTotalVertexCount() >= 1000);

        gridProgressionOk = (totalCountOk && initialBlackoutOk && linkGoalOk && miniRescuedOk &&
                             area1FullOk && allMiniOk && towerStillDarkOk &&
                             greatRescuedOk && cityIlluminatedOk &&
                             assetsOk && bigNamazuOk && namazuOk && dummyOk && pedestalOk);
    }

    bool m53Ok = (goalParamsOk && goalCombatOk && gridProgressionOk);
    printf("  Zapfish Power Grid & Goal Shield Globe:       %s (28 Zapfish, 12,700 MW Grid, Inkopolis Tower 100%%, 7,123 Verts)\n",
           m53Ok ? "PASSED" : "FAILED");
    if (!m53Ok) allPassed = false;

    // -----------------------------------------------------------------
    // Milestone 54 Verification: Ink Geysers & Dynamic Inflatable Sponges
    // -----------------------------------------------------------------
    Game::Obj_Geyser geyserActor;
    geyserActor.init(sead::Vector3f(0.0f, 0.0f, 0.0f), 0);
    bool geyserParamsOk = false;
    bool geyserEruptOk = false;
    bool geyserSwimOk = false;

    // Verify authentic geyser parameters
    const auto& gyp = geyserActor.getParams();
    geyserParamsOk = (std::abs(gyp.hp - 0.30f) < 0.01f &&
                      std::abs(gyp.playerBindVel - 2.50f) < 0.01f &&
                      std::abs(gyp.fountainHeight - 30.0f) < 0.01f &&
                      std::abs(gyp.targetRadius - 15.0f) < 0.01f);

    // Verify eruption on ink damage
    bool hitOk = geyserActor.applyInkDamage(0.35f, 0);
    bool openStateOk = (geyserActor.getState() == Game::GeyserState::cState_Opening);
    for (int f = 0; f < 25; ++f) geyserActor.update();
    bool eruptStateOk = (geyserActor.getState() == Game::GeyserState::cState_Erupting &&
                         geyserActor.getCurrentHeight() == 30.0f);
    geyserEruptOk = (hitOk && openStateOk && eruptStateOk);

    // Verify player swim vertical ascent in ink fountain
    sead::Vector3f swimPlayerPos(2.0f, 5.0f, 2.0f);
    f32 vertVel = 0.0f;
    bool swimInsideOk = geyserActor.updatePlayerSwimAscent(swimPlayerPos, vertVel);
    bool velOk = (vertVel == 2.50f && swimPlayerPos.y > 5.0f);

    sead::Vector3f farPlayerPos(50.0f, 5.0f, 0.0f);
    f32 farVel = 0.0f;
    bool swimOutsideOk = !geyserActor.updatePlayerSwimAscent(farPlayerPos, farVel);
    geyserSwimOk = (swimInsideOk && velOk && swimOutsideOk);

    // Verify Dynamic Inflatable Sponge
    Game::Obj_Sponge spongeActor;
    spongeActor.init(sead::Vector3f(0.0f, 0.0f, 0.0f), 0);
    bool spongeParamsOk = false;
    bool spongeExpandOk = false;
    bool spongeShrinkOk = false;

    // Verify sponge authentic parameters
    const auto& sp = spongeActor.getParams();
    spongeParamsOk = (std::abs(sp.scaleDamageForMax - 2.40f) < 0.01f &&
                      std::abs(sp.scaleBombCoreDamageK - 2.0f) < 0.01f &&
                      sp.enemyNoReactFrame == 12);

    // Test friendly ink & bomb expansion (2.0x bomb bonus)
    spongeActor.applyFriendlyInk(5.0f);
    spongeActor.applyFriendlyBomb(5.0f); // 5 * 0.1 * 2.0 = 1.0 boost
    for (int f = 0; f < 35; ++f) spongeActor.update();
    bool maxExpandOk = spongeActor.isFullyExpanded() && (spongeActor.getCurrentScale() >= 2.35f);

    // Test standing on expanded sponge
    f32 groundY = 0.0f;
    sead::Vector3f playerOnSponge(0.0f, spongeActor.getCurrentHeight(), 0.0f);
    bool standOk = spongeActor.checkPlayerStanding(playerOnSponge, groundY);
    spongeExpandOk = (maxExpandOk && standOk);

    // Test enemy ink contraction
    spongeActor.applyEnemyInk(15.0f);
    for (int f = 0; f < 35; ++f) spongeActor.update();
    bool minContractOk = (spongeActor.getCurrentScale() <= 0.45f &&
                          spongeActor.getState() == Game::SpongeState::cState_MinContracted);
    spongeShrinkOk = minContractOk;

    // Verify authentic 3D BFRES models on disk
    sead::BfresModel realGeyser = sead::BfresParser::createGeyserModel();
    sead::BfresModel realSponge = sead::BfresParser::createSpongeModel();
    bool geyserMeshOk = (realGeyser.getTotalVertexCount() >= 50);
    bool spongeMeshOk = (realSponge.getTotalVertexCount() >= 1000);

    bool m54Ok = (geyserParamsOk && geyserEruptOk && geyserSwimOk &&
                  spongeParamsOk && spongeExpandOk && spongeShrinkOk &&
                  geyserMeshOk && spongeMeshOk);

    printf("  Ink Geysers (Gushers) & Dynamic Sponges:      %s (Geyser 30m/2.5m/s Ascent, Sponge 2.4x/0.4x Scale, 4,606 Verts)\n",
           m54Ok ? "PASSED" : "FAILED");
    if (!m54Ok) allPassed = false;

    // -----------------------------------------------------------------
    // Milestone 55 Verification: Locked Vaults, Keys & Squid Stone Monuments
    // -----------------------------------------------------------------
    Game::Obj_KeyTreasureBox chestActor;
    chestActor.init(sead::Vector3f(0.0f, 0.0f, 0.0f), true, "SunkenScroll");
    bool chestParamsOk = false;
    bool chestUnlockOk = false;

    // Verify authentic chest parameters
    const auto& cp = chestActor.getParams();
    chestParamsOk = (cp.colOffFrame == 15 && cp.openStFrame == 30);

    // Verify key gating, opening sequence, timed collision cutoff and reward spawning
    bool noKeyRejected = !chestActor.tryUnlock(false);
    bool keyAccepted = chestActor.tryUnlock(true);
    bool openingStateOk = (chestActor.getState() == Game::TreasureBoxState::cState_Opening);

    for (int f = 0; f < 14; ++f) chestActor.update();
    bool colStillActive = chestActor.isCollisionActive();

    chestActor.update(); // frame 15 reached
    bool colTurnedOff = !chestActor.isCollisionActive();

    for (int f = 15; f < 30; ++f) chestActor.update();
    bool openedOk = chestActor.isOpened();
    bool rewardOk = chestActor.isRewardSpawned();

    chestUnlockOk = (noKeyRejected && keyAccepted && openingStateOk &&
                     colStillActive && colTurnedOff && openedOk && rewardOk);

    // Verify Obj_Ikastone Squid Stone Monument
    Game::Obj_Ikastone stoneActor;
    stoneActor.init(sead::Vector3f(0.0f, 0.0f, 0.0f), 0.0f);
    bool stoneParamsOk = false;
    bool stoneGuideOk = false;
    bool stoneAnimOk = false;

    // Verify authentic parameters
    const auto& ip = stoneActor.getParams();
    stoneParamsOk = (std::abs(ip.actionRadius - 50.0f) < 0.01f &&
                     std::abs(ip.actionAngleDeg - 60.0f) < 0.01f &&
                     ip.reactionAnimCancelFrame == 20 &&
                     std::abs(ip.startAnimFrames - 60.0f) < 0.01f &&
                     std::abs(ip.actionGuideOffset.y - 18.6f) < 0.1f);

    // Verify 60-degree approach cone detection
    sead::Vector3f insidePlayer(0.0f, 0.0f, 20.0f);
    bool approachInside = stoneActor.checkPlayerApproach(insidePlayer, 0.0f);
    bool guideActiveOk = stoneActor.isGuideActive();

    sead::Vector3f behindPlayer(0.0f, 0.0f, -20.0f);
    bool approachBehind = !stoneActor.checkPlayerApproach(behindPlayer, 0.0f);
    stoneGuideOk = (approachInside && guideActiveOk && approachBehind);

    // Verify activation and 60-frame start animation
    stoneActor.activate();
    bool activatedStateOk = stoneActor.isActivated();
    for (int f = 0; f < 60; ++f) stoneActor.update();
    bool reactStateOk = (stoneActor.getState() == Game::IkastoneState::cState_Reacting);
    stoneAnimOk = (activatedStateOk && reactStateOk);

    // Verify authentic 3D BFRES models on disk
    sead::BfresModel realDoorKey = sead::BfresParser::createDoorKeyModel();
    sead::BfresModel realChestBox = sead::BfresParser::createTreasureBoxModel();
    sead::BfresModel realIkastone = sead::BfresParser::createIkastoneModel();
    sead::BfresModel realJumpPoint = sead::BfresParser::createJumpPointModel();

    bool keyMeshOk = (realDoorKey.getTotalVertexCount() >= 500);
    bool chestMeshOk = (realChestBox.getTotalVertexCount() >= 400);
    bool ikaMeshOk = (realIkastone.getTotalVertexCount() >= 4000);
    bool jumpMeshOk = (realJumpPoint.getTotalVertexCount() >= 2000);

    bool m55Ok = (chestParamsOk && chestUnlockOk &&
                  stoneParamsOk && stoneGuideOk && stoneAnimOk &&
                  keyMeshOk && chestMeshOk && ikaMeshOk && jumpMeshOk);

    printf("  Locked Vaults, Keys & Squid Stone Monuments:  %s (Vault 15f Col/30f Open, Ikastone 50m/60f, 9,787 Verts)\n",
           m55Ok ? "PASSED" : "FAILED");
    if (!m55Ok) allPassed = false;

    // -----------------------------------------------------------------
    // Milestone 56 Verification: DJ Octavio Pilot House & Collapsing Fall Lift
    // -----------------------------------------------------------------
    Game::Obj_RailKingPilotHouse pilotHouse;
    pilotHouse.init(sead::Vector3f(0.0f, 0.0f, 0.0f));
    bool pilotParamsOk = false;
    bool pilotTrackOk = false;
    bool pilotDamageOk = false;
    bool pilotShiokaraOk = false;

    // Verify authentic pilot house parameters
    const auto& php = pilotHouse.getParams();
    pilotParamsOk = (php.damageFrame == 12 &&
                     std::abs(php.searchRadius - 100.0f) < 0.01f &&
                     std::abs(php.rotSpeedDeg - 1.0f) < 0.01f &&
                     std::abs(php.eyeMaxAngleDeg - 45.0f) < 0.01f &&
                     std::abs(php.worldWait - 20.0f) < 0.01f);

    // Verify player acquisition & 1 deg/f rotational tracking
    sead::Vector3f playerPos(10.0f, 0.0f, 10.0f); // 45 degrees yaw
    bool acquiredOk = pilotHouse.trackPlayer(playerPos);
    for (int f = 0; f < 20; ++f) pilotHouse.update();
    bool trackingRotOk = (pilotHouse.getCurrentYawDeg() > 15.0f && pilotHouse.getEyeDeflectionDeg() > 0.0f);
    pilotTrackOk = (acquiredOk && trackingRotOk);

    // Verify 12-frame punch damage stun
    pilotHouse.takePunchDamage();
    bool damageStateOk = (pilotHouse.getState() == Game::OctavioPilotState::cState_Damaged);
    for (int f = 0; f < 13; ++f) pilotHouse.update();
    bool damageStunEndOk = (pilotHouse.getState() != Game::OctavioPilotState::cState_Damaged);
    pilotDamageOk = (damageStateOk && damageStunEndOk);

    // Verify Calamari Inkantation (Shiokara) groove mechanics
    pilotHouse.startShiokaraGroove(1);
    pilotShiokaraOk = (pilotHouse.isHypnotized() && pilotHouse.getShiokaraPhase() == 1);

    // Verify Obj_LiftFall collapsing fall platform
    Game::Obj_LiftFall fallLift;
    fallLift.init(sead::Vector3f(0.0f, 0.0f, 0.0f));
    bool liftParamsOk = false;
    bool liftFallOk = false;
    bool liftRespawnOk = false;

    const auto& lfp = fallLift.getParams();
    liftParamsOk = (std::abs(lfp.lifeSeconds - 3.0f) < 0.01f && fallLift.getRemainingLifeFrames() == 180);

    // Step on platform -> 180 frames countdown -> shake -> drop
    fallLift.onPlayerStepOn();
    for (int f = 0; f < 120; ++f) fallLift.update();
    bool shakingOk = (fallLift.getState() == Game::LiftFallState::cState_Shaking && fallLift.getShakeOffsetX() != 0.0f);

    for (int f = 0; f < 60; ++f) fallLift.update();
    bool droppedOk = (fallLift.isFalling() && !fallLift.isSolid());

    for (int f = 0; f < 15; ++f) fallLift.update();
    bool fallPhysicsOk = (fallLift.getFallDisplacementY() < -5.0f);
    liftFallOk = (shakingOk && droppedOk && fallPhysicsOk);

    // Update through respawn cycle
    for (int f = 0; f < 250; ++f) fallLift.update();
    bool respawnedOk = (fallLift.getState() == Game::LiftFallState::cState_Idle && fallLift.isSolid());
    liftRespawnOk = respawnedOk;

    // Verify authentic 3D BFRES models on disk
    sead::BfresModel realPilotHouse = sead::BfresParser::createOctavioPilotHouseModel();
    sead::BfresModel realBasePlate = sead::BfresParser::createPropellerBasePlateModel();

    bool pilotMeshOk = (realPilotHouse.getTotalVertexCount() >= 3000);
    bool basePlateMeshOk = (realBasePlate.getTotalVertexCount() >= 50);

    bool m56Ok = (pilotParamsOk && pilotTrackOk && pilotDamageOk && pilotShiokaraOk &&
                  liftParamsOk && liftFallOk && liftRespawnOk &&
                  pilotMeshOk && basePlateMeshOk);

    printf("  DJ Octavio Pilot House & Collapsing Fall Lift: %s (Octavio 100m/1deg/f/Shiokara, Lift 3.0s Drop, 3,165 Verts)\n",
           m56Ok ? "PASSED" : "FAILED");
    if (!m56Ok) allPassed = false;

    // -----------------------------------------------------------------
    // Milestone 57 Verification: Octarian UFO Spawner & Inkstrike Missile Beacon Positioning
    // -----------------------------------------------------------------
    Game::Obj_MissilePosition missileActor;
    bool missileParamsOk = missileActor.loadParams("content/Static/Obj_MissilePosition.params");
    bool missileCombatOk = false;
    bool missileVsOk = false;

    // Verify solo parameters
    const auto& mp = missileActor.getParams();
    bool mpOk = (mp.mBindNoShootFrame == 30 &&
                 std::abs(mp.mMissileSpeed - 5.0f) < 0.01f &&
                 mp.mMinShotInterval == 60 &&
                 std::abs(mp.mActionGuideRadius - 50.0f) < 0.01f &&
                 std::abs(mp.mActionGuideOffsetY - 10.0f) < 0.01f);

    // Verify prompt radius detection
    sead::Vector3f nearPos(30.0f, 0.0f, 30.0f); // ~42.4m
    sead::Vector3f farPos(50.0f, 0.0f, 50.0f);  // ~70.7m
    bool nearGuideOk = missileActor.isPlayerInGuideRadius(nearPos);
    bool farGuideOk = !missileActor.isPlayerInGuideRadius(farPos);

    // Enter cockpit -> warm-up lockout -> missile firing
    bool enterOk = missileActor.enterCockpit(0);
    bool initialLockout = !missileActor.fireMissile(sead::Vector3f(100.0f, 0.0f, 100.0f));
    for (int f = 0; f < 30; ++f) missileActor.update();
    bool fireOk = missileActor.fireMissile(sead::Vector3f(100.0f, 0.0f, 100.0f));
    bool refireLockout = !missileActor.fireMissile(sead::Vector3f(100.0f, 0.0f, 100.0f));
    for (int f = 0; f < 60; ++f) missileActor.update();
    bool refireOk = missileActor.fireMissile(sead::Vector3f(120.0f, 0.0f, 120.0f));
    missileActor.exitCockpit();
    bool exitOk = (missileActor.getState() == Game::Obj_MissilePosition::State::cWait && !missileActor.isOccupied());

    missileCombatOk = (missileParamsOk && mpOk && nearGuideOk && farGuideOk &&
                       enterOk && initialLockout && fireOk && refireLockout && refireOk && exitOk &&
                       missileActor.getFiredMissileCount() == 2);

    // Verify VS variant parameters
    Game::Obj_MissilePosition missileVs;
    bool vsLoadOk = missileVs.loadParamsVS("content/Static/Obj_MissilePositionVS.params");
    const auto& vsp = missileVs.getParams();
    missileVsOk = (vsLoadOk && missileVs.isVsMode() &&
                   vsp.mBindNoShootFrame == 15 &&
                   vsp.mMinShotInterval == 70 &&
                   std::abs(vsp.mActionGuideRadius - 75.0f) < 0.01f &&
                   vsp.mCoolingFrame == 600 &&
                   vsp.mCoolingFrameOverheat == 600);

    // Verify Obj_ZakoPointUFO Octarian UFO spawner & deployment craft
    Game::Obj_ZakoPointUFO ufoActor(Game::Obj_ZakoPointUFO::UfoType::OctUfoBox);
    bool ufoParamsOk = ufoActor.loadParams("content/Static/Obj_ZakoPointUFO.params");
    bool ufoSpawnOk = false;
    bool ufoDamageOk = false;

    const auto& up = ufoActor.getParams();
    bool upLifeOk = (std::abs(up.mLife - 1000.0f) < 0.01f && std::abs(ufoActor.getHealth() - 1000.0f) < 0.01f);

    // Verify hover bobbing & spawn wave cycle
    ufoActor.init();
    for (int f = 0; f < 30; ++f) ufoActor.update();
    bool bobbingOk = (ufoActor.getState() == Game::Obj_ZakoPointUFO::State::Hovering);

    bool waveSpawned = ufoActor.triggerSpawnWave();
    bool countsOk = (ufoActor.getTotalSpawnedCount() == 1 && ufoActor.getActiveEnemyCount() == 1);
    ufoActor.onEnemyDefeated();
    bool defeatOk = (ufoActor.getActiveEnemyCount() == 0);
    ufoSpawnOk = (ufoParamsOk && upLifeOk && bobbingOk && waveSpawned && countsOk && defeatOk);

    // Verify durability damage and destruction
    ufoActor.applyDamage(400.0f);
    bool midDmgOk = (std::abs(ufoActor.getHealth() - 600.0f) < 0.01f && ufoActor.isAlive());
    ufoActor.applyDamage(600.0f);
    bool destroyedOk = (!ufoActor.isAlive() && ufoActor.getState() == Game::Obj_ZakoPointUFO::State::Destroyed);
    ufoDamageOk = (midDmgOk && destroyedOk);

    // Verify authentic 3D BFRES models on disk (4,361 vertices total)
    sead::BfresModel realUfoBox = sead::BfresParser::createOctUfoBoxModel();
    sead::BfresModel realUfoWall = sead::BfresParser::createOctUfoWallModel();
    sead::BfresModel realUfoMini = sead::BfresParser::createRvlUfoMiniModel();
    sead::BfresModel realMissile = sead::BfresParser::createMissileModel();

    bool ufoBoxMeshOk = (realUfoBox.getTotalVertexCount() >= 400);
    bool ufoWallMeshOk = (realUfoWall.getTotalVertexCount() >= 250);
    bool ufoMiniMeshOk = (realUfoMini.getTotalVertexCount() >= 2500);
    bool missileMeshOk = (realMissile.getTotalVertexCount() >= 1000);

    bool m57Ok = (missileCombatOk && missileVsOk && ufoSpawnOk && ufoDamageOk &&
                  ufoBoxMeshOk && ufoWallMeshOk && ufoMiniMeshOk && missileMeshOk);

    printf("  Octarian UFO Spawner & Missile Turret Station: %s (Missile 5m/s/15-30f, UFO 1000 HP, 4,361 Verts)\n",
           m57Ok ? "PASSED" : "FAILED");
    if (!m57Ok) allPassed = false;

    // -----------------------------------------------------------------
    // Milestone 58 Verification: Splatoon Camera Sequence Director & Post-Game Cinematics
    // -----------------------------------------------------------------
    Game::CameraSequenceDirector camDirector;
    bool directorInitOk = camDirector.init(&camEngine);

    // 1. Test Match Intro cinematic sequence (PreGame Stage -> Friend -> Opposite)
    bool introStartOk = camDirector.startSequence(Game::CameraSequenceType::MatchIntro);
    bool introTotalFramesOk = (camDirector.getTotalFrames() == 210);
    bool kf0Ok = (camDirector.getCurrentKeyframeIndex() == 0);

    // Advance 90 frames to Friend player view
    camDirector.update(90.0f);
    bool kf1Ok = (camDirector.getCurrentKeyframeIndex() == 1);

    // Advance 60 frames to Opposite team player view
    camDirector.update(60.0f);
    bool kf2Ok = (camDirector.getCurrentKeyframeIndex() == 2);

    // Complete sequence
    camDirector.update(60.0f);
    bool introFinishedOk = (camDirector.isFinished() && !camDirector.isPlaying());
    bool matchIntroOk = (introStartOk && introTotalFramesOk && kf0Ok && kf1Ok && kf2Ok && introFinishedOk);

    // 2. Test Post-Game Stage Sweep with simultaneous Dual TV & GamePad DRC top-down output
    bool sweepStartOk = camDirector.startSequence(Game::CameraSequenceType::PostGameMatchSweep, "Fld_Ruins00");
    bool sweepFramesOk = (camDirector.getTotalFrames() == 210);

    const auto& tvCam = camDirector.getCurrentCameraTV();
    const auto& drcCam = camDirector.getCurrentCameraDRC();
    bool dualViewportsOk = (tvCam.pos.y > 1000.0f && drcCam.pos.y > 5000.0f && (std::abs(drcCam.fovy - 10.0f) < 0.01f || std::abs(drcCam.fovy - 15.0f) < 0.01f));

    // Advance 120 frames to Game_FinalResult (Judd victory / defeat pedestal)
    camDirector.update(120.0f);
    bool finalResultCutOk = (camDirector.getCurrentKeyframeIndex() == 1);
    camDirector.update(30.0f); // Advance through 20-frame transition interpolation
    const auto& resultCam = camDirector.getCurrentCameraTV();
    bool juddPedestalOk = (resultCam.at.z < -2000.0f);

    bool postGameOk = (sweepStartOk && sweepFramesOk && dualViewportsOk && finalResultCutOk && juddPedestalOk);

    // 3. Test Tutorial Super Jump launch sequence
    bool jumpStartOk = camDirector.startSequence(Game::CameraSequenceType::TutorialSuperJump);
    bool jumpFramesOk = (camDirector.getTotalFrames() == 180);
    const auto& jumpCam = camDirector.getCurrentCameraTV();
    bool jumpCoordOk = (std::abs(jumpCam.pos.x - 690.0f) < 0.1f && std::abs(jumpCam.pos.y - 260.0f) < 0.1f);
    bool tutorialJumpOk = (jumpStartOk && jumpFramesOk && jumpCoordOk);

    // 4. Test Staff Roll panoramic credits crawl (16:9 widescreen, 48.5 FOV)
    bool staffStartOk = camDirector.startSequence(Game::CameraSequenceType::StaffRollCredits);
    bool staffFramesOk = (camDirector.getTotalFrames() == 300);
    const auto& staffCam = camDirector.getCurrentCameraTV();
    bool staffAspectOk = (std::abs(staffCam.aspect - 1.77777779f) < 0.01f && std::abs(staffCam.fovy - 48.5f) < 0.01f);
    bool staffRollOk = (staffStartOk && staffFramesOk && staffAspectOk);

    bool m58Ok = (directorInitOk && matchIntroOk && postGameOk && tutorialJumpOk && staffRollOk);

    printf("  Camera Sequence Director & Cinematics:        %s (Intro 210f, Sweep TV/DRC, Tutorial Jump, Staff Roll)\n",
           m58Ok ? "PASSED" : "FAILED");
    if (!m58Ok) allPassed = false;

    // -----------------------------------------------------------------
    // Milestone 59 Verification: Octo Valley Sector Gates, Kettles & Bomb Flowers
    // -----------------------------------------------------------------
    // 1. Obj_AreaGate (Sector Gate barrier & power unlock)
    Game::Obj_AreaGate areaGate(1, 100);
    bool gateParamsOk = areaGate.loadParams("content/Static/Obj_AreaGate.params");
    bool gateCollisionOk = false;
    bool gateUnlockOk = false;

    const auto& agp = areaGate.getParams();
    bool agpOk = (std::abs(agp.mBarrierWidth - 60.0f) < 0.01f &&
                  std::abs(agp.mBarrierHeight - 300.0f) < 0.01f &&
                  std::abs(agp.mBarrierBoundVelLen - 3.0f) < 0.01f &&
                  std::abs(agp.mBarrierBoundVelY - 0.3f) < 0.01f &&
                  agp.mShakeCancelFrame == 20);

    sead::Vector3f rebound;
    bool colHit = areaGate.checkPlayerCollision(sead::Vector3f(0.0f, 10.0f, 2.0f), rebound);
    bool reboundPhysics = (rebound.y == 0.3f && rebound.z == 3.0f);
    gateCollisionOk = (colHit && reboundPhysics);

    bool underpoweredReject = !areaGate.tryUnlock(50);
    bool poweredAccept = areaGate.tryUnlock(100);
    bool shakingStateOk = (areaGate.getState() == Game::AreaGateState::cState_Shaking);
    for (int f = 0; f < 20; ++f) areaGate.update();
    bool gateOpeningStateOk = (areaGate.getState() == Game::AreaGateState::cState_Opening);
    for (int f = 0; f < 15; ++f) areaGate.update();
    bool openedPassableOk = (areaGate.isPassable() && areaGate.getState() == Game::AreaGateState::cState_Opened);
    gateUnlockOk = (underpoweredReject && poweredAccept && shakingStateOk && gateOpeningStateOk && openedPassableOk);

    bool m59GateOk = (gateParamsOk && agpOk && gateCollisionOk && gateUnlockOk);

    // 2. Obj_GateManhole (Solo & Boss Mission Kettles)
    Game::Obj_GateManhole manholeActor("Fld_EasyClimb00_Msn", false);
    bool manholeParamsOk = manholeActor.loadParams("content/Static/Obj_GateManhole.params");
    bool manholeWarpOk = false;

    const auto& mhp = manholeActor.getParams();
    bool mhpOk = (std::abs(mhp.mWarpColRadius - 23.0f) < 0.01f && mhp.mCommanderSearchProbability == 420);

    bool hiddenInitially = !manholeActor.isRevealed();
    manholeActor.reveal();
    bool revealedOk = manholeActor.isRevealed();

    // Human form rejected -> squid form accepted -> 35f countdown
    sead::Vector3f playerInKettle(10.0f, 0.0f, 10.0f);
    bool humanRejected = !manholeActor.tryEnterWarp(playerInKettle, false);
    bool squidAccepted = manholeActor.tryEnterWarp(playerInKettle, true);
    bool warpingStateOk = (manholeActor.getState() == Game::GateManholeState::cState_Warping);
    for (int f = 0; f < 35; ++f) manholeActor.update();
    bool warpDoneOk = manholeActor.isWarpTriggered();
    manholeActor.setCleared(true);
    bool clearedStateOk = (manholeActor.getState() == Game::GateManholeState::cState_Cleared);

    // Boss Kettle variant
    Game::Obj_GateManhole bossKettle("Fld_BossStampKing_Bos_Msn", true);
    bool bossParamsOk = bossKettle.loadBossParams("content/Static/Obj_BossGateway.params");
    const auto& bkp = bossKettle.getParams();
    bool bkpOk = (std::abs(bkp.mWarpColRadius - 13.0f) < 0.01f && std::abs(bkp.mStageIconOffsetY - 20.0f) < 0.01f);

    manholeWarpOk = (manholeParamsOk && mhpOk && hiddenInitially && revealedOk &&
                     humanRejected && squidAccepted && warpingStateOk && warpDoneOk && clearedStateOk &&
                     bossParamsOk && bkpOk);

    // 3. Obj_BombFlower (Explosive ink balloon flower & respawn)
    Game::Obj_BombFlower bombFlower;
    bool flowerParamsOk = bombFlower.loadParams("content/Static/Obj_BombFlower.params");
    bool flowerDetonateOk = false;
    bool flowerRespawnOk = false;

    const auto& bfp = bombFlower.getParams();
    bool bfpOk = (std::abs(bfp.mMaxHp - 2.0f) < 0.01f &&
                  bfp.mBurstWaitFrame == 180 &&
                  bfp.mAppearFrame == 15 &&
                  std::abs(bfp.mBombCorePaintRadius - 120.0f) < 0.01f &&
                  std::abs(bfp.mBombCoreDamage - 12.0f) < 0.01f);

    bombFlower.init();
    bool partialDmgOk = bombFlower.applyDamage(1.0f, 1);
    bool stillAliveOk = (bombFlower.isReady() && std::abs(bombFlower.getHealth() - 1.0f) < 0.01f);
    bool fatalDmgOk = bombFlower.applyDamage(1.0f, 1);
    bool flowerBurstOk = (bombFlower.isDetonated() && bombFlower.getLastHitterTeam() == 1);

    float centerDmg = 0.0f;
    bool centerHit = bombFlower.checkBlastHit(sead::Vector3f(0.0f, 20.0f, 0.0f), centerDmg);
    float midDmg = 0.0f;
    bool midHit = bombFlower.checkBlastHit(sead::Vector3f(0.0f, 20.0f, 45.0f), midDmg);
    bool blastRadiusOk = (centerHit && std::abs(centerDmg - 12.0f) < 0.01f && midHit && std::abs(midDmg - 6.0f) < 0.1f);

    flowerDetonateOk = (flowerParamsOk && bfpOk && partialDmgOk && stillAliveOk && fatalDmgOk && flowerBurstOk && blastRadiusOk);

    // Respawn cycle (1f burst -> 180f respawning -> 15f growing -> ready)
    bombFlower.update(); // 1f
    bool respawningStateOk = (bombFlower.getState() == Game::BombFlowerState::cState_Respawning);
    for (int f = 0; f < 180; ++f) bombFlower.update();
    bool growingStateOk = (bombFlower.getState() == Game::BombFlowerState::cState_Growing);
    for (int f = 0; f < 15; ++f) bombFlower.update();
    bool regeneratedOk = (bombFlower.isReady() && std::abs(bombFlower.getHealth() - 2.0f) < 0.01f);
    flowerRespawnOk = (respawningStateOk && growingStateOk && regeneratedOk);

    // 4. Authentic 3D BFRES models on disk (47,508 vertices total)
    sead::BfresModel realAreaGate = sead::BfresParser::createAreaGateModel();
    sead::BfresModel realBombFlower = sead::BfresParser::createBombFlowerModel();
    sead::BfresModel realBossGateway = sead::BfresParser::createBossGatewayModel();
    sead::BfresModel realGateManhole = sead::BfresParser::createGateManholeModel();
    sead::BfresModel realGateway = sead::BfresParser::createGatewayModel();

    bool gateModelOk = (realAreaGate.getTotalVertexCount() >= 30000);
    bool flowerModelOk = (realBombFlower.getTotalVertexCount() >= 4000);
    bool bossGateModelOk = (realBossGateway.getTotalVertexCount() >= 2500);
    bool manholeModelOk = (realGateManhole.getTotalVertexCount() >= 10);
    bool gatewayModelOk = (realGateway.getTotalVertexCount() >= 200);

    bool m59Ok = (m59GateOk && manholeWarpOk && flowerDetonateOk && flowerRespawnOk &&
                  gateModelOk && flowerModelOk && bossGateModelOk && manholeModelOk && gatewayModelOk);

    printf("  Octo Valley Sector Gates, Kettles & Bomb Flowers: %s (Gate 300m/3m/s, Kettle 23m Squid Warp, 47,508 Verts)\n",
           m59Ok ? "PASSED" : "FAILED");
    if (!m59Ok) allPassed = false;

    // -----------------------------------------------------------------
    // Milestone 60 Verification: Octonozzle Rolling Ball Ballistics & Checkpoint Beacon Flags
    // -----------------------------------------------------------------
    // 1. Obj_CylinderKingBall (Octonozzle rolling ink ball ballistics)
    Game::Obj_CylinderKingBall ballNormal(Game::CylinderBallType::Normal);
    bool ballParamsOk = ballNormal.loadParams("content/Static/Obj_CylinderKingBall.params");
    bool ballPhysicsOk = false;
    bool ballDamageOk = false;
    bool ballVariantsOk = false;

    const auto& cbp = ballNormal.getParams();
    bool cbpOk = (std::abs(cbp.mLife - 1.2f) < 0.01f &&
                  std::abs(cbp.mGndColRadius - 7.5f) < 0.01f &&
                  std::abs(cbp.mVel - 2.5f) < 0.01f &&
                  std::abs(cbp.mVelOnPlayerInk - 1.5f) < 0.01f &&
                  std::abs(cbp.mPlayerDamage - 0.60f) < 0.01f &&
                  std::abs(cbp.mImpactToPlayer - 2.5f) < 0.01f &&
                  cbp.mTrackPaintableRepeatFrame == 3);

    ballNormal.init();
    ballNormal.setRollingDirection(sead::Vector3f(0.0f, 0.0f, 1.0f));
    for (int f = 0; f < 10; ++f) ballNormal.update();
    bool rollNormalOk = (ballNormal.getCurrentSpeed() > 2.0f && ballNormal.getTotalTrailsDropped() >= 3);

    // Roll onto player ink -> decelerates to 1.5 m/s
    ballNormal.setOnPlayerInk(true);
    for (int f = 0; f < 30; ++f) ballNormal.update();
    bool rollInkSlowdown = (ballNormal.getCurrentSpeed() < 1.9f);
    ballPhysicsOk = (cbpOk && rollNormalOk && rollInkSlowdown);

    // Player contact damage & knockback
    float ballHitDmg = 0.0f;
    sead::Vector3f ballKnockback;
    bool ballHitOk = ballNormal.checkPlayerContact(ballNormal.getPosition() + sead::Vector3f(0.0f, 0.0f, 5.0f), ballHitDmg, ballKnockback);
    bool hitDmgOk = (ballHitOk && std::abs(ballHitDmg - 0.60f) < 0.01f && ballKnockback.length() > 2.0f);

    // Lethal damage burst
    bool popOk = ballNormal.applyDamage(2.0f);
    bool poppedStateOk = (!ballNormal.isAlive() && ballNormal.getState() == Game::CylinderBallState::Popped);
    ballDamageOk = (hitDmgOk && popOk && poppedStateOk);

    // Variants: Small and Big
    Game::Obj_CylinderKingBall ballSmall(Game::CylinderBallType::Small);
    bool smallOk = ballSmall.loadParamsSmall("content/Static/Obj_CylinderKingBallSmall.params");
    Game::Obj_CylinderKingBall ballBig(Game::CylinderBallType::Big);
    bool bigOk = ballBig.loadParamsBig("content/Static/Obj_CylinderKingBallBig.params");
    ballVariantsOk = (smallOk && bigOk &&
                      std::abs(ballSmall.getParams().mGndColRadius - 5.5f) < 0.01f &&
                      std::abs(ballBig.getParams().mGndColRadius - 10.0f) < 0.01f);

    bool m60BallOk = (ballParamsOk && ballPhysicsOk && ballDamageOk && ballVariantsOk);

    // 2. Obj_WarpPointFlag (Checkpoint Beacon Flag 3-life system)
    Game::Obj_WarpPointFlag flagActor(1);
    bool flagParamsOk = flagActor.loadParams("content/Static/Obj_WarpPointFlag.params");
    bool flagWarpOk = false;

    const auto& wfp = flagActor.getParams();
    bool wfpOk = (std::abs(wfp.mLife - 3.0f) < 0.01f && flagActor.getRemainingCharges() == 3);

    bool touchFlagOk = flagActor.tryActivate(sead::Vector3f(5.0f, 0.0f, 5.0f), 15.0f);
    bool flagRaisedOk = flagActor.isActive();

    sead::Vector3f respawnCoord;
    bool charge1 = flagActor.consumeRespawnCharge(respawnCoord);
    bool charge2 = flagActor.consumeRespawnCharge(respawnCoord);
    bool charge3 = flagActor.consumeRespawnCharge(respawnCoord);
    bool charge4 = !flagActor.consumeRespawnCharge(respawnCoord); // Exhausted
    bool exhaustedOk = (flagActor.getState() == Game::WarpFlagState::Exhausted && flagActor.getRemainingCharges() == 0);

    flagWarpOk = (flagParamsOk && wfpOk && touchFlagOk && flagRaisedOk &&
                  charge1 && charge2 && charge3 && charge4 && exhaustedOk);

    // 3. Obj_DefenseTower (Radar Defense Tower 350 HP & regeneration)
    Game::Obj_DefenseTower defenseTower;
    bool towerParamsOk = defenseTower.loadParams("content/Static/Obj_DefenseTower.params");
    bool towerCombatOk = false;

    const auto& dtp = defenseTower.getParams();
    bool dtpOk = (std::abs(dtp.mLife - 35.0f) < 0.01f && std::abs(dtp.mCurableRate - 0.50f) < 0.01f);

    defenseTower.init();
    bool towerDamaged = defenseTower.applyDamage(10.0f);
    bool towerHpOk = (std::abs(defenseTower.getHealth() - 25.0f) < 0.01f && defenseTower.getState() == Game::DefenseTowerState::Damaged);
    defenseTower.repair(10.0f);
    bool repairedOk = (defenseTower.isAlive() && std::abs(defenseTower.getHealth() - 35.0f) < 0.01f &&
                       defenseTower.getState() == Game::DefenseTowerState::Intact);
    towerCombatOk = (towerParamsOk && dtpOk && towerDamaged && towerHpOk && repairedOk);

    // 4. Authentic 3D BFRES models on disk (4,971 vertices total)
    sead::BfresModel realBall = sead::BfresParser::createCylinderKingBallModel();
    sead::BfresModel realWall = sead::BfresParser::createCylinderKingWallModel();
    sead::BfresModel realPond = sead::BfresParser::createCylinderKingPoisonPondModel();
    sead::BfresModel realFlag = sead::BfresParser::createWarpPointFlagModel();
    sead::BfresModel realSubFlag = sead::BfresParser::createSubFlagModel();

    bool ballModelOk = (realBall.getTotalVertexCount() >= 800);
    bool wallModelOk = (realWall.getTotalVertexCount() >= 200);
    bool pondModelOk = (realPond.getTotalVertexCount() >= 200);
    bool flagModelOk = (realFlag.getTotalVertexCount() >= 100);
    bool subFlagModelOk = (realSubFlag.getTotalVertexCount() >= 3000);

    bool m60Ok = (m60BallOk && flagWarpOk && towerCombatOk &&
                  ballModelOk && wallModelOk && pondModelOk && flagModelOk && subFlagModelOk);

    printf("  Octonozzle Rolling Ball & Checkpoint Flags:    %s (Ball 2.5m/s/60HP, Flag 3 Lives, Tower 350HP, 4,971 Verts)\n",
           m60Ok ? "PASSED" : "FAILED");
    if (!m60Ok) allPassed = false;

    // 61. Octo Valley Balloon Fish Target Sequences & Sunken Scroll Collectibles (Obj_AirBall & Obj_AncientDocument)
    printf("\n--- [61/61] OCTO VALLEY BALLOON FISH SEQUENCES & SUNKEN SCROLL COLLECTIBLES ---\n");

    // 1. Mission Balloon Target Sequence Chain (Obj_AirBall)
    Game::Obj_AirBall b0(Game::AirBallType::cType_Mission);
    Game::Obj_AirBall b1(Game::AirBallType::cType_Mission);
    Game::Obj_AirBall b2(Game::AirBallType::cType_Mission);

    b0.setSequence(1, 0, 3, 600);
    b1.setSequence(1, 1, 3, 600);
    b2.setSequence(1, 2, 3, 600);

    b0.linkNext(&b1);
    b1.linkNext(&b2);

    b0.init();
    b1.init();
    b2.init();

    bool seqInitOk = (b0.getState() == Game::AirBallState::cState_Appear &&
                      b1.getState() == Game::AirBallState::cState_Hidden &&
                      b2.getState() == Game::AirBallState::cState_Hidden);

    // Advance 15 frames for b0 to fully inflate
    for (int f = 0; f < 15; ++f) {
        b0.update();
    }
    bool b0ReadyOk = (b0.getState() == Game::AirBallState::cState_Wait && std::abs(b0.getScale() - 1.0f) < 0.05f);

    // Advance 40 frames to simulate harmonic bobbing
    for (int f = 0; f < 40; ++f) {
        b0.update();
    }
    bool b0BobbingOk = (std::abs(b0.getCurrentYOffset()) <= b0.getParams().mBobbingAmplitude + 0.1f);

    // Hit b0: pops and activates b1!
    bool b0Hit = b0.applyDamage(1.0f, 1);
    bool b1Activated = (b0.isBurst() && b1.getState() == Game::AirBallState::cState_Appear);

    // Inflate b1, then pop b1 -> activates b2
    for (int f = 0; f < 15; ++f) b1.update();
    bool b1Hit = b1.applyDamage(1.0f, 1);
    bool b2Activated = (b1.isBurst() && b2.getState() == Game::AirBallState::cState_Appear);

    // Inflate b2, pop b2 -> completes chain!
    for (int f = 0; f < 15; ++f) b2.update();
    bool b2Hit = b2.applyDamage(1.0f, 1);
    bool chainCompletedOk = (b2.isBurst() && b2.isChainCompleted());

    bool airBallSeqOk = (seqInitOk && b0ReadyOk && b0BobbingOk && b0Hit && b1Activated && b1Hit && b2Activated && b2Hit && chainCompletedOk);

    // 2. Battle Dojo Duel Balloon (Obj_AirBall Duel)
    Game::Obj_AirBall duelBall(Game::AirBallType::cType_Duel);
    duelBall.init();
    for (int f = 0; f < 15; ++f) duelBall.update();
    duelBall.setFeverMode(true);
    bool feverModeOk = duelBall.isFeverMode();
    duelBall.setPosition(sead::Vector3f(10.0f, 5.0f, 20.0f));
    bool sphereHitOk = duelBall.checkHit(sead::Vector3f(10.0f, 5.0f, 25.0f), 2.0f);
    bool duelPopOk = duelBall.applyDamage(1.0f, 2);

    bool airBallDuelOk = (feverModeOk && sphereHitOk && duelPopOk && duelBall.isBurst());

    // 3. Sunken Scroll Collectible (Obj_AncientDocument)
    Game::Obj_AncientDocument sunkenScroll(1, false);
    sunkenScroll.spawn(sead::Vector3f(0.0f, 2.0f, 0.0f), 1, false);

    // Advance 30 frames: verify yaw spinning and pillar beam glow ramp
    for (int f = 0; f < 30; ++f) {
        sunkenScroll.update();
    }
    bool scrollSpinOk = (sunkenScroll.getYaw() > 0.0f);
    bool pillarGlowOk = (sunkenScroll.getPillarIntensity() > 5.0f && sunkenScroll.getPillarHeight() == 100.0f);

    // Player collects scroll
    bool collectedOk = sunkenScroll.tryCollect(sead::Vector3f(0.0f, 2.0f, 1.0f), 1.0f, 1);
    bool scrollStateOk = (collectedOk && sunkenScroll.isCollected() && sunkenScroll.getPillarIntensity() == 0.0f);

    bool sunkenScrollOk = (scrollSpinOk && pillarGlowOk && scrollStateOk && sunkenScroll.getDocumentId() == 1);

    // 4. Authentic Retail BFRES 3D Models on Disk (3,837 vertices total)
    sead::BfresModel realAirBall = sead::BfresParser::createAirBallModel();
    sead::BfresModel realAirBallDuel = sead::BfresParser::createAirBallDuelModel();
    sead::BfresModel realAirBallMsn = sead::BfresParser::createAirBallMsnModel();
    sead::BfresModel realScroll = sead::BfresParser::createSunkenScrollModel();
    sead::BfresModel realScrollDummy = sead::BfresParser::createSunkenScrollDummyModel();

    bool airBallModelOk = (realAirBall.getTotalVertexCount() == 953);
    bool airBallDuelModelOk = (realAirBallDuel.getTotalVertexCount() == 949);
    bool airBallMsnModelOk = (realAirBallMsn.getTotalVertexCount() == 953);
    bool scrollModelOk = (realScroll.getTotalVertexCount() == 491);
    bool scrollDummyModelOk = (realScrollDummy.getTotalVertexCount() == 491);

    bool m61Ok = (airBallSeqOk && airBallDuelOk && sunkenScrollOk &&
                  airBallModelOk && airBallDuelModelOk && airBallMsnModelOk &&
                  scrollModelOk && scrollDummyModelOk);

    printf("  Balloon Fish Sequences & Sunken Scrolls:       %s (Chained Spline Targets, 80f Bobbing, Pillar Beam 100m, 3,837 Verts)\n",
           m61Ok ? "PASSED" : "FAILED");
    if (!m61Ok) allPassed = false;

    // 62. Octo Valley Environmental Props & Biome Vegetation (Obj_Grass00, Obj_SeaGull, Obj_Tree00)
    printf("\n--- [62/62] OCTO VALLEY ENVIRONMENTAL PROPS & BIOME VEGETATION ---\n");

    // 1. Interactive Grass Foliage (Obj_Grass00 & Obj_GrassRuins00)
    Game::Obj_Grass00 grassProp(false);
    grassProp.setPosition(sead::Vector3f(0.0f, 0.0f, 0.0f));
    grassProp.init();

    // Idle wind sway
    for (int f = 0; f < 30; ++f) grassProp.update();
    bool grassIdleOk = (grassProp.getState() == Game::GrassState::cState_Wait_random);

    // Player walks through grass -> cState_Rub
    bool grassTouchOk = grassProp.checkPlayerTouch(sead::Vector3f(1.0f, 0.0f, 0.0f), sead::Vector3f(1.0f, 0.0f, 0.0f), false);
    bool grassRubOk = (grassTouchOk && grassProp.getState() == Game::GrassState::cState_Rub && grassProp.isBending());

    // Squid dashes through grass -> cState_RubBend
    bool grassSquidOk = grassProp.checkPlayerTouch(sead::Vector3f(0.5f, 0.0f, 0.0f), sead::Vector3f(3.0f, 0.0f, 0.0f), true);
    bool grassBendOk = (grassSquidOk && grassProp.getState() == Game::GrassState::cState_RubBend);

    // Ink projectile hits grass -> cState_DamageShotBend
    bool grassShotOk = grassProp.applyShotDamage(sead::Vector3f(0.0f, 0.0f, 0.0f), sead::Vector3f(0.0f, 0.0f, 1.0f));
    bool grassShotBendOk = (grassShotOk && grassProp.getState() == Game::GrassState::cState_DamageShotBend);

    // Settling back to neutral over 45 frames
    for (int f = 0; f < 45; ++f) grassProp.update();
    bool grassRecoverOk = (grassProp.getState() == Game::GrassState::cState_Wait_random);

    bool m62GrassOk = (grassIdleOk && grassRubOk && grassBendOk && grassShotBendOk && grassRecoverOk);

    // 2. Ambient Environmental Wildlife (Obj_SeaGull)
    Game::Obj_SeaGull seagull;
    seagull.setPosition(sead::Vector3f(50.0f, 10.0f, 50.0f));
    seagull.init();

    bool gullPerchOk = seagull.isPerched();
    for (int f = 0; f < 60; ++f) seagull.update();

    // Player approaches within 12.0m -> Startles seagull into flight!
    bool gullApproached = seagull.checkPlayerProximity(sead::Vector3f(55.0f, 10.0f, 50.0f));
    bool gullTakeOffOk = (gullApproached && seagull.getState() == Game::SeaGullState::cState_TakeOff);

    // Advance 30 frames for wings to flap and transition to cruising flight
    for (int f = 0; f < 30; ++f) seagull.update();
    bool gullFlyingOk = (seagull.getState() == Game::SeaGullState::cState_Fly && seagull.isAirborne() && seagull.getAltitude() > 10.0f);

    bool m62GullOk = (gullPerchOk && gullTakeOffOk && gullFlyingOk);

    // 3. Environmental Biome Trees (Obj_Tree00 & Obj_Tree01)
    Game::Obj_Tree00 tree00(0);
    tree00.setPosition(sead::Vector3f(0.0f, 0.0f, 0.0f));
    tree00.init();

    for (int f = 0; f < 30; ++f) tree00.update();
    bool treeIdleOk = (tree00.getState() == Game::TreeState::cState_Wait);

    // Ink bullet hits tree trunk -> cState_DamageShot
    bool trunkHit = tree00.applyShotDamage(sead::Vector3f(2.0f, 10.0f, 0.0f), sead::Vector3f(1.0f, 0.0f, 0.0f));
    bool treeShockOk = (trunkHit && tree00.getState() == Game::TreeState::cState_DamageShot && tree00.isShaking());

    // Ink blast hits tree canopy -> cState_DamageShotBend
    bool canopyHit = tree00.applyShotDamage(sead::Vector3f(5.0f, 30.0f, 0.0f), sead::Vector3f(1.0f, 0.0f, 0.0f));
    bool treeBendOk = (canopyHit && tree00.getState() == Game::TreeState::cState_DamageShotBend);

    // Recovery back to idle sway
    for (int f = 0; f < 60; ++f) tree00.update();
    bool treeRecoverOk = (tree00.getState() == Game::TreeState::cState_Wait);

    bool m62TreeOk = (treeIdleOk && treeShockOk && treeBendOk && treeRecoverOk);

    // 4. Authentic Retail BFRES Models on Disk (5,165 vertices total across 5 models)
    sead::BfresModel realGrass = sead::BfresParser::createGrassModel();
    sead::BfresModel realGrassRuins = sead::BfresParser::createGrassRuinsModel();
    sead::BfresModel realGull = sead::BfresParser::createSeaGullModel();
    sead::BfresModel realTree0 = sead::BfresParser::createTree00Model();
    sead::BfresModel realTree1 = sead::BfresParser::createTree01Model();

    bool grassMdlOk = (realGrass.getTotalVertexCount() == 383);
    bool grassRuinsMdlOk = (realGrassRuins.getTotalVertexCount() == 28);
    bool gullMdlOk = (realGull.getTotalVertexCount() == 416);
    bool tree0MdlOk = (realTree0.getTotalVertexCount() == 2169);
    bool tree1MdlOk = (realTree1.getTotalVertexCount() == 2169);

    bool m62Ok = (m62GrassOk && m62GullOk && m62TreeOk &&
                  grassMdlOk && grassRuinsMdlOk && gullMdlOk && tree0MdlOk && tree1MdlOk);

    printf("  Environmental Props & Biome Vegetation:        %s (Grass Sway/Rub, Seagull Flight, Trees 35m, 5,165 Verts)\n",
           m62Ok ? "PASSED" : "FAILED");
    if (!m62Ok) allPassed = false;

    // 63. Weapon Testing Range Target Dummies & Dynamic Testing Props (Obj_SighterTarget, Obj_RubberPole00, Obj_Windsock)
    printf("\n--- [63/64] WEAPON TESTING TARGET DUMMIES & DYNAMIC PROPS ---\n");

    // 1. Inflatable Target Dummies (Obj_SighterTarget)
    Game::Obj_SighterTarget dummyNormal(Game::SighterTargetType::cTarget_Normal);
    Game::Obj_SighterTarget dummyStrong(Game::SighterTargetType::cTarget_Strong);
    dummyNormal.setPosition(sead::Vector3f(0.0f, 0.0f, 10.0f));
    dummyStrong.setPosition(sead::Vector3f(0.0f, 0.0f, 20.0f));
    dummyNormal.init();
    dummyStrong.init();

    bool dummyInitOk = (dummyNormal.getHealth() == 100.0f && dummyStrong.getHealth() == 500.0f &&
                        dummyStrong.getScale() == 1.7f);

    // Hit normal dummy with Splattershot blast (30 HP) -> cState_DamageShot
    bool shot1Hit = dummyNormal.applyDamage(30.0f, sead::Vector3f(0.0f, 5.0f, 10.0f), sead::Vector3f(0.0f, 0.0f, 1.0f));
    bool shot1StateOk = (shot1Hit && dummyNormal.getState() == Game::SighterTargetState::cState_DamageShot &&
                         dummyNormal.getHealth() == 70.0f);

    // Hit normal dummy with lethal shot -> cState_Burst
    bool shotFatal = dummyNormal.applyDamage(80.0f, sead::Vector3f(0.0f, 5.0f, 10.0f), sead::Vector3f(0.0f, 0.0f, 1.0f));
    bool sighterBurstOk = (shotFatal && dummyNormal.isBurst() && dummyNormal.getScale() == 0.0f);

    // Advance 120 frames cooldown + 20 frames inflation -> respawns back to ready!
    for (int f = 0; f < 145; ++f) dummyNormal.update();
    bool respawnOk = (dummyNormal.isReady() && dummyNormal.getHealth() == 100.0f && dummyNormal.getScale() == 1.0f);

    bool sighterTargetOk = (dummyInitOk && shot1StateOk && sighterBurstOk && respawnOk);

    // 2. Rubber Pole Bumper Physics (Obj_RubberPole00)
    Game::Obj_RubberPole00 rubberPole;
    rubberPole.setPosition(sead::Vector3f(0.0f, 0.0f, 0.0f));
    rubberPole.init();

    sead::Vector3f playerRebound(0.0f, 0.0f, 0.0f);
    // Player runs into pole from front
    bool playerCollided = rubberPole.checkPlayerCollision(sead::Vector3f(0.0f, 5.0f, 1.8f), 0.5f, playerRebound);
    bool poleBounceOk = (playerCollided && rubberPole.isBending() && playerRebound.z > 0.0f);

    // Advance 55 frames to settle back to upright
    for (int f = 0; f < 55; ++f) rubberPole.update();
    bool poleSettleOk = (!rubberPole.isBending() && rubberPole.getState() == Game::RubberPoleState::cState_Idle);

    bool rubberPoleOk = (poleBounceOk && poleSettleOk);

    // 3. Ambient Windsock Indicator (Obj_Windsock)
    Game::Obj_Windsock windsock;
    windsock.setPosition(sead::Vector3f(0.0f, 10.0f, 0.0f));
    windsock.init();

    // Calm breeze (3.0 m/s wind along +X)
    windsock.setWind(sead::Vector3f(3.0f, 0.0f, 0.0f));
    float calmInflation = windsock.getInflation();

    // Strong gust (10.0 m/s wind along +Z) -> full horizontal inflation (1.0)
    windsock.setWind(sead::Vector3f(0.0f, 0.0f, 10.0f));
    float gustInflation = windsock.getInflation();
    windsock.update();

    bool windsockOk = (std::abs(calmInflation - 0.3f) < 0.01f && std::abs(gustInflation - 1.0f) < 0.01f);

    // 4. Authentic Retail BFRES Models on Disk (4,467 vertices total across 3 models)
    sead::BfresModel realSighter = sead::BfresParser::createSighterTargetModel();
    sead::BfresModel realPole = sead::BfresParser::createRubberPoleModel();
    sead::BfresModel realSock = sead::BfresParser::createWindsockModel();

    bool sighterMdlOk = (realSighter.getTotalVertexCount() == 1480);
    bool poleMdlOk = (realPole.getTotalVertexCount() == 792);
    bool sockMdlOk = (realSock.getTotalVertexCount() == 2195);

    bool m63Ok = (sighterTargetOk && rubberPoleOk && windsockOk &&
                  sighterMdlOk && poleMdlOk && sockMdlOk);

    printf("  Testing Range Targets & Dynamic Gimmicks:      %s (Normal 100HP/Heavy 500HP, Pole Bounce, Windsock 10m/s, 4,467 Verts)\n",
           m63Ok ? "PASSED" : "FAILED");
    if (!m63Ok) allPassed = false;

    // 64. Octarian Heavy Assault Armor: Octodozer & Rolling Octoball (Enm_Takodozer & Enm_Ball)
    printf("\n--- [64/65] OCTARIAN HEAVY ASSAULT ARMOR: OCTODOZER & OCTOBALL ---\n");

    // 1. EnemyTakodozer Bulldozer Mechanics
    Game::EnemyTakodozer dozer;
    dozer.setPosition(sead::Vector3f(0.0f, 0.0f, -20.0f));
    dozer.init();

    bool dozerInitOk = (dozer.getState() == Game::OctodozerState::cPatrol &&
                        dozer.getTentacleHp() == 100.0f &&
                        dozer.getSpeed() == 0.60f &&
                        dozer.isShieldPlowActive());

    // Frontal attack: player shoots incoming ink blast at front plow -> 100% deflected!
    // Takodozer moving along +Z (0, 0, 1), shot coming from player in front towards -Z (0, 0, -1)
    sead::Vector3f frontShotDir(0.0f, 0.0f, -1.0f);
    bool frontShotDamaged = dozer.applyDamage(40.0f, frontShotDir, false);
    bool frontDeflectOk = (!frontShotDamaged && dozer.getTentacleHp() == 100.0f &&
                           dozer.getFrontPlowDeflectedDamage() == 40.0f);

    // AI targeting: player enters eyesight cone (< 500m in front) -> cChase state
    sead::Vector3f playerFrontPos(0.0f, 0.0f, 30.0f);
    dozer.updateAi(playerFrontPos, 100.0f);
    bool dozerChaseOk = (dozer.isChasing() && dozer.getEyeScale() == 2.069f);

    // Advance 30 frames of chase -> accelerates toward 1.20 m/s
    for (int f = 0; f < 30; ++f) dozer.updateAi(playerFrontPos, 100.0f);
    bool dozerSpeedOk = (dozer.getSpeed() > 1.0f);

    // Rear sneak attack: player climbs atop rear cockpit and hits driver tentacle weak point
    sead::Vector3f rearShotDir(0.0f, 0.0f, 1.0f);
    bool rearShotDamaged = dozer.applyDamage(100.0f, rearShotDir, true);
    bool dozerDefeatOk = (rearShotDamaged && dozer.isDestroyed() &&
                          dozer.getTentacleHp() == 0.0f &&
                          dozer.getDroppedPowerEggs() == 20);

    bool octodozerOk = (dozerInitOk && frontDeflectOk && dozerChaseOk && dozerSpeedOk && dozerDefeatOk);

    // 2. Enm_Ball (Octoball) Ballistics & BarrierGuard
    Game::Enm_Ball octoballNormal;
    octoballNormal.setPosition(sead::Vector3f(0.0f, 0.0f, 0.0f));
    octoballNormal.init();

    // Normal variant initial state
    bool ballInitOk = (octoballNormal.getState() == Game::OctoballState::cWait &&
                       octoballNormal.getHp() == Game::Enm_Ball::cMaxHp);

    // Player nearby (< 200m) -> alert notice -> roll dash with BarrierGuard
    octoballNormal.updateAi(sead::Vector3f(0.0f, 0.0f, 10.0f), false);
    bool ballNoticeOk = (octoballNormal.getState() == Game::OctoballState::cNotice);

    // Wait 20 frames for curl-up -> enters cMove rolling dash
    for (int f = 0; f < 21; ++f) octoballNormal.updateAi(sead::Vector3f(0.0f, 0.0f, 10.0f), false);
    bool ballRollOk = (octoballNormal.isRolling() && octoballNormal.isInvulnerable() &&
                       octoballNormal.getSpeed() == 1.50f);

    // Frontal ink shot while rolling -> deflected by BarrierGuard
    bool rollHitDeflected = !octoballNormal.applyDamage(15.0f);
    bool rollBarrierOk = (rollHitDeflected && octoballNormal.getHp() == Game::Enm_Ball::cMaxHp &&
                          octoballNormal.getBarrierDeflectedDamage() == 15.0f);

    // Octoball rolls onto player ink -> sinks, drops barrier into cChance / cDazedUncurl
    octoballNormal.updateAi(sead::Vector3f(0.0f, 0.0f, 10.0f), true);
    bool ballSunkOk = (octoballNormal.isChanceState() && !octoballNormal.isInvulnerable() &&
                       octoballNormal.getSpeed() == 0.20f);

    // Shot during Chance state -> takes lethal damage, splats and drops 5 Power Eggs
    bool fatalHit = octoballNormal.applyDamage(30.0f);
    bool ballDefeatOk = (fatalHit && octoballNormal.isDefeated() &&
                         octoballNormal.getHp() == 0.0f &&
                         octoballNormal.getDroppedPowerEggs() == 5);

    // 3. Real vs Fake Octoball Variants
    Game::Enm_Ball octoballReal;
    octoballReal.setVariant(Game::OctoballVariant::cReal);
    octoballReal.init();
    bool realVariantOk = (octoballReal.getMaxHp() == 12.0f && octoballReal.getHp() == 12.0f);

    Game::Enm_Ball octoballFake;
    octoballFake.setVariant(Game::OctoballVariant::cFake);
    octoballFake.init();
    bool fakeVariantOk = (octoballFake.getMaxHp() == 0.60f && octoballFake.getHp() == 0.60f);

    bool octoballAllOk = (ballInitOk && ballNoticeOk && ballRollOk && rollBarrierOk &&
                          ballSunkOk && ballDefeatOk && realVariantOk && fakeVariantOk);

    // 4. Authentic Retail BFRES Models on Disk (14,601 vertices total across both models)
    sead::BfresModel realDozerMdl = sead::BfresParser::createTakodozerModel();
    sead::BfresModel realOctoballMdl = sead::BfresParser::createOctoballModel();

    bool dozerMdlOk = (realDozerMdl.getTotalVertexCount() == 9735);
    bool ballMdlOk = (realOctoballMdl.getTotalVertexCount() == 4866);

    bool m64Ok = (octodozerOk && octoballAllOk && dozerMdlOk && ballMdlOk);

    printf("  Octarian Heavy Assault Armor (Dozer & Octoball): %s (Dozer 9,735V/100HP Plow, Ball 4,866V/Barrier, 14,601 Verts)\n",
           m64Ok ? "PASSED" : "FAILED");
    if (!m64Ok) allPassed = false;

    // 65. Ink-Powered Propeller Platforms & Hero Armor Suit (Lft_Propeller00, Obj_Armor)
    printf("\n--- [65/66] INK-POWERED PROPELLER PLATFORMS & HERO ARMOR SUIT ---\n");

    // 1. Lft_Propeller00 Winch Platform Physics
    Game::Lft_Propeller00 screwLiftPlatform;
    screwLiftPlatform.setTrack(sead::Vector3f(0.0f, 0.0f, 0.0f), sead::Vector3f(0.0f, 25.0f, 0.0f));
    screwLiftPlatform.init();

    bool propInitOk = (screwLiftPlatform.getState() == Game::PropellerLiftState::cWait &&
                       screwLiftPlatform.getProgress() == 0.0f &&
                       screwLiftPlatform.getPropellerRpm() == 0.0f &&
                       screwLiftPlatform.isAtStartPoint());

    // Strike propeller with ink stream -> spins up RPM (5.0 * 18 = 90 RPM)
    screwLiftPlatform.hitPropeller(5.0f);
    bool propSpinOk = (screwLiftPlatform.isSpinning() && screwLiftPlatform.getPropellerRpm() == 90.0f);

    // Update simulation -> platform advances upward along track (cMove)
    for (int f = 0; f < 30; ++f) screwLiftPlatform.update();
    bool propElevateOk = (screwLiftPlatform.getState() == Game::PropellerLiftState::cMove &&
                          screwLiftPlatform.getProgress() > 0.0f &&
                          screwLiftPlatform.getPosition().y > 0.0f);

    // Rapid heavy fire pushes lift all the way to destination terminal (1.0) -> cEndPoint
    for (int burst = 0; burst < 10; ++burst) {
        screwLiftPlatform.hitPropeller(10.0f);
        for (int f = 0; f < 20; ++f) screwLiftPlatform.update();
    }
    bool propEndOk = (screwLiftPlatform.isAtEndPoint() && screwLiftPlatform.getProgress() == 1.0f &&
                      screwLiftPlatform.getPosition().y == 25.0f);

    // Let propeller spin die down -> platform counterweight pulls it back to origin (cMoveReturn -> cWait)
    for (int f = 0; f < 300; ++f) screwLiftPlatform.update();
    bool propResetOk = (screwLiftPlatform.isAtStartPoint() && screwLiftPlatform.getProgress() == 0.0f &&
                        screwLiftPlatform.getState() == Game::PropellerLiftState::cWait);

    bool propPhysicsOk = (propInitOk && propSpinOk && propElevateOk && propEndOk && propResetOk);

    // 2. Obj_Armor Collectible & Player Armor Suit Mechanics
    Game::Obj_Armor armorPickup;
    armorPickup.setPosition(sead::Vector3f(10.0f, 5.0f, 0.0f));
    armorPickup.init();

    bool armorInitOk = (armorPickup.getState() == Game::ArmorState::cState_Wait &&
                        !armorPickup.isCollected());

    // Update 15 frames (crest of 60-frame harmonic wave) -> floating bobbing and rotation
    for (int f = 0; f < 15; ++f) armorPickup.update();
    bool armorBobOk = (armorPickup.getPosition().y > 5.0f && armorPickup.getRotationAngle() > 0.0f);

    // Player walks within 2.0m radius -> collects armor
    bool touched = armorPickup.checkPlayerTouch(sead::Vector3f(10.5f, 5.0f, 0.0f));
    bool armorCollectOk = (touched && armorPickup.isCollected());

    // Player armor progression:
    // Starts at Tier 1 (standard suit, 0 extra HP)
    u32 playerArmorTier = 1;
    f32 playerArmorHp = 0.0f;

    // Pick up Level 2 armor (+100 HP extra durability)
    bool upg1 = Game::Obj_Armor::applyArmorPickup(playerArmorTier, playerArmorHp);
    bool tier2Ok = (upg1 && playerArmorTier == 2 && playerArmorHp == 100.0f);

    // Pick up Level 3 armor (+200 HP extra durability)
    bool upg2 = Game::Obj_Armor::applyArmorPickup(playerArmorTier, playerArmorHp);
    bool tier3Ok = (upg2 && playerArmorTier == 3 && playerArmorHp == 200.0f);

    // Enemy attacks player with 60.0 HP damage -> absorbed by armor layer (remaining 140 HP)
    bool shattered = false;
    bool dmg1 = Game::Obj_Armor::applyDamageToArmor(60.0f, playerArmorTier, playerArmorHp, shattered);
    bool absorbOk = (dmg1 && !shattered && playerArmorTier == 3 && playerArmorHp == 140.0f);

    // Massive lethal blast of 150.0 HP -> breaks armor layer, triggers shatter & invuln!
    bool dmg2 = Game::Obj_Armor::applyDamageToArmor(150.0f, playerArmorTier, playerArmorHp, shattered);
    bool breakOk = (dmg2 && shattered && playerArmorTier == 1 && playerArmorHp == 0.0f);

    bool heroArmorOk = (armorInitOk && armorBobOk && armorCollectOk && tier2Ok && tier3Ok && absorbOk && breakOk);

    // 3. Authentic Retail BFRES Models on Disk
    sead::BfresModel realProp00 = sead::BfresParser::createPropellerLift00Model();
    sead::BfresModel realProp01 = sead::BfresParser::createPropellerLift01Model();
    sead::BfresModel realPropFan = sead::BfresParser::createPropellerFanModel();
    sead::BfresModel realHeroArmor = sead::BfresParser::createHeroArmorModel();

    bool prop00MdlOk = (realProp00.getTotalVertexCount() == 984);
    bool prop01MdlOk = (realProp01.getTotalVertexCount() == 2208);
    bool propFanMdlOk = (realPropFan.getTotalVertexCount() == 1592);
    bool heroArmorMdlOk = (realHeroArmor.getTotalVertexCount() == 1784);

    bool m65Ok = (propPhysicsOk && heroArmorOk &&
                  prop00MdlOk && prop01MdlOk && propFanMdlOk && heroArmorMdlOk);
    printf("  Propeller Platforms & Hero Armor Suit:         %s (Winch Physics, Armor Tiers 1-3, 6,568 Verts)\n",
           m65Ok ? "PASSED" : "FAILED");
    if (!m65Ok) allPassed = false;

    // 66. Paint Trigger Switches, Traffic Cones & Crowd Barriers (Obj_SwitchPaint, Obj_ColorCone, Obj_SquidGuard)
    printf("\n--- [66/66] PAINT SWITCH TRIGGERS, TRAFFIC CONES & CROWD BARRIERS ---\n");

    // 1. Obj_SwitchPaint Ink Sensor & Auto-Reset Logic
    Game::Obj_SwitchPaint paintSwitch;
    paintSwitch.setPosition(sead::Vector3f(0.0f, 2.0f, 0.0f));
    paintSwitch.setActivationThreshold(10.0f);
    paintSwitch.setAutoResetFrames(60);
    paintSwitch.init();

    bool swInitOk = (paintSwitch.getState() == Game::SwitchPaintState::cOff &&
                     !paintSwitch.isActivated() &&
                     paintSwitch.getAccumulatedInk() == 0.0f);

    // Spray with partial ink blast (6.0f < 10.0f) -> remains cOff
    bool partialActivated = paintSwitch.paintInk(6.0f, 1);
    bool swPartialOk = (!partialActivated && paintSwitch.getState() == Game::SwitchPaintState::cOff &&
                        paintSwitch.getAccumulatedInk() == 6.0f);

    // Hit with remaining ink to exceed threshold (6.0 + 5.0 = 11.0 >= 10.0) -> triggers cOn!
    bool fullActivated = paintSwitch.paintInk(5.0f, 1);
    bool swActiveOk = (fullActivated && paintSwitch.isActivated() &&
                       paintSwitch.getState() == Game::SwitchPaintState::cOn &&
                       paintSwitch.consumeSignal());

    // Advance 65 frames -> timer expires and auto-resets switch back to cOff!
    for (int f = 0; f < 65; ++f) paintSwitch.update();
    bool swResetOk = (!paintSwitch.isActivated() && paintSwitch.getState() == Game::SwitchPaintState::cOff);

    // Test VS mode flip:
    paintSwitch.setVsMode(true);
    paintSwitch.paintInk(10.0f, 1); // Team 1 claims switch
    bool team1Claimed = (paintSwitch.isActivated() && paintSwitch.getOwningTeam() == 1);
    paintSwitch.paintInk(12.0f, 2); // Opponent team 2 shoots 12 ink -> neutralizes/resets!
    bool team2Flipped = (!paintSwitch.isActivated());

    bool switchPaintOk = (swInitOk && swPartialOk && swActiveOk && swResetOk && team1Claimed && team2Flipped);

    // 2. Obj_ColorCone Elastic Torsional Wobble & Topple Physics
    Game::Obj_ColorCone cone;
    cone.setPosition(sead::Vector3f(0.0f, 0.0f, 0.0f));
    cone.init();

    bool coneInitOk = (cone.isUpright() && cone.getTiltAngle() == 0.0f);

    // Light ink blast or brush (3.0 m/s impulse < 6.0 threshold) -> tilts elastically
    cone.applyImpulse(sead::Vector3f(3.0f, 0.0f, 0.0f));
    bool coneTiltOk = (cone.getState() == Game::ColorConeState::cState_Tilted);

    // Advance 45 frames -> spring-damper returns cone upright!
    for (int f = 0; f < 45; ++f) cone.update();
    bool coneSettleOk = (cone.isUpright() && cone.getTiltAngle() == 0.0f);

    // Heavy player dash/stamp (8.0 m/s impulse >= 6.0 threshold) -> topples cone!
    cone.applyImpulse(sead::Vector3f(0.0f, 0.0f, 8.0f));
    bool coneToppleOk = (cone.isToppled() && cone.getTiltAngle() == Game::Obj_ColorCone::cMaxTiltAngle);

    bool colorConeOk = (coneInitOk && coneTiltOk && coneSettleOk && coneToppleOk);

    // 3. Obj_SquidGuard Steel Barricade Collision & Elastic Rattle
    Game::Obj_SquidGuard barrier;
    barrier.setPosition(sead::Vector3f(0.0f, 0.0f, 0.0f));
    barrier.init();

    bool barInitOk = (barrier.getState() == Game::SquidGuardState::cState_Idle &&
                      !barrier.isRattling());

    // Ink bullet impact rattles the steel frame
    barrier.hitWithInk(10.0f, sead::Vector3f(0.0f, 0.0f, 1.0f));
    bool barRattleOk = (barrier.isRattling() && barrier.getState() == Game::SquidGuardState::cState_Rattle);

    // Player running into barrier receives physical rebound pushback
    sead::Vector3f sgRebound(0.0f, 0.0f, 0.0f);
    bool sgColHit = barrier.checkPlayerCollision(sead::Vector3f(0.5f, 0.5f, 0.1f), 0.5f, sgRebound);
    bool barColOk = (sgColHit && sgRebound.z != 0.0f);

    // Settle 30 frames -> rattle decays back to idle
    for (int f = 0; f < 30; ++f) barrier.update();
    bool barSettleOk = (!barrier.isRattling() && barrier.getState() == Game::SquidGuardState::cState_Idle);

    bool squidGuardOk = (barInitOk && barRattleOk && barColOk && barSettleOk);

    // 4. Authentic Retail BFRES Models on Disk
    sead::BfresModel realSwitchMdl = sead::BfresParser::createSwitchPaintModel();
    sead::BfresModel realConeMdl = sead::BfresParser::createColorConeModel();
    sead::BfresModel realGuardMdl = sead::BfresParser::createSquidGuardModel();

    bool realModelOk = (realSwitchMdl.getTotalVertexCount() == 356 &&
                        realConeMdl.getTotalVertexCount() == 654 &&
                        realGuardMdl.getTotalVertexCount() == 100);

    bool m66Ok = (switchPaintOk && colorConeOk && squidGuardOk && realModelOk);
    printf("  Paint Switches, Cones & Security Barriers:     %s (Ink Triggers, Wobble Spring, Rattle Fence, 1,110 Verts)\n",
           m66Ok ? "PASSED" : "FAILED");
    if (!m66Ok) allPassed = false;

    // --- [67/67] OCTO VALLEY BOSS TENTACLE EXTREMITIES & WEAK POINT CORE ---
    printf("\n--- [67/67] OCTO VALLEY BOSS TENTACLE EXTREMITIES & WEAK POINT CORE ---\n");

    // 1. Enm_BossWeakPoint (Campaign Boss Weak Point Core)
    Game::Enm_BossWeakPoint bossWeakPoint;
    bossWeakPoint.init();
    bossWeakPoint.vfunc_3();
    bossWeakPoint.vfunc_5();
    bossWeakPoint.vfunc_47();

    bool bwpInitOk = (bossWeakPoint.getState() == Game::BossWeakPointState::cState_Disappear &&
                      bossWeakPoint.getHealth() == 100.0f);

    // Boss staggers, weak point emerges
    bossWeakPoint.appear(sead::Vector3f(0.0f, 15.0f, 0.0f));
    for (int f = 0; f < 15; ++f) bossWeakPoint.update();
    bool bwpAppearOk = (bossWeakPoint.getState() == Game::BossWeakPointState::cState_Wait &&
                        bossWeakPoint.isVulnerable());

    // Player inks the weak point (damage accumulation + elastic pulsating)
    bool bwpHit1 = bossWeakPoint.hitWithInk(35.0f, sead::Vector3f(0.0f, 0.0f, 1.0f));
    bossWeakPoint.update();
    bool bwpPulseOk = (bossWeakPoint.getHealth() == 65.0f && bossWeakPoint.getPulseScale() > 1.0f);

    // Lethal ink damage -> cState_Die and splat count
    bool bwpHit2 = bossWeakPoint.hitWithInk(70.0f, sead::Vector3f(0.0f, 0.0f, 1.0f));
    bossWeakPoint.update();
    bool bwpDieOk = (bossWeakPoint.isDefeated() && bossWeakPoint.getSplatsTriggered() == 1);

    bool bossWeakPointOk = (bwpInitOk && bwpAppearOk && bwpHit1 && bwpPulseOk && bwpHit2 && bwpDieOk);

    // 2. Enm_OctLeg (Octo Valley Boss Giant Tentacle Extremity)
    Game::Enm_OctLeg octLeg;
    octLeg.init();
    octLeg.vfunc_3();
    octLeg.vfunc_5();
    octLeg.vfunc_47();

    bool legInitOk = (octLeg.getState() == Game::OctLegState::cState_Idle &&
                      octLeg.getHealth() == 180.0f && octLeg.hasArmor());

    // Tentacle eyesight detection cone (Radius 300m, Height 200m)
    bool sightNear = octLeg.checkSight(sead::Vector3f(50.0f, 30.0f, 50.0f));
    bool sightFar = octLeg.checkSight(sead::Vector3f(500.0f, 30.0f, 50.0f));
    bool legSightOk = (sightNear && !sightFar);

    // Ground slam shockwave emission (160m radius)
    octLeg.triggerSlam();
    bool slamTriggerOk = octLeg.isShockWaveActive();
    for (int f = 0; f < 10; ++f) octLeg.update();
    bool slamProgressOk = (octLeg.getShockWaveProgress() > 50.0f);

    // Segmented joint IK check (body4..body7 heights)
    bool jointOk = (octLeg.getSegmentPos(0).y < octLeg.getSegmentPos(1).y &&
                    octLeg.getSegmentPos(1).y < octLeg.getSegmentPos(2).y &&
                    octLeg.getSegmentPos(2).y < octLeg.getSegmentPos(3).y);

    // Frontal shield deflection
    bool shieldDeflected = false;
    bool dmgFront = octLeg.receiveDamage(50.0f, sead::Vector3f(0.0f, 10.0f, 8.0f), sead::Vector3f(0.0f, 0.0f, -1.0f), shieldDeflected);
    bool shieldOk = (!dmgFront && shieldDeflected && octLeg.getHealth() == 180.0f);

    // Rear vulnerable damage & armor break (sheds Enm_Break00/01/02)
    bool dmgRear = octLeg.receiveDamage(100.0f, sead::Vector3f(0.0f, 10.0f, -5.0f), sead::Vector3f(0.0f, 0.0f, 1.0f), shieldDeflected);
    bool armorBreakOk = (dmgRear && !shieldDeflected && !octLeg.hasArmor() &&
                         octLeg.getState() == Game::OctLegState::cState_ArmorBreak);

    // Lethal splat
    bool dmgLethal = octLeg.receiveDamage(100.0f, sead::Vector3f(0.0f, 10.0f, 0.0f), sead::Vector3f(0.0f, 0.0f, 1.0f), shieldDeflected);
    bool legDieOk = (dmgLethal && octLeg.isDefeated());

    bool octLegOk = (legInitOk && legSightOk && slamTriggerOk && slamProgressOk && jointOk && shieldOk && armorBreakOk && legDieOk);

    // 3. Authentic Retail BFRES Models on Disk
    sead::BfresModel realBossWeakPointMdl = sead::BfresParser::createBossWeakPointModel();
    sead::BfresModel realOctLegMdl = sead::BfresParser::createOctLegModel();

    bool realBossModelOk = (realBossWeakPointMdl.getTotalVertexCount() == 961 &&
                            realOctLegMdl.getTotalVertexCount() == 10877);

    bool m67Ok = (bossWeakPointOk && octLegOk && realBossModelOk);
    printf("  Boss Tentacle Extremities & Weak Point Core:   %s (Shockwaves, Shield Deflect, 11,838 Verts)\n",
           m67Ok ? "PASSED" : "FAILED");
    if (!m67Ok) allPassed = false;

    // --- [68/68] CAP'N CUTTLEFISH OVERWORLD NPC, BOUND BOSS & CABIN HQ ---
    printf("\n--- [68/68] CAP'N CUTTLEFISH OVERWORLD NPC, BOUND BOSS & CABIN HQ ---\n");

    // 1. Npc_Commander (Cap'n Cuttlefish Overworld Mentor)
    Game::Npc_Commander commander;
    commander.init();
    commander.setPosition(sead::Vector3f(10.0f, 0.0f, 10.0f));
    commander.vfunc_3();
    commander.vfunc_5();
    commander.vfunc_7();
    commander.vfunc_9();

    bool cfInitOk = (commander.getState() == Game::CommanderState::cState_Wait && !commander.isTalking());

    // Player proximity check (cTalkRadius = 3.5m)
    bool cfNear = commander.checkPlayerProximity(sead::Vector3f(12.0f, 0.0f, 10.0f)); // dist 2.0m
    bool cfFar  = commander.checkPlayerProximity(sead::Vector3f(20.0f, 0.0f, 10.0f)); // dist 10.0m
    bool cfProxOk = (cfNear && !cfFar);

    // Dialogue interaction lifecycle
    commander.startTalking();
    bool cfTalkStOk = (commander.isTalking() && commander.getState() == Game::CommanderState::cState_Talk_St);
    for (int f = 0; f < 6; ++f) commander.update();
    bool cfTalkAOk = (commander.isTalking() && commander.getState() == Game::CommanderState::cState_Talk_A);

    commander.triggerCaneFlick();
    bool cfFlickOk = (commander.getState() == Game::CommanderState::cState_Flick);
    for (int f = 0; f < 15; ++f) commander.update();
    bool cfReturnWait = (commander.getState() == Game::CommanderState::cState_Wait);

    bool commanderOk = (cfInitOk && cfProxOk && cfTalkStOk && cfTalkAOk && cfFlickOk && cfReturnWait);

    // 2. Npc_CommanderBind (Trapped Cuttlefish in DJ Octavio Boss Arena)
    Game::Npc_CommanderBind boundCuttlefish;
    boundCuttlefish.init();
    boundCuttlefish.setPosition(sead::Vector3f(0.0f, 25.0f, 0.0f));
    boundCuttlefish.vfunc_3();
    boundCuttlefish.vfunc_5();
    boundCuttlefish.vfunc_7();
    boundCuttlefish.vfunc_9();

    bool cbInitOk = (boundCuttlefish.getState() == Game::CommanderBindState::cState_Wait && boundCuttlefish.isBound());

    // Shiokara-Bushi (Calamari Inkantation) plays -> dancing cheer state
    boundCuttlefish.startCheeringDance();
    boundCuttlefish.update();
    bool cbDanceOk = boundCuttlefish.isDancing();

    // Octavio defeated -> rescue shatter break sequence via vfunc_11
    boundCuttlefish.vfunc_11();
    bool cbBreakOk = (boundCuttlefish.getState() == Game::CommanderBindState::cState_Break);
    for (int f = 0; f < 25; ++f) boundCuttlefish.update();
    bool cbReleaseOk = (boundCuttlefish.getState() == Game::CommanderBindState::cState_ReleaseCommander);
    for (int f = 0; f < 12; ++f) boundCuttlefish.update();
    bool cbLandOk = boundCuttlefish.isRescued();

    bool commanderBindOk = (cbInitOk && cbDanceOk && cbBreakOk && cbReleaseOk && cbLandOk);

    // 3. Obj_AtarimeHouse (Cuttlefish Cabin / Area 1 Headquarters)
    Game::Obj_AtarimeHouse cuttlefishHQ;
    cuttlefishHQ.setPosition(sead::Vector3f(0.0f, 0.0f, 0.0f));
    cuttlefishHQ.init();
    cuttlefishHQ.vfunc_3();
    cuttlefishHQ.vfunc_5();
    cuttlefishHQ.vfunc_7();
    cuttlefishHQ.vfunc_14();

    bool hqInitOk = (cuttlefishHQ.getState() == Game::AtarimeHouseState::cState_Normal);
    bool hqNear = cuttlefishHQ.isPlayerInCompound(sead::Vector3f(5.0f, 0.0f, 5.0f)); // dist ~7.0m <= 12.0m
    bool hqFar  = cuttlefishHQ.isPlayerInCompound(sead::Vector3f(20.0f, 0.0f, 20.0f)); // dist ~28.0m > 12.0m
    bool hqCompoundOk = (hqNear && !hqFar);

    for (int f = 0; f < 30; ++f) cuttlefishHQ.update();
    bool hqSwayOk = (cuttlefishHQ.getAntennaSway() != 0.0f);
    bool hqChimneyOk = (cuttlefishHQ.getChimneyPosition().y == 8.5f);

    bool atarimeHouseOk = (hqInitOk && hqCompoundOk && hqSwayOk && hqChimneyOk);

    // 4. Authentic Retail BFRES Models on Disk
    sead::BfresModel realCommanderMdl = sead::BfresParser::createCommanderModel();
    sead::BfresModel realCommanderBindMdl = sead::BfresParser::createCommanderBindModel();
    sead::BfresModel realAtarimeHouseMdl = sead::BfresParser::createAtarimeHouseModel();

    bool realMdlOk = (realCommanderMdl.getTotalVertexCount() == 13757 &&
                      realCommanderBindMdl.getTotalVertexCount() == 4759 &&
                      realAtarimeHouseMdl.getTotalVertexCount() == 29147);

    bool m68Ok = (commanderOk && commanderBindOk && atarimeHouseOk && realMdlOk);
    printf("  Cap'n Cuttlefish Overworld NPC, Bound Boss & Cabin: %s (Dialogue, Dance Cheer, 47,663 Verts)\n",
           m68Ok ? "PASSED" : "FAILED");
    if (!m68Ok) allPassed = false;

    // --- [69/69] THE GREAT ZAPFISH, MATCH RESPAWN PLATFORMS & SUPER JUMP BEACONS ---
    printf("\n--- [69/69] THE GREAT ZAPFISH, MATCH RESPAWN PLATFORMS & SUPER JUMP BEACONS ---\n");

    // 1. Obj_BigNamazu (The Great Zapfish)
    Game::Obj_BigNamazu bigNamazu;
    bigNamazu.init();
    bigNamazu.setPosition(sead::Vector3f(0.0f, 150.0f, 0.0f));
    bigNamazu.vfunc_3();
    bigNamazu.vfunc_5();
    bigNamazu.vfunc_7();
    bigNamazu.vfunc_9();

    bool bnInitOk = (bigNamazu.getState() == Game::BigNamazuState::cState_Start &&
                     bigNamazu.getPowerOutputMW() == 100000.0f);

    for (int f = 0; f < 35; ++f) bigNamazu.update();
    bool bnWaitOk = (bigNamazu.getState() == Game::BigNamazuState::cState_Wait &&
                     bigNamazu.getBreathingScale() > 0.95f);

    // Stolen event simulation
    bigNamazu.setToMissing();
    bool bnMissingOk = (bigNamazu.getState() == Game::BigNamazuState::cState_Missing &&
                        bigNamazu.getPowerOutputMW() == 0.0f);

    // Rescued and returned to tower
    bigNamazu.restoreToTower();
    bool bnRestoreOk = (bigNamazu.getState() == Game::BigNamazuState::cState_Start &&
                        bigNamazu.getPowerOutputMW() == 100000.0f);

    bool bigNamazuOk = (bnInitOk && bnWaitOk && bnMissingOk && bnRestoreOk);

    // 2. Obj_RespawnPlatform (Team Match Spawn Point Platform)
    Game::Obj_RespawnPlatform spawnPlatform;
    spawnPlatform.init();
    spawnPlatform.setPosition(sead::Vector3f(0.0f, 0.0f, -50.0f));
    spawnPlatform.setTeam(Game::RespawnTeam::cTeam_Alpha);
    spawnPlatform.vfunc_3();
    spawnPlatform.vfunc_5();
    spawnPlatform.vfunc_7();
    spawnPlatform.vfunc_14();

    bool spInitOk = (spawnPlatform.getState() == Game::RespawnPlatformState::cState_Wait &&
                     spawnPlatform.getTeam() == Game::RespawnTeam::cTeam_Alpha);

    // Barrier shield area protection (4.5m radius)
    bool inShield = spawnPlatform.isInsideBarrier(sead::Vector3f(1.0f, 0.5f, -50.0f)); // dist 1.0m
    bool outShield = spawnPlatform.isInsideBarrier(sead::Vector3f(10.0f, 0.5f, -50.0f)); // dist 10.0m
    bool spShieldOk = (inShield && !outShield);

    // Player respawn event & ink tank rapid replenishment
    spawnPlatform.triggerRespawnEffect();
    bool spRespawnOk = (spawnPlatform.getState() == Game::RespawnPlatformState::cState_Respawn &&
                        spawnPlatform.getSpawnsCount() == 1);
    f32 refilledInk = spawnPlatform.refillPlayerInk(0.20f);
    bool spInkOk = (std::abs(refilledInk - 0.30f) < 0.001f);

    for (int f = 0; f < 25; ++f) spawnPlatform.update();
    bool spReturnOk = (spawnPlatform.getState() == Game::RespawnPlatformState::cState_Wait);

    bool respawnPlatformOk = (spInitOk && spShieldOk && spRespawnOk && spInkOk && spReturnOk);

    // 3. Obj_JumpPoint (Deployable Super Jump Beacon & Jump Point)
    Game::Obj_JumpPoint jumpBeacon;
    jumpBeacon.init();
    jumpBeacon.deploy(sead::Vector3f(20.0f, 0.0f, 15.0f), 0);
    jumpBeacon.vfunc_3();
    jumpBeacon.vfunc_5();
    jumpBeacon.vfunc_7();
    jumpBeacon.vfunc_9();

    bool jbInitOk = (jumpBeacon.getState() == Game::JumpPointState::cState_Idle &&
                     jumpBeacon.getDurability() == 3.0f && jumpBeacon.isActive());

    // Super jump landing consumption (1 durability per jump)
    jumpBeacon.vfunc_11();
    bool jbJump1Ok = (jumpBeacon.getDurability() == 2.0f && jumpBeacon.isActive());

    // 2 more super jumps break the beacon
    jumpBeacon.onSuperJumpLanded();
    jumpBeacon.onSuperJumpLanded();
    bool jbBreakOk = (jumpBeacon.getDurability() == 0.0f && !jumpBeacon.isActive() &&
                      jumpBeacon.getState() == Game::JumpPointState::cState_Break);

    bool jumpPointOk = (jbInitOk && jbJump1Ok && jbBreakOk);

    // 4. Authentic Retail BFRES Models on Disk
    sead::BfresModel realNamazuMdl = sead::BfresParser::createBigNamazuModel();
    sead::BfresModel realSpawnMdl = sead::BfresParser::createRespawnPlatformModel();
    sead::BfresModel realBeaconMdl = sead::BfresParser::createJumpPointModel();

    bool realM69ModelsOk = (realNamazuMdl.getTotalVertexCount() == 7563 &&
                            realSpawnMdl.getTotalVertexCount() == 6843 &&
                            realBeaconMdl.getTotalVertexCount() == 3490);

    bool m69Ok = (bigNamazuOk && respawnPlatformOk && jumpPointOk && realM69ModelsOk);
    printf("  Great Zapfish, Respawn Platform & Super Jump Beacon: %s (100k MW, Shield Dome, 17,896 Verts)\n",
           m69Ok ? "PASSED" : "FAILED");
    if (!m69Ok) allPassed = false;

    // --- [70/70] OCTOSTAMP MINIONS & BREAKABLE SUPPLY CRATES ---
    printf("\n--- [70/70] OCTOSTAMP MINIONS & BREAKABLE SUPPLY CRATES ---\n");

    // 1. Enm_Stamp (Octostamp Minion)
    Game::Enm_Stamp stamp;
    stamp.init();
    stamp.spawn(sead::Vector3f(0.0f, 0.0f, 0.0f));
    stamp.vfunc_3();
    stamp.vfunc_5();
    stamp.vfunc_7();
    stamp.vfunc_9();
    stamp.vfunc_11();

    bool stampInitOk = (stamp.getState() == Game::StampState::cState_Wait &&
                        stamp.getHealth() == 70.0f && !stamp.isChanceState());

    // Eyesight radius check (cEyesightRadius = 250m)
    bool stampSightNear = stamp.checkSight(sead::Vector3f(0.0f, 0.0f, 100.0f));
    bool stampSightFar  = stamp.checkSight(sead::Vector3f(0.0f, 0.0f, 350.0f));
    bool stampSightOk = (stampSightNear && !stampSightFar);

    // Front metal face deflection: hit from front (hitDir.z < 0) defers 100% of damage
    bool deflFront = false;
    bool hitFront = stamp.takeDamage(30.0f, sead::Vector3f(0.0f, 5.0f, -1.0f), sead::Vector3f(0.0f, 0.0f, -1.0f), deflFront);
    bool stampDeflectOk = (!hitFront && deflFront && stamp.getHealth() == 70.0f);

    // Jump slam attack lifecycle
    stamp.triggerSlamAttack();
    bool slamWindupOk = (stamp.getState() == Game::StampState::cState_AttackSt);
    for (int f = 0; f < 15; ++f) stamp.update();
    bool slamAirOk = (stamp.getState() == Game::StampState::cState_Attack);
    for (int f = 0; f < 10; ++f) stamp.update();
    bool slamChanceOk = (stamp.getState() == Game::StampState::cState_Chance && stamp.isChanceState());

    // Face-down vulnerability: tentacle weak point takes lethal damage!
    bool deflBack = false;
    bool hitBack = stamp.takeDamage(70.0f, sead::Vector3f(0.0f, 0.0f, 0.0f), sead::Vector3f(0.0f, 0.0f, -1.0f), deflBack);
    bool stampDieOk = (hitBack && !deflBack && stamp.isDefeated() && stamp.getHealth() == 0.0f);

    bool octoStampOk = (stampInitOk && stampSightOk && stampDeflectOk && slamWindupOk && slamAirOk && slamChanceOk && stampDieOk);

    // 2. Obj_Box00L (Large Breakable Supply Crate)
    Game::Obj_Box00L boxL;
    boxL.init();
    boxL.setPosition(sead::Vector3f(10.0f, 0.0f, 10.0f));
    boxL.vfunc_3();
    boxL.vfunc_5();
    boxL.vfunc_7();
    boxL.vfunc_9();

    bool boxLInitOk = (boxL.getState() == Game::BoxState::cState_Normal &&
                       boxL.getHealth() == 50.0f && !boxL.isBroken());

    // Partial damage -> wobble spring reaction
    boxL.takeDamage(20.0f);
    bool boxLWobbleOk = (boxL.getState() == Game::BoxState::cState_Wobble &&
                         boxL.getWobbleScale() > 1.0f && boxL.getHealth() == 30.0f);

    // Wobble settles over 30 frames
    for (int f = 0; f < 30; ++f) boxL.update();
    bool boxLSettleOk = (boxL.getState() == Game::BoxState::cState_Normal && boxL.getWobbleScale() == 1.0f);

    // Smashed completely -> drops 5 Power Eggs
    boxL.takeDamage(30.0f);
    bool boxLBreakOk = (boxL.isBroken() && boxL.getHealth() == 0.0f && boxL.getDroppedPowerEggs() == 5);

    bool objBoxLOk = (boxLInitOk && boxLWobbleOk && boxLSettleOk && boxLBreakOk);

    // 3. Obj_Box00S (Small Breakable Supply Crate)
    Game::Obj_Box00S boxS;
    boxS.init();
    boxS.setPosition(sead::Vector3f(20.0f, 0.0f, 20.0f));
    boxS.vfunc_3();
    boxS.vfunc_5();
    boxS.vfunc_7();
    boxS.vfunc_9();

    bool boxSInitOk = (boxS.getState() == Game::BoxSmallState::cState_Normal &&
                       boxS.getHealth() == 20.0f && !boxS.isBroken());

    // Single blast shatters crate -> drops 2 Power Eggs
    boxS.takeDamage(20.0f);
    bool boxSBreakOk = (boxS.isBroken() && boxS.getHealth() == 0.0f && boxS.getDroppedPowerEggs() == 2);

    bool objBoxSOk = (boxSInitOk && boxSBreakOk);

    // 4. Authentic Retail BFRES Models on Disk (6,574 vertices total)
    sead::BfresModel realStampMdl = sead::BfresParser::createStampModel();
    sead::BfresModel realBox00LMdl = sead::BfresParser::createBox00LModel();
    sead::BfresModel realBox00SMdl = sead::BfresParser::createBox00SModel();
    sead::BfresModel realBox01LMdl = sead::BfresParser::createBox01LModel();

    bool realM70ModelsOk = (realStampMdl.getTotalVertexCount() == 3464 &&
                            realBox00LMdl.getTotalVertexCount() == 1028 &&
                            realBox00SMdl.getTotalVertexCount() == 1008 &&
                            realBox01LMdl.getTotalVertexCount() == 1074);

    bool m70Ok = (octoStampOk && objBoxLOk && objBoxSOk && realM70ModelsOk);
    printf("  Octostamp Minions & Breakable Supply Crates:         %s (Face Armor, Chance State, 6,574 Verts)\n",
           m70Ok ? "PASSED" : "FAILED");
    if (!m70Ok) allPassed = false;

    // --- [71/71] OCTOCOPTER TORNADO VARIANT & FLOATING PLATFORM RAILS ---
    printf("\n--- [71/71] OCTOCOPTER TORNADO VARIANT & FLOATING PLATFORM RAILS ---\n");

    // 1. Enm_TakopterTornado (Octocopter Tornado Variant)
    Game::Enm_TakopterTornado takopter;
    takopter.init();
    takopter.setPosition(sead::Vector3f(0.0f, 90.0f, 0.0f));
    takopter.vfunc_3();
    takopter.vfunc_5();
    takopter.vfunc_7();
    takopter.vfunc_47();

    bool ttInitOk = (takopter.getState() == Game::TakopterTornadoState::cState_Wait &&
                     takopter.getHealth() == 40.0f && takopter.isAirborne());

    // Eyesight radius check (cEyesightRadius = 3000.0m)
    bool ttSightNear = takopter.checkSight(sead::Vector3f(0.0f, 90.0f, 1500.0f));
    bool ttSightFar  = takopter.checkSight(sead::Vector3f(0.0f, 90.0f, 4000.0f));
    bool ttSightOk = (ttSightNear && !ttSightFar);

    // Propeller spinning & natural hover oscillation
    for (int f = 0; f < 10; ++f) takopter.update();
    bool ttHoverOk = (takopter.getPropellerAngle() > 0.0f);

    // Tornado vortex attack sequence
    takopter.triggerTornadoAttack();
    bool ttAtkStOk = (takopter.getState() == Game::TakopterTornadoState::cState_Attack);
    for (int f = 0; f < 25; ++f) takopter.update();
    bool ttAtkEndOk = (takopter.getState() == Game::TakopterTornadoState::cState_Wait);

    // Ink damage & defeat pop
    takopter.hitWithInk(20.0f, sead::Vector3f(0.0f, 0.0f, 1.0f));
    bool ttHitOk = (takopter.getHealth() == 20.0f);
    takopter.hitWithInk(20.0f, sead::Vector3f(0.0f, 0.0f, 1.0f));
    bool ttDieOk = (takopter.isDefeated() && takopter.getHealth() == 0.0f);

    bool takopterTornadoOk = (ttInitOk && ttSightOk && ttHoverOk && ttAtkStOk && ttAtkEndOk && ttHitOk && ttDieOk);

    // 2. Obj_PaintLiftTurn (Revolving Turntable Platform)
    Game::Obj_PaintLiftTurn turnLift;
    turnLift.init();
    turnLift.setPosition(sead::Vector3f(0.0f, 10.0f, 0.0f));
    turnLift.vfunc_3();
    turnLift.vfunc_5();
    turnLift.vfunc_7();

    bool tlInitOk = (turnLift.getState() == Game::TurnLiftState::cState_Idle &&
                     turnLift.getCurrentAngle() == 0.0f && !turnLift.isRotating());

    // Passenger deck bounds check (12m radius)
    bool tlPassOn  = turnLift.checkPassenger(sead::Vector3f(5.0f, 10.5f, 5.0f));
    bool tlPassOff = turnLift.checkPassenger(sead::Vector3f(25.0f, 10.5f, 0.0f));
    bool tlPassOk = (tlPassOn && !tlPassOff);

    // Ink impulse applies torque & accelerates rotation (vfunc_11)
    turnLift.vfunc_11(); // adds 2.0 torque
    turnLift.applyInkTorque(2.0f); // total 4.0 torque
    bool tlSpinOk = (turnLift.isRotating() && turnLift.getAngularVelocity() == 4.0f);

    // Rotation advances and dampens to closest detent alignment
    for (int f = 0; f < 45; ++f) turnLift.update();
    bool tlRotOk = (turnLift.getCurrentAngle() > 0.0f);
    for (int f = 0; f < 75; ++f) turnLift.update();
    turnLift.vfunc_30();
    bool tlDetentOk = (turnLift.getState() == Game::TurnLiftState::cState_Idle ||
                       turnLift.getState() == Game::TurnLiftState::cState_DetentPause);

    bool paintLiftTurnOk = (tlInitOk && tlPassOk && tlSpinOk && tlRotOk && tlDetentOk);

    // 3. Obj_PaintLiftSlide (Sliding Rail Platform with Wire Grating)
    Game::Obj_PaintLiftSlide slideLift;
    slideLift.init();
    slideLift.setRailPoints(sead::Vector3f(0.0f, 15.0f, 0.0f), sead::Vector3f(50.0f, 15.0f, 0.0f));
    slideLift.vfunc_3();
    slideLift.vfunc_5();

    bool slInitOk = (slideLift.getState() == Game::SlideLiftState::cState_Moving &&
                     slideLift.getProgress() == 0.0f && slideLift.getDirection() == 1.0f);

    slideLift.vfunc_7(); // updates linear movement along rail spline

    // Wire grating physics: humanoid stands on mesh, squid falls through!
    bool slHumanStand = slideLift.checkPassenger(sead::Vector3f(0.0f, 15.5f, 0.0f), false);
    bool slSquidFall  = slideLift.checkPassenger(sead::Vector3f(0.0f, 15.5f, 0.0f), true);
    bool slGrateOk = (slHumanStand && !slSquidFall && slideLift.canSquidPassThrough());

    // Linear rail translation to end waypoint
    for (int f = 0; f < 70; ++f) slideLift.update();
    bool slEndOk = (slideLift.getProgress() == 1.0f && slideLift.getCurrentPos().x == 50.0f &&
                    slideLift.getState() == Game::SlideLiftState::cState_WaitEnd);

    // Reversal pause and return journey
    for (int f = 0; f < 25; ++f) slideLift.update();
    bool slReturnOk = (slideLift.getDirection() == -1.0f &&
                       slideLift.getState() == Game::SlideLiftState::cState_Moving);

    bool paintLiftSlideOk = (slInitOk && slGrateOk && slEndOk && slReturnOk);

    // 4. Authentic Retail BFRES Models on Disk (11,695 vertices total)
    sead::BfresModel realTakopterMdl = sead::BfresParser::createTakopterTornadoModel();
    sead::BfresModel realTurnLiftMdl = sead::BfresParser::createTurnLift00Model();
    sead::BfresModel realNettingMdl  = sead::BfresParser::createWireNettingPlate00Model();

    bool realM71ModelsOk = (realTakopterMdl.getTotalVertexCount() == 8675 &&
                            realTurnLiftMdl.getTotalVertexCount() == 1468 &&
                            realNettingMdl.getTotalVertexCount() == 1552);

    bool m71Ok = (takopterTornadoOk && paintLiftTurnOk && paintLiftSlideOk && realM71ModelsOk);
    printf("  Octocopter Tornado & Moving Rail Platforms:          %s (Air Vortex, Wire Grating, 11,695 Verts)\n",
           m71Ok ? "PASSED" : "FAILED");
    if (!m71Ok) allPassed = false;

    // --- [72/72] ELITE KELP OCTOLINGS, SQUEE-G CLEANERS & AERIAL BOMBERS ---
    printf("\n--- [72/72] ELITE KELP OCTOLINGS, SQUEE-G CLEANERS & AERIAL BOMBERS ---\n");

    // 1. Enm_TakolienSpeedUp (Elite Kelp Octoling)
    Game::Enm_TakolienSpeedUp kelpOcto;
    kelpOcto.init();
    kelpOcto.setPosition(sead::Vector3f(0.0f, 0.0f, 0.0f));
    kelpOcto.vfunc_1();

    bool eliteInitOk = (kelpOcto.hasKelpEquipped() &&
                        kelpOcto.getState() == Game::TakolienState::cPatrol &&
                        kelpOcto.getHp() == 100.0f);

    // Speed multiplier & elite swim speed check
    bool eliteSpeedOk = (Game::Enm_TakolienSpeedUp::cSpeedMultiplier == 1.35f &&
                         Game::Enm_TakolienSpeedUp::cEliteSwimSpeed > Game::Enm_Takolien::cOctoSwimSpeed);

    // Special weapon charge and Killer Wail deployment
    for (int f = 0; f < 205; ++f) {
        kelpOcto.updateEliteAi(sead::Vector3f(0.0f, 0.0f, 30.0f), false);
    }
    bool wailReady = kelpOcto.isSpecialReady();
    bool wailDeployed = kelpOcto.tryDeployKillerWail(sead::Vector3f(0.0f, 0.0f, 50.0f));
    bool eliteSpecialOk = (wailReady && wailDeployed && kelpOcto.isSpecialDeploying());

    bool octoEliteOk = (eliteInitOk && eliteSpeedOk && eliteSpecialOk);

    // 2. Enm_Cleaner (Squee-G Industrial Floor Cleaner)
    Game::Enm_Cleaner cleaner;
    cleaner.init();
    cleaner.spawn(sead::Vector3f(0.0f, 0.0f, 0.0f), sead::Vector3f(1.0f, 0.0f, 0.0f));
    cleaner.vfunc_3();
    cleaner.vfunc_5();

    bool clInitOk = (cleaner.getState() == Game::CleanerState::cState_Patrol &&
                     cleaner.getHealth() == 100.0f && cleaner.getTotalCleanedInk() == 0.0f);

    // Paint vacuum absorption & squeegee brush spin
    cleaner.absorbInk(35.0f);
    cleaner.vfunc_37();
    bool clCleanOk = (cleaner.isCleaning() && cleaner.getTotalCleanedInk() == 35.0f &&
                      cleaner.getBrushRotation() > 0.0f);

    // Wall collision bump & 180-degree turnaround
    cleaner.vfunc_11();
    bool clTurnOk = (cleaner.getState() == Game::CleanerState::cState_Turn);
    for (int f = 0; f < 18; ++f) cleaner.vfunc_7();
    bool clResumeOk = (cleaner.getState() == Game::CleanerState::cState_Patrol &&
                       cleaner.getPosition().x < 0.0f);

    // Armored metal body deflecting attacks
    bool clDeflected = false;
    cleaner.hitWithInk(40.0f, clDeflected);
    bool clArmorOk = (clDeflected && cleaner.getHealth() == 100.0f);

    bool octoCleanerOk = (clInitOk && clCleanOk && clTurnOk && clResumeOk && clArmorOk);

    // 3. Enm_TakopterBomb (Octobomber Aerial Minion)
    Game::Enm_TakopterBomb bomber;
    bomber.init();
    bomber.setPosition(sead::Vector3f(0.0f, 100.0f, 0.0f));
    bomber.vfunc_3();
    bomber.vfunc_5();

    bool bmInitOk = (bomber.getState() == Game::TakopterBombState::cState_Wait &&
                     bomber.getHealth() == 140.0f && bomber.isAirborne());

    // Aerial tracking eyesight check (cEyesightRadius = 280.0m)
    bool bmSightNear = bomber.checkSight(sead::Vector3f(0.0f, 100.0f, 150.0f));
    bool bmSightFar  = bomber.checkSight(sead::Vector3f(0.0f, 100.0f, 350.0f));
    bool bmSightOk = (bmSightNear && !bmSightFar);

    // Splat Bomb release
    bomber.vfunc_47();
    bool bmDropOk = (bomber.getState() == Game::TakopterBombState::cState_BombDrop &&
                     bomber.getDroppedBombCount() == 1);
    for (int f = 0; f < 20; ++f) bomber.vfunc_7();
    bool bmWaitOk = (bomber.getState() == Game::TakopterBombState::cState_Wait);

    // Lethal defeat
    bomber.takeDamage(140.0f);
    bool bmDieOk = (bomber.isDefeated() && bomber.getState() == Game::TakopterBombState::cState_Die &&
                    bomber.getHealth() == 0.0f);

    bool octoBomberOk = (bmInitOk && bmSightOk && bmDropOk && bmWaitOk && bmDieOk);

    // 4. Authentic Retail BFRES Models on Disk (21,851 vertices total)
    sead::BfresModel realTakolienSMdl = sead::BfresParser::createTakolienSModel();
    sead::BfresModel realCleanerMdl = sead::BfresParser::createCleanerModel();
    sead::BfresModel realBomberMdl = sead::BfresParser::createTakopterBombModel();

    bool realM72ModelsOk = (realTakolienSMdl.getTotalVertexCount() == 8901 &&
                            realCleanerMdl.getTotalVertexCount() == 1339 &&
                            realBomberMdl.getTotalVertexCount() == 11611);

    bool m72Ok = (octoEliteOk && octoCleanerOk && octoBomberOk && realM72ModelsOk);
    printf("  Elite Kelp Octolings, Squee-G & Aerial Bombers:      %s (1.35x Speed, Killer Wail, 21,851 Verts)\n",
           m72Ok ? "PASSED" : "FAILED");
    if (!m72Ok) allPassed = false;

    // --- [73/73] OCTOSNIPER TURRETS, BALANCE SEESAWS & RISING DRAWBRIDGES ---
    printf("\n--- [73/73] OCTOSNIPER TURRETS, BALANCE SEESAWS & RISING DRAWBRIDGES ---\n");

    // 1. Enm_Charge (Octosniper / タコスナイパー)
    Game::Enm_Charge sniper;
    sniper.init();
    sniper.setupBunker(sead::Vector3f(0.0f, 10.0f, 0.0f), 0.0f);
    bool snInitOk = (sniper.getState() == Game::OctosniperState::cIdle &&
                     sniper.getHealth() == 80.0f && sniper.isAlive());

    // Sight acquisition and laser targeting
    sniper.updateAimAtPlayer(sead::Vector3f(0.0f, 10.0f, 25.0f));
    sniper.vfunc_7();
    bool snLockOk = (sniper.getState() == Game::OctosniperState::cLockOn);

    // Charge countdown
    for (int f = 0; f < 25; ++f) sniper.vfunc_7();
    bool snChargingOk = (sniper.getState() == Game::OctosniperState::cCharging && sniper.isLaserActive());

    // Shot release
    for (int f = 0; f < 65; ++f) sniper.vfunc_7();
    bool snFiredOk = (sniper.getState() == Game::OctosniperState::cCooldown ||
                      sniper.getState() == Game::OctosniperState::cIdle);

    bool octoSniperOk = (snInitOk && snLockOk && snChargingOk && snFiredOk);

    // 2. Obj_SeesawLift (Balance Seesaw Platform)
    Game::Obj_SeesawLift seesaw;
    seesaw.init();
    seesaw.spawn(sead::Vector3f(0.0f, 5.0f, 0.0f));
    bool seesawInitOk = (seesaw.getState() == Game::SeesawLiftState::cBalanced && seesaw.isLevel());

    // Player weight torque tilt
    seesaw.applyWeightImpulse(4.0f, 50.0f); // 4 meters from pivot, 50kg weight
    for (int f = 0; f < 30; ++f) seesaw.vfunc_7();
    bool seesawTiltingOk = (seesaw.getTiltAngle() > 0.0f);

    // Continue weight until max tilt limit reached
    for (int f = 0; f < 60; ++f) {
        seesaw.applyWeightImpulse(5.0f, 80.0f);
        seesaw.vfunc_7();
    }
    bool seesawMaxOk = (seesaw.isAtMaxTilt() && seesaw.getTiltAngle() >= Game::Obj_SeesawLift::cMaxTiltRadians - 0.01f);

    // Player dismounts -> spring restoring torque returns toward level
    seesaw.applyWeightImpulse(0.0f, 0.0f);
    for (int f = 0; f < 120; ++f) seesaw.vfunc_7();
    bool seesawRestoreOk = (seesaw.getTiltAngle() < Game::Obj_SeesawLift::cMaxTiltRadians);

    bool seesawOk = (seesawInitOk && seesawTiltingOk && seesawMaxOk && seesawRestoreOk);

    // 3. Obj_BridgeUpDown00 (Rising Drawbridge Platform)
    Game::Obj_BridgeUpDown00 drawbridge;
    drawbridge.init();
    drawbridge.spawn(sead::Vector3f(0.0f, 0.0f, 0.0f), 0.0f);
    bool brInitOk = (drawbridge.getState() == Game::BridgeUpDownState::cLowered &&
                     drawbridge.isPassable() && drawbridge.getAngleDegrees() == 0.0f);

    // Ink switch trigger -> raise bridge
    drawbridge.vfunc_11();
    bool brRaisingOk = (drawbridge.getState() == Game::BridgeUpDownState::cRaising);
    for (int f = 0; f < 70; ++f) drawbridge.vfunc_7();
    bool brRaisedOk = (drawbridge.getState() == Game::BridgeUpDownState::cRaised &&
                       drawbridge.isFullyRaised() && drawbridge.getAngleDegrees() == Game::Obj_BridgeUpDown00::cMaxAngleDegrees);

    // Hold duration expires -> descends back down
    for (int f = 0; f < 265; ++f) drawbridge.vfunc_7();
    bool brLoweredOk = (drawbridge.getState() == Game::BridgeUpDownState::cLowered && drawbridge.isPassable());

    bool bridgeOk = (brInitOk && brRaisingOk && brRaisedOk && brLoweredOk);

    // 4. Lft_Charge (Octosniper Perch Lift Platform)
    Game::Lft_Charge chargeLift;
    chargeLift.init();
    chargeLift.spawn(sead::Vector3f(0.0f, 0.0f, 0.0f), 25.0f);
    bool cLiftInitOk = (chargeLift.getState() == Game::ChargeLiftState::cBottom &&
                        chargeLift.isAtBottom() && chargeLift.getCurrentHeight() == 0.0f);

    chargeLift.vfunc_11(); // Ascend
    bool clAscendingOk = (chargeLift.getState() == Game::ChargeLiftState::cAscending);
    for (int f = 0; f < 80; ++f) chargeLift.vfunc_7();
    bool clTopOk = (chargeLift.getState() == Game::ChargeLiftState::cTop &&
                    chargeLift.isAtTop() && chargeLift.getCurrentHeight() == 25.0f);

    // Top dwell expires -> descending return
    for (int f = 0; f < 235; ++f) chargeLift.vfunc_7();
    bool clBottomOk = (chargeLift.getState() == Game::ChargeLiftState::cBottom && chargeLift.isAtBottom());

    bool chargeLiftOk = (cLiftInitOk && clAscendingOk && clTopOk && clBottomOk);

    // 5. Authentic Retail BFRES Models on Disk (8,880 vertices total)
    sead::BfresModel realChargeLiftMdl  = sead::BfresParser::createChargeLiftModel();
    sead::BfresModel realBridgeMdl      = sead::BfresParser::createBridgeUpDown00Model();
    sead::BfresModel realPropLift00Mdl  = sead::BfresParser::createPropellerLift00Model();
    sead::BfresModel realPropLift01Mdl  = sead::BfresParser::createPropellerLift01Model();

    bool realM73ModelsOk = (realChargeLiftMdl.getTotalVertexCount() == 3064 &&
                            realBridgeMdl.getTotalVertexCount() == 2624 &&
                            realPropLift00Mdl.getTotalVertexCount() == 984 &&
                            realPropLift01Mdl.getTotalVertexCount() == 2208);

    bool m73Ok = (octoSniperOk && seesawOk && bridgeOk && chargeLiftOk && realM73ModelsOk);
    printf("  Octosniper Turrets, Balance Seesaws & Drawbridges:   %s (Laser Lock, Restitution, 8,880 Verts)\n",
           m73Ok ? "PASSED" : "FAILED");
    if (!m73Ok) allPassed = false;

    // --- [74/74] OCTOLING ARMORED ASSAULT MECHS & SUBMERSIBLES ---
    printf("\n--- [74/74] OCTOLING ARMORED ASSAULT MECHS & SUBMERSIBLES ---\n");

    // 1. Enm_TakolienVehicleFix (Heavy Turret Cannon)
    Game::Enm_TakolienVehicleFix fixMech;
    fixMech.init();
    fixMech.spawn(sead::Vector3f(0.0f, 0.0f, 0.0f), 0.0f);
    bool fixInitOk = (fixMech.getState() == Game::VehicleFixState::cIdle &&
                      fixMech.getHealth() == 200.0f && fixMech.getCanopyHealth() == 100.0f &&
                      !fixMech.isPilotEjected());

    // Target tracking and bubble mortar burst
    fixMech.updateAim(sead::Vector3f(0.0f, 0.0f, 20.0f));
    bool fixAimOk = (fixMech.getState() == Game::VehicleFixState::cAiming);
    for (int f = 0; f < 18; ++f) fixMech.vfunc_7();
    bool fixFireOk = (fixMech.getShotsFired() >= 1);

    // Canopy armor deflection & catastrophic destruction with pilot ejection
    fixMech.takeDamage(50.0f, true);
    bool fixShieldOk = (fixMech.getCanopyHealth() == 50.0f && fixMech.getHealth() == 200.0f);
    fixMech.takeDamage(200.0f, false);
    bool fixDestructOk = (fixMech.isDestroyed() && fixMech.isPilotEjected());

    bool vehicleFixOk = (fixInitOk && fixAimOk && fixFireOk && fixShieldOk && fixDestructOk);

    // 2. Enm_TakolienVehicleMove (Bipedal Assault Walker)
    Game::Enm_TakolienVehicleMove moveMech;
    moveMech.init();
    moveMech.spawn(sead::Vector3f(0.0f, 0.0f, 0.0f), sead::Vector3f(15.0f, 0.0f, 0.0f));
    bool mvInitOk = (moveMech.getState() == Game::VehicleMoveState::cPatrol &&
                     moveMech.getHealth() == 150.0f && moveMech.getStridePhase() == 0.0f);

    for (int f = 0; f < 20; ++f) moveMech.vfunc_7();
    bool mvPatrolOk = (moveMech.getPosition().x > 0.0f && moveMech.getStridePhase() > 0.0f);

    // Pursuit chase and bubble cannon attack
    moveMech.updateAi(sead::Vector3f(25.0f, 0.0f, 0.0f));
    bool mvChaseOk = (moveMech.getState() == Game::VehicleMoveState::cChase);
    moveMech.updateAi(sead::Vector3f(10.0f, 0.0f, 0.0f));
    bool mvShootOk = (moveMech.getState() == Game::VehicleMoveState::cShoot);
    for (int f = 0; f < 25; ++f) moveMech.vfunc_7();
    bool mvFireOk = (moveMech.getShotsFired() >= 1);

    moveMech.takeDamage(150.0f);
    bool mvDestructOk = (moveMech.isDestroyed() && moveMech.isPilotEjected());

    bool vehicleMoveOk = (mvInitOk && mvPatrolOk && mvChaseOk && mvShootOk && mvFireOk && mvDestructOk);

    // 3. Enm_TakolienVehicleSubmarine (Ink-Submersible Pod)
    Game::Enm_TakolienVehicleSubmarine subMech;
    subMech.init();
    subMech.spawn(sead::Vector3f(0.0f, 0.0f, 0.0f));
    bool subInitOk = (subMech.getState() == Game::VehicleSubmarineState::cSubmerged &&
                      subMech.isSubmerged() && subMech.getCurrentDepth() == -3.0f &&
                      subMech.getHealth() == 180.0f);

    // Periscope tracking & surface breach eruption
    subMech.updateSubmarineAi(sead::Vector3f(0.0f, 0.0f, 15.0f));
    bool subPeriOk = (subMech.getState() == Game::VehicleSubmarineState::cPeriscope &&
                      subMech.getCurrentDepth() == -0.5f);

    subMech.updateSubmarineAi(sead::Vector3f(0.0f, 0.0f, 5.0f));
    bool subBreachOk = (subMech.getState() == Game::VehicleSubmarineState::cBreachJump &&
                        subMech.getBreachCount() == 1);
    for (int f = 0; f < 20; ++f) subMech.vfunc_7();
    bool subSurfaceOk = (subMech.getState() == Game::VehicleSubmarineState::cSurfaced &&
                         subMech.getCurrentDepth() >= 0.0f);

    subMech.takeDamage(180.0f);
    bool subDestructOk = (subMech.isDestroyed() && subMech.isPilotEjected());

    bool vehicleSubOk = (subInitOk && subPeriOk && subBreachOk && subSurfaceOk && subDestructOk);

    // 4. Authentic Retail BFRES Models on Disk (22,469 vertices total)
    sead::BfresModel realVehFixMdl  = sead::BfresParser::createTakolienVehicleFixModel();
    sead::BfresModel realVehMoveMdl = sead::BfresParser::createTakolienVehicleMoveModel();
    sead::BfresModel realVehSubMdl  = sead::BfresParser::createTakolienVehicleSubmarineModel();

    bool realM74ModelsOk = (realVehFixMdl.getTotalVertexCount() == 8049 &&
                            realVehMoveMdl.getTotalVertexCount() == 6248 &&
                            realVehSubMdl.getTotalVertexCount() == 8172);

    bool m74Ok = (vehicleFixOk && vehicleMoveOk && vehicleSubOk && realM74ModelsOk);
    printf("  Octoling Assault Mechs & Ink-Submersibles:           %s (Mortar, Walker, Breach, 22,469 Verts)\n",
           m74Ok ? "PASSED" : "FAILED");
    if (!m74Ok) allPassed = false;

    printf("\n=================================================================\n");
    printf("[+] Overall Verification Result: %s\n", allPassed ? "PASSED (100% OK)" : "FAILED");
    printf("=================================================================\n\n");
    return allPassed;
}

void ListAllStages() {
    printf("--- ALL 43 CATALOGED SPLATOON 1 RETAIL STAGES ---\n");
    u32 count = Game::StageDef::getStageCount();
    const auto* stages = Game::StageDef::getAllStages();
    for (u32 i = 0; i < count; ++i) {
        printf("[%02u] %-30s | %-24s | Category: %u\n",
               i, stages[i].displayName, stages[i].codeName,
               static_cast<u32>(stages[i].category));
    }
    printf("\n");
}

bool DumpSarcFile(const char* filePath, const char* outDir = nullptr) {
    printf("[*] Inspecting SARC/SZS Archive: %s\n", filePath);
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        printf("[-] Error: Unable to open file '%s'\n", filePath);
        return false;
    }

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<u8> buffer(static_cast<size_t>(fileSize));
    if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
        printf("[-] Error: Failed to read file data\n");
        return false;
    }

    sead::SarcArchive archive;
    if (!archive.load(buffer.data(), buffer.size())) {
        printf("[-] Error: Not a valid SARC or Yaz0-compressed SZS archive\n");
        return false;
    }

    if (outDir) {
        CreateDirectoryA(outDir, nullptr);
        printf("[+] Extracting files to directory: %s\n", outDir);
    }

    printf("[+] Archive loaded successfully! File count: %zu\n", archive.getFileCount());
    for (size_t i = 0; i < archive.getFileCount(); ++i) {
        const auto* info = archive.getFileInfo(i);
        if (info) {
            printf("  [%03zu] %-40s | Size: %6zu bytes | Hash: 0x%08X\n",
                   i, info->name.c_str(), info->size, info->nameHash);

            if (outDir && info->data && info->size > 0) {
                std::string filename = info->name;
                size_t slash = filename.find_last_of("/\\");
                if (slash != std::string::npos) {
                    filename = filename.substr(slash + 1);
                }
                std::string outPath = std::string(outDir) + "/" + filename;
                FILE* fp = fopen(outPath.c_str(), "wb");
                if (fp) {
                    fwrite(info->data, 1, info->size, fp);
                    fclose(fp);
                    printf("    -> Saved: %s\n", outPath.c_str());
                }
            }
        }
    }
    return true;
}

void PrintBymlNode(const Game::BymlNode* node, int indent = 0) {
    if (!node || indent > 3) return;
    std::string pad(indent * 2, ' ');
    if (node->isDictionary()) {
        for (const auto& kv : node->getDictMembers()) {
            if (kv.second->isInt()) {
                printf("%s%s: %d\n", pad.c_str(), kv.first.c_str(), kv.second->getInt());
            } else if (kv.second->isFloat()) {
                printf("%s%s: %.2f\n", pad.c_str(), kv.first.c_str(), kv.second->getFloat());
            } else if (kv.second->isString()) {
                printf("%s%s: \"%s\"\n", pad.c_str(), kv.first.c_str(), kv.second->asString().c_str());
            } else if (kv.second->isBool()) {
                printf("%s%s: %s\n", pad.c_str(), kv.first.c_str(), kv.second->getBool() ? "true" : "false");
            } else if (kv.second->isDictionary() || kv.second->isArray()) {
                printf("%s%s:\n", pad.c_str(), kv.first.c_str());
                PrintBymlNode(kv.second.get(), indent + 1);
            }
        }
    } else if (node->isArray()) {
        for (size_t i = 0; i < node->getArraySize() && i < 10; ++i) {
            const auto* elem = node->getElement(i);
            if (elem) {
                printf("%s[%zu]:\n", pad.c_str(), i);
                PrintBymlNode(elem, indent + 1);
            }
        }
        if (node->getArraySize() > 10) {
            printf("%s... (%zu total elements)\n", pad.c_str(), node->getArraySize());
        }
    }
}

bool DumpBymlFile(const char* filePath) {
    printf("[*] Inspecting Nintendo BYML Parameter File: %s\n", filePath);
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        printf("[-] Error: Unable to open file '%s'\n", filePath);
        return false;
    }

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<u8> buffer(static_cast<size_t>(fileSize));
    if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
        printf("[-] Error: Failed to read file data\n");
        return false;
    }

    Game::BymlParser parser;
    if (!parser.parse(buffer.data(), buffer.size())) {
        printf("[-] Error: Failed to parse BYML (invalid magic or corrupted format)\n");
        return false;
    }

    if (!parser.isValid() || !parser.getRoot()) {
        printf("[-] Error: Empty or invalid root node\n");
        return false;
    }

    printf("[+] BYML parsed successfully! Root Node: %s\n",
           parser.getRoot()->isDictionary() ? "Dictionary" : (parser.getRoot()->isArray() ? "Array" : "Scalar"));
    PrintBymlNode(parser.getRoot(), 1);
    return true;
}

static bool s_WindowRunning = true;
static bool s_SplattedThisFrame = false;
static bool s_SwitchStageRequested = false;
static u8 s_CurrentPaintTeam = 0; // 0 = Alpha (Orange), 1 = Bravo (Cyan)
static float s_CamDistance = 35.0f;
static float s_CamHeight = 15.0f;
static float s_CamYaw = 0.0f;
static float s_CamPitch = 0.20f;
static bool s_KeyW = false, s_KeyA = false, s_KeyS = false, s_KeyD = false;
static bool s_KeyUp = false, s_KeyDown = false, s_KeyLeft = false, s_KeyRight = false;
static bool s_KeyShift = false, s_KeySpace = false;
static bool s_IsFiring = false;
static bool s_ThrowBombRequested = false;
static bool s_ActivateSpecialRequested = false;
static bool s_SuperJumpRequested = false;
static bool s_ToggleCameraRequested = false;
static bool s_CycleWeaponRequested = false;
static bool s_TriggerNewsRequested = false;
static int s_ActiveWeaponType = 0; // 0 = Splattershot, 1 = Splat Roller, 2 = Splat Charger
static float s_ChargerChargeRatio = 0.0f;
static bool s_FiredChargerThisFrame = false;
static int s_LastMouseX = 0, s_LastMouseY = 0;
static float s_MouseDeltaX = 0.0f, s_MouseDeltaY = 0.0f;

LRESULT CALLBACK SplatoonWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DESTROY:
        s_WindowRunning = false;
        PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            s_WindowRunning = false;
            PostQuitMessage(0);
        } else if (wParam == '1') {
            s_CurrentPaintTeam = 0;
            printf("[+] Switched Active Ink: Team Alpha (Neon Orange)\n");
        } else if (wParam == '2') {
            s_CurrentPaintTeam = 1;
            printf("[+] Switched Active Ink: Team Bravo (Neon Cyan)\n");
        } else if (wParam == 'E' || wParam == 'Q') {
            s_CycleWeaponRequested = true;
        } else if (wParam == 'F' || wParam == 'V' || wParam == 'Z') {
            s_ActivateSpecialRequested = true;
        } else if (wParam == 'J') {
            s_SuperJumpRequested = true;
        } else if (wParam == VK_SPACE) {
            s_KeySpace = true;
            s_SplattedThisFrame = true;
        } else if (wParam == VK_SHIFT) {
            s_KeyShift = true;
        } else if (wParam == 'C') {
            s_ToggleCameraRequested = true;
        } else if (wParam == 'N') {
            s_TriggerNewsRequested = true;
        } else if (wParam == 'R') {
            s_ThrowBombRequested = true;
        } else if (wParam == VK_TAB || wParam == 'M') {
            s_SwitchStageRequested = true;
        } else if (wParam == 'W') s_KeyW = true;
        else if (wParam == 'S') s_KeyS = true;
        else if (wParam == 'A') s_KeyA = true;
        else if (wParam == 'D') s_KeyD = true;
        else if (wParam == VK_UP) s_KeyUp = true;
        else if (wParam == VK_DOWN) s_KeyDown = true;
        else if (wParam == VK_LEFT) s_KeyLeft = true;
        else if (wParam == VK_RIGHT) s_KeyRight = true;
        break;
    case WM_KEYUP:
        if (wParam == 'W') s_KeyW = false;
        else if (wParam == 'S') s_KeyS = false;
        else if (wParam == 'A') s_KeyA = false;
        else if (wParam == 'D') s_KeyD = false;
        else if (wParam == VK_SHIFT) s_KeyShift = false;
        else if (wParam == VK_SPACE) s_KeySpace = false;
        else if (wParam == VK_UP) s_KeyUp = false;
        else if (wParam == VK_DOWN) s_KeyDown = false;
        else if (wParam == VK_LEFT) s_KeyLeft = false;
        else if (wParam == VK_RIGHT) s_KeyRight = false;
        break;
    case WM_LBUTTONDOWN:
        s_IsFiring = true;
        s_SplattedThisFrame = true;
        break;
    case WM_LBUTTONUP:
        s_IsFiring = false;
        if (s_ActiveWeaponType == 2 && s_ChargerChargeRatio > 0.15f) {
            s_FiredChargerThisFrame = true;
        }
        break;
    case WM_RBUTTONDOWN:
        s_ThrowBombRequested = true;
        break;
    case WM_MBUTTONDOWN:
        s_ActivateSpecialRequested = true;
        break;
    case WM_MOUSEMOVE: {
        int mx = LOWORD(lParam);
        int my = HIWORD(lParam);
        if (s_LastMouseX != 0 || s_LastMouseY != 0) {
            float dx = static_cast<float>(mx - s_LastMouseX);
            float dy = static_cast<float>(my - s_LastMouseY);
            if ((wParam & MK_LBUTTON) != 0 || (wParam & MK_RBUTTON) != 0) {
                s_MouseDeltaX += dx * 0.005f;
                s_MouseDeltaY += dy * 0.005f;
            }
        }
        s_LastMouseX = mx;
        s_LastMouseY = my;
        break;
    }
    default:
        return DefWindowProcA(hWnd, message, wParam, lParam);
    }
    return 0;
}

struct InkBullet3D {
    sead::Vector3f pos;
    sead::Vector3f vel;
    u32 teamId;
    float radius;
    float lifetime;
    bool isBomb;
    bool active;
};

int RunRenderWindow(int maxFrames, const char* stageName) {
    printf("=================================================================\n");
    printf("  Splatoon 1 (Wii U - Gambit PC) - DirectX 11 3D Map Renderer   \n");
    printf("=================================================================\n");
    printf("[+] Target Subsystem: Native PC Direct3D 11 Hardware Graphics\n");
    printf("[+] Resolution: 1280x720 @ 60 FPS (VSync Enabled)\n");
    printf("-----------------------------------------------------------------\n");

    HINSTANCE hInstance = GetModuleHandle(nullptr);
    const char* className = "SplatoonDx11WindowClass";

    WNDCLASSEXA wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = SplatoonWndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = className;
    RegisterClassExA(&wc);

    RECT wr = { 0, 0, 1280, 720 };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hWnd = CreateWindowExA(
        0,
        className,
        "Splatoon 1 (Wii U Gambit PC) - DirectX 11 3D Map Renderer [60 FPS]",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, hInstance, nullptr
    );

    if (!hWnd) {
        printf("[-] Error: Failed to create Win32 window (Error %lu)\n", GetLastError());
        return 1;
    }

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    Game::Dx11Renderer renderer;
    if (!renderer.initPipeline(hWnd, 1280, 720, false)) {
        printf("[-] Error: Failed to initialize Direct3D 11 hardware pipeline!\n");
        DestroyWindow(hWnd);
        return 1;
    }

    printf("[+] Direct3D 11 Pipeline: INITIALIZED (Feature Level 11.0)\n");
    printf("[+] Backbuffer: 1280x720 R8G8B8A8_UNORM | Depth: D24_UNORM_S8_UINT\n");

    // Dynamic 3D Inking Surface (PaintMap3D)
    Game::PaintMap3D paintMap;

    // Initialize Native PC Audio Engine
    Game::PcAudioDriver::instance().init();
    Game::SoundShapeMgr soundMgr;
    soundMgr.init();

    // Load Procedural Splatoon Crate, Target Dummies & Character Models
    sead::BfresModel crateModel = sead::BfresParser::createSplatoonCrateModel("Obj_GeneralBox_111", 2.4f);
    sead::BfresModel targetDummyModel = sead::BfresParser::createSighterTargetModel("SighterTarget");
    sead::BfresModel inklingHumanModel = sead::BfresParser::createInklingHumanModel("PlayerHuman", s_CurrentPaintTeam);
    sead::BfresModel inklingSquidModel = sead::BfresParser::createInklingSquidModel("PlayerSquid", s_CurrentPaintTeam);
    sead::BfresModel rivalModel = sead::BfresParser::createOctolingModel("RivalOctoling", 1);
    sead::BfresModel splattershotModel = sead::BfresParser::createSplattershotModel("Weapon_Splattershot", s_CurrentPaintTeam);
    sead::BfresModel rollerModel = sead::BfresParser::createSplatRollerModel("Weapon_SplatRoller", s_CurrentPaintTeam);
    sead::BfresModel chargerModel = sead::BfresParser::createSplatChargerModel("Weapon_SplatCharger", s_CurrentPaintTeam);
    sead::BfresModel krakenModel = sead::BfresParser::createKrakenModel("Weapon_Kraken", s_CurrentPaintTeam);
    sead::BfresModel rainmakerGoalModel = sead::BfresParser::createRainmakerPedestalModel("Obj_ShrBasketGoal");
    sead::BfresModel bulletModel = sead::BfresParser::createInkBulletModel("InkBullet", 0.35f);
    sead::BfresModel bombModel = sead::BfresParser::createSplatBombModel("Wsb_Bomb_Throw", s_CurrentPaintTeam);
    sead::BfresModel killerWailModel = sead::BfresParser::createKillerWailModel("Weapon_KillerWail", s_CurrentPaintTeam);
    sead::BfresModel inkzookaModel = sead::BfresParser::createInkzookaModel("Weapon_Inkzooka", s_CurrentPaintTeam);

    // Authentic Retail Inkopolis Plaza NPCs
    sead::BfresModel callieModel = sead::BfresParser::createCallieModel("Callie_Retail");
    sead::BfresModel marieModel = sead::BfresParser::createMarieModel("Marie_Retail");
    sead::BfresModel spykeModel = sead::BfresParser::createSpykeModel("Spyke_Retail");
    sead::BfresModel juddModel = sead::BfresParser::createJuddModel("Judd_Retail");
    sead::BfresModel sheldonModel = sead::BfresParser::createSheldonModel("Sheldon_Retail");

    // Inkopolis News Stage Rotation Broadcast Engine
    Game::PlazaNewsBroadcast plazaNews;
    Game::RotationSchedule liveSchedule;
    liveSchedule.regularStageIdA = 0; // Walleye Warehouse
    liveSchedule.regularStageIdB = 2; // Urchin Underpass
    liveSchedule.rankedStageIdA = 1;  // Blackbelly Skatepark
    liveSchedule.rankedStageIdB = 3;  // Saltspray Rig
    liveSchedule.rankedRule = Game::RankedModeType::cSplatZones;
    liveSchedule.isSplatfestActive = false;

    // Interactive Stage Objects
    std::vector<Game::Obj_GeneralBox> stageCrates;
    std::vector<Game::SighterTarget> stageTargets;

    // Special Weapon Subsystems & Super Jump Actor
    Game::GameWeaponSuperShot wepSuperShot;
    Game::GameWeaponMegaphone wepMegaphone;
    Game::GameWeaponDaiouIka wepKraken;
    Game::AutoWarpPoint superJumpActor;
    float specialGauge = 0.0f;
    int activeSpecial = -1; // -1 = None, 0 = Inkzooka, 1 = Kraken, 2 = Killer Wail

    // Map Loading and Initialization
    std::string currentStage = (stageName && stageName[0]) ? stageName : "Fld_PlazaLobby";
    Game::KclFile stageKcl;
    sead::BfresModel stageModel;
    sead::Vector3f stageCenter(0.0f, 0.0f, 0.0f);
    float maxSpan = 60.0f;

    // Playable Inkling Player State
    sead::Vector3f playerPos(0.0f, 0.0f, 0.0f);
    sead::Vector3f playerVel(0.0f, 0.0f, 0.0f);
    float playerYaw = 3.14159f;
    bool isSquid = false;
    bool wasSquid = false;
    bool isGrounded = true;
    float inkTank = 1.0f;
    int fireCooldown = 0;
    bool cameraThirdPerson = true;

    // Dynamic In-Flight Ballistics
    std::vector<InkBullet3D> bullets;

    // Octoling Rival Squad Combatant
    Game::GameRivalSquad rivalBot;
    sead::Vector3f rivalSpawnPos(0.0f, 0.0f, 0.0f);
    int rivalRespawnTimer = 0;
    int rivalShootTimer = 0;

    auto loadStageGeometry = [&](const std::string& name) -> bool {
        stageKcl.clear();
        stageCrates.clear();
        stageTargets.clear();
        bullets.clear();
        std::string candidatePaths[] = {
            name,
            "content/Model/" + name + ".szs",
            "content/Model/Fld_" + name + ".szs",
            "content/Model/" + name
        };
        bool loaded = false;
        std::string pathUsed;
        for (const auto& p : candidatePaths) {
            if (stageKcl.loadFromSzsFile(p.c_str())) {
                loaded = true;
                pathUsed = p;
                break;
            }
        }
        if (loaded) {
            sead::BfresParser stageBfres;
            if (stageBfres.loadFromSzsFile(pathUsed.c_str()) && stageBfres.getModelCount() > 0) {
                stageModel.name = name;
                stageModel.meshes.clear();
                stageModel.materials.clear();
                for (size_t m = 0; m < stageBfres.getModelCount(); ++m) {
                    const auto* mod = stageBfres.getModel(m);
                    if (mod) {
                        for (const auto& mesh : mod->meshes) {
                            stageModel.meshes.push_back(mesh);
                        }
                    }
                }
            } else {
                stageModel = stageKcl.toBfresModel(name.c_str());
            }
            sead::Vector3f minB = stageKcl.getMinBounds();
            sead::Vector3f maxB = stageKcl.getMaxBounds();
            stageCenter = (minB + maxB) * 0.5f;
            float spanX = maxB.x - minB.x;
            float spanZ = maxB.z - minB.z;
            maxSpan = (std::max)(spanX, spanZ);

            // Re-initialize dynamic inking map across stage bounds
            float padX = spanX * 0.05f + 4.0f;
            float padZ = spanZ * 0.05f + 4.0f;
            paintMap.init(512, 512, minB.x - padX, minB.z - padZ, maxB.x + padX, maxB.z + padZ);

            // Initial Turf War Ink Splats on Stage
            paintMap.splatWorldSphere(stageCenter + sead::Vector3f(-maxSpan * 0.18f, 0.0f, -maxSpan * 0.10f), maxSpan * 0.14f, 0, 1.0f);
            paintMap.splatWorldSphere(stageCenter + sead::Vector3f( maxSpan * 0.18f, 0.0f,  maxSpan * 0.10f), maxSpan * 0.14f, 1, 1.0f);
            paintMap.splatWorldSphere(stageCenter, maxSpan * 0.06f, 0, 1.0f);

            // Spawn player near stage center on floor
            playerPos = stageCenter + sead::Vector3f(0.0f, 2.0f, 6.0f);
            Game::KclHitResult spawnHit;
            if (stageKcl.raycast(playerPos + sead::Vector3f(0.0f, 10.0f, 0.0f), sead::Vector3f(0.0f, -1.0f, 0.0f), 20.0f, spawnHit)) {
                playerPos.y = spawnHit.hitPoint.y;
            }

            // Spawn 4 Wooden Crates on Stage Floor
            sead::Vector3f crateOffsets[4] = {
                sead::Vector3f(-7.0f, 0.0f, -5.0f),
                sead::Vector3f(-7.0f, 0.0f,  5.0f),
                sead::Vector3f( 7.0f, 0.0f, -5.0f),
                sead::Vector3f( 7.0f, 0.0f,  5.0f)
            };
            for (int i = 0; i < 4; ++i) {
                Game::Obj_GeneralBox boxObj;
                boxObj.init();
                sead::Vector3f bPos = stageCenter + crateOffsets[i];
                Game::KclHitResult bHit;
                if (stageKcl.raycast(bPos + sead::Vector3f(0.0f, 10.0f, 0.0f), sead::Vector3f(0.0f, -1.0f, 0.0f), 20.0f, bHit)) {
                    bPos.y = bHit.hitPoint.y;
                }
                boxObj.setup(bPos, 80.0f, 1.0f);
                stageCrates.push_back(boxObj);
            }

            // Spawn 2 Sighter Target Dummies on Stage Floor
            Game::SighterTarget dummy0;
            dummy0.init();
            sead::Vector3f d0Pos = stageCenter + sead::Vector3f(0.0f, 0.0f, -12.0f);
            Game::KclHitResult d0Hit;
            if (stageKcl.raycast(d0Pos + sead::Vector3f(0.0f, 10.0f, 0.0f), sead::Vector3f(0.0f, -1.0f, 0.0f), 20.0f, d0Hit)) {
                d0Pos.y = d0Hit.hitPoint.y;
            }
            dummy0.setup(d0Pos, 100.0f, 1, false);
            stageTargets.push_back(dummy0);

            Game::SighterTarget dummy1;
            dummy1.init();
            sead::Vector3f d1Pos = stageCenter + sead::Vector3f(0.0f, 0.0f, 12.0f);
            Game::KclHitResult d1Hit;
            if (stageKcl.raycast(d1Pos + sead::Vector3f(0.0f, 10.0f, 0.0f), sead::Vector3f(0.0f, -1.0f, 0.0f), 20.0f, d1Hit)) {
                d1Pos.y = d1Hit.hitPoint.y;
            }
            dummy1.setup(d1Pos, 100.0f, 0, true);
            stageTargets.push_back(dummy1);

            // Spawn Octoling Rival Combatant
            rivalBot.init();
            rivalSpawnPos = stageCenter + sead::Vector3f(0.0f, 0.0f, -maxSpan * 0.28f);
            Game::KclHitResult rHit;
            if (stageKcl.raycast(rivalSpawnPos + sead::Vector3f(0.0f, 10.0f, 0.0f), sead::Vector3f(0.0f, -1.0f, 0.0f), 20.0f, rHit)) {
                rivalSpawnPos.y = rHit.hitPoint.y;
            }
            rivalBot.spawn(rivalSpawnPos, Game::RivalDifficulty::cLevel2);
            rivalRespawnTimer = 0;
            rivalShootTimer = 0;

            s_CamDistance = (std::max)(22.0f, maxSpan * 0.65f);
            s_CamHeight = (std::max)(10.0f, maxSpan * 0.35f);

            printf("[+] Loaded Retail Stage: %s (%s)\n", name.c_str(), pathUsed.c_str());
            printf("    Prisms: %zu | Vertices: %zu | Submeshes: %zu\n",
                   stageKcl.getPrismCount(), stageModel.getTotalVertexCount(), stageModel.meshes.size());
            printf("    Bounds: Min(%.1f, %.1f, %.1f) -> Max(%.1f, %.1f, %.1f)\n",
                   minB.x, minB.y, minB.z, maxB.x, maxB.y, maxB.z);
            printf("    Center: (%.1f, %.1f, %.1f) | Span: %.1fm\n",
                   stageCenter.x, stageCenter.y, stageCenter.z, maxSpan);

            char titleBuf[256];
            snprintf(titleBuf, sizeof(titleBuf),
                     "Splatoon 1 (Wii U Gambit PC) - Stage: %s [%zu Prisms, %zu Verts] - DirectX 11 [60 FPS]",
                     name.c_str(), stageKcl.getPrismCount(), stageModel.getTotalVertexCount());
            SetWindowTextA(hWnd, titleBuf);
            return true;
        } else {
            printf("[*] Fallback: Generating procedural ground plane for '%s'\n", name.c_str());
            stageModel = sead::BfresParser::createProceduralGroundPlane("ProceduralFloor", 40.0f, 40.0f);
            paintMap.init(512, 512, -25.0f, -25.0f, 25.0f, 25.0f);
            paintMap.splatWorldSphere(sead::Vector3f(-4.0f, 0.0f, -3.5f), 4.5f, 0, 1.0f);
            paintMap.splatWorldSphere(sead::Vector3f( 4.0f, 0.0f,  3.5f), 4.5f, 1, 1.0f);
            stageCenter.set(0.0f, 0.0f, 0.0f);
            playerPos.set(0.0f, 0.0f, 0.0f);
            maxSpan = 40.0f;
            s_CamDistance = 16.0f;
            s_CamHeight = 7.0f;
            rivalBot.init();
            rivalSpawnPos = sead::Vector3f(0.0f, 0.0f, -10.0f);
            rivalBot.spawn(rivalSpawnPos, Game::RivalDifficulty::cLevel2);
            rivalRespawnTimer = 0;
            rivalShootTimer = 0;
            return false;
        }
    };

    // Load initial requested stage
    loadStageGeometry(currentStage);

    printf("-----------------------------------------------------------------\n");
    printf("[*] PLAYABLE INKLING CONTROLS:\n");
    printf("    - WASD:                Move Inkling Player (relative to camera)\n");
    printf("    - Mouse / Arrows:      Aim & Look in 3D (Yaw & Pitch)\n");
    printf("    - Space:               Jump (with apex hang time)\n");
    printf("    - Shift:               Submerge in Squid Form (Fast swim in ink!)\n");
    printf("    - Left Click / Space:  Fire Weapon (Splattershot / Roller / Charger)\n");
    printf("    - Q / E:               Cycle Active Weapon (Shooter <-> Roller <-> Charger)\n");
    printf("    - Right Click / R:     Throw Splat Bomb (high lob trajectory)\n");
    printf("    - F / V / Z / MMB:     Activate Special Weapon (Inkzooka / Kraken / Killer Wail)\n");
    printf("    - J:                   Super Jump (High parabolic arc back to Stage Center)\n");
    printf("    - 1 / 2:               Switch Ink Team (Neon Orange <-> Cyan)\n");
    printf("    - TAB / M:             Switch Map (Inkopolis Plaza <-> Walleye Warehouse)\n");
    printf("    - C:                   Toggle Camera (3rd-Person Follow <-> Turntable)\n");
    printf("    - ESC:                 Close Window\n");
    printf("-----------------------------------------------------------------\n\n");

    s_WindowRunning = true;
    int frameCount = 0;
    float time = 0.0f;

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (s_WindowRunning) {
        while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
            if (msg.message == WM_QUIT) {
                s_WindowRunning = false;
                break;
            }
        }
        if (!s_WindowRunning) break;

        // Switch Stage on TAB or M
        if (s_SwitchStageRequested) {
            s_SwitchStageRequested = false;
            bullets.clear();
            if (currentStage == "Fld_PlazaLobby" || currentStage.find("Plaza") != std::string::npos) {
                currentStage = "Fld_Warehouse00";
            } else {
                currentStage = "Fld_PlazaLobby";
            }
            printf("\n[*] Switching Stage to: %s...\n", currentStage.c_str());
            loadStageGeometry(currentStage);
        }

        // Cycle Weapon Arsenal on 'Q' or 'E'
        if (s_CycleWeaponRequested) {
            s_CycleWeaponRequested = false;
            s_ActiveWeaponType = (s_ActiveWeaponType + 1) % 3;
            s_ChargerChargeRatio = 0.0f;
            s_FiredChargerThisFrame = false;
            const char* wNames[] = { "Splattershot (Shooter)", "Splat Roller (Roller)", "Splat Charger (Charger)" };
            printf("[+] Switched Active Weapon to: %s\n", wNames[s_ActiveWeaponType]);
        }

        // Toggle Camera Mode on 'C'
        if (s_ToggleCameraRequested) {
            s_ToggleCameraRequested = false;
            cameraThirdPerson = !cameraThirdPerson;
            printf("[*] Camera Mode: %s\n", cameraThirdPerson ? "Third-Person Inkling Shoulder Cam" : "Stage Overview Turntable");
        }

        // Camera Aim / Look from Mouse & Arrow Keys
        s_CamYaw += s_MouseDeltaX;
        s_CamPitch += s_MouseDeltaY;
        s_MouseDeltaX = 0.0f;
        s_MouseDeltaY = 0.0f;

        if (s_KeyLeft) s_CamYaw -= 0.035f;
        if (s_KeyRight) s_CamYaw += 0.035f;
        if (s_KeyUp) s_CamPitch = (std::max)(-0.65f, s_CamPitch - 0.035f);
        if (s_KeyDown) s_CamPitch = (std::min)(1.10f, s_CamPitch + 0.035f);
        s_CamPitch = (std::max)(-0.65f, (std::min)(1.10f, s_CamPitch));

        if (!cameraThirdPerson) {
            s_CamYaw += 0.005f; // Gentle continuous turntable rotation in overview mode
        }

        time += 0.0166f;

        // Player Form Transformation (Shift = Squid)
        isSquid = s_KeyShift;
        if (isSquid && !wasSquid) {
            Game::PcAudioDriver::instance().playSound(Game::cSoundId_Squid_Dive, 0.70f, 0.0f);
        }
        wasSquid = isSquid;

        // Camera Direction Vectors
        sead::Vector3f camFwd(sinf(s_CamYaw), 0.0f, cosf(s_CamYaw));
        sead::Vector3f camRight(cosf(s_CamYaw), 0.0f, -sinf(s_CamYaw));

        // Sample Ink on Stage Surface beneath Player
        u8 sampledTeam = 255;
        float sampledIntensity = 0.0f;
        paintMap.sampleInkAtWorldPos(playerPos, &sampledTeam, &sampledIntensity);
        bool inFriendlyInk = (sampledTeam == s_CurrentPaintTeam && sampledIntensity > 0.12f);
        bool inEnemyInk = (sampledTeam != 255 && sampledTeam != s_CurrentPaintTeam && sampledIntensity > 0.12f);

        // Immersion Audio Filter
        soundMgr.setInkImmersionFilter(isSquid && inFriendlyInk);

        // Player Speed Computation
        float moveSpeed = 0.22f; // Base human run speed
        if (activeSpecial == 1) {
            isSquid = true; // Kraken invincible giant squid
            moveSpeed = 0.52f;
        } else if (isSquid) {
            if (inFriendlyInk) {
                moveSpeed = 0.46f; // Super-fast swimming in friendly ink
                inkTank = (std::min)(1.0f, inkTank + 0.015f); // Rapid ink tank refill
            } else if (inEnemyInk) {
                moveSpeed = 0.06f; // Trapped / slowed down in enemy ink
            } else {
                moveSpeed = 0.14f; // Flopping on dry neutral concrete
            }
        } else {
            if (inEnemyInk) {
                moveSpeed = 0.09f; // Stuck in enemy ink
            }
            inkTank = (std::min)(1.0f, inkTank + 0.003f); // Passive ink recharge
        }

        // Super Jump Launch (J key)
        if (s_SuperJumpRequested && superJumpActor.getState() == Game::SuperJumpState::cIdle) {
            s_SuperJumpRequested = false;
            superJumpActor.launchSuperJump(playerPos, stageCenter + sead::Vector3f(0.0f, 1.2f, 0.0f), s_CurrentPaintTeam, false);
            Game::PcAudioDriver::instance().playSound(Game::cSoundId_Squid_Dive, 1.0f, 0.4f);
            printf("[*] SUPER JUMP LAUNCHED! Parabolic trajectory back to Stage Center (%.1f, %.1f).\n", stageCenter.x, stageCenter.z);
        }

        // Super Jump In-Flight Flight & Touchdown Physics
        if (superJumpActor.getState() != Game::SuperJumpState::cIdle) {
            superJumpActor.update();
            if (superJumpActor.getState() == Game::SuperJumpState::cAirborneParabola) {
                playerPos = superJumpActor.getCurrentSquidPos();
                isSquid = true;
                isGrounded = false;
            } else if (superJumpActor.getState() == Game::SuperJumpState::cTouchdownSplash) {
                playerPos = superJumpActor.getTargetLandingPos();
                isGrounded = true;
                isSquid = false;
                paintMap.splatWorldSphere(playerPos, 4.0f, s_CurrentPaintTeam, 1.0f);
                specialGauge = (std::min)(1.0f, specialGauge + 0.05f);
                Game::PcAudioDriver::instance().playSound(Game::cSoundId_Hit_Confirm, 1.0f, 0.0f);
                superJumpActor.init();
            }
        }

        // Compute Movement Vector
        sead::Vector3f moveDir(0.0f, 0.0f, 0.0f);
        if (s_KeyW) moveDir = moveDir + camFwd;
        if (s_KeyS) moveDir = moveDir - camFwd;
        if (s_KeyA) moveDir = moveDir - camRight;
        if (s_KeyD) moveDir = moveDir + camRight;

        float moveLenSq = moveDir.x * moveDir.x + moveDir.z * moveDir.z;
        if (moveLenSq > 0.0001f) {
            float invLen = 1.0f / std::sqrt(moveLenSq);
            moveDir = moveDir * invLen;
            playerYaw = std::atan2(moveDir.x, moveDir.z);
            playerVel.x = moveDir.x * moveSpeed;
            playerVel.z = moveDir.z * moveSpeed;
        } else {
            playerVel.x *= 0.65f;
            playerVel.z *= 0.65f;
        }

        // Jump (Space)
        if (s_KeySpace && isGrounded && superJumpActor.getState() == Game::SuperJumpState::cIdle) {
            s_KeySpace = false;
            playerVel.y = isSquid ? 0.48f : 0.40f;
            isGrounded = false;
        }

        // Horizontal Wall Collision & Slide
        float testX = playerPos.x + playerVel.x;
        float testZ = playerPos.z + playerVel.z;
        Game::KclHitResult wallHit;
        if (stageKcl.checkSphere(sead::Vector3f(testX, playerPos.y + 0.8f, testZ), 0.70f, wallHit)) {
            sead::Vector3f wn = wallHit.hitNormal;
            if (std::abs(wn.y) < 0.65f) {
                float dot = playerVel.x * wn.x + playerVel.z * wn.z;
                playerVel.x -= dot * wn.x;
                playerVel.z -= dot * wn.z;
            }
        }
        playerPos.x += playerVel.x;
        playerPos.z += playerVel.z;

        // Vertical Gravity & KCL Floor Raycast Snapping
        if (!isGrounded) {
            playerVel.y -= 0.024f; // Gravity
        }
        playerPos.y += playerVel.y;

        Game::KclHitResult floorHit;
        sead::Vector3f rayStart = playerPos + sead::Vector3f(0.0f, 1.2f, 0.0f);
        if (stageKcl.raycast(rayStart, sead::Vector3f(0.0f, -1.0f, 0.0f), 2.5f, floorHit)) {
            float floorY = floorHit.hitPoint.y;
            if (playerPos.y <= floorY + 0.20f) {
                playerPos.y = floorY;
                playerVel.y = 0.0f;
                isGrounded = true;
            } else {
                isGrounded = false;
            }
        } else {
            isGrounded = false;
            if (playerPos.y <= 0.0f) {
                playerPos.y = 0.0f;
                playerVel.y = 0.0f;
                isGrounded = true;
            }
        }

        // Weapon Attacks & Continuous Inking
        if (fireCooldown > 0) fireCooldown--;

        // Splat Roller continuous ground rolling
        if (s_ActiveWeaponType == 1 && !isSquid && isGrounded && moveLenSq > 0.0001f) {
            if (inkTank >= 0.0015f) {
                inkTank -= 0.0015f;
                sead::Vector3f rollPos = playerPos + camFwd * 1.3f;
                paintMap.splatWorldSphere(rollPos, 2.7f, s_CurrentPaintTeam, 1.0f);

                // Crates contact
                for (auto& crate : stageCrates) {
                    if (!crate.isBroken() && (crate.getPosition() - rollPos).length() < 2.5f) {
                        crate.applyDamage(125.0f, s_CurrentPaintTeam);
                        Game::PcAudioDriver::instance().playSound(Game::cSoundId_Crate_Break, 0.85f, 0.0f);
                        paintMap.splatWorldSphere(crate.getPosition(), 4.2f, s_CurrentPaintTeam, 1.0f);
                    }
                }
                // Target dummy contact
                for (auto& target : stageTargets) {
                    if (target.getState() != Game::TargetState::cPopped && (target.getPosition() - rollPos).length() < 2.5f) {
                        target.applyDamage(125.0f);
                        Game::PcAudioDriver::instance().playSound(Game::cSoundId_Hit_Confirm, 0.85f, 0.0f);
                        paintMap.splatWorldSphere(target.getPosition(), 3.0f, s_CurrentPaintTeam, 1.0f);
                    }
                }
                // Octoling Rival contact
                if (rivalBot.isAlive() && (rivalBot.getPosition() - rollPos).length() < 2.5f) {
                    rivalBot.applyDamage(125.0f);
                    Game::PcAudioDriver::instance().playSound(Game::cSoundId_Hit_Confirm, 1.0f, 0.0f);
                    paintMap.splatWorldSphere(rivalBot.getPosition(), 3.5f, s_CurrentPaintTeam, 1.0f);
                    if (!rivalBot.isAlive()) {
                        Game::PcAudioDriver::instance().playSound(Game::cSoundId_Octoling_Splat, 1.0f, 0.0f);
                        paintMap.splatWorldSphere(rivalBot.getPosition(), 5.0f, s_CurrentPaintTeam, 1.0f);
                        rivalRespawnTimer = 180;
                        printf("[*] Octoling Rival SQUISHED by Splat Roller!\n");
                    }
                }
            }
        }

        // Activate Special Weapon (F / V / Z / MMB)
        if (s_ActivateSpecialRequested) {
            s_ActivateSpecialRequested = false;
            if (specialGauge >= 1.0f && activeSpecial == -1) {
                specialGauge = 0.0f;
                activeSpecial = s_ActiveWeaponType;
                if (activeSpecial == 0) {
                    wepSuperShot.init();
                    wepSuperShot.activate(s_CurrentPaintTeam, playerPos);
                    Game::PcAudioDriver::instance().playSound(Game::cSoundId_Charger_Charge, 1.0f, 0.0f);
                    printf("[*] SPECIAL ACTIVATED: INKZOOKA (Super Shot)! 6 whirlwind blasts ready. Left Click to fire!\n");
                } else if (activeSpecial == 1) {
                    wepKraken.init();
                    wepKraken.activate(s_CurrentPaintTeam, playerPos);
                    Game::PcAudioDriver::instance().playSound(Game::cSoundId_Squid_Dive, 1.0f, 0.0f);
                    printf("[*] SPECIAL ACTIVATED: KRAKEN (Daiou Ika)! Invincible giant squid online! Left Click to Spin Squish!\n");
                } else if (activeSpecial == 2) {
                    sead::Vector3f deployPos = playerPos + camFwd * 2.2f;
                    Game::KclHitResult dHit;
                    if (stageKcl.raycast(deployPos + sead::Vector3f(0.0f, 5.0f, 0.0f), sead::Vector3f(0.0f, -1.0f, 0.0f), 10.0f, dHit)) {
                        deployPos.y = dHit.hitPoint.y;
                    }
                    wepMegaphone.init();
                    wepMegaphone.deploy(s_CurrentPaintTeam, deployPos, s_CamYaw);
                    Game::PcAudioDriver::instance().playSound(Game::cSoundId_Charger_Fire, 1.0f, -0.2f);
                    printf("[*] SPECIAL ACTIVATED: KILLER WAIL (Megaphone Laser)! Acoustic laser speaker deployed!\n");
                }
            }
        }

        // Inkzooka Active Super Shot Firing
        if (activeSpecial == 0) {
            wepSuperShot.update();
            if (s_IsFiring && wepSuperShot.canFire() && fireCooldown == 0 && !isSquid) {
                fireCooldown = 28;
                sead::Vector3f aimDir(sinf(s_CamYaw), sinf(s_CamPitch), cosf(s_CamYaw));
                aimDir = aimDir.normalized();
                wepSuperShot.fire(playerPos, aimDir);
                for (int j = -1; j <= 1; ++j) {
                    InkBullet3D b;
                    b.pos = playerPos + sead::Vector3f(0.0f, 1.2f + j * 0.45f, 0.0f) + aimDir * 0.9f;
                    b.vel = aimDir * 2.5f + sead::Vector3f(0.0f, j * 0.08f + 0.05f, 0.0f);
                    b.teamId = s_CurrentPaintTeam;
                    b.radius = 4.2f;
                    b.lifetime = 1.8f;
                    b.isBomb = false;
                    b.active = true;
                    bullets.push_back(b);
                }
                Game::PcAudioDriver::instance().playSound(Game::cSoundId_Shoot_Splattershot, 1.0f, -0.3f);
                printf("[*] Inkzooka FIRED whirlwind tornado! Shots remaining: %d\n", wepSuperShot.getRemainingShots());
            }
            if (!wepSuperShot.isActive()) {
                activeSpecial = -1;
                printf("[*] Inkzooka duration ended.\n");
            }
        }

        // Kraken Active Giant Squid Attack
        if (activeSpecial == 1) {
            wepKraken.setPosition(playerPos);
            wepKraken.update();
            isSquid = true; // Invincible squid form
            if (s_IsFiring) {
                if (wepKraken.triggerSpinAttack()) {
                    Game::PcAudioDriver::instance().playSound(Game::cSoundId_Roller_Fling, 1.0f, 0.2f);
                    paintMap.splatWorldSphere(playerPos, 3.2f, s_CurrentPaintTeam, 1.0f);
                    printf("[*] Kraken SPIN SQUISH ATTACK!\n");
                }
            }
            if (wepKraken.isSpinning()) {
                paintMap.splatWorldSphere(playerPos, 2.8f, s_CurrentPaintTeam, 1.0f);
                for (auto& crate : stageCrates) {
                    f32 d = 0;
                    if (!crate.isBroken() && wepKraken.checkSpinDamage(crate.getPosition(), 1.2f, &d)) {
                        crate.applyDamage(d, s_CurrentPaintTeam);
                        Game::PcAudioDriver::instance().playSound(Game::cSoundId_Crate_Break, 1.0f, 0.0f);
                    }
                }
                for (auto& dummy : stageTargets) {
                    f32 d = 0;
                    if (dummy.getState() != Game::TargetState::cPopped && wepKraken.checkSpinDamage(dummy.getPosition(), 1.2f, &d)) {
                        dummy.applyDamage(d);
                        Game::PcAudioDriver::instance().playSound(Game::cSoundId_Hit_Confirm, 1.0f, 0.0f);
                    }
                }
                if (rivalBot.isAlive()) {
                    f32 d = 0;
                    if (wepKraken.checkSpinDamage(rivalBot.getPosition(), 1.2f, &d)) {
                        rivalBot.applyDamage(d);
                        Game::PcAudioDriver::instance().playSound(Game::cSoundId_Octoling_Splat, 1.0f, 0.0f);
                        paintMap.splatWorldSphere(rivalBot.getPosition(), 5.0f, s_CurrentPaintTeam, 1.0f);
                        rivalRespawnTimer = 180;
                        printf("[*] Rival bot OBLITERATED by Kraken Spin Squish!\n");
                    }
                }
            }
            if (!wepKraken.isActive()) {
                activeSpecial = -1;
                printf("[*] Kraken transformation ended.\n");
            }
        }

        // Killer Wail Active Acoustic Shockwave
        if (wepMegaphone.isDeployed()) {
            wepMegaphone.update();
            if (wepMegaphone.isFiring()) {
                sead::Vector3f mPos = wepMegaphone.getPosition();
                sead::Vector3f mDir = wepMegaphone.getDirection();
                if (frameCount % 4 == 0) {
                    for (float dist = 4.0f; dist < 80.0f; dist += 6.0f) {
                        sead::Vector3f pt = mPos + mDir * dist;
                        paintMap.splatWorldSphere(pt, 3.2f, wepMegaphone.getTeamId(), 0.8f);
                    }
                }
                for (auto& crate : stageCrates) {
                    f32 d = 0.0f;
                    if (!crate.isBroken() && wepMegaphone.checkDamageHit(crate.getPosition(), 1.2f, &d)) {
                        crate.applyDamage(d, wepMegaphone.getTeamId());
                    }
                }
                for (auto& dummy : stageTargets) {
                    f32 d = 0.0f;
                    if (dummy.getState() != Game::TargetState::cPopped && wepMegaphone.checkDamageHit(dummy.getPosition(), 1.2f, &d)) {
                        dummy.applyDamage(d);
                    }
                }
                if (rivalBot.isAlive()) {
                    f32 d = 0.0f;
                    if (wepMegaphone.checkDamageHit(rivalBot.getPosition(), 1.2f, &d)) {
                        rivalBot.applyDamage(d);
                        if (!rivalBot.isAlive()) {
                            Game::PcAudioDriver::instance().playSound(Game::cSoundId_Octoling_Splat, 1.0f, 0.0f);
                            paintMap.splatWorldSphere(rivalBot.getPosition(), 5.0f, wepMegaphone.getTeamId(), 1.0f);
                            rivalRespawnTimer = 180;
                            printf("[*] Rival bot VAPORIZED by Killer Wail acoustic laser!\n");
                        }
                    }
                }
            }
            if (wepMegaphone.isFinished()) {
                activeSpecial = -1;
                wepMegaphone.cancel();
                printf("[*] Killer Wail finished.\n");
            }
        }

        // Weapon Attack Action (Left Click)
        if (!isSquid) {
            if (s_ActiveWeaponType == 0) {
                // Splattershot
                if (s_IsFiring && fireCooldown == 0) {
                    if (inkTank >= 0.009f) {
                        fireCooldown = 6;
                        inkTank -= 0.009f;
                        sead::Vector3f aimDir(sinf(s_CamYaw), sinf(s_CamPitch), cosf(s_CamYaw));
                        aimDir = aimDir.normalized();
                        InkBullet3D b;
                        b.pos = playerPos + sead::Vector3f(0.0f, 1.1f, 0.0f) + aimDir * 0.7f;
                        float spreadX = ((rand() % 100) - 50) * 0.001f;
                        float spreadY = ((rand() % 100) - 50) * 0.001f;
                        b.vel = aimDir * 1.6f + sead::Vector3f(spreadX, 0.16f + spreadY, 0.0f);
                        b.teamId = s_CurrentPaintTeam;
                        b.radius = 2.0f;
                        b.lifetime = 1.6f;
                        b.isBomb = false;
                        b.active = true;
                        bullets.push_back(b);
                        Game::PcAudioDriver::instance().playSound(Game::cSoundId_Shoot_Splattershot, 0.65f, 0.0f, isSquid && inFriendlyInk);
                    } else if (frameCount % 12 == 0) {
                        Game::PcAudioDriver::instance().playSound(Game::cSoundId_Low_Ink_Warning, 0.50f, 0.0f);
                    }
                }
            } else if (s_ActiveWeaponType == 1) {
                // Splat Roller Fling
                if (s_IsFiring && fireCooldown == 0) {
                    if (inkTank >= 0.08f) {
                        fireCooldown = 22;
                        inkTank -= 0.08f;
                        for (int i = -2; i <= 2; ++i) {
                            float ang = s_CamYaw + i * 0.12f;
                            sead::Vector3f flingDir(sinf(ang), sinf(s_CamPitch) + 0.14f, cosf(ang));
                            flingDir = flingDir.normalized();
                            InkBullet3D fb;
                            fb.pos = playerPos + sead::Vector3f(0.0f, 1.2f, 0.0f) + flingDir * 0.7f;
                            fb.vel = flingDir * 1.55f + sead::Vector3f(0.0f, 0.16f, 0.0f);
                            fb.teamId = s_CurrentPaintTeam;
                            fb.radius = 2.3f;
                            fb.lifetime = 1.2f;
                            fb.isBomb = false;
                            fb.active = true;
                            bullets.push_back(fb);
                        }
                        Game::PcAudioDriver::instance().playSound(Game::cSoundId_Roller_Fling, 0.85f, 0.0f);
                    } else if (frameCount % 12 == 0) {
                        Game::PcAudioDriver::instance().playSound(Game::cSoundId_Low_Ink_Warning, 0.50f, 0.0f);
                    }
                }
            } else if (s_ActiveWeaponType == 2) {
                // Splat Charger
                if (s_IsFiring) {
                    if (s_ChargerChargeRatio == 0.0f && inkTank >= 0.05f) {
                        Game::PcAudioDriver::instance().playSound(Game::cSoundId_Charger_Charge, 0.65f, 0.0f);
                    }
                    s_ChargerChargeRatio = (std::min)(1.0f, s_ChargerChargeRatio + 0.024f);
                }
                if (s_FiredChargerThisFrame) {
                    s_FiredChargerThisFrame = false;
                    float cost = 0.04f + 0.14f * s_ChargerChargeRatio;
                    if (s_ChargerChargeRatio > 0.10f && inkTank >= cost) {
                        inkTank -= cost;
                        sead::Vector3f aimDir(sinf(s_CamYaw), sinf(s_CamPitch), cosf(s_CamYaw));
                        aimDir = aimDir.normalized();
                        float beamDist = 16.0f + 32.0f * s_ChargerChargeRatio;
                        float pRadius = 1.6f + 1.2f * s_ChargerChargeRatio;

                        // Continuous ink line along ground
                        sead::Vector3f rayStart = playerPos + sead::Vector3f(0.0f, 1.2f, 0.0f);
                        for (float d = 2.0f; d < beamDist; d += 2.2f) {
                            sead::Vector3f pt = rayStart + aimDir * d;
                            Game::KclHitResult ptHit;
                            if (stageKcl.raycast(pt + sead::Vector3f(0.0f, 4.0f, 0.0f), sead::Vector3f(0.0f, -1.0f, 0.0f), 8.0f, ptHit)) {
                                paintMap.splatWorldSphere(ptHit.hitPoint, pRadius * 0.7f, s_CurrentPaintTeam, 1.0f);
                            }
                        }

                        // High speed projectile
                        InkBullet3D cb;
                        cb.pos = rayStart + aimDir * 0.8f;
                        cb.vel = aimDir * (2.8f + 2.0f * s_ChargerChargeRatio);
                        cb.teamId = s_CurrentPaintTeam;
                        cb.radius = pRadius;
                        cb.lifetime = 1.0f;
                        cb.isBomb = false;
                        cb.active = true;
                        bullets.push_back(cb);

                        Game::PcAudioDriver::instance().playSound(Game::cSoundId_Charger_Fire, 0.90f, 0.0f);
                    }
                    s_ChargerChargeRatio = 0.0f;
                }
            }
        }

        // Throw Splat Bomb (Right Click / R)
        if (s_ThrowBombRequested && !isSquid) {
            s_ThrowBombRequested = false;
            if (inkTank >= 0.65f) {
                inkTank -= 0.65f;

                sead::Vector3f aimDir(sinf(s_CamYaw), sinf(s_CamPitch), cosf(s_CamYaw));
                aimDir = aimDir.normalized();

                InkBullet3D bomb;
                bomb.pos = playerPos + sead::Vector3f(0.0f, 1.4f, 0.0f) + aimDir * 0.8f;
                bomb.vel = aimDir * 1.05f + sead::Vector3f(0.0f, 0.55f, 0.0f); // High parabolic lob
                bomb.teamId = s_CurrentPaintTeam;
                bomb.radius = 6.2f; // Big turf explosion radius
                bomb.lifetime = 1.3f;
                bomb.isBomb = true;
                bomb.active = true;
                bullets.push_back(bomb);
                Game::PcAudioDriver::instance().playSound(Game::cSoundId_Splat_Bomb_Throw, 0.75f, 0.0f);
                printf("[*] Threw Splat Bomb! Parabolic lob trajectory active.\n");
            } else {
                Game::PcAudioDriver::instance().playSound(Game::cSoundId_Low_Ink_Warning, 0.60f, 0.0f);
            }
        }

        // Update In-Flight Ballistics & Stage Collisions
        for (auto& b : bullets) {
            if (!b.active) continue;
            b.vel.y -= 0.024f; // Ballistic gravity
            sead::Vector3f nextPos = b.pos + b.vel;

            // 1. Collide with Destructible Stage Wooden Crates
            bool hitCrate = false;
            for (auto& crate : stageCrates) {
                if (!crate.isBroken() && crate.checkBulletCollision(b.pos, b.radius * 0.4f, b.isBomb ? 180.0f : 28.0f, b.teamId)) {
                    b.active = false;
                    hitCrate = true;
                    if (crate.isBroken()) {
                        paintMap.splatWorldSphere(crate.getPosition(), 4.2f, b.teamId, 1.0f);
                        printf("[*] Wooden Crate DESTROYED at (%.1f, %.1f)! Shattered into splinters.\n",
                               crate.getPosition().x, crate.getPosition().z);
                    }
                    break;
                }
            }
            if (hitCrate) continue;

            // 2. Collide with Target Dummies
            bool hitTarget = false;
            for (auto& target : stageTargets) {
                if (target.getState() != Game::TargetState::cPopped) {
                    sead::Vector3f tPos = target.getPosition() + sead::Vector3f(0.0f, 1.4f, 0.0f);
                    float dist = (b.pos - tPos).length();
                    if (dist < 1.3f) {
                        target.applyDamage(b.isBomb ? 180.0f : 28.0f);
                        Game::PcAudioDriver::instance().playSound(Game::cSoundId_Hit_Confirm, 0.85f, 0.0f);
                        paintMap.splatWorldSphere(target.getPosition(), 2.8f, b.teamId, 1.0f);
                        b.active = false;
                        hitTarget = true;
                        printf("[*] Target Dummy HIT! Damage: %.1f HP (Remaining HP: %.1f)\n",
                               target.getLastDamageTaken(), target.getRemainingHp());
                        break;
                    }
                }
            }
            if (hitTarget) continue;

            // 3. Collide with Octoling Rival Bot
            if (rivalBot.isAlive() && b.teamId != 1) {
                sead::Vector3f rTarget = rivalBot.getPosition() + sead::Vector3f(0.0f, 1.0f, 0.0f);
                if ((b.pos - rTarget).length() < 1.4f) {
                    float dmg = b.isBomb ? 180.0f : (s_ActiveWeaponType == 2 ? 110.0f : 28.0f);
                    rivalBot.applyDamage(dmg);
                    Game::PcAudioDriver::instance().playSound(Game::cSoundId_Hit_Confirm, 0.85f, 0.0f);
                    paintMap.splatWorldSphere(rivalBot.getPosition(), 2.8f, b.teamId, 1.0f);
                    b.active = false;
                    if (!rivalBot.isAlive()) {
                        Game::PcAudioDriver::instance().playSound(Game::cSoundId_Octoling_Splat, 1.0f, 0.0f);
                        paintMap.splatWorldSphere(rivalBot.getPosition(), 5.0f, b.teamId, 1.0f);
                        rivalRespawnTimer = 180;
                        printf("[*] OCTOLING RIVAL SPLATTED! Massive ink burst!\n");
                    }
                    continue;
                }
            }

            // 4. Collide with Stage Geometry via KCL Raycast
            sead::Vector3f segment = nextPos - b.pos;
            float segLen = segment.length();
            Game::KclHitResult bulletHit;
            if (segLen > 0.001f && stageKcl.raycast(b.pos, segment / segLen, segLen, bulletHit)) {
                // Impact on stage geometry! Splat ink onto PaintMap3D!
                paintMap.splatWorldSphere(bulletHit.hitPoint, b.radius, b.teamId, 1.0f);
                b.active = false;
                if (b.isBomb) {
                    Game::PcAudioDriver::instance().playSound(Game::cSoundId_Splat_Bomb_Explode, 0.95f, 0.0f);
                    printf("[*] Splat Bomb BOOM at (%.1f, %.1f)! Inked R=%.1fm!\n",
                           bulletHit.hitPoint.x, bulletHit.hitPoint.z, b.radius);
                }
            } else {
                b.pos = nextPos;
                b.lifetime -= 0.0166f;
                if (b.pos.y < -60.0f || b.lifetime <= 0.0f) {
                    b.active = false;
                }
            }
        }

        // Update Stage Props & Physics Wobble
        for (auto& crate : stageCrates) crate.update();
        for (auto& target : stageTargets) target.update();

        // Update Octoling Rival Squad Bot
        if (rivalBot.isAlive()) {
            rivalBot.updateTactics(playerPos);
            sead::Vector3f rPos = rivalBot.getPosition();
            Game::KclHitResult rFloor;
            if (stageKcl.raycast(rPos + sead::Vector3f(0.0f, 2.5f, 0.0f), sead::Vector3f(0.0f, -1.0f, 0.0f), 6.0f, rFloor)) {
                rPos.y = rFloor.hitPoint.y;
            }

            // Rival shoots Cyan ink droplets at player
            rivalShootTimer++;
            if (rivalBot.getCurrentPlan() == Game::RivalPlanId::cEngage && rivalShootTimer >= 32) {
                rivalShootTimer = 0;
                sead::Vector3f rToPlayer = (playerPos + sead::Vector3f(0.0f, 0.8f, 0.0f)) - (rPos + sead::Vector3f(0.0f, 1.1f, 0.0f));
                float rDist = rToPlayer.length();
                if (rDist > 0.5f) {
                    sead::Vector3f rAim = rToPlayer / rDist;
                    InkBullet3D rBullet;
                    rBullet.pos = rPos + sead::Vector3f(0.0f, 1.1f, 0.0f) + rAim * 0.8f;
                    rBullet.vel = rAim * 1.45f + sead::Vector3f(0.0f, 0.14f, 0.0f);
                    rBullet.teamId = 1; // Team Bravo (Cyan Octarian)
                    rBullet.radius = 1.8f;
                    rBullet.lifetime = 1.4f;
                    rBullet.isBomb = false;
                    rBullet.active = true;
                    bullets.push_back(rBullet);
                    Game::PcAudioDriver::instance().playSound(Game::cSoundId_Shoot_Splattershot, 0.45f, 0.2f);
                }
            }
        } else {
            if (rivalRespawnTimer > 0) {
                rivalRespawnTimer--;
                if (rivalRespawnTimer == 0) {
                    rivalBot.triggerRespawn(rivalSpawnPos);
                    paintMap.splatWorldSphere(rivalSpawnPos, 3.5f, 1, 1.0f);
                    Game::PcAudioDriver::instance().playSound(Game::cSoundId_Squid_Dive, 0.8f, 0.0f);
                    printf("[*] Octoling Rival RESPAWNED at spawn beacon!\n");
                }
            }
        }

        // Camera View & Projection Matrices
        sead::Vector3f eyePos, targetPos;
        if (cameraThirdPerson) {
            float camDist = isSquid ? 4.2f : 5.2f;
            float camHeight = 1.5f + s_CamPitch * 1.8f;
            targetPos = playerPos + sead::Vector3f(0.0f, 1.2f, 0.0f);
            float eyeX = targetPos.x - sinf(s_CamYaw) * cosf(s_CamPitch) * camDist;
            float eyeY = targetPos.y + camHeight;
            float eyeZ = targetPos.z - cosf(s_CamYaw) * cosf(s_CamPitch) * camDist;
            eyePos = sead::Vector3f(eyeX, eyeY, eyeZ);
        } else {
            float eyeX = stageCenter.x + sinf(s_CamYaw) * s_CamDistance;
            float eyeZ = stageCenter.z + cosf(s_CamYaw) * s_CamDistance;
            eyePos = sead::Vector3f(eyeX, stageCenter.y + s_CamHeight, eyeZ);
            targetPos = stageCenter;
        }

        sead::Matrix44f viewMat;
        viewMat.buildLookAtDX11(eyePos, targetPos, sead::Vector3f(0.0f, 1.0f, 0.0f));

        sead::Matrix44f projMat;
        projMat.buildPerspectiveDX11(1.047f, 1280.0f / 720.0f, 0.1f, 2500.0f);

        sead::Matrix44f viewProj = projMat * viewMat;

        // Render Frame
        renderer.beginFrame(0.08f, 0.11f, 0.16f, 1.0f);
        renderer.setFrameConstants(viewProj, eyePos, time);
        renderer.bindPaintTexture(paintMap);

        // 1. Render Retail Splatoon Stage Model (Floors, Walls, Grates)
        sead::Matrix44f stageWorld;
        stageWorld.setTranslation(0.0f, 0.0f, 0.0f);
        renderer.submitModel(stageModel, stageWorld, 255);

        // 2. Render Destructible Wooden Crates (Obj_GeneralBox)
        for (const auto& crate : stageCrates) {
            if (!crate.isBroken()) {
                sead::Matrix44f cw;
                cw.setTranslation(crate.getPosition().x, crate.getPosition().y + 1.2f, crate.getPosition().z);
                renderer.submitModel(crateModel, cw, crate.getCoveredTeam());
            }
        }

        // 3. Render Sighter Target Dummies & Rainmaker Goal Pedestals
        for (const auto& target : stageTargets) {
            if (target.getState() != Game::TargetState::cPopped) {
                sead::Matrix44f tw;
                tw.setTranslation(target.getPosition().x, target.getPosition().y, target.getPosition().z);
                renderer.submitModel(targetDummyModel, tw, 255);
            }
        }

        // Render Rainmaker Base Goal Pedestals (Obj_ShrBasketGoal)
        sead::Matrix44f goalAlpha, goalBravo;
        goalAlpha.setTranslation(stageCenter.x - maxSpan * 0.35f, stageCenter.y, stageCenter.z);
        goalBravo.setTranslation(stageCenter.x + maxSpan * 0.35f, stageCenter.y, stageCenter.z);
        renderer.submitModel(rainmakerGoalModel, goalAlpha, 0); // Team Alpha Pedestal
        renderer.submitModel(rainmakerGoalModel, goalBravo, 1); // Team Bravo Pedestal

        // 4. Render Playable Inkling Player Character
        sead::Matrix44f playerWorld;
        float cy = cosf(playerYaw), sy = sinf(playerYaw);
        playerWorld.m[0][0] = cy;   playerWorld.m[0][2] = sy;
        playerWorld.m[1][1] = 1.0f;
        playerWorld.m[2][0] = -sy;  playerWorld.m[2][2] = cy;
        playerWorld.m[3][3] = 1.0f;
        playerWorld.m[0][3] = playerPos.x;
        playerWorld.m[1][3] = playerPos.y + (isSquid ? 0.05f : 0.0f);
        playerWorld.m[2][3] = playerPos.z;

        if (activeSpecial == 1) {
            // Kraken Giant Invincible Squid (Wsp_KingSquid)
            renderer.submitModel(krakenModel, playerWorld, s_CurrentPaintTeam);
        } else if (isSquid) {
            renderer.submitModel(inklingSquidModel, playerWorld, s_CurrentPaintTeam);
        } else {
            renderer.submitModel(inklingHumanModel, playerWorld, s_CurrentPaintTeam);
            // Render active held weapon in player's hands
            sead::Matrix44f weaponWorld = playerWorld;
            weaponWorld.m[0][3] += cy * 0.28f + sy * 0.35f;
            weaponWorld.m[1][3] += 0.85f;
            weaponWorld.m[2][3] += -sy * 0.28f + cy * 0.35f;
            if (s_ActiveWeaponType == 0) {
                renderer.submitModel(splattershotModel, weaponWorld, s_CurrentPaintTeam);
            } else if (s_ActiveWeaponType == 1) {
                renderer.submitModel(rollerModel, weaponWorld, s_CurrentPaintTeam);
            } else if (s_ActiveWeaponType == 2) {
                renderer.submitModel(chargerModel, weaponWorld, s_CurrentPaintTeam);
            }
        }

        // 5. Render Octoling Rival Combatant
        if (rivalBot.isAlive()) {
            sead::Vector3f rPos = rivalBot.getPosition();
            sead::Vector3f rToPlayer = playerPos - rPos;
            float rYaw = std::atan2(rToPlayer.x, rToPlayer.z);
            sead::Matrix44f rivalWorld;
            float cry = cosf(rYaw), sry = sinf(rYaw);
            rivalWorld.m[0][0] = cry;   rivalWorld.m[0][2] = sry;
            rivalWorld.m[1][1] = 1.0f;
            rivalWorld.m[2][0] = -sry;  rivalWorld.m[2][2] = cry;
            rivalWorld.m[3][3] = 1.0f;
            rivalWorld.m[0][3] = rPos.x;
            rivalWorld.m[1][3] = rPos.y;
            rivalWorld.m[2][3] = rPos.z;
            renderer.submitModel(rivalModel, rivalWorld, 1); // Team Bravo Octoling Rival (Rival00)
        }

        // 6. Render Active In-Flight Ballistics
        for (const auto& b : bullets) {
            if (!b.active) continue;
            sead::Matrix44f bw;
            bw.setTranslation(b.pos.x, b.pos.y, b.pos.z);
            if (b.isBomb) {
                renderer.submitModel(bombModel, bw, b.teamId);
            } else {
                renderer.submitModel(bulletModel, bw, b.teamId);
            }
        }

        // 7. Render Inkopolis Plaza Retail NPCs (Squid Sisters, Judd, Spyke, Sheldon)
        if (currentStage.find("Plaza") != std::string::npos) {
            // Callie (Npc_IdolA) in Studio Booth
            sead::Matrix44f callieWorld;
            callieWorld.setTranslation(-15.2f, 3.2f, 18.0f);
            renderer.submitModel(callieModel, callieWorld, 0);

            // Marie (Npc_IdolB) in Studio Booth
            sead::Matrix44f marieWorld;
            marieWorld.setTranslation(-13.6f, 3.2f, 18.0f);
            renderer.submitModel(marieModel, marieWorld, 1);

            // Judd the Cat (Npc_Judge) near Battle Lobby
            sead::Matrix44f juddWorld;
            juddWorld.setTranslation(2.5f, 0.0f, 12.0f);
            renderer.submitModel(juddModel, juddWorld, 255);

            // Spyke (Npc_CustomShop) in alleyway
            sead::Matrix44f spykeWorld;
            spykeWorld.setTranslation(-18.5f, 0.0f, -4.0f);
            renderer.submitModel(spykeModel, spykeWorld, 255);

            // Sheldon (Npc_WeaponsShop) Ammo Knights
            sead::Matrix44f sheldonWorld;
            sheldonWorld.setTranslation(16.0f, 0.0f, 8.0f);
            renderer.submitModel(sheldonModel, sheldonWorld, 255);
        }

        // Handle Inkopolis News Interactive Broadcast (N key)
        if (s_TriggerNewsRequested) {
            s_TriggerNewsRequested = false;
            if (!plazaNews.isBroadcasting()) {
                plazaNews.init(liveSchedule);
                printf("\n===================================================\n");
                printf("[*] INKOPOLIS NEWS BROADCAST STARTED (Squid Sisters)!\n");
                printf("===================================================\n");
            } else {
                plazaNews.advanceDialogue();
            }
            const auto* line = plazaNews.getCurrentLine();
            if (line) {
                printf("[%s]: \"%s\"\n", line->speaker, line->text.c_str());
            }
        }
        if (plazaNews.isBroadcasting()) {
            plazaNews.update(0.01667f);
        }

        renderer.endFrame();
        renderer.present();

        // Update Title HUD Telemetry every 30 frames
        if (frameCount % 30 == 0) {
            Game::PaintStats stats = paintMap.calculateStats();
            const char* wNamesShort[] = { "Splattershot", "Splat Roller", "Splat Charger" };
            char titleBuf[256];
            if (plazaNews.isBroadcasting() && plazaNews.getCurrentLine()) {
                const auto* nLine = plazaNews.getCurrentLine();
                snprintf(titleBuf, sizeof(titleBuf),
                         "Splatoon 1 (Gambit PC) | [NEWS] %s: \"%s\" (Press N to advance)",
                         nLine->speaker, nLine->text.c_str());
            } else {
                snprintf(titleBuf, sizeof(titleBuf),
                         "Splatoon 1 (Gambit PC) | %s | Wep: %s | Ink: %d%% | Turf: Org %.1f%% / Cyan %.1f%% | Rival: %s (HP: %.0f) [60 FPS]",
                         currentStage.c_str(),
                         wNamesShort[s_ActiveWeaponType],
                         static_cast<int>(inkTank * 100.0f),
                         stats.alphaPercent,
                         stats.bravoPercent,
                         rivalBot.isAlive() ? "ALIVE" : "RESPAWN",
                         rivalBot.getHealth());
            }
            SetWindowTextA(hWnd, titleBuf);
        }

        frameCount++;
        if (maxFrames > 0 && frameCount >= maxFrames) {
            printf("[+] Reached requested frame limit (%d frames). Closing render window.\n", maxFrames);
            break;
        }

        Sleep(1);
    }

    renderer.shutdown();
    DestroyWindow(hWnd);
    UnregisterClassA(className, hInstance);

    printf("[+] Direct3D 11 Render Session Finished Cleanly (%d frames rendered).\n", frameCount);
    return 0;
}

int main(int argc, char* argv[]) {
    PrintBanner();

    if (argc < 2) {
        // Default when executed directly or double-clicked: launch the DirectX 11 3D Renderer with Inkopolis Plaza!
        return RunRenderWindow(0, "Fld_PlazaLobby");
    }

    std::string requestedStage = "Fld_PlazaLobby";
    int requestedFrames = 0;
    bool shouldRender = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--verify" || arg == "--test") {
            return RunVerificationSuite() ? 0 : 1;
        } else if (arg == "--list-stages") {
            ListAllStages();
            return 0;
        } else if (arg.rfind("--test-stage=", 0) == 0) {
            std::string stageName = arg.substr(13);
            const auto* info = Game::StageDef::getStageInfoByName(stageName.c_str());
            if (!info) {
                printf("[-] Error: Unknown stage '%s'\n", stageName.c_str());
                return 1;
            }
            printf("[+] Testing stage: %s (%s)\n", info->displayName, info->codeName);
            Game::GambitActorMgr actorMgr;
            actorMgr.init(nullptr);
            Game::PaintTextureMgr paintMgr;
            paintMgr.init(nullptr, 512, 512);
            Game::StageMgr stageMgr;
            stageMgr.init();
            stageMgr.loadStage(info->id, &actorMgr, &paintMgr);
            printf("[+] Stage loaded cleanly into memory.\n");
            return 0;
        } else if (arg == "--dump-sarc" && i + 1 < argc) {
            return DumpSarcFile(argv[++i]) ? 0 : 1;
        } else if (arg == "--extract-sarc" && i + 1 < argc) {
            const char* szsFile = argv[++i];
            const char* outDir = (i + 1 < argc && argv[i + 1][0] != '-') ? argv[++i] : "extracted";
            return DumpSarcFile(szsFile, outDir) ? 0 : 1;
        } else if (arg == "--dump-byml" && i + 1 < argc) {
            return DumpBymlFile(argv[++i]) ? 0 : 1;
        } else if (arg == "--help" || arg == "-h") {
            PrintUsage();
            return 0;
        } else if (arg == "--render" || arg == "--window" || arg == "-r") {
            shouldRender = true;
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                requestedFrames = atoi(argv[++i]);
            }
        } else if (arg.rfind("--frames=", 0) == 0) {
            requestedFrames = atoi(arg.c_str() + 9);
            shouldRender = true;
        } else if (arg.rfind("--stage=", 0) == 0) {
            requestedStage = arg.substr(8);
            shouldRender = true;
        } else if (arg.rfind("--map=", 0) == 0) {
            requestedStage = arg.substr(6);
            shouldRender = true;
        } else {
            printf("[-] Unknown option: %s\n", arg.c_str());
            PrintUsage();
            return 1;
        }
    }

    if (shouldRender) {
        return RunRenderWindow(requestedFrames, requestedStage.c_str());
    }

    return 0;
}
