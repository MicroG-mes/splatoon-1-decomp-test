#pragma once

#include "types.h"

namespace Game {

enum class InkStandingType : u32 {
    cNeutral = 0,    // Unpainted dry ground
    cFriendly = 1,   // Player team's ink
    cEnemy = 2       // Opposing team's hostile ink
};

class PlayerInkState {
public:
    static constexpr f32 cMaxHealth = 100.0f;
    static constexpr f32 cEnemyInkDamageCap = 50.0f;    // Max HP damage enemy ink can deal
    static constexpr f32 cEnemyInkDamageRate = 0.30f;   // HP per frame
    static constexpr f32 cSafetyShoesDamageRate = 0.12f;// HP per frame with Ink Resistance Up
    static constexpr f32 cFriendlyRegenRate = 1.00f;    // HP per frame in friendly ink
    static constexpr f32 cNeutralRegenRate = 0.20f;     // HP per frame on neutral ground
    static constexpr f32 cEnemySpeedFactor = 0.28f;     // Speed multiplier in enemy ink
    static constexpr f32 cSafetyShoesSpeed = 0.45f;     // Speed multiplier with Ink Resistance Up

    PlayerInkState();
    ~PlayerInkState();

    void reset();
    void update(InkStandingType standingType, bool isSquidSubmerged, bool hasInkResistanceAbility);

    // Take combat damage (from bullets, bombs, specials)
    void takeDamage(f32 amount);

    f32 getHealth() const { return mHealth; }
    f32 getSpeedMultiplier() const { return mSpeedMultiplier; }
    bool isDamaged() const { return mHealth < cMaxHealth; }
    bool isEnemyInkCapped() const { return mHealth <= cEnemyInkDamageCap; }
    f32 getDamageRatio() const { return 1.0f - (mHealth / cMaxHealth); }

private:
    f32 mHealth;
    f32 mSpeedMultiplier;
    u32 mImmersionFrames;
    u32 mDamageCooldownFrames;
};

} // namespace Game
