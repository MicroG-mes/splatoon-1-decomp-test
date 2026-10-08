#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

struct KettleInfo {
    u32 missionId;
    sead::Vector3f pos;
    bool isDiscovered;
    bool isCleared;
    bool hasSunkenScroll;
};

class Fld_World : public GambitActor {
public:
    Fld_World();
    virtual ~Fld_World() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void selectSector(u32 sectorIndex); // 0 to 4
    void discoverKettle(u32 missionId);
    void clearKettle(u32 missionId, bool foundScroll);

    u32 getCurrentSector() const { return mCurrentSector; }
    bool isSectorBossUnlocked(u32 sectorIndex) const;
    u32 getTotalZapfishRescued() const;

    KettleInfo* getKettle(u32 missionId);

protected:
    u32 mCurrentSector;
    KettleInfo mKettles[28]; // 27 missions + 1 final boss
    s32 mStateTimer;

    undefined mReserved[0x38];
};

} // namespace Game
