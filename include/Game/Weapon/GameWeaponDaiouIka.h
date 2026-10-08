#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * GameWeaponDaiouIka
 * Kraken special weapon transformation and spinning attack
 */
class GameWeaponDaiouIka : public GambitActor {
public:
    GameWeaponDaiouIka();
    virtual ~GameWeaponDaiouIka() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

protected:
    undefined mModuleData[0x40];
};

} // namespace Game
