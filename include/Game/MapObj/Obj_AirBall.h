#pragma once

#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include <string>

namespace Game {

enum class AirBallType {
    cType_Mission = 0,    // Octo Valley target sequence balloon
    cType_Duel = 1,       // Battle Dojo score balloon
    cType_StaffRoll = 2   // Credits staff roll balloon
};

enum class AirBallState {
    cState_Hidden = 0,      // Inactive, awaiting sequence trigger
    cState_Appear = 1,      // Spawning/inflating (Appear animation, 15f)
    cState_Wait = 2,        // Floating idle harmonic bobbing
    cState_DamageShot = 3,  // Flinching/ink splash hit
    cState_Burst = 4,       // Popped/detonated
    cState_Timeout = 5      // Sequence time expired
};

class Obj_AirBall : public GambitActor {
public:
    struct Params {
        float mMaxHp = 1.0f;                    // Health threshold to pop
        float mCollisionRadius = 10.0f;         // Sphere collision radius (DAT_100b80c0)
        int mPeriodFrames = 80;                 // Harmonic bobbing period (DAT_100b80b4: 80.0f)
        float mBobbingAmplitude = 1.5f;         // Vertical bobbing amplitude (DAT_100b80bc: 1.5f)
        float mSpringKp = 0.04f;                // Spring stiffness (DAT_100b80b0: 0.04f)
        float mSpringKd = 0.30f;                // Spring damping (DAT_100b80ac: 0.30f)
        int mAppearFrames = 15;                 // Spawning frames
        int mDamageShotFrames = 10;             // Damage reaction frames
        int mSequenceTimeLimit = 600;           // Sequence countdown limit (10 seconds)
        int mEggRewardCount = 5;                // Power eggs rewarded
        int mDuelScorePoints = 1;               // Battle Dojo points (2 in fever)
    };

    Obj_AirBall(AirBallType type = AirBallType::cType_Mission);
    virtual ~Obj_AirBall() = default;

    virtual void init() override;
    virtual void update() override;

    bool loadParams(const char* filePath = "content/Static/Obj_AirBall_AnmItp.params");

    AirBallType getType() const { return mType; }
    AirBallState getState() const { return mState; }
    const Params& getParams() const { return mParams; }
    const std::string& getName() const { return mName; }

    float getHealth() const { return mHealth; }
    float getScale() const { return mScale; }
    float getCurrentYOffset() const { return mBobbingY; }
    int getTimeRemaining() const { return mTimeRemaining; }
    bool isBurst() const { return mState == AirBallState::cState_Burst; }
    bool isChainCompleted() const { return mChainCompleted; }

    // Sequence chaining configuration
    void setSequence(int seqId, int stepIndex, int maxSteps, int timeLimit = 600);
    void linkNext(Obj_AirBall* next) { mLinkedNext = next; }
    int getSequenceId() const { return mSequenceId; }
    int getStepIndex() const { return mStepIndex; }
    int getMaxSteps() const { return mMaxSteps; }

    // Activation & Damage
    void triggerAppear();
    bool applyDamage(float damage, u32 teamId = 0);
    bool checkHit(const sead::Vector3f& targetPos, float targetRadius = 1.0f) const;

    // Battle Dojo fever mode toggle
    void setFeverMode(bool fever) { mIsFever = fever; }
    bool isFeverMode() const { return mIsFever; }

private:
    std::string mName = "Obj_AirBall";
    AirBallType mType = AirBallType::cType_Mission;
    AirBallState mState = AirBallState::cState_Hidden;
    Params mParams;

    float mHealth = 1.0f;
    float mScale = 0.0f;
    float mBobbingY = 0.0f;
    float mBobbingVelocity = 0.0f;
    int mFrameCounter = 0;
    int mStateTimer = 0;
    int mTimeRemaining = 0;
    u32 mLastHitterTeam = 0;
    bool mIsFever = false;

    // Sequence linkage
    int mSequenceId = 0;
    int mStepIndex = 0;
    int mMaxSteps = 1;
    bool mChainCompleted = false;
    Obj_AirBall* mLinkedNext = nullptr;
};

} // namespace Game
