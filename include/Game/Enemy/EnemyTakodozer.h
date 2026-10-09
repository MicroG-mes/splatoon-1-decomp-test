#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include "sead/math/seadMatrix.h"

namespace Game {

enum class OctodozerState : u32 {
    cStart         = 0,
    cWait          = 1,
    cPatrol        = 2,
    cFind          = 3,
    cChase         = 4,
    cLost          = 5,
    cDie           = 6,

    // Legacy backwards-compatible aliases
    cChargeForward = 4,
    cTurnAround    = 1,
    cTentacleHit   = 5,
    cDestroyed     = 6
};

/**
 * EnemyTakodozer / Enm_Takodozer
 * Address: vtable @ 0x10088dfc
 * Heavy Octarian armored assault bulldozer with impenetrable front plow
 * and exposed tentacle driver cockpit in rear.
 *
 * Real PowerPC methods:
 *   vfunc_3  @ 0x023AEE18 - Model & resource initialization
 *   vfunc_5  @ 0x023AEFC8 - Bone lookup (foot_L_F, foot_L_B, foot_R_F, foot_R_B, eye_L, eye_R, Scrap)
 *   vfunc_7  @ 0x023B0238 - Update tick, lost target timer 300f
 *   vfunc_14 @ 0x023AEB20 - Collision and frontal deflection
 *   vfunc_47 @ 0x023B3DE0 - Animation/skeleton sync
 *   vfunc_52 @ 0x023B052C - Target tracking / eye directing
 *   vfunc_54 @ 0x023B137C - Sound component event
 *   vfunc_60 @ 0x023B1468 - State dispatcher
 */
class EnemyTakodozer : public GambitActor {
public:
    static constexpr f32 cDefaultLife             = 100.0f;
    static constexpr f32 cPatrolSpeed             = 0.60f;
    static constexpr f32 cChaseSpeed              = 1.20f;
    static constexpr f32 cAcceleration            = 0.03f;
    static constexpr f32 cEyesightRadius          = 500.0f;
    static constexpr f32 cEyesightAngleDeg        = 15.0f;
    static constexpr f32 cEyesightRadius2         = 50.0f;
    static constexpr f32 cEyesightAngle2Deg       = 225.0f;
    static constexpr f32 cTrackPaintRadius        = 30.0f;
    static constexpr f32 cDiePaintRadius          = 80.0f;
    static constexpr f32 cChaseEyeScale           = 2.069f;
    static constexpr s32 cLostTargetCooldownFrame = 300;
    static constexpr s32 cDroppedPowerEggsOnDie   = 20;

    EnemyTakodozer();
    virtual ~EnemyTakodozer() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC vtable slots matching Ghidra
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_14();
    virtual void vfunc_47();
    virtual void vfunc_52();
    virtual void vfunc_54();
    virtual void vfunc_60();

    // Damage interface
    bool applyDamage(f32 damage, const sead::Vector3f& hitDir, bool isRearHit);
    void applyWeakpointDamage(f32 damage);
    bool isFrontShieldHit(const sead::Vector3f& hitDir) const;

    // AI & movement
    void updatePatrol(f32 pathLength = 50.0f);
    void updateAi(const sead::Vector3f& playerPos, f32 pathLength = 50.0f);

    // Getters & inspection
    OctodozerState getState() const { return mState; }
    f32 getTentacleHp() const { return mTentacleHp; }
    f32 getMaxHp() const { return mMaxHp; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    const sead::Vector3f& getDirection() const { return mMoveDirVec; }
    f32 getSpeed() const { return mCurrentSpeed; }
    bool isDestroyed() const { return mState == OctodozerState::cDie; }
    bool isChasing() const { return mState == OctodozerState::cChase; }
    bool isShieldPlowActive() const { return mState != OctodozerState::cDie; }
    s32 getDroppedPowerEggs() const { return mDroppedEggs; }
    f32 getFrontPlowDeflectedDamage() const { return mDeflectedDamage; }
    f32 getEyeScale() const { return mEyeScale; }

    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    void paintDozerTrail();
    void turnAround();

    OctodozerState mState;
    s32 mStateTimer;
    s32 mLostTargetTimer;

    sead::Vector3f mPosition;      // 0x140
    sead::Vector3f mMoveDirVec;
    f32 mMoveDir;                  // +1.0 or -1.0
    f32 mCurrentSpeed;
    f32 mTargetSpeed;
    f32 mTentacleHp;
    f32 mMaxHp;
    f32 mEyeScale;
    f32 mDeflectedDamage;
    s32 mDroppedEggs;

    // Bone indices mapped at vfunc_5
    s16 mBoneScrap;                // 0x30C: driver weak point cockpit
    s16 mBoneNotAnimRoot;          // 0x310
    s16 mBoneFootLF;               // 0x314: foot_L_F
    s16 mBoneFootLB;               // 0x318: foot_L_B
    s16 mBoneFootRF;               // 0x31C: foot_R_F
    s16 mBoneFootRB;               // 0x320: foot_R_B
    s16 mBoneEyeL;                 // 0x324: eye_L
    s16 mBoneEyeR;                 // 0x328: eye_R

    undefined mPadding[0x48];
};

// Binary symbol aliases
using Enm_Takodozer = EnemyTakodozer;

} // namespace Game
