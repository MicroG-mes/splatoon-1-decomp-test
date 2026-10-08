#include "Game/Map/StageMgr.h"
#include "Game/Map/MapTable.h"
#include <cstdio>

namespace Game {

StageMgr* StageMgr::sInstance = nullptr;

StageMgr::StageMgr()
    : mCurrentStage(nullptr),
      mSpawnAlphaActor(nullptr),
      mSpawnBravoActor(nullptr),
      mSpongeActor(nullptr),
      mRollingLiftActor(nullptr),
      mGrateActor(nullptr),
      mInkRailActor(nullptr),
      mAreaPoleActor(nullptr),
      mTowerActor(nullptr),
      mRainmakerActor(nullptr) {
    sInstance = this;
}

StageMgr::~StageMgr() {
    if (sInstance == this) {
        sInstance = nullptr;
    }
}

void StageMgr::init() {
    GambitActor::init();
    mCurrentStage = StageDef::getStageInfo(StageId::cStage_Warehouse00_Vss);
}

void StageMgr::update() {
    GambitActor::update();
}

void StageMgr::draw() {
    GambitActor::draw();
}

void StageMgr::unloadCurrentStage(GambitActorMgr* actorMgr) {
    if (!actorMgr) return;

    if (mSpawnAlphaActor)  { actorMgr->unregisterActor(mSpawnAlphaActor);  delete mSpawnAlphaActor;  mSpawnAlphaActor = nullptr; }
    if (mSpawnBravoActor)  { actorMgr->unregisterActor(mSpawnBravoActor);  delete mSpawnBravoActor;  mSpawnBravoActor = nullptr; }
    if (mSpongeActor)      { actorMgr->unregisterActor(mSpongeActor);      delete mSpongeActor;      mSpongeActor = nullptr; }
    if (mRollingLiftActor) { actorMgr->unregisterActor(mRollingLiftActor); delete mRollingLiftActor; mRollingLiftActor = nullptr; }
    if (mGrateActor)       { actorMgr->unregisterActor(mGrateActor);       delete mGrateActor;       mGrateActor = nullptr; }
    if (mInkRailActor)     { actorMgr->unregisterActor(mInkRailActor);     delete mInkRailActor;     mInkRailActor = nullptr; }
    if (mAreaPoleActor)    { actorMgr->unregisterActor(mAreaPoleActor);    delete mAreaPoleActor;    mAreaPoleActor = nullptr; }
    if (mTowerActor)       { actorMgr->unregisterActor(mTowerActor);       delete mTowerActor;       mTowerActor = nullptr; }
    if (mRainmakerActor)   { actorMgr->unregisterActor(mRainmakerActor);   delete mRainmakerActor;   mRainmakerActor = nullptr; }
}

void StageMgr::populateStageActors(GambitActorMgr* actorMgr) {
    if (!actorMgr || !mCurrentStage) return;

    // 1. Team Spawn Bases
    mSpawnAlphaActor = new RespawnPos();
    mSpawnAlphaActor->init();
    mSpawnAlphaActor->setPosition(mCurrentStage->spawnAlpha);
    actorMgr->registerActor(mSpawnAlphaActor);

    mSpawnBravoActor = new RespawnPos();
    mSpawnBravoActor->init();
    mSpawnBravoActor->setPosition(mCurrentStage->spawnBravo);
    actorMgr->registerActor(mSpawnBravoActor);

    // 2. Stage-Specific Mechanics & Objectives
    if (mCurrentStage->category == StageCategory::cVersus) {
        // Splat Zones objective pole at center
        mAreaPoleActor = new AreaPole();
        mAreaPoleActor->init();
        mAreaPoleActor->setPosition(mCurrentStage->objectiveCenter);
        mAreaPoleActor->setZoneRadius(7.0f);
        actorMgr->registerActor(mAreaPoleActor);

        // Tower Control riding along stage longitudinal axis
        mTowerActor = new Obj_QuarryBeltYagura();
        mTowerActor->init();
        mTowerActor->setPosition(mCurrentStage->objectiveCenter);
        actorMgr->registerActor(mTowerActor);

        // Stage Props
        mSpongeActor = new Obj_Sponge();
        mSpongeActor->init();
        mSpongeActor->setPosition(sead::Vector3f(
            mCurrentStage->objectiveCenter.x + 8.0f,
            mCurrentStage->objectiveCenter.y,
            mCurrentStage->objectiveCenter.z - 6.0f
        ));
        actorMgr->registerActor(mSpongeActor);

        mRollingLiftActor = new RollingLift();
        mRollingLiftActor->init();
        mRollingLiftActor->setTrackWaypoints(
            sead::Vector3f(mCurrentStage->objectiveCenter.x - 10.0f, mCurrentStage->objectiveCenter.y + 1.0f, mCurrentStage->objectiveCenter.z),
            sead::Vector3f(mCurrentStage->objectiveCenter.x + 10.0f, mCurrentStage->objectiveCenter.y + 1.0f, mCurrentStage->objectiveCenter.z)
        );
        actorMgr->registerActor(mRollingLiftActor);

        mGrateActor = new Obj_Grate();
        mGrateActor->init();
        mGrateActor->setupGrate(
            sead::Vector3f(mCurrentStage->objectiveCenter.x, mCurrentStage->objectiveCenter.y + 3.0f, mCurrentStage->objectiveCenter.z + 8.0f),
            8.0f, 6.0f
        );
        actorMgr->registerActor(mGrateActor);

        mInkRailActor = new InkRail();
        mInkRailActor->init();
        mInkRailActor->setupSpline(
            mCurrentStage->spawnAlpha,
            sead::Vector3f(mCurrentStage->objectiveCenter.x - 5.0f, mCurrentStage->objectiveCenter.y + 4.0f, mCurrentStage->objectiveCenter.z)
        );
        mInkRailActor->activateByInk(0);
        actorMgr->registerActor(mInkRailActor);
    }
}

bool StageMgr::loadStage(StageId id, GambitActorMgr* actorMgr, PaintTextureMgr* paintMgr) {
    const StageInfo* info = StageDef::getStageInfo(id);
    if (!info) return false;

    printf("[+] StageMgr: Loading Stage [%s] (%s) - Category: %u\n",
           info->codeName, info->displayName, static_cast<u32>(info->category));

    // Teardown previous stage actors
    unloadCurrentStage(actorMgr);

    mCurrentStage = info;

    // Initialize DRC Minimap subsystem
    const MapParam* mapParam = MapTable::getInstance()->findStageById(static_cast<s32>(id));
    mDrcMap.init(mapParam, 0);

    // Reset paint texture canvas
    if (paintMgr) {
        paintMgr->clear();
        // Seed team home ink patches near spawn bases
        for (float r = 0.0f; r < 5.0f; r += 1.2f) {
            paintMgr->paintSplat(
                sead::Vector3f(info->spawnAlpha.x + r, info->spawnAlpha.y, info->spawnAlpha.z + r),
                3.5f, PaintColor::TeamAlpha
            );
            paintMgr->paintSplat(
                sead::Vector3f(info->spawnBravo.x - r, info->spawnBravo.y, info->spawnBravo.z - r),
                3.5f, PaintColor::TeamBravo
            );
        }
    }

    // Spawn new stage actors
    populateStageActors(actorMgr);

    printf("[+] StageMgr: Stage [%s] Loaded Successfully! (Bounds: [%.1f, %.1f] to [%.1f, %.1f])\n",
           info->displayName,
           info->boundsMin.x, info->boundsMin.z,
           info->boundsMax.x, info->boundsMax.z);
    return true;
}

bool StageMgr::loadStageByCodeName(const char* codeName, GambitActorMgr* actorMgr, PaintTextureMgr* paintMgr) {
    const StageInfo* info = StageDef::getStageInfoByCodeName(codeName);
    if (!info) return false;
    return loadStage(info->id, actorMgr, paintMgr);
}

bool StageMgr::loadStageByName(const char* name, GambitActorMgr* actorMgr, PaintTextureMgr* paintMgr) {
    const StageInfo* info = StageDef::getStageInfoByName(name);
    if (!info) return false;
    return loadStage(info->id, actorMgr, paintMgr);
}

} // namespace Game
