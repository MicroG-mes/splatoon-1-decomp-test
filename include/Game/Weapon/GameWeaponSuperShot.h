#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * GameWeaponSuperShot
 * Inkzooka special weapon launcher
 */
class GameWeaponSuperShot : public GambitActor {
public:
    GameWeaponSuperShot();
    virtual ~GameWeaponSuperShot() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

protected:
    undefined mModuleData[0x40];
};

} // namespace Game
