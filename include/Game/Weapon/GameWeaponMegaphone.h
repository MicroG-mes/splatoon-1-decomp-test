#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * GameWeaponMegaphone
 * Killer Wail acoustic ink laser special weapon
 */
class GameWeaponMegaphone : public GambitActor {
public:
    GameWeaponMegaphone();
    virtual ~GameWeaponMegaphone() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

protected:
    undefined mModuleData[0x40];
};

} // namespace Game
