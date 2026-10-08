#include "Game/Weapon/SuperWeaponShelter.h"
#include <algorithm>
#include <cmath>

namespace Game {

SuperWeaponShelter::SuperWeaponShelter()
    : mPosition(0.0f, 0.0f, 0.0f),
      mExtensionHeight(0.0f),
      mCanopyRadius(0.0f),
      mDeployFlag(0),
      mRetractFlag(0),
      mTimer(0),
      mCycleCounter(0),
      mDurability(cMaxDurability),
      mTeamId(0),
      mState(ShelterDeployState::cClosed) {
}

SuperWeaponShelter::~SuperWeaponShelter() = default;

void SuperWeaponShelter::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mExtensionHeight = 0.0f;
    mCanopyRadius = 0.0f;
    mDeployFlag = 0;
    mRetractFlag = 0;
    mTimer = 0;
    mCycleCounter = 0;
    mDurability = cMaxDurability;
    mTeamId = 0;
    mState = ShelterDeployState::cClosed;
}

void SuperWeaponShelter::deploy(const sead::Vector3f& anchorPos, u32 teamId) {
    mPosition = anchorPos;
    mTeamId = teamId;
    mDeployFlag = 1;
    mRetractFlag = 0;
    mTimer = 60; // 1 second deployment time
    mState = ShelterDeployState::cOpening;
    mDurability = cMaxDurability;
}

void SuperWeaponShelter::close() {
    mDeployFlag = 0;
    mRetractFlag = 1;
    mTimer = 45;
    mState = ShelterDeployState::cRetracting;
}

void SuperWeaponShelter::update() {
    GambitActor::update();
    vfunc_7();
}

void SuperWeaponShelter::draw() {
    GambitActor::draw();
    vfunc_9();
}

/**
 * SuperWeaponShelter__vfunc_7 @ 0x027745B8
 * Authentic PowerPC canopy height & extension trigonometry.
 */
void SuperWeaponShelter::vfunc_7() {
    if (mState == ShelterDeployState::cOpening) {
        mExtensionHeight = std::min(1.0f, mExtensionHeight + 0.035f);
        mCanopyRadius = mExtensionHeight * cMaxCanopyRadius;
        if (mTimer > 0) mTimer--;
        if (mExtensionHeight >= 1.0f) {
            mState = ShelterDeployState::cOpened;
        }
    } else if (mState == ShelterDeployState::cRetracting) {
        mExtensionHeight = std::max(0.0f, mExtensionHeight - 0.045f);
        mCanopyRadius = mExtensionHeight * cMaxCanopyRadius;
        if (mExtensionHeight <= 0.0f) {
            mState = ShelterDeployState::cClosed;
        }
    }

    mCycleCounter = (mCycleCounter + 1) % 18;
}

void SuperWeaponShelter::vfunc_9() {
    // Draw canopy mesh when deployed
}

/**
 * SuperWeaponShelter__vfunc_15 @ 0x0277456C
 * Damage deflection and durability absorption.
 */
bool SuperWeaponShelter::vfunc_15(f32 damage, const sead::Vector3f& hitPos) {
    if (mState != ShelterDeployState::cOpened) return false;

    f32 dist = (hitPos - mPosition).length();
    if (dist <= mCanopyRadius + 0.5f) {
        mDurability -= damage;
        if (mDurability <= 0.0f) {
            mDurability = 0.0f;
            close();
        }
        return true; // Damage blocked by shelter
    }
    return false;
}

} // namespace Game
