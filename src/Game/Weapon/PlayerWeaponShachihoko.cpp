#include "Game/Weapon/PlayerWeaponShachihoko.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cmath>

namespace Game {

PlayerWeaponShachihoko::PlayerWeaponShachihoko()
    : mState(ShachihokoWeaponState::cIdle),
      mChargeFrames(0.0f),
      mRecoilTimer(0),
      mTeamId(0) {
}

PlayerWeaponShachihoko::~PlayerWeaponShachihoko() {
}

void PlayerWeaponShachihoko::init() {
    GambitActor::init();
    mState = ShachihokoWeaponState::cIdle;
    mChargeFrames = 0.0f;
    mRecoilTimer = 0;
    mTeamId = 0;
}

void PlayerWeaponShachihoko::startCharging(u32 teamId) {
    if (mState == ShachihokoWeaponState::cIdle) {
        mTeamId = teamId;
        mState = ShachihokoWeaponState::cCharging;
        mChargeFrames = 0.0f;
    }
}

void PlayerWeaponShachihoko::releaseTrigger(const sead::Vector3f& muzzlePos, f32 yawAngle) {
    if (mState == ShachihokoWeaponState::cCharging) {
        f32 chargeRatio = mChargeFrames / cMaxChargeTime;
        if (chargeRatio > 1.0f) chargeRatio = 1.0f;

        dischargeTornadoShot(muzzlePos, yawAngle, chargeRatio);

        mState = ShachihokoWeaponState::cRecoil;
        mRecoilTimer = 0;
        mChargeFrames = 0.0f;
    }
}

void PlayerWeaponShachihoko::dischargeTornadoShot(const sead::Vector3f& muzzlePos, f32 yawAngle, f32 chargeRatio) {
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (!paint) return;

    f32 dirX = std::sin(yawAngle);
    f32 dirZ = std::cos(yawAngle);
    f32 travelDist = 8.0f + (cMaxShotDistance - 8.0f) * chargeRatio;
    f32 blastRadius = 2.0f + (3.5f * chargeRatio);

    // Ink ground trail along projectile path
    for (f32 d = 2.0f; d <= travelDist; d += 4.0f) {
        sead::Vector3f trail(
            muzzlePos.x + dirX * d,
            muzzlePos.y,
            muzzlePos.z + dirZ * d
        );
        paint->splatInk(trail, 1.8f * chargeRatio, mTeamId);
    }

    // Impact explosion at terminal destination
    sead::Vector3f impactPos(
        muzzlePos.x + dirX * travelDist,
        muzzlePos.y,
        muzzlePos.z + dirZ * travelDist
    );
    paint->splatInk(impactPos, blastRadius, mTeamId);
}

void PlayerWeaponShachihoko::update() {
    switch (mState) {
        case ShachihokoWeaponState::cCharging:
            mChargeFrames += 1.0f;
            if (mChargeFrames > cMaxChargeTime) {
                mChargeFrames = cMaxChargeTime;
            }
            break;

        case ShachihokoWeaponState::cRecoil:
            mRecoilTimer++;
            if (mRecoilTimer >= 25) { // 25 frames recoil recovery
                mState = ShachihokoWeaponState::cIdle;
                mRecoilTimer = 0;
            }
            break;

        case ShachihokoWeaponState::cIdle:
        default:
            break;
    }
}

void PlayerWeaponShachihoko::draw() {
    GambitActor::draw();
}

} // namespace Game
