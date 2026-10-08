#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class SplatBombState : u32 {
    cInAirAirborne = 0,
    cGroundedFuse  = 1,
    cExploding     = 2,
    cFinished      = 3
};

class BulletBombNormal : public GambitActor {
public:
    BulletBombNormal();
    virtual ~BulletBombNormal() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Decompiled vfunc_178 from Gambit.elf (0x0221266C)
    virtual void vfunc_178();
    virtual s32 canExplode() const { return 0; } // vtable + 0x57C
    virtual void explode(const void* effectParam = nullptr); // vtable + 0x53C

    void throwBomb(const sead::Vector3f& startPos, const sead::Vector3f& initVel, u32 team);

    SplatBombState getState() const { return mState; }
    bool isFinished() const { return mBombState == 3 || mState == SplatBombState::cFinished; }

    f32 getInnerDamage() const { return mInnerDamage; }
    f32 getOuterDamage() const { return mOuterDamage; }
    const sead::Vector3f& getPosition() const { return mPosition; }

protected:
    u8 mReserved0_0x8[0x14];
    s32 mBombState;    // 0x1C: 2 = fuse active, 3 = exploding

    SplatBombState mState;
    s32 mFuseTimer;

    sead::Vector3f mPosition;
    sead::Vector3f mVelocity;
    u32 mTeam;

    f32 mInnerDamage;  // 180.0 (Lethal)
    f32 mOuterDamage;  // 30.0
    f32 mBlastRadius;  // 4.5m
};

} // namespace Game
