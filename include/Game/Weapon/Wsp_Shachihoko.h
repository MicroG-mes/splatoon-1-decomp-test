#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include "Game/Collision/KclFile.h"
#include "Game/Paint/PaintMap3D.h"
#include <string>

namespace Game {

enum class ShachihokoShieldState : u32 {
    cShieldCharging = 0,
    cShieldBursting = 1,
    cFreePickup     = 2,
    cCarried        = 3,
    cResetting      = 4
};

// Authentic Retail Parameters decompiled from GameGachihoko.cpp (Address 02530f84)
struct RainmakerParams {
    f32 mOffsetY;                          // 1.2m
    f32 mGachihokoColRadius;               // 1.5m
    u32 mVictoryPlayerTimeLimitFrame;      // 3600 frames (60.0s countdown)
    f32 mBarrierRadius;                    // 3.2m
    f32 mBarrierMaxScale;                  // 1.4f
    bool mIsBarrierReject;                 // true
    f32 mBarrierBoundVelLen;               // 12.0f
    f32 mBarrierBoundVelY;                 // 6.5f
    f32 mBombCorePaintRadius;              // 6.8m
    f32 mBombCoreDamageRadius;             // 5.5m
    f32 mBombCoreDamage;                   // 180.0 HP (OHKO lethal damage)
    f32 mCrossPaintRayLength;              // 18.0m
    f32 mCrossPaintRadius;                 // 2.4m
    u32 mHikikomoriCountdownSpeed;         // 2x countdown speed when retreating/camping
    u32 mHikikomoriStartFrame;             // 300 frames (5.0s grace period)
    u32 mTryGetGachihokoFrame;             // 60 frames (1.0s pickup animation)
    u32 mReallocateWaitFrame;              // 180 frames (3.0s out-of-bounds reset)

    RainmakerParams();
};

struct ShachihokoProjectile {
    sead::Vector3f pos;
    sead::Vector3f vel;
    f32 chargeLevel;    // 0.0 to 1.0
    f32 radius;
    f32 damage;
    f32 blastRadius;
    u32 teamId;
    f32 lifeTimer;
    bool active;
    bool hasExploded;
};

class Wsp_Shachihoko : public GambitActor {
public:
    static constexpr f32 cBurstThreshold = 500.0f;
    static constexpr f32 cBurstDamage = 180.0f; // OHKO
    static constexpr f32 cBurstRadius = 6.8f;
    static constexpr s32 cCarrierTimeLimit = 3600; // 60 seconds at 60fps

    Wsp_Shachihoko();
    virtual ~Wsp_Shachihoko() override;

    virtual void init() override;
    void init(const sead::Vector3f& initialSpawnPos);
    virtual void update() override;
    virtual void draw() override;

    // Shield Inking & Mechanics
    void applyInkToShield(s32 teamId, f32 amount);
    bool pickup(u32 playerId, s32 teamId);
    void drop(const sead::Vector3f& dropPos);
    void forceReset();

    // Advanced Frame Update with Base Camping Detection & Inking
    void updateAdvanced(f32 deltaTime, bool inHikikomoriArea, PaintMap3D* paintMap = nullptr);

    // Rainmaker Weapon Charged Shot
    bool startCharge();
    void updateCharge(f32 deltaTime);
    ShachihokoProjectile releaseShot(const sead::Vector3f& muzzlePos, const sead::Vector3f& dir);

    // Projectile Ballistics & Stage Collision
    static bool updateProjectile(ShachihokoProjectile& proj, const KclFile& stageKcl,
                                 PaintMap3D* paintMap, f32 deltaTime,
                                 sead::Vector3f* outBurstPos = nullptr);

    // Queries
    ShachihokoShieldState getShieldState() const { return mState; }
    s32 getCarrierPlayerId() const { return mCarrierPlayerId; }
    s32 getCarrierTeamId() const { return mCarrierTeamId; }
    s32 getRemainingCarrierFrames() const { return mCarrierTimer; }
    f32 getRemainingTimeSeconds() const { return mCarrierTimer / 60.0f; }
    f32 getShieldAlphaHp() const { return mShieldAlphaHp; }
    f32 getShieldBravoHp() const { return mShieldBravoHp; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

    f32 getChargeRatio() const { return mChargeRatio; }
    bool isExpiredExplosion() const { return mExpiredExplosionTriggered; }
    bool canSuperJump() const { return false; } // Carrier cannot Super Jump

    const RainmakerParams& getParams() const { return mParams; }

protected:
    void triggerShieldBurst(s32 winningTeam);

    RainmakerParams mParams;
    sead::Vector3f mPosition;
    sead::Vector3f mInitPos;
    ShachihokoShieldState mState;
    s32 mCarrierPlayerId; // -1 if not carried
    s32 mCarrierTeamId;   // -1 if not carried
    s32 mCarrierTimer;
    u32 mHikikomoriGraceFrames;

    f32 mShieldAlphaHp;
    f32 mShieldBravoHp;
    s32 mBurstTimer;
    u32 mResetTimerFrames;

    f32 mChargeRatio;
    bool mIsCharging;
    bool mExpiredExplosionTriggered;

    undefined mReserved[0x38];
};

} // namespace Game
