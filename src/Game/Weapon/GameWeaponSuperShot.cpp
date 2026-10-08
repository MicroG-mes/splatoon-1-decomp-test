#include "Game/Weapon/GameWeaponSuperShot.h"

namespace Game {

GameWeaponSuperShot::GameWeaponSuperShot()
    : mPosition(0.0f, 0.0f, 0.0f),
      mLastFireDir(0.0f, 0.0f, 1.0f),
      mDurationTimer(0),
      mCooldownTimer(0),
      mRemainingShots(0),
      mTeamId(0),
      mIsActive(false) {
}

GameWeaponSuperShot::~GameWeaponSuperShot() = default;

void GameWeaponSuperShot::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mLastFireDir.set(0.0f, 0.0f, 1.0f);
    mDurationTimer = 0;
    mCooldownTimer = 0;
    mRemainingShots = 0;
    mTeamId = 0;
    mIsActive = false;
}

void GameWeaponSuperShot::activate(u32 teamId, const sead::Vector3f& spawnPos) {
    mTeamId = teamId;
    mPosition = spawnPos;
    mRemainingShots = cMaxShots;
    mDurationTimer = cDurationFrames;
    mCooldownTimer = 0;
    mIsActive = true;
}

void GameWeaponSuperShot::deactivate() {
    mIsActive = false;
    mDurationTimer = 0;
    mRemainingShots = 0;
}

bool GameWeaponSuperShot::canFire() const {
    return mIsActive && mCooldownTimer == 0 && mRemainingShots > 0;
}

bool GameWeaponSuperShot::fire(const sead::Vector3f& firePos, const sead::Vector3f& aimDir, f32 /*speed*/) {
    if (!canFire()) return false;

    mPosition = firePos;
    mLastFireDir = aimDir;
    mCooldownTimer = cCooldownFrames;
    mRemainingShots--;

    if (mRemainingShots <= 0) {
        mIsActive = false;
    }
    return true;
}

void GameWeaponSuperShot::update() {
    GambitActor::update();

    if (!mIsActive) return;

    if (mCooldownTimer > 0) {
        mCooldownTimer--;
    }

    if (mDurationTimer > 0) {
        mDurationTimer--;
        if (mDurationTimer == 0) {
            deactivate();
        }
    }
}

void GameWeaponSuperShot::draw() {
    GambitActor::draw();
}

} // namespace Game
