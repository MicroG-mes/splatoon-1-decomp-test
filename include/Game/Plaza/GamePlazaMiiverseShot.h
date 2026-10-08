#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * GamePlazaMiiverseShot
 * Inkopolis Plaza Miiverse post billboard renderer
 */
class GamePlazaMiiverseShot : public GambitActor {
public:
    GamePlazaMiiverseShot();
    virtual ~GamePlazaMiiverseShot() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

protected:
    undefined mModuleData[0x40];
};

} // namespace Game
