#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class BlasterType : u32 {
    cNormal = 0,     // Standard Blaster / Custom Blaster (125.0 direct, 50.0-70.0 splash)
    cRange  = 1,     // Range Blaster (125.0 direct, 50.0-70.0 splash, long range)
    cRapid  = 2,     // Rapid Blaster (85.0 direct, 17.5-35.0 splash)
    cLuna   = 3      // Luna Blaster (125.0 direct, 50.0-70.0 splash, short range wide blast)
};

// Decompiled from PPC: BulletPlayerNormalExplosionShotBase @ 0x0225C9EC, 0x0225C8F0
class BulletPlayerNormalExplosionShotBase : public GambitActor {
public:
    BulletPlayerNormalExplosionShotBase();
    virtual ~BulletPlayerNormalExplosionShotBase() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void fireBlaster(BlasterType type, const sead::Vector3f& pos, const sead::Vector3f& dir, u32 teamId);
    void triggerAirBurst();

    f32 computeExplosionDamageAtDistance(f32 distFromBlastCenter) const;
    f32 computeDamageAgainstTarget(u32 targetType, f32 distFromBlastCenter, u32 targetTeamId) const;

    bool isBurst() const { return mIsBurst; }
    f32 getDirectDamage() const { return mDirectDamage; }
    f32 getMaxSplashDamage() const { return mMaxSplashDamage; }
    f32 getMinSplashDamage() const { return mMinSplashDamage; }
    f32 getBlastRadius() const { return mBlastRadius; }
    u32 getTeamId() const { return mTeamId; }
    const sead::Vector3f& getPosition() const { return mPosition; }

private:
    BlasterType mType;
    sead::Vector3f mPosition;
    sead::Vector3f mVelocity;
    u32 mTeamId;
    f32 mDirectDamage;
    f32 mMaxSplashDamage;
    f32 mMinSplashDamage;
    f32 mBlastRadius;
    f32 mShieldMultiplier;
    f32 mBeakonMultiplier;
    s32 mAirBurstTimer;
    s32 mMaxFlightFrames;
    bool mIsBurst;
};

} // namespace Game
