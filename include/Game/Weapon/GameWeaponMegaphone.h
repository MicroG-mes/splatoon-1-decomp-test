#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * GameWeaponMegaphone
 * Killer Wail acoustic megaphone laser special weapon
 * Address / vtable: vtable @ 0x100DB380
 * Deploys giant speaker array that charges up with siren tone,
 * then unleashes an unblockable 100m acoustic shockwave tunnel.
 */
class GameWeaponMegaphone : public GambitActor {
public:
    static constexpr s32 cWarmupDuration = 70;     // 1.16s acoustic siren warning
    static constexpr s32 cBlastDuration  = 160;    // 2.66s continuous sonic beam
    static constexpr f32 cBeamLength     = 100.0f; // 100 meters range
    static constexpr f32 cBeamRadius     = 4.2f;   // 4.2 meters shockwave radius
    static constexpr f32 cDamagePerFrame = 4.5f;   // 270 HP/second lethal penetration

    GameWeaponMegaphone();
    virtual ~GameWeaponMegaphone() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void deploy(u32 teamId, const sead::Vector3f& deployPos, float yaw);
    void cancel();

    bool isDeployed() const { return mIsDeployed; }
    bool isWarmup() const { return mIsDeployed && (mTimer < cWarmupDuration); }
    bool isFiring() const { return mIsFiring; }
    bool isFinished() const { return mIsFinished; }

    f32 getWarmupProgress() const;
    f32 getBlastProgress() const;

    bool checkDamageHit(const sead::Vector3f& targetPos, f32 targetRadius, f32* outDamage) const;

    const sead::Vector3f& getPosition() const { return mPosition; }
    const sead::Vector3f& getDirection() const { return mDirection; }
    float getYaw() const { return mYaw; }
    u32 getTeamId() const { return mTeamId; }

protected:
    sead::Vector3f mPosition;
    sead::Vector3f mDirection;
    float mYaw;
    s32 mTimer;
    u32 mTeamId;
    bool mIsDeployed;
    bool mIsFiring;
    bool mIsFinished;

    undefined mReserved[0x38];
};

} // namespace Game
