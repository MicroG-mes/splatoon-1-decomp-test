#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "Game/ShootingRange/SighterTarget.h"

namespace Game {

enum class ShootingRangeState : u32 {
    cWaitSceneFade = 0,
    cProgress      = 1,
    cExitToShop    = 2
};

class Fld_ShootingRange_Shr : public GambitActor {
public:
    Fld_ShootingRange_Shr();
    virtual ~Fld_ShootingRange_Shr() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void resetAllInk();
    void exitToShop();

    ShootingRangeState getState() const { return mState; }
    bool isExitRequested() const { return mState == ShootingRangeState::cExitToShop; }

    SighterTarget* getTarget(u32 index);
    u32 getTargetCount() const { return 5; }

protected:
    ShootingRangeState mState;
    s32 mStateTimer;

    SighterTarget mTargets[5];

    undefined mReserved[0x38];
};

} // namespace Game
