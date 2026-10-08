#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class ChargerState : u32 {
    cIdle       = 0,
    cCharging   = 1,
    cFullCharge = 2,
    cFiring     = 3
};

class GameWeaponCharger : public GambitActor {
public:
    GameWeaponCharger();
    virtual ~GameWeaponCharger() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void startCharging();
    void releaseCharge();

    ChargerState getState() const { return mState; }
    f32 getChargeRatio() const { return mChargeRatio; }
    bool isFullyCharged() const { return mState == ChargerState::cFullCharge; }

    f32 computeDamage() const;
    f32 computeRange() const;

protected:
    void fireShot();

    ChargerState mState;
    s32 mStateTimer;

    f32 mChargeRatio; // 0.0 to 1.0
    s32 mFramesToFullCharge;

    f32 mBaseDamage;
    f32 mMaxDamage;
    f32 mBaseRange;
    f32 mMaxRange;

    bool mIsLaserActive;
    undefined mReserved[0x38];
};

} // namespace Game
