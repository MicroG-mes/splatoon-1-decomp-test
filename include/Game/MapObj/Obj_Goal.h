#pragma once

#include "types.h"
#include "sead/math/seadVector.h"
#include <string>

namespace Game {

enum class GoalShieldState : u32 {
    cState_Shielded = 0,
    cState_ShieldFlicker,
    cState_ShieldBroken,
    cState_ZapfishFreed,
    cState_Collected
};

struct GoalShieldParams {
    f32 damageEffectAlphaAt01;          // mDamageEffect_AlphaAtDamageRateZeroPointOne (0.25)
    f32 bombCoreDamageK;                // mBombCoreDamageK (5.0)
    f32 rollerSplashDamageK;            // mRollerSplashDamageK (1.5)
    f32 rollerCoreDamageK;              // mRollerCoreDamageK (4.0)
    f32 chargeBulletDamageK;            // mChargeBulletDamageK (1.25)
    f32 barrierRadiusMaxFlicker;        // mBarrierRadiusMaxFlicker (0.20)
    f32 barrierRadiusFlickerKd;         // mBarrierRadiusFlickerKd (0.96)
    s32 barrierRadiusFlickerCycleFrame; // mBarrierRadiusFlickerCycleFrame (12)
    f32 barrierRadiusFlickerCycleGap;   // mBarrierRadiusFlickerCycleGap (0.20)
    s32 barrierRadiusFlickerCancelFrame;// mBarrierRadiusFlickerCancelFrame (5)

    // Animation interpolation parameters
    f32 animBossWait;                   // 30.0
    f32 animRelieved;                   // 20.0
    f32 animWait;                       // 30.0

    bool load(const char* goalParamsPath, const char* anmParamsPath);
};

/**
 * Obj_Goal
 * Protective Zapfish containment globe & mission goal pedestal actor.
 * Guarded by ink-crackable barrier with authentic ballistics scaling.
 */
class Obj_Goal {
public:
    static constexpr f32 cMaxShieldHp = 100.0f;
    static constexpr f32 cTouchRadius = 2.2f;

    Obj_Goal();
    ~Obj_Goal();

    void init(const sead::Vector3f& pos, u32 stageNo);
    void update();

    // Damage intake methods with authentic multipliers from Obj_Goal.params
    bool applyBulletDamage(f32 baseDamage);
    bool applyChargedDamage(f32 baseDamage);
    bool applyBombDamage(f32 baseDamage);
    bool applyRollerSmash(f32 baseDamage);
    bool applyRollerSplash(f32 baseDamage);

    // Player touch interaction to collect Zapfish
    bool checkPlayerTouch(const sead::Vector3f& playerPos);

    // Getters
    GoalShieldState getState() const { return mState; }
    f32 getShieldHp() const { return mShieldHp; }
    f32 getShieldHpRatio() const { return mShieldHp / cMaxShieldHp; }
    f32 getCurrentBarrierRadius() const { return mCurrentBarrierRadius; }
    bool isShieldBroken() const { return mState >= GoalShieldState::cState_ShieldBroken; }
    bool isCollected() const { return mState == GoalShieldState::cState_Collected; }
    u32 getStageNo() const { return mStageNo; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    const GoalShieldParams& getParams() const { return mParams; }

private:
    sead::Vector3f mPosition;
    GoalShieldState mState;
    GoalShieldParams mParams;
    f32 mShieldHp;
    f32 mCurrentBarrierRadius;
    f32 mBaseBarrierRadius;
    s32 mFlickerTimer;
    s32 mFreedTimer;
    u32 mStageNo;

    void triggerFlicker();
    void breakShield();
};

} // namespace Game
