#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class MarkingBombState : u32 {
    cInAirAirborne = 0,
    cSensorPulse   = 1,
    cFinished      = 2
};

/**
 * BulletBombMarking / BulletBombMarkingCore
 * Address: vtable @ 0x10034608
 * Point Sensor / Disruptor tracking sub-weapon projectile.
 *
 * Real PowerPC methods:
 *   vfunc_7  @ 0x02210a5c - Trigger pulse flag (*(0xF4) |= 1) & splash
 *   vfunc_11 @ 0x02210a94 - Detonation at (0xE8, 0xEC, 0xF0)
 *   vfunc_88 @ 0x02210af0 - Distance & enemy team check (*team != 0x2C)
 */
class BulletBombMarking : public GambitActor {
public:
    static constexpr f32 cPulseRadius = 5.0f;
    static constexpr s32 cMarkingDuration = 600; // 10 seconds of enemy tracking
    static constexpr f32 cGravity = 0.038f;

    BulletBombMarking();
    virtual ~BulletBombMarking() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Vtable slots matching Ghidra 0x10034608
    virtual void vfunc_7();
    virtual void vfunc_11();
    virtual f64  vfunc_88(u32 param2, const u32* targetTeam, const sead::Vector3f* targetPos);

    void throwBomb(const sead::Vector3f& startPos, const sead::Vector3f& initVel, u32 team, u32 ownerPlayerId);
    bool checkMarkEnemy(const sead::Vector3f& enemyPos, u32 enemyTeam) const;

    MarkingBombState getState() const { return mState; }
    bool isFinished() const { return mState == MarkingBombState::cFinished; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    u32 getTeam() const { return mTeamId; }

protected:
    void triggerPulse();

    MarkingBombState mState;
    s32 mTimer;

    sead::Vector3f mPosition;
    sead::Vector3f mVelocity;

    // Struct offsets aligned to PowerPC Espresso:
    // +0x2C: mTeamId
    u32 mTeamId;                       // 0x2C
    u32 mOwnerPlayerId;                // 0x30

    undefined mPadding1[0x40];
    f32 mMarkingSphereRadius;          // 0x84
    undefined mPadding2[0x38];
    f32 mMarkingEffectRadius;          // 0xC0
    undefined mPadding3[0x24];

    sead::Vector3f mDetonationPos;     // 0xE8 - 0xF4
    u32 mMarkingFlags;                 // 0xF4 (bit 0 = active pulse)
};

// Internal binary alias
using BulletBombMarkingCore = BulletBombMarking;

} // namespace Game
