#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * GameInkRail
 * Rideable ink rail spline path and squid sliding interaction
 */
class GameInkRail : public GambitActor {
public:
    GameInkRail();
    virtual ~GameInkRail() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

protected:
    undefined mModuleData[0x40];
};

} // namespace Game
