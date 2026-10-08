#pragma once

#include "types.h"
#include "Game/Item/GameItemBase.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * GameItemShachihoko
 * Rainmaker golden fish objective item and carrier charge shot
 */
class GameItemShachihoko : public GameItemBase {
public:
    GameItemShachihoko();
    virtual ~GameItemShachihoko() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

protected:
    undefined mModuleData[0x40];
};

} // namespace Game
