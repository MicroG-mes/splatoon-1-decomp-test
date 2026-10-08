#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * GamePlayerJointHuman
 * Humanoid bone joint and animation controller
 */
class GamePlayerJointHuman : public GambitActor {
public:
    GamePlayerJointHuman();
    virtual ~GamePlayerJointHuman() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

protected:
    undefined mModuleData[0x40];
};

} // namespace Game
