#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class DevilBombState : u32 {
    cAirborne       = 1,
    cAttachedSurface = 2,
    cDetonated      = 3
};

/**
 * BulletBombDevil / BulletBombDevilCore
 * Address: vtable @ 0x10033BD4
 * Suction Bomb - attaches to any surface, then detonates with large radial blast.
 *
 * Real PowerPC methods:
 *   vfunc_7  @ 0x0220bf50 - HitSplash trigger & flag 0x1 on offset 0xF4
 *   vfunc_11 @ 0x0220bf88 - Radial blast detonation at (0xE8, 0xEC, 0xF0)
 *   vfunc_88 @ 0x0220bfe4 - Damage calculation (180.0 HP) against enemy players
 */
class BulletBombDevil : public GambitActor {
public:
    static constexpr f32 cBlastRadius = 5.2f;
    static constexpr f32 cBaseDamage = 180.0f;
    static constexpr s32 cFuseDuration = 120; // 2.0s fuse after sticking to surface
    static constexpr f32 cGravity = 0.038f;

    BulletBombDevil();
    virtual ~BulletBombDevil() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Vtable slots matching Ghidra 0x10033BD4
    virtual void vfunc_7();
    virtual void vfunc_11();
    virtual f64  vfunc_88(u32 param2, const u32* targetTeam, const sead::Vector3f* targetPos);

    void throwBomb(const sead::Vector3f& startPos, const sead::Vector3f& initVel, u32 team, u32 ownerPlayerId);
    void attachToSurface(const sead::Vector3f& contactPos, const sead::Vector3f& normal);

    DevilBombState getState() const { return mBombState; }
    bool isDetonated() const { return mBombState == DevilBombState::cDetonated; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    u32 getTeam() const { return mTeamId; }

protected:
    void explode();

    // Struct members aligned to Espresso PowerPC offsets:
    // +0x1C: Bomb state enum
    DevilBombState mBombState;         // 0x1C

    // +0x2C: Team ID
    u32 mTeamId;                       // 0x2C
    u32 mOwnerPlayerId;                // 0x30

    undefined mPadding1[0x40];
    f32 mBlastSphereRadius;            // 0x84
    undefined mPadding2[0x38];
    f32 mExplosionDamageRadius;        // 0xC0
    undefined mPadding3[0x24];

    sead::Vector3f mDetonationPos;     // 0xE8 - 0xF4
    u32 mStateFlags;                   // 0xF4 (bit 0 = triggered)

    s32 mFuseTimer;
    sead::Vector3f mVelocity;
    sead::Vector3f mSurfaceNormal;
};

// Internal binary alias
using BulletBombDevilCore = BulletBombDevil;

} // namespace Game
