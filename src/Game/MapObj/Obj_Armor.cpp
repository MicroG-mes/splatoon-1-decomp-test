#include "Game/MapObj/Obj_Armor.h"
#include <cmath>
#include <algorithm>

namespace Game {

Obj_Armor::Obj_Armor()
    : mBasePosition(0.0f, 0.0f, 0.0f),
      mPosition(0.0f, 0.0f, 0.0f),
      mState(ArmorState::cState_Wait),
      mRotAngle(0.0f),
      mTimer(0) {
}

Obj_Armor::~Obj_Armor() {
}

void Obj_Armor::setPosition(const sead::Vector3f& pos) {
    mBasePosition = pos;
    mPosition = pos;
}

void Obj_Armor::init() {
    GambitActor::init();
    mPosition = mBasePosition;
    mState = ArmorState::cState_Wait;
    mRotAngle = 0.0f;
    mTimer = 0;
}

void Obj_Armor::vfunc_3() {
    // Resource & model load (Obj_Armor.szs)
}

void Obj_Armor::vfunc_5() {
    // Initial parameter setup
}

void Obj_Armor::vfunc_14() {
    // Touch collision collection event (ArmorGet)
}

void Obj_Armor::vfunc_47() {
    // Floating animation sync
}

bool Obj_Armor::checkPlayerTouch(const sead::Vector3f& playerPos) {
    if (mState != ArmorState::cState_Wait) {
        return false;
    }

    f32 dx = playerPos.x - mPosition.x;
    f32 dy = playerPos.y - mPosition.y;
    f32 dz = playerPos.z - mPosition.z;
    f32 dist = std::sqrt(dx * dx + dy * dy + dz * dz);

    if (dist <= cTouchRadius) {
        mState = ArmorState::cState_Got;
        vfunc_14();
        return true;
    }
    return false;
}

bool Obj_Armor::applyArmorPickup(u32& currentTier, f32& currentArmorHp) {
    if (currentTier < cMaxTier) {
        currentTier++;
        currentArmorHp = static_cast<f32>(currentTier - 1) * cArmorDurabilityPerTier;
        return true;
    }
    return false;
}

bool Obj_Armor::applyDamageToArmor(f32 damage, u32& currentTier, f32& currentArmorHp, bool& outShattered) {
    outShattered = false;
    if (currentTier <= 1) {
        return false; // No extra armor layer to absorb damage
    }

    if (currentArmorHp > damage) {
        currentArmorHp -= damage;
        return true;
    }

    // Damage exceeds armor layer durability: shatter armor!
    currentArmorHp = 0.0f;
    currentTier = 1; // Reverts back to normal suit
    outShattered = true; // Triggers ArmorBreak and invulnerability
    return true;
}

void Obj_Armor::update() {
    if (mState == ArmorState::cState_Wait) {
        mTimer++;
        mRotAngle += cRotateSpeed;
        if (mRotAngle > 6.2831853f) {
            mRotAngle -= 6.2831853f;
        }

        // Harmonic vertical bobbing float: 60-frame period
        f32 phase = (static_cast<f32>(mTimer % 60) / 60.0f) * 6.2831853f;
        mPosition.y = mBasePosition.y + std::sin(phase) * cBobAmplitude;
    }
}

void Obj_Armor::draw() {
    if (mState == ArmorState::cState_Wait) {
        GambitActor::draw();
    }
}

} // namespace Game
