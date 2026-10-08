#include "Game/Map/StageActorSpawner.h"
#include "Game/Actor/GambitActorMgr.h"
#include "Game/MapObj/RespawnPos.h"
#include "Game/Rule/AreaPole.h"
#include "Game/Rule/Obj_QuarryBeltYagura.h"
#include "Game/MapObj/Obj_Sponge.h"
#include "Game/MapObj/RollingLift.h"
#include "Game/MapObj/Obj_Grate.h"
#include "Game/MapObj/InkRail.h"
#include <cstdio>

namespace Game {

// Authentic decompilation of FUN_021102c4 actor spawner
GambitActor* StageActorSpawner::spawnUnitConfigActor(
    const StageUnitConfig& config,
    GambitActorMgr* actorMgr
) {
    const std::string& name = config.unitConfigName;

    // Check "Rails" (s_Rails_1001f93c)
    if (name == "Rails" || name == "Obj_InkRail") {
        InkRail* rail = new InkRail();
        rail->init();
        rail->setPosition(config.position);
        rail->setRotation(config.rotation);
        rail->setupSpline(config.position, config.position + sead::Vector3f(15.0f, 6.0f, 15.0f));
        rail->activateByInk(static_cast<u32>(config.teamId));
        if (actorMgr) actorMgr->registerActor(rail);
        return rail;
    }

    // Check "TestObj_Lift" / "ScrewLiftVs" / "ScrewLift" (0x1001fa44, 0x1001f9a4, 0x1001f9fc)
    if (name == "TestObj_Lift" || name == "ScrewLiftVs" || name == "ScrewLift" || name == "RollingLift") {
        RollingLift* lift = new RollingLift();
        lift->init();
        lift->setPosition(config.position);
        lift->setRotation(config.rotation);
        sead::Vector3f endPt = config.position + sead::Vector3f(config.scale.x > 0.0f ? config.scale.x * 10.0f : 15.0f, 0.0f, 0.0f);
        lift->setTrackWaypoints(config.position, endPt);
        if (actorMgr) actorMgr->registerActor(lift);
        return lift;
    }

    // Check "Obj_Sponge" / "TestObj_Container"
    if (name == "Obj_Sponge" || name == "TestObj_Container") {
        Obj_Sponge* sponge = new Obj_Sponge();
        sponge->init();
        sponge->setPosition(config.position);
        sponge->setRotation(config.rotation);
        if (actorMgr) actorMgr->registerActor(sponge);
        return sponge;
    }

    // Check "Obj_Grate"
    if (name == "Obj_Grate") {
        Obj_Grate* grate = new Obj_Grate();
        grate->init();
        grate->setupGrate(config.position, config.scale.x, config.scale.z);
        if (actorMgr) actorMgr->registerActor(grate);
        return grate;
    }

    // Check "AreaPole" (Splat Zones objective)
    if (name == "AreaPole" || name == "AreaGoal") {
        AreaPole* pole = new AreaPole();
        pole->init();
        pole->setPosition(config.position);
        if (actorMgr) actorMgr->registerActor(pole);
        return pole;
    }

    // Check "Obj_QuarryBeltYagura" / "Conveyer"
    if (name == "Obj_QuarryBeltYagura" || name == "Conveyer") {
        Obj_QuarryBeltYagura* tower = new Obj_QuarryBeltYagura();
        tower->init();
        tower->setPosition(config.position);
        tower->setRotation(config.rotation);
        if (actorMgr) actorMgr->registerActor(tower);
        return tower;
    }

    // Check "RespawnPos" / "PlayerSpawnPoint"
    if (name == "RespawnPos" || name == "PlayerSpawnPoint") {
        RespawnPos* spawn = new RespawnPos();
        spawn->init();
        spawn->setPosition(config.position);
        spawn->setRotation(config.rotation);
        spawn->setupSpawn(config.position, static_cast<u32>(config.teamId));
        if (actorMgr) actorMgr->registerActor(spawn);
        return spawn;
    }

    // Default / generic map object actor
    GambitActor* actor = new GambitActor();
    actor->init();
    actor->setPosition(config.position);
    actor->setRotation(config.rotation);
    if (actorMgr) actorMgr->registerActor(actor);
    return actor;
}

} // namespace Game
