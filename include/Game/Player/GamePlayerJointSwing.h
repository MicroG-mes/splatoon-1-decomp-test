#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * GamePlayerJointSwing
 * Secondary physics simulation for hair tentacles and gear sway
 */
class GamePlayerJointSwing : public GambitActor {
public:
    GamePlayerJointSwing();
    virtual ~GamePlayerJointSwing() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

protected:
    undefined mModuleData[0x40];
};

} // namespace Game
