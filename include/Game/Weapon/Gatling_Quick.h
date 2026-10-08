#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class GatlingState : u32 {
    cIdle        = 0,
    cCharging    = 1,
    cFiring      = 2,
    cCooldown    = 3
};

class Gatling_Quick : public GambitActor {
public:
    static constexpr f32 cMaxCharge = 2.0f;       // 2 charge rings
    static constexpr f32 cChargeRate = 0.035f;    // Charge speed per frame
    static constexpr f32 cInkPerBullet = 2.25f;
    static constexpr f32 cBulletDamage = 28.0f;
    static constexpr s32 cFireIntervalFrames = 4;

    Gatling_Quick();
    virtual ~Gatling_Quick() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void startCharging();
    void releaseTrigger();
    bool canShoot() const;

    GatlingState getState() const { return mState; }
    f32 getChargeLevel() const { return mChargeLevel; }
    f32 getBarrelSpinSpeed() const { return mBarrelSpinSpeed; }
    u32 getRemainingBullets() const { return mRemainingBullets; }
    bool isFullyCharged() const { return mChargeLevel >= cMaxCharge; }

protected:
    void fireBullet();

    GatlingState mState;
    f32 mChargeLevel;
    f32 mBarrelSpinSpeed;
    u32 mRemainingBullets;
    s32 mShotTimer;
    u32 mTeamId;

    undefined mReserved[0x38];
};

} // namespace Game
