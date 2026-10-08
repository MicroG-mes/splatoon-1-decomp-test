#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class KingSquidState : u32 {
    cInactive     = 0,
    cTransforming = 1,
    cActive       = 2,
    cSpinAttack   = 3,
    cReverting    = 4
};

class PlayerKingSquid : public GambitActor {
public:
    static constexpr f32 cSpinAttackRadius = 3.2f;
    static constexpr f32 cSpinAttackDamage = 160.0f;
    static constexpr s32 cDefaultDuration = 300; // 5 seconds at 60fps
    static constexpr s32 cSpinAttackDuration = 30;

    PlayerKingSquid();
    virtual ~PlayerKingSquid() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Decompiled vtables and lifecycle from Gambit.elf (0x100EBD48)
    virtual void vfunc_1(); // 0x02267E08: State cleanup & reset
    virtual void vfunc_2(); // 0x02267F7C: Deactivate Kraken / revert vulnerability
    virtual void vfunc_3(s32 duration); // 0x02268074: Activate Kraken
    virtual void vfunc_11(); // 0x022647A8: Spin attack & HitSplash trigger
    virtual f64 vfunc_88(s32 param2, const s32* pTargetOwnerId); // 0x02266810: Kraken damage calculation (1.0x contact, 3.5x spin leap)

    void activate(const sead::Vector3f& startPos, u32 teamId, s32 durationFrames = cDefaultDuration);
    bool triggerSpinAttack();
    void updateMovement(const sead::Vector3f& moveDir, f32 speed);

    KingSquidState getState() const { return mState; }
    bool isActive() const { return mIsActive != 0; }
    bool isInvulnerable() const { return mIsInvulnerable != 0; }
    bool isSpinAttacking() const { return mState == KingSquidState::cSpinAttack; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    u32 getTeamId() const { return mTeamId; }

protected:
    // Authentic binary layout from Ghidra decompilation
    u8 mReserved0_0x8[4];
    s8 mPlayerTeamIndex;      // 0x0C
    s8 mReserved_0x0D[3];
    u8 mIsInvulnerable;       // 0x10: 1 when kraken active
    u8 mIsActive;             // 0x11: 1 during kraken lifetime
    u8 mReserved_0x12[0x3F];
    u8 mHasContactTarget;     // 0x51
    u8 mReserved_0x52[0x32];
    sead::Vector3f mSplashPos;// 0x84
    sead::Vector3f mHitboxPos;// 0xB4
    s32 mAttackTimer;         // 0xC0: decremented during spin attack
    sead::Vector3f mPosition; // 0xC4
    u32 mTeamId;
    KingSquidState mState;
    s32 mDurationTimer;
    s32 mStateTimer;
    f32 mScaleFactor;
    u32 mAttackMask;          // 0x120
    u32 mCollisionFlags;      // 0x20C
    u8  mSpinAttackCounter;   // 0x20D
};

} // namespace Game
