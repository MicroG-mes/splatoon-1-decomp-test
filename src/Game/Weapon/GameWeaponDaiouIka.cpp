#include "Game/Weapon/GameWeaponDaiouIka.h"

namespace Game {

GameWeaponDaiouIka::GameWeaponDaiouIka()
    : mPosition(0.0f, 0.0f, 0.0f),
      mDurationTimer(0),
      mSpinTimer(0),
      mCooldownTimer(0),
      mTeamId(0),
      mIsActive(false),
      mIsSpinning(false) {
}

GameWeaponDaiouIka::~GameWeaponDaiouIka() = default;

void GameWeaponDaiouIka::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mDurationTimer = 0;
    mSpinTimer = 0;
    mCooldownTimer = 0;
    mTeamId = 0;
    mIsActive = false;
    mIsSpinning = false;
}

void GameWeaponDaiouIka::activate(u32 teamId, const sead::Vector3f& startPos) {
    mTeamId = teamId;
    mPosition = startPos;
    mDurationTimer = cDurationFrames;
    mSpinTimer = 0;
    mCooldownTimer = 0;
    mIsActive = true;
    mIsSpinning = false;
}

void GameWeaponDaiouIka::deactivate() {
    mIsActive = false;
    mDurationTimer = 0;
    mSpinTimer = 0;
    mIsSpinning = false;
}

bool GameWeaponDaiouIka::triggerSpinAttack() {
    if (!mIsActive || mSpinTimer > 0 || mCooldownTimer > 0) {
        return false;
    }
    mIsSpinning = true;
    mSpinTimer = cSpinDuration;
    mCooldownTimer = cSpinCooldown;
    return true;
}

bool GameWeaponDaiouIka::checkSpinDamage(const sead::Vector3f& targetPos, f32 targetRadius, f32* outDamage) const {
    if (!mIsActive || !mIsSpinning) {
        return false;
    }
    sead::Vector3f delta = targetPos - mPosition;
    f32 distSq = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
    f32 maxDist = cSpinRadius + targetRadius;
    if (distSq <= maxDist * maxDist) {
        if (outDamage) {
            *outDamage = cSpinDamage;
        }
        return true;
    }
    return false;
}

void GameWeaponDaiouIka::update() {
    GambitActor::update();

    if (!mIsActive) return;

    if (mDurationTimer > 0) {
        mDurationTimer--;
        if (mDurationTimer == 0) {
            deactivate();
            return;
        }
    }

    if (mCooldownTimer > 0) {
        mCooldownTimer--;
    }

    if (mSpinTimer > 0) {
        mSpinTimer--;
        if (mSpinTimer == 0) {
            mIsSpinning = false;
        }
    }
}

void GameWeaponDaiouIka::draw() {
    GambitActor::draw();
}

} // namespace Game
