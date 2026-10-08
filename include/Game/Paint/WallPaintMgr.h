#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * WallPaintMgr
 * Vertical surface and 3D wall ink texture projection manager
 */
class WallPaintMgr : public GambitActor {
public:
    WallPaintMgr();
    virtual ~WallPaintMgr() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

protected:
    undefined mModuleData[0x40];
};

} // namespace Game
