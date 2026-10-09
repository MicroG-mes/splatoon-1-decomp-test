#pragma once

#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include <string>

namespace Game {

enum class SighterTargetType {
    cTarget_Normal = 0,    // Standard dummy: 100 HP, 1.0x scale
    cTarget_Strong = 1     // Heavy dummy: 500 HP (Defense Up), 1.7x scale
};

enum class SighterTargetState {
    cState_Ready = 0,           // Upright and vulnerable
    cState_DamageShot = 1,      // Wobbling from ink bullet impact
    cState_DamageShotBend = 2,  // Sustained deflection bend
    cState_Burst = 3,           // Popped / exploded into ink splash
    cState_Respawn = 4          // Inflating back into place (Expand animation)
};

class Obj_SighterTarget : public GambitActor {
public:
    struct Params {
        float mHpNormal = 100.0f;                    // Standard health
        float mHpStrong = 500.0f;                    // Defense-Up health
        float mScaleStrong = 1.7f;                   // Scale for strong dummy
        float mColRadiusBulletNormal = 5.0f;         // Normal shot collision radius
        float mColHeightBulletNormal = 15.0f;        // Normal shot collision height
        float mColRadiusBulletStrong = 8.5f;         // Strong shot collision radius
        float mColHeightBulletStrong = 25.5f;        // Strong shot collision height
        int mNoDamageRefreshFrame = 120;             // Frames before auto-heal begins
        int mBurstWaitFrame = 120;                   // Cooldown before respawn inflation
        float mBendKp = 0.05f;                       // Spring stiffness
        float mBendKd = 0.30f;                       // Spring damping
        float mMaxBendAngle = 0.50f;                 // Max deflection angle (~28 deg)
    };

    Obj_SighterTarget(SighterTargetType type = SighterTargetType::cTarget_Normal);
    virtual ~Obj_SighterTarget() = default;

    virtual void init() override;
    virtual void update() override;

    bool loadParams(const char* filePath = "content/Static/Obj_SighterTarget.params");

    SighterTargetType getType() const { return mType; }
    SighterTargetState getState() const { return mState; }
    const Params& getParams() const { return mParams; }
    const std::string& getName() const { return mName; }

    float getHealth() const { return mHealth; }
    float getMaxHealth() const { return mMaxHp; }
    float getScale() const { return mCurrentScale; }
    float getBendAngle() const { return mBendAngle; }
    const sead::Vector3f& getBendDirection() const { return mBendDir; }
    bool isBurst() const { return mState == SighterTargetState::cState_Burst; }
    bool isReady() const { return mState == SighterTargetState::cState_Ready; }

    bool applyDamage(float damage, const sead::Vector3f& hitPos, const sead::Vector3f& shotDir, u32 teamId = 0);
    bool checkHit(const sead::Vector3f& shotPos, float shotRadius = 0.5f) const;

private:
    std::string mName = "Obj_SighterTarget";
    SighterTargetType mType = SighterTargetType::cTarget_Normal;
    SighterTargetState mState = SighterTargetState::cState_Ready;
    Params mParams;

    float mHealth = 100.0f;
    float mMaxHp = 100.0f;
    float mTargetScale = 1.0f;
    float mCurrentScale = 1.0f;
    float mBendAngle = 0.0f;
    float mBendVelocity = 0.0f;
    sead::Vector3f mBendDir = sead::Vector3f(0.0f, 0.0f, 1.0f);
    int mTimer = 0;
    int mDamageCooldownTimer = 0;
    u32 mLastTeam = 0;
};

} // namespace Game
