#include "Game/Weapon/Wsp_Shachihoko.h"
#include <algorithm>
#include <cmath>

namespace Game {

RainmakerParams::RainmakerParams()
    : mOffsetY(1.2f)
    , mGachihokoColRadius(1.5f)
    , mVictoryPlayerTimeLimitFrame(3600) // 60 seconds
    , mBarrierRadius(3.2f)
    , mBarrierMaxScale(1.4f)
    , mIsBarrierReject(true)
    , mBarrierBoundVelLen(12.0f)
    , mBarrierBoundVelY(6.5f)
    , mBombCorePaintRadius(6.8f)
    , mBombCoreDamageRadius(5.5f)
    , mBombCoreDamage(180.0f) // OHKO
    , mCrossPaintRayLength(18.0f)
    , mCrossPaintRadius(2.4f)
    , mHikikomoriCountdownSpeed(2) // 2x countdown speed when retreating/camping
    , mHikikomoriStartFrame(300)   // 5 seconds grace period
    , mTryGetGachihokoFrame(60)    // 1.0s pickup animation
    , mReallocateWaitFrame(180)    // 3.0s reset timer
{
}

Wsp_Shachihoko::Wsp_Shachihoko()
    : mPosition(0.0f, 0.0f, 0.0f)
    , mInitPos(0.0f, 0.0f, 0.0f)
    , mState(ShachihokoShieldState::cShieldCharging)
    , mCarrierPlayerId(-1)
    , mCarrierTeamId(-1)
    , mCarrierTimer(cCarrierTimeLimit)
    , mHikikomoriGraceFrames(0)
    , mShieldAlphaHp(0.0f)
    , mShieldBravoHp(0.0f)
    , mBurstTimer(0)
    , mResetTimerFrames(0)
    , mChargeRatio(0.0f)
    , mIsCharging(false)
    , mExpiredExplosionTriggered(false)
{
}

Wsp_Shachihoko::~Wsp_Shachihoko() {
}

void Wsp_Shachihoko::init() {
    init(sead::Vector3f(0.0f, 0.0f, 0.0f));
}

void Wsp_Shachihoko::init(const sead::Vector3f& initialSpawnPos) {
    mInitPos = initialSpawnPos;
    mPosition = initialSpawnPos;
    mState = ShachihokoShieldState::cShieldCharging;
    mCarrierPlayerId = -1;
    mCarrierTeamId = -1;
    mCarrierTimer = cCarrierTimeLimit;
    mHikikomoriGraceFrames = 0;
    mShieldAlphaHp = 0.0f;
    mShieldBravoHp = 0.0f;
    mBurstTimer = 0;
    mResetTimerFrames = 0;
    mChargeRatio = 0.0f;
    mIsCharging = false;
    mExpiredExplosionTriggered = false;
}

void Wsp_Shachihoko::draw() {
    // Rendered via DX11 mesh pipeline
}

void Wsp_Shachihoko::applyInkToShield(s32 teamId, f32 amount) {
    if (mState != ShachihokoShieldState::cShieldCharging) return;

    if (teamId == 0) {
        mShieldAlphaHp += amount;
        mShieldBravoHp = (std::max)(0.0f, mShieldBravoHp - amount * 0.5f);
        if (mShieldAlphaHp >= cBurstThreshold) {
            triggerShieldBurst(0);
        }
    } else {
        mShieldBravoHp += amount;
        mShieldAlphaHp = (std::max)(0.0f, mShieldAlphaHp - amount * 0.5f);
        if (mShieldBravoHp >= cBurstThreshold) {
            triggerShieldBurst(1);
        }
    }
}

void Wsp_Shachihoko::triggerShieldBurst(s32 winningTeam) {
    mState = ShachihokoShieldState::cShieldBursting;
    mBurstTimer = 15; // 15 frames burst shockwave animation
}

bool Wsp_Shachihoko::pickup(u32 playerId, s32 teamId) {
    if (mState != ShachihokoShieldState::cFreePickup && mState != ShachihokoShieldState::cShieldBursting) {
        return false;
    }
    mState = ShachihokoShieldState::cCarried;
    mCarrierPlayerId = static_cast<s32>(playerId);
    mCarrierTeamId = teamId;
    mCarrierTimer = cCarrierTimeLimit;
    mHikikomoriGraceFrames = 0;
    mChargeRatio = 0.0f;
    mIsCharging = false;
    mExpiredExplosionTriggered = false;
    return true;
}

void Wsp_Shachihoko::drop(const sead::Vector3f& dropPos) {
    mState = ShachihokoShieldState::cFreePickup;
    mPosition = dropPos;
    mCarrierPlayerId = -1;
    mCarrierTeamId = -1;
    mChargeRatio = 0.0f;
    mIsCharging = false;
    mResetTimerFrames = 0;
}

void Wsp_Shachihoko::forceReset() {
    mState = ShachihokoShieldState::cShieldCharging;
    mPosition = mInitPos;
    mCarrierPlayerId = -1;
    mCarrierTeamId = -1;
    mShieldAlphaHp = 0.0f;
    mShieldBravoHp = 0.0f;
    mCarrierTimer = cCarrierTimeLimit;
    mExpiredExplosionTriggered = false;
}

void Wsp_Shachihoko::update() {
    updateAdvanced(0.01667f, false, nullptr);
}

void Wsp_Shachihoko::updateAdvanced(f32 deltaTime, bool inHikikomoriArea, PaintMap3D* paintMap) {
    if (mState == ShachihokoShieldState::cShieldBursting) {
        mBurstTimer--;
        if (mBurstTimer <= 0) {
            mState = ShachihokoShieldState::cFreePickup;
        }
    } else if (mState == ShachihokoShieldState::cCarried) {
        s32 drainSpeed = 1;

        if (inHikikomoriArea) {
            mHikikomoriGraceFrames++;
            if (mHikikomoriGraceFrames > mParams.mHikikomoriStartFrame) {
                // Camping in own base: timer ticks 2x faster!
                drainSpeed = static_cast<s32>(mParams.mHikikomoriCountdownSpeed);
            }
        } else {
            mHikikomoriGraceFrames = 0;
        }

        mCarrierTimer -= drainSpeed;
        if (mCarrierTimer <= 0) {
            mCarrierTimer = 0;
            mExpiredExplosionTriggered = true;

            if (paintMap) {
                u32 enemyTeam = (mCarrierTeamId == 0) ? 1 : 0;
                paintMap->splatWorldSphere(mPosition, mParams.mBombCorePaintRadius, enemyTeam, 1.0f);
            }

            forceReset();
        }
    } else if (mState == ShachihokoShieldState::cFreePickup) {
        mResetTimerFrames++;
        if (mResetTimerFrames > 900) { // 15 seconds dropped -> resets
            forceReset();
        }
    }
}

bool Wsp_Shachihoko::startCharge() {
    if (mState != ShachihokoShieldState::cCarried) return false;
    mIsCharging = true;
    mChargeRatio = 0.0f;
    return true;
}

void Wsp_Shachihoko::updateCharge(f32 deltaTime) {
    if (!mIsCharging) return;
    mChargeRatio = (std::min)(1.0f, mChargeRatio + deltaTime * 1.0f);
}

ShachihokoProjectile Wsp_Shachihoko::releaseShot(const sead::Vector3f& muzzlePos, const sead::Vector3f& dir) {
    ShachihokoProjectile p;
    p.pos = muzzlePos;
    p.chargeLevel = mChargeRatio;
    p.teamId = (mCarrierTeamId >= 0) ? static_cast<u32>(mCarrierTeamId) : 0;
    p.lifeTimer = 0.0f;
    p.active = true;
    p.hasExploded = false;

    // Linear interpolation based on charge level
    f32 speed = 18.0f + mChargeRatio * 14.0f; // 18.0 to 32.0 m/s
    p.vel = dir.normalized() * speed;
    p.radius = 0.45f + mChargeRatio * 0.75f;
    p.damage = 60.0f + mChargeRatio * 120.0f; // 60.0 to 180.0 HP (OHKO lethal at full charge)
    p.blastRadius = 2.2f + mChargeRatio * 3.3f; // 2.2m to 5.5m blast

    mIsCharging = false;
    mChargeRatio = 0.0f;

    return p;
}

bool Wsp_Shachihoko::updateProjectile(
    ShachihokoProjectile& proj,
    const KclFile& stageKcl,
    PaintMap3D* paintMap,
    f32 deltaTime,
    sead::Vector3f* outBurstPos
) {
    if (!proj.active || proj.hasExploded) return false;

    proj.vel.y -= 18.0f * deltaTime;
    sead::Vector3f step = proj.vel * deltaTime;
    f32 stepDist = step.length();
    sead::Vector3f stepDir = (stepDist > 0.001f) ? (step * (1.0f / stepDist)) : sead::Vector3f(0.0f, -1.0f, 0.0f);

    // Continuous paint trail on ground beneath ink tornado
    if (paintMap) {
        paintMap->splatWorldSphere(proj.pos, proj.radius * 1.8f, proj.teamId, 0.85f);
    }

    // Raycast collision against stage geometry
    KclHitResult hit;
    if (stageKcl.raycast(proj.pos, stepDir, stepDist + proj.radius, hit)) {
        proj.pos = hit.hitPoint;
        proj.hasExploded = true;
        proj.active = false;

        if (outBurstPos) {
            *outBurstPos = hit.hitPoint;
        }

        if (paintMap) {
            paintMap->splatWorldSphere(hit.hitPoint, proj.blastRadius, proj.teamId, 1.0f);
        }
        return true;
    }

    proj.pos += step;
    proj.lifeTimer += deltaTime;

    if (proj.lifeTimer > 3.5f) {
        proj.hasExploded = true;
        proj.active = false;
        if (paintMap) {
            paintMap->splatWorldSphere(proj.pos, proj.blastRadius, proj.teamId, 1.0f);
        }
        return true;
    }

    return false;
}

} // namespace Game
