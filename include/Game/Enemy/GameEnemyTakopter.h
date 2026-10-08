#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * GameEnemyTakopter
 * Flying Twintacle Octotrooper with propeller flight AI
 */
class GameEnemyTakopter : public GambitActor {
public:
    GameEnemyTakopter();
    virtual ~GameEnemyTakopter() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

protected:
    undefined mModuleData[0x40];
};

} // namespace Game
