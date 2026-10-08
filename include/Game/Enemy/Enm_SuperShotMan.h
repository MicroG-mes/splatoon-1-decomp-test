#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class OctosniperState : u32 {
    cScanningSight = 0,
    cChargingShot  = 1,
    cFireSnipe     = 2,
    cDuckCover     = 3,
    cHitStagger    = 4,
    cDefeated      = 5
};

class Enm_SuperShotMan : public GambitActor {
public:
    static constexpr f32 cMaxHp = 80.0f;
    static constexpr f32 cMaxLaserDistance = 35.0f;
    static constexpr s32 cChargeDuration = 60; // 1 second laser lock
    static constexpr s32 cCoverDuration = 120; // 2 seconds reloading behind bunker

    Enm_SuperShotMan();
    virtual ~Enm_SuperShotMan() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void updateAi(const sead::Vector3f& playerPos, bool hasLineOfSight);
    void applyDamage(f32 damage);

    OctosniperState getState() const { return mState; }
    f32 getHp() const { return mHp; }
    bool isLaserLocked() const { return mState == OctosniperState::cChargingShot; }
    bool isCovered() const { return mState == OctosniperState::cDuckCover; }
    const sead::Vector3f& getAimTarget() const { return mAimTarget; }
    const sead::Vector3f& getPosition() const { return mPosition; }

    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    void dischargeSniperBullet();

    sead::Vector3f mPosition;
    sead::Vector3f mAimTarget;
    f32 mHp;
    OctosniperState mState;
    s32 mStateTimer;

    undefined mReserved[0x38];
};

} // namespace Game
