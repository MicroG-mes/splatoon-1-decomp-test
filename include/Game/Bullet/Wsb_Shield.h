#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class ShieldState : u32 {
    cThrowing    = 0,
    cDeploying   = 1,
    cActiveWall  = 2,
    cBreaking    = 3,
    cFinished    = 4
};

class Wsb_Shield : public GambitActor {
public:
    Wsb_Shield();
    virtual ~Wsb_Shield() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic Ghidra Decompiled vfuncs (from Shield vtable 0x10038890)
    virtual void vfunc_3(); // 0x0221EC80: Shield initialization & resource loading
    virtual void vfunc_7(); // 0x0221EDDC: Shield update & ink curtain deflection check

    void deploy(const sead::Vector3f& pos, f32 yawAngle, u32 team);
    void applyDamage(f32 damage);

    ShieldState getState() const { return mState; }
    f32 getRemainingHp() const { return mCurrentHp; }
    u32 getTeam() const { return mTeam; }

    bool blocksBullet(const sead::Vector3f& bulletPos, u32 bulletTeam) const;

protected:
    u8 mReserved0_0x8[0xBC];
    sead::Vector3f mPosition; // 0xC4, 0xC8, 0xCC
    u8 mReserved1_0xD0[0x58];
    u8 mHitDeflectionFlag;    // 0x128: set on projectile impact, cleared in vfunc_7
    u8 mReserved2_0x129[0x5B];
    f32 mMaxHp;               // 0x184: 800.0f
    u8 mReserved3_0x188[0x14];
    void* mCurtainEffectPtr;  // 0x19C

    ShieldState mState;
    s32 mStateTimer;
    f32 mYaw;
    u32 mTeam;
    f32 mCurrentHp;
    f32 mWallWidth;
    f32 mWallHeight;
};

} // namespace Game
