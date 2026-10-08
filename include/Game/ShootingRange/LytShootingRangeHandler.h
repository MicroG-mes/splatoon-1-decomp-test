#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "Game/ShootingRange/Fld_ShootingRange_Shr.h"
#include "cafe/vpad.h"

namespace Game {

class LytShootingRangeHandler : public GambitActor {
public:
    LytShootingRangeHandler();
    virtual ~LytShootingRangeHandler() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void handleInput(const VPADStatus& vpad);

    void recordDamageDealt(f32 damage);
    f32 getCurrentDps() const { return mCurrentDps; }
    f32 getTotalDamageDealt() const { return mTotalDamage; }

    bool isExitRequested() const { return mIsExitRequested; }

protected:
    Fld_ShootingRange_Shr mField;

    f32 mTotalDamage;
    f32 mCurrentDps;
    f32 mDamageInWindow;
    s32 mDpsWindowTimer;

    bool mIsExitRequested;
    s32 mAnimTimer;

    undefined mReserved[0x38];
};

} // namespace Game
