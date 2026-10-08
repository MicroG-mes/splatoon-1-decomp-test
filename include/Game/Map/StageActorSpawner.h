#pragma once

#include "types.h"
#include "sead/math/seadVector.h"
#include <string>

namespace Game {

class GambitActorMgr;
class GambitActor;

// UnitConfig entry parsed from stage BYML / UnitConfig data
struct StageUnitConfig {
    std::string unitConfigName;  // e.g. "Obj_AirWall", "Obj_Seesaw", "Rails", "ScrewLiftVs"
    sead::Vector3f position;
    sead::Vector3f rotation;
    sead::Vector3f scale;
    s32 teamId;
    s32 param0;
    s32 param1;
    bool isScrew;
    bool isHiagariFloat;
};

class StageActorSpawner {
public:
    // Authentic decompiled dispatcher (recovered from 0x021102c4)
    static GambitActor* spawnUnitConfigActor(
        const StageUnitConfig& config,
        GambitActorMgr* actorMgr
    );
};

} // namespace Game
