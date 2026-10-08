#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * GamePlayerJointSquid
 * Squid form bone joint and tentacle deformation controller
 */
class GamePlayerJointSquid : public GambitActor {
public:
    GamePlayerJointSquid();
    virtual ~GamePlayerJointSquid() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

protected:
    undefined mModuleData[0x40];
};

} // namespace Game
