#include "Game/MapObj/Obj_Goal.h"
#include "Game/System/AglParameter.h"
#include <cmath>

namespace Game {

bool GoalShieldParams::load(const char* goalParamsPath, const char* anmParamsPath) {
    // Defaults matching authentic retail values
    damageEffectAlphaAt01 = 0.25f;
    bombCoreDamageK = 5.0f;
    rollerSplashDamageK = 1.5f;
    rollerCoreDamageK = 4.0f;
    chargeBulletDamageK = 1.25f;
    barrierRadiusMaxFlicker = 0.20f;
    barrierRadiusFlickerKd = 0.95999998f;
    barrierRadiusFlickerCycleFrame = 12;
    barrierRadiusFlickerCycleGap = 0.20f;
    barrierRadiusFlickerCancelFrame = 5;

    animBossWait = 30.0f;
    animRelieved = 20.0f;
    animWait = 30.0f;

    if (goalParamsPath) {
        AglParameterObj obj;
        if (obj.loadFromFile(goalParamsPath)) {
            damageEffectAlphaAt01 = obj.getFloat("mDamageEffect_AlphaAtDamageRateZeroPointOne", damageEffectAlphaAt01);
            bombCoreDamageK = obj.getFloat("mBombCoreDamageK", bombCoreDamageK);
            rollerSplashDamageK = obj.getFloat("mRollerSplashDamageK", rollerSplashDamageK);
            rollerCoreDamageK = obj.getFloat("mRollerCoreDamageK", rollerCoreDamageK);
            chargeBulletDamageK = obj.getFloat("mChargeBulletDamageK", chargeBulletDamageK);
            barrierRadiusMaxFlicker = obj.getFloat("mBarrierRadiusMaxFlicker", barrierRadiusMaxFlicker);
            barrierRadiusFlickerKd = obj.getFloat("mBarrierRadiusFlickerKd", barrierRadiusFlickerKd);
            barrierRadiusFlickerCycleFrame = obj.getInt("mBarrierRadiusFlickerCycleFrame", barrierRadiusFlickerCycleFrame);
            barrierRadiusFlickerCycleGap = obj.getFloat("mBarrierRadiusFlickerCycleGap", barrierRadiusFlickerCycleGap);
            barrierRadiusFlickerCancelFrame = obj.getInt("mBarrierRadiusFlickerCancelFrame", barrierRadiusFlickerCancelFrame);
        }
    }

    if (anmParamsPath) {
        AglParameterObj anmObj;
        if (anmObj.loadFromFile(anmParamsPath)) {
            animBossWait = anmObj.getFloat("BossWait", animBossWait);
            animRelieved = anmObj.getFloat("Relieved", animRelieved);
            animWait = anmObj.getFloat("Wait", animWait);
        }
    }

    return true;
}

Obj_Goal::Obj_Goal()
    : mPosition(0.0f, 0.0f, 0.0f)
    , mState(GoalShieldState::cState_Shielded)
    , mShieldHp(cMaxShieldHp)
    , mCurrentBarrierRadius(2.0f)
    , mBaseBarrierRadius(2.0f)
    , mFlickerTimer(0)
    , mFreedTimer(0)
    , mStageNo(1) {
    mParams.load("content/Static/Obj_Goal.params", "content/Static/Obj_Goal_Namazu_AnmItp.params");
}

Obj_Goal::~Obj_Goal() {}

void Obj_Goal::init(const sead::Vector3f& pos, u32 stageNo) {
    mPosition = pos;
    mStageNo = stageNo;
    mState = GoalShieldState::cState_Shielded;
    mShieldHp = cMaxShieldHp;
    mBaseBarrierRadius = 2.0f;
    mCurrentBarrierRadius = mBaseBarrierRadius;
    mFlickerTimer = 0;
    mFreedTimer = 0;
    mParams.load("content/Static/Obj_Goal.params", "content/Static/Obj_Goal_Namazu_AnmItp.params");
}

void Obj_Goal::triggerFlicker() {
    mState = GoalShieldState::cState_ShieldFlicker;
    mFlickerTimer = mParams.barrierRadiusFlickerCycleFrame;
    mCurrentBarrierRadius = mBaseBarrierRadius + mParams.barrierRadiusMaxFlicker;
}

void Obj_Goal::breakShield() {
    mShieldHp = 0.0f;
    mState = GoalShieldState::cState_ZapfishFreed;
    mCurrentBarrierRadius = 0.0f;
    mFreedTimer = 0;
}

bool Obj_Goal::applyBulletDamage(f32 baseDamage) {
    if (mState >= GoalShieldState::cState_ShieldBroken) return false;
    mShieldHp -= baseDamage;
    if (mShieldHp <= 0.0f) {
        breakShield();
    } else {
        triggerFlicker();
    }
    return true;
}

bool Obj_Goal::applyChargedDamage(f32 baseDamage) {
    if (mState >= GoalShieldState::cState_ShieldBroken) return false;
    f32 dmg = baseDamage * mParams.chargeBulletDamageK;
    mShieldHp -= dmg;
    if (mShieldHp <= 0.0f) {
        breakShield();
    } else {
        triggerFlicker();
    }
    return true;
}

bool Obj_Goal::applyBombDamage(f32 baseDamage) {
    if (mState >= GoalShieldState::cState_ShieldBroken) return false;
    f32 dmg = baseDamage * mParams.bombCoreDamageK;
    mShieldHp -= dmg;
    if (mShieldHp <= 0.0f) {
        breakShield();
    } else {
        triggerFlicker();
    }
    return true;
}

bool Obj_Goal::applyRollerSmash(f32 baseDamage) {
    if (mState >= GoalShieldState::cState_ShieldBroken) return false;
    f32 dmg = baseDamage * mParams.rollerCoreDamageK;
    mShieldHp -= dmg;
    if (mShieldHp <= 0.0f) {
        breakShield();
    } else {
        triggerFlicker();
    }
    return true;
}

bool Obj_Goal::applyRollerSplash(f32 baseDamage) {
    if (mState >= GoalShieldState::cState_ShieldBroken) return false;
    f32 dmg = baseDamage * mParams.rollerSplashDamageK;
    mShieldHp -= dmg;
    if (mShieldHp <= 0.0f) {
        breakShield();
    } else {
        triggerFlicker();
    }
    return true;
}

void Obj_Goal::update() {
    if (mState == GoalShieldState::cState_ShieldFlicker) {
        if (mFlickerTimer > 0) {
            mFlickerTimer--;
            // Radius decays exponentially back to base
            f32 delta = mCurrentBarrierRadius - mBaseBarrierRadius;
            mCurrentBarrierRadius = mBaseBarrierRadius + delta * mParams.barrierRadiusFlickerKd;
        } else {
            mCurrentBarrierRadius = mBaseBarrierRadius;
            mState = GoalShieldState::cState_Shielded;
        }
    } else if (mState == GoalShieldState::cState_ZapfishFreed) {
        mFreedTimer++;
    }
}

bool Obj_Goal::checkPlayerTouch(const sead::Vector3f& playerPos) {
    if (mState != GoalShieldState::cState_ZapfishFreed) {
        return false;
    }

    f32 dx = playerPos.x - mPosition.x;
    f32 dy = playerPos.y - mPosition.y;
    f32 dz = playerPos.z - mPosition.z;
    f32 distSq = dx * dx + dy * dy + dz * dz;

    if (distSq <= (cTouchRadius * cTouchRadius)) {
        mState = GoalShieldState::cState_Collected;
        return true;
    }

    return false;
}

} // namespace Game
