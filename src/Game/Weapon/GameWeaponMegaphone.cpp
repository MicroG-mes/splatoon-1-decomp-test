#include "Game/Weapon/GameWeaponMegaphone.h"
#include <cmath>

namespace Game {

GameWeaponMegaphone::GameWeaponMegaphone()
    : mPosition(0.0f, 0.0f, 0.0f),
      mDirection(0.0f, 0.0f, 1.0f),
      mYaw(0.0f),
      mTimer(0),
      mTeamId(0),
      mIsDeployed(false),
      mIsFiring(false),
      mIsFinished(false) {
}

GameWeaponMegaphone::~GameWeaponMegaphone() = default;

void GameWeaponMegaphone::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mDirection.set(0.0f, 0.0f, 1.0f);
    mYaw = 0.0f;
    mTimer = 0;
    mTeamId = 0;
    mIsDeployed = false;
    mIsFiring = false;
    mIsFinished = false;
}

void GameWeaponMegaphone::deploy(u32 teamId, const sead::Vector3f& deployPos, float yaw) {
    mTeamId = teamId;
    mPosition = deployPos;
    mYaw = yaw;
    mDirection.set(sinf(yaw), 0.0f, cosf(yaw));
    mTimer = 0;
    mIsDeployed = true;
    mIsFiring = false;
    mIsFinished = false;
}

void GameWeaponMegaphone::cancel() {
    mIsDeployed = false;
    mIsFiring = false;
    mIsFinished = true;
}

f32 GameWeaponMegaphone::getWarmupProgress() const {
    if (!mIsDeployed) return 0.0f;
    if (mTimer >= cWarmupDuration) return 1.0f;
    return static_cast<f32>(mTimer) / static_cast<f32>(cWarmupDuration);
}

f32 GameWeaponMegaphone::getBlastProgress() const {
    if (!mIsFiring) return 0.0f;
    s32 blastTimer = mTimer - cWarmupDuration;
    if (blastTimer < 0) return 0.0f;
    if (blastTimer >= cBlastDuration) return 1.0f;
    return static_cast<f32>(blastTimer) / static_cast<f32>(cBlastDuration);
}

bool GameWeaponMegaphone::checkDamageHit(const sead::Vector3f& targetPos, f32 targetRadius, f32* outDamage) const {
    if (!mIsFiring) return false;

    // Vector from megaphone to target
    sead::Vector3f toTarget = targetPos - mPosition;

    // Project along megaphone forward direction
    f32 projDist = toTarget.x * mDirection.x + toTarget.y * mDirection.y + toTarget.z * mDirection.z;
    if (projDist < 0.0f || projDist > cBeamLength) {
        return false;
    }

    // Closest point on line
    sead::Vector3f linePoint = mPosition + mDirection * projDist;
    f32 perpDistSq = (targetPos.x - linePoint.x) * (targetPos.x - linePoint.x) +
                     (targetPos.y - linePoint.y) * (targetPos.y - linePoint.y) +
                     (targetPos.z - linePoint.z) * (targetPos.z - linePoint.z);

    f32 maxDist = cBeamRadius + targetRadius;
    if (perpDistSq <= maxDist * maxDist) {
        if (outDamage) {
            *outDamage = cDamagePerFrame;
        }
        return true;
    }
    return false;
}

void GameWeaponMegaphone::update() {
    GambitActor::update();

    if (!mIsDeployed || mIsFinished) return;

    mTimer++;

    if (mTimer < cWarmupDuration) {
        mIsFiring = false;
    } else if (mTimer < (cWarmupDuration + cBlastDuration)) {
        mIsFiring = true;
    } else {
        mIsFiring = false;
        mIsFinished = true;
    }
}

void GameWeaponMegaphone::draw() {
    GambitActor::draw();
}

} // namespace Game
