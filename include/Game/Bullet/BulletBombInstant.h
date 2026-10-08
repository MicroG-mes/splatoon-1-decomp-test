#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class BurstBombState : u32 {
    cAirborne   = 1,
    cContact    = 2,
    cDetonated  = 3
};

/**
 * BulletBombInstant / BulletBombInstant_StaffRoll
 * Address: vtable @ 0x1003444C
 * Burst Bomb - explodes instantly upon colliding with any surface or actor.
 *
 * Real PowerPC methods:
 *   vfunc_176 @ 0x0220d9ac - Reset / cleanup
 *   vfunc_178 @ 0x0220de38 - Impact detonation check (state == 2 || state == 3)
 *   vfunc_87  @ 0x0220d9e4 - Target damage query
 */
class BulletBombInstant : public GambitActor {
public:
    static constexpr f32 cDirectDamage = 60.0f;
    static constexpr f32 cNearSplashDamage = 35.0f;
    static constexpr f32 cFarSplashDamage = 20.0f;
    static constexpr f32 cBlastRadius = 2.8f;
    static constexpr f32 cGravity = 0.038f;

    BulletBombInstant();
    virtual ~BulletBombInstant() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Vtable slots matching Ghidra 0x1003444C
    virtual void vfunc_176();
    virtual void vfunc_178();
    virtual f32  vfunc_87(u32 targetTeam, const sead::Vector3f& targetPos);

    void throwBomb(const sead::Vector3f& startPos, const sead::Vector3f& initVel, u32 team, u32 ownerPlayerId);
    void impactSurface(const sead::Vector3f& hitPos);

    BurstBombState getState() const { return mBombState; }
    bool isDetonated() const { return mBombState == BurstBombState::cDetonated; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    u32 getTeam() const { return mTeamId; }
    u32 getOwnerPlayerId() const { return mOwnerPlayerId; }

protected:
    void explode();

    // Struct members aligned to Espresso PowerPC offsets:
    // +0x1C: Bomb state enum
    BurstBombState mBombState;         // 0x1C

    // +0x2C: Team ID
    u32 mTeamId;                       // 0x2C
    u32 mOwnerPlayerId;                // 0x30

    undefined mPadding1[0x40];
    f32 mBlastSphereRadius;            // 0x84

    sead::Vector3f mVelocity;
    s32 mTimer;
};

// Internal binary alias
using BulletBombInstant_StaffRoll = BulletBombInstant;

} // namespace Game
