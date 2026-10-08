#pragma once

#include "types.h"
#include "Game/Weapon/PlayerWeaponBase.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class BigLaserState : u32 {
    cPlacingSpeaker = 0,
    cWarningPitch   = 1,
    cSonicBlast     = 2,
    cCooldown       = 3,
    cFinished       = 4
};

/**
 * PlayerWeaponBigLaser (Killer Wail / Megaphone Laser)
 * Address: vtable @ 0x100EFB2C
 *
 * Real PowerPC methods:
 *   vfunc_35 @ 0x026d73e4 - Sub-action controller invocation
 *   vfunc_70 @ 0x026d7424 - Export weapon context pointer
 */
class PlayerWeaponBigLaser : public PlayerWeaponBase {
public:
    static constexpr f32 cBeamRadius = 3.5f;
    static constexpr f32 cBeamLength = 100.0f; // Infinite terrain-piercing beam
    static constexpr f32 cDamagePerFrame = 5.0f;
    static constexpr s32 cWarningDuration = 60; // 1 second warmup
    static constexpr s32 cBlastDuration = 120;  // 2 seconds sonic blast

    PlayerWeaponBigLaser();
    virtual ~PlayerWeaponBigLaser() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void deploy(const sead::Vector3f& pos, f32 yawAngle, u32 teamId);
    bool checkHitTarget(const sead::Vector3f& targetPos) const;

    BigLaserState getState() const { return mState; }
    bool isFinished() const { return mState == BigLaserState::cFinished; }
    bool isBlasting() const { return mState == BigLaserState::cSonicBlast; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    f32 getYawAngle() const { return mYawAngle; }
    u32 getTeamId() const { return mTeamId; }

protected:
    sead::Vector3f mPosition;
    f32 mYawAngle;
    u32 mTeamId;
    BigLaserState mState;
    s32 mTimer;
};

} // namespace Game
