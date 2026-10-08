#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class SlosherState : u32 {
    cIdle       = 0,
    cSwingThrow = 1,
    cRecovery   = 2
};

class GameWeaponSlosher : public GambitActor {
public:
    GameWeaponSlosher();
    virtual ~GameWeaponSlosher() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void triggerSlosh();

    SlosherState getState() const { return mState; }
    f32 getDirectDamage() const { return mDirectDamage; }

protected:
    void spawnInkVolley();

    SlosherState mState;
    s32 mStateTimer;

    f32 mDirectDamage;
    f32 mSplashDamage;
    f32 mArcHeight;
    f32 mRange;

    undefined mReserved[0x38];
};

} // namespace Game
