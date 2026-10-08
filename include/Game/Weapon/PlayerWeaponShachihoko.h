#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class ShachihokoWeaponState : u32 {
    cIdle     = 0,
    cCharging = 1,
    cFired    = 2,
    cRecoil   = 3
};

class PlayerWeaponShachihoko : public GambitActor {
public:
    static constexpr f32 cMaxChargeTime = 70.0f;
    static constexpr f32 cMaxShotDistance = 25.0f;
    static constexpr f32 cFullChargeDamage = 180.0f;
    static constexpr f32 cSpeedPenaltyMultiplier = 0.80f;

    PlayerWeaponShachihoko();
    virtual ~PlayerWeaponShachihoko() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void startCharging(u32 teamId);
    void releaseTrigger(const sead::Vector3f& muzzlePos, f32 yawAngle);

    ShachihokoWeaponState getState() const { return mState; }
    f32 getChargeProgress() const { return mChargeFrames / cMaxChargeTime; }
    bool isFullyCharged() const { return mChargeFrames >= cMaxChargeTime; }
    f32 getSpeedPenalty() const { return cSpeedPenaltyMultiplier; }

protected:
    void dischargeTornadoShot(const sead::Vector3f& muzzlePos, f32 yawAngle, f32 chargeRatio);

    ShachihokoWeaponState mState;
    f32 mChargeFrames;
    s32 mRecoilTimer;
    u32 mTeamId;

    undefined mReserved[0x38];
};

} // namespace Game
