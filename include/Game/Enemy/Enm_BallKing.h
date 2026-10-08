#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class BallKingPhase : u32 {
    cPhase1 = 1,
    cPhase2 = 2,
    cPhase3 = 3
};

enum class BallKingState : u32 {
    cRevUpEngine     = 0,
    cRollingDash     = 1,
    cSpinoutSkid     = 2,
    cFlippedExposed  = 3,
    cRecoverRighting = 4,
    cDefeated        = 5
};

/**
 * Enm_BallKing (Octowhirl / Rollonium Boss)
 * Address: vtable @ 0x1005EFB8
 *
 * Real PowerPC methods:
 *   vfunc_7  @ 0x022af2dc - AI tick, vulnerability & stun timers, shield reactivation
 *   vfunc_11 @ 0x022af5b4 - Hit reaction & ink spinout collision
 *   vfunc_26 @ 0x022c5a38 - Engine rev & torque spin setup
 *   vfunc_28 @ 0x022c5ac0 - Collision detachment cleanup
 *   vfunc_30 @ 0x022a7f90 - Eject & detach clamshell armor collision bodies
 *   vfunc_31 @ 0x022a8018 - Attach & close clamshell armor collision bodies
 */
class Enm_BallKing : public GambitActor {
public:
    static constexpr f32 cMaxRollSpeedPhase1 = 0.5f;
    static constexpr f32 cMaxRollSpeedPhase2 = 0.7f;
    static constexpr f32 cMaxRollSpeedPhase3 = 0.95f;

    Enm_BallKing();
    virtual ~Enm_BallKing() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic Ghidra Decompiled vfuncs
    virtual void vfunc_7();
    virtual void vfunc_11();
    virtual void vfunc_26();
    virtual void vfunc_28();
    virtual void vfunc_30(); // 0x022A7F90: Eject clamshell armor
    virtual void vfunc_31(); // 0x022A8018: Attach clamshell armor

    void updateBossAi(const sead::Vector3f& playerPos, bool isRollingOnPlayerInk);
    void applyTentacleDamage(f32 damage);

    BallKingState getState() const { return mState; }
    BallKingPhase getPhase() const { return mPhase; }
    f32 getTentacleHp() const { return mTentacleHp; }
    bool isTentacleExposed() const { return mState == BallKingState::cFlippedExposed; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    bool isShieldActive() const { return mIsShieldActive != 0; }
    bool isClamshellDetached() const { return mLeftArmDetached != 0 && mRightArmDetached != 0; }

protected:
    void triggerSpinout();

    BallKingState mState;
    BallKingPhase mPhase;
    s32 mStateTimer;

    sead::Vector3f mPosition;
    sead::Vector3f mMoveDirection;
    f32 mCurrentSpeed;
    f32 mTentacleHp;
    s32 mExposedDurationFrames;

    // Struct members aligned to Espresso PowerPC offsets:
    undefined mPaddingShield[0x40];
    u8 mIsShieldActive;            // 0xAC: Shield armor bit

    undefined mPaddingPhase[0x127];
    s32 mAttackMode;               // 0x1D8: Mode 2 = rolling dash

    undefined mPaddingTimers[0x1C0];
    s32 mVulnerabilityTimer;       // 0x39C: Vulnerability countdown
    s32 mStunTimer;                // 0x3A0: Stun timer countdown
    u32 mCollisionMask;            // 0x3D8: Collision state bitmask

    undefined mPaddingArms[0x11A];
    u8 mShieldResetFlag;           // 0x4F3
    undefined mPaddingArm2[2];
    u8 mLeftArmDetached;           // 0x4F6
    u8 mRightArmDetached;          // 0x4F7
    undefined mPaddingArm3[4];
    u8 mAnimBehaviorMode;          // 0x4FC
};

} // namespace Game
