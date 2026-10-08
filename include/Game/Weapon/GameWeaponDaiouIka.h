#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * GameWeaponDaiouIka
 * Kraken (Great King Squid) special weapon transformation & spin attack
 * Address / vtable: vtable @ 0x100DAB44
 * Transforms into giant invulnerable squid, moving at high velocity and
 * executing lethal 360-degree spin squish jump attacks.
 */
class GameWeaponDaiouIka : public GambitActor {
public:
    static constexpr s32 cDurationFrames = 300;    // 5.0 seconds
    static constexpr s32 cSpinDuration   = 20;     // 0.33s jump spin
    static constexpr s32 cSpinCooldown   = 30;     // 0.50s between spins
    static constexpr f32 cSpinDamage     = 160.0f; // Fatal 1-hit KO
    static constexpr f32 cSpinRadius     = 2.4f;   // Hitbox radius
    static constexpr f32 cSwimSpeed      = 0.52f;  // Top swim speed

    GameWeaponDaiouIka();
    virtual ~GameWeaponDaiouIka() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void activate(u32 teamId, const sead::Vector3f& startPos);
    void deactivate();

    bool triggerSpinAttack();
    bool checkSpinDamage(const sead::Vector3f& targetPos, f32 targetRadius, f32* outDamage) const;

    bool isActive() const { return mIsActive; }
    bool isInvincible() const { return mIsActive; }
    bool isSpinning() const { return mIsSpinning; }
    s32 getRemainingDuration() const { return mDurationTimer; }
    u32 getTeamId() const { return mTeamId; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mPosition;
    s32 mDurationTimer;
    s32 mSpinTimer;
    s32 mCooldownTimer;
    u32 mTeamId;
    bool mIsActive;
    bool mIsSpinning;

    undefined mReserved[0x38];
};

} // namespace Game
