#include "Game/Player/PlayerInkState.h"
#include <algorithm>

namespace Game {

PlayerInkState::PlayerInkState()
    : mHealth(cMaxHealth)
    , mSpeedMultiplier(1.0f)
    , mImmersionFrames(0)
    , mDamageCooldownFrames(0)
{
}

PlayerInkState::~PlayerInkState() {
}

void PlayerInkState::reset() {
    mHealth = cMaxHealth;
    mSpeedMultiplier = 1.0f;
    mImmersionFrames = 0;
    mDamageCooldownFrames = 0;
}

void PlayerInkState::takeDamage(f32 amount) {
    mHealth = (std::max)(0.0f, mHealth - amount);
    mDamageCooldownFrames = 60; // 1 second before regen begins
}

void PlayerInkState::update(InkStandingType standingType, bool isSquidSubmerged, bool hasInkResistanceAbility) {
    if (mDamageCooldownFrames > 0) {
        mDamageCooldownFrames--;
    }

    if (standingType == InkStandingType::cEnemy) {
        // Enemy ink slows down player movement
        mSpeedMultiplier = hasInkResistanceAbility ? cSafetyShoesSpeed : cEnemySpeedFactor;

        // Enemy ink damages player up to 50 HP cap
        f32 damageRate = hasInkResistanceAbility ? cSafetyShoesDamageRate : cEnemyInkDamageRate;
        if (mHealth > cEnemyInkDamageCap) {
            mHealth = (std::max)(cEnemyInkDamageCap, mHealth - damageRate);
        }
        mImmersionFrames = 0;
    } else if (standingType == InkStandingType::cFriendly) {
        mSpeedMultiplier = 1.0f;
        mImmersionFrames++;

        // Fast regeneration in friendly ink if not taking damage
        if (mDamageCooldownFrames == 0) {
            f32 rate = isSquidSubmerged ? (cFriendlyRegenRate * 1.5f) : cFriendlyRegenRate;
            mHealth = (std::min)(cMaxHealth, mHealth + rate);
        }
    } else {
        // Neutral unpainted ground
        mSpeedMultiplier = 1.0f;
        mImmersionFrames = 0;

        // Slow natural regeneration on neutral ground
        if (mDamageCooldownFrames == 0) {
            mHealth = (std::min)(cMaxHealth, mHealth + cNeutralRegenRate);
        }
    }
}

} // namespace Game
