#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * GameWeaponSuperShot
 * Inkzooka special weapon launcher (Super Shot)
 * Address / vtable: vtable @ 0x100DB9F8
 * Launches rapid vertical whirlwind spirals of pressurized ink.
 */
class GameWeaponSuperShot : public GambitActor {
public:
    static constexpr s32 cMaxShots = 6;
    static constexpr s32 cDurationFrames = 360; // 6.0 seconds
    static constexpr s32 cCooldownFrames = 35;  // 0.58s between blasts
    static constexpr f32 cDirectDamage = 120.0f; // Fatal one-shot in Splatoon 1

    GameWeaponSuperShot();
    virtual ~GameWeaponSuperShot() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void activate(u32 teamId, const sead::Vector3f& spawnPos);
    void deactivate();

    bool canFire() const;
    bool fire(const sead::Vector3f& firePos, const sead::Vector3f& aimDir, f32 speed = 2.4f);

    bool isActive() const { return mIsActive; }
    s32 getRemainingShots() const { return mRemainingShots; }
    s32 getRemainingDuration() const { return mDurationTimer; }
    u32 getTeamId() const { return mTeamId; }
    const sead::Vector3f& getPosition() const { return mPosition; }

protected:
    sead::Vector3f mPosition;
    sead::Vector3f mLastFireDir;
    s32 mDurationTimer;
    s32 mCooldownTimer;
    s32 mRemainingShots;
    u32 mTeamId;
    bool mIsActive;

    undefined mReserved[0x38];
};

} // namespace Game
