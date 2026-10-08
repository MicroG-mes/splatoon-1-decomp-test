#pragma once

#include "types.h"
#include "Game/Bullet/GameBullet.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * BulletPlayerNormalExplosionShotBase / GameBulletPlayerNormalShotBase
 * Address: vtable @ 0x1004A4F8
 * Core projectile ballistics and paint collision physics for all Shooter weapons.
 *
 * Real PowerPC methods:
 *   vfunc_4   @ 0x02252e88 - No-op stub
 *   vfunc_7   @ 0x0225b11c - Ballistic physics tick (prev pos, lifetime accumulator, splash)
 *   vfunc_11  @ 0x0225bf28 - Terrain & collision test
 *   vfunc_90  @ 0x02252ee8 - Hit damage & team affiliation evaluation
 *   vfunc_92  @ 0x0225de50 - Range / damage falloff computation
 *   vfunc_108 @ 0x02253098 - Impact callback (flag 0x2000)
 *   vfunc_118 @ 0x0225e2f8 - Ground detonation & paint burst (flag 0x80000)
 *   vfunc_132 @ 0x0225424c - 2D horizontal velocity normalization (frsqrte)
 *   vfunc_135 @ 0x02254868 - Reset context
 */
class GameBulletPlayerNormalShotBase : public GameBullet {
public:
    GameBulletPlayerNormalShotBase();
    virtual ~GameBulletPlayerNormalShotBase() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Vtable methods matching Ghidra 0x1004A4F8
    virtual void vfunc_4();
    virtual void vfunc_7();
    virtual void vfunc_11();
    virtual u32  vfunc_90(u32 hitActorType, const u32* targetTeamId);
    virtual void vfunc_92(u16* outDamageData);
    virtual void vfunc_108(u32 param2, u32* param3);
    virtual void vfunc_118();
    virtual void vfunc_132(f32* outDir2D, const f32* inVel);
    virtual void vfunc_135(u32* param2);

    u32 getBulletState() const { return mBulletState; }
    bool isDetonated() const { return (mBulletState & 0x80000) != 0; }
    const sead::Vector3f& getHitPos() const { return mHitPos; }

protected:
    // Espresso PowerPC offsets:
    // +0x2C: mTeamId (from GameBullet / Actor)
    // +0x88: mDirectOrigin (Vector3f)
    sead::Vector3f mDirectOrigin;      // 0x88 - 0x94
    undefined mPadding1[0x48];         // 0x94 - 0xDC

    // +0xDC: Initial / flight velocity
    sead::Vector3f mFlightVelocity;    // 0xDC - 0xE8
    undefined mPadding2[0x4C];         // 0xE8 - 0x134

    // +0x134: Current position
    sead::Vector3f mCurrentPos;        // 0x134 - 0x140
    undefined mPadding3[0x44];         // 0x140 - 0x184

    // +0x184: Bullet state bitmask
    u32 mBulletState;                  // 0x184
    undefined mPadding4[0x7C];         // 0x188 - 0x204

    // +0x204: Fixed-point lifetime counter (+0x100000 per frame)
    s32 mLifeCounterFixed;             // 0x204
    undefined mPadding5[0x4];          // 0x208 - 0x20C

    // +0x20C: Velocity vector
    sead::Vector3f mStepVelocity;      // 0x20C - 0x218

    // +0x218: Previous position
    sead::Vector3f mPrevPosition;      // 0x218 - 0x224
    undefined mPadding6[0x4];          // 0x224 - 0x228

    // +0x228: Hit flags (0x800 enemy, 0x1000 friend, 0x2000 shield/obstacle)
    u32 mHitFlags;                     // 0x228
    undefined mPadding7[0x10];         // 0x22C - 0x23C

    // +0x23C: Hit position
    sead::Vector3f mHitPos;            // 0x23C - 0x248
    undefined mPadding8[0x14];         // 0x248 - 0x25C

    // +0x25C: Hit velocity
    sead::Vector3f mHitVelocity;       // 0x25C - 0x268
};

// Internal binary typedef
using BulletPlayerNormalExplosionShotBase = GameBulletPlayerNormalShotBase;

} // namespace Game
