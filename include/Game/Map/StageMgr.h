#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "Game/Actor/GambitActorMgr.h"
#include "Game/Map/StageDef.h"
#include "Game/Paint/PaintTextureMgr.h"
#include "Game/MapObj/RespawnPos.h"
#include "Game/MapObj/Obj_Sponge.h"
#include "Game/MapObj/Obj_Grate.h"
#include "Game/MapObj/RollingLift.h"
#include "Game/MapObj/InkRail.h"
#include "Game/Rule/AreaPole.h"
#include "Game/Rule/Obj_QuarryBeltYagura.h"
#include "Game/Rule/GachihokoClone.h"
#include "Game/Map/GameDrcMap.h"

namespace Game {

class StageMgr : public GambitActor {
public:
    static StageMgr* instance() { return sInstance; }

    StageMgr();
    virtual ~StageMgr() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    bool loadStage(StageId id, GambitActorMgr* actorMgr, PaintTextureMgr* paintMgr);
    bool loadStageByCodeName(const char* codeName, GambitActorMgr* actorMgr, PaintTextureMgr* paintMgr);
    bool loadStageByName(const char* name, GambitActorMgr* actorMgr, PaintTextureMgr* paintMgr);

    void unloadCurrentStage(GambitActorMgr* actorMgr);

    const StageInfo* getCurrentStageInfo() const { return mCurrentStage; }
    StageId getCurrentStageId() const { return mCurrentStage ? mCurrentStage->id : StageId::cStage_Warehouse00_Vss; }

    // Spawn points
    const sead::Vector3f& getSpawnAlpha() const { return mCurrentStage->spawnAlpha; }
    const sead::Vector3f& getSpawnBravo() const { return mCurrentStage->spawnBravo; }
    const sead::Vector3f& getObjectiveCenter() const { return mCurrentStage->objectiveCenter; }

    GameDrcMap* getDrcMap() { return &mDrcMap; }
    const GameDrcMap* getDrcMap() const { return &mDrcMap; }

protected:
    void populateStageActors(GambitActorMgr* actorMgr);

    static StageMgr* sInstance;
    const StageInfo* mCurrentStage;
    GameDrcMap mDrcMap;

    // Stage runtime actor instances
    RespawnPos*          mSpawnAlphaActor;
    RespawnPos*          mSpawnBravoActor;
    Obj_Sponge*          mSpongeActor;
    RollingLift*         mRollingLiftActor;
    Obj_Grate*           mGrateActor;
    InkRail*             mInkRailActor;
    AreaPole*            mAreaPoleActor;
    Obj_QuarryBeltYagura* mTowerActor;
    GachihokoClone*      mRainmakerActor;
};

} // namespace Game
