#include "Game/Enemy/Enm_MouthKingTooth.h"
#include <cstring>

namespace Game {

Enm_MouthKingTooth::Enm_MouthKingTooth()
    : mIndex(0),
      mArmorType(ToothArmorType::cNormal),
      mHp(40.0f),
      mMaxHp(40.0f),
      mIsBroken(false),
      mIsGoldTooth(false),
      mShakeTimer(0),
      mDurabilityId(0x1BA) {
    std::memset(mReserved, 0, sizeof(mReserved));
}

Enm_MouthKingTooth::~Enm_MouthKingTooth() {
}

void Enm_MouthKingTooth::init() {
    GambitActor::init();
    mIsBroken = false;
    mIsGoldTooth = (mArmorType == ToothArmorType::cGold);
    mShakeTimer = 0;
    vfunc_47();
}

// 0x02357470: Decompiled vfunc_7 (Durability tick, gold tooth check, broken state)
void Enm_MouthKingTooth::vfunc_7() {
    if (mShakeTimer > 0) {
        mShakeTimer--;
    }

    if (mIsBroken) {
        // Tooth has shattered/ejected from Octomaw's jaw
        mHp = 0.0f;
    }
}

// 0x02357700: Decompiled vfunc_11 (Jaw transform matrix sync)
void Enm_MouthKingTooth::vfunc_11() {
    // Synchronize tooth transform relative to jaw socket
}

// 0x0235E168: Decompiled vfunc_47 (Stage collision mesh registration)
void Enm_MouthKingTooth::vfunc_47() {
    // Register bounding collision box for ink blocking
}

// 0x0235780C: Decompiled vfunc_52 (Tooth shatter animation trigger)
void Enm_MouthKingTooth::vfunc_52() {
    mIsBroken = true;
    mHp = 0.0f;
}

void Enm_MouthKingTooth::setupTooth(u32 index, ToothArmorType armor) {
    mIndex = index;
    mArmorType = armor;
    mIsGoldTooth = (armor == ToothArmorType::cGold);

    switch (armor) {
        case ToothArmorType::cNormal:
            mMaxHp = 40.0f;
            mDurabilityId = 0x1BA;
            break;
        case ToothArmorType::cReinforced:
            mMaxHp = 80.0f;
            mDurabilityId = 0x300;
            break;
        case ToothArmorType::cGold:
            mMaxHp = 120.0f;
            mDurabilityId = 0x651;
            break;
    }
    mHp = mMaxHp;
    mIsBroken = false;
    mShakeTimer = 0;
}

void Enm_MouthKingTooth::applyDamage(f32 damage) {
    if (mIsBroken) return;

    mHp -= damage;
    mShakeTimer = 15; // 15 frames rattle on hit

    if (mHp <= 0.0f) {
        breakTooth();
    }
}

void Enm_MouthKingTooth::breakTooth() {
    vfunc_52();
}

void Enm_MouthKingTooth::resetTooth() {
    mHp = mMaxHp;
    mIsBroken = false;
    mShakeTimer = 0;
}

void Enm_MouthKingTooth::update() {
    vfunc_11();
    vfunc_7();
}

void Enm_MouthKingTooth::draw() {
    GambitActor::draw();
}

} // namespace Game
