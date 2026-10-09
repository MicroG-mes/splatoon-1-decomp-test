#pragma once

#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include <string>

namespace Game {

enum class CylinderBallType {
    Normal = 0,   // Radius 7.5, HP 1.2
    Small = 1,    // Radius 5.5, HP 10.0
    Big = 2       // Radius 10.0, HP 10.0
};

enum class CylinderBallState {
    Rolling = 0,
    Stunned = 1,
    Popped = 2
};

class Obj_CylinderKingBall : public GambitActor {
public:
    struct Params {
        float mLife = 1.2f;                     // HP
        float mGndColRadius = 7.5f;             // Rolling collision radius
        float mVel = 2.5f;                      // Base roll speed (2.5 m/s)
        float mVelOnPlayerInk = 1.5f;           // Slowed speed on player ink (1.5 m/s)
        float mAcc = 1.0f;                      // Acceleration
        float mAccOnPlayerInk = 0.2f;           // Acceleration on friendly ink
        float mPlayerDamage = 0.60f;            // Contact damage to player (60 HP)
        float mImpactToPlayer = 2.5f;           // Player knockback velocity (2.5 m/s)
        float mDiePaintRadius = 80.0f;          // Death burst paint radius (8.0m)
        int mTrackPaintableRepeatFrame = 3;     // Ink trail drop cadence (every 3 frames)
    };

    Obj_CylinderKingBall(CylinderBallType type = CylinderBallType::Normal);
    virtual ~Obj_CylinderKingBall() = default;

    virtual void init() override;
    virtual void update() override;

    bool loadParams(const char* filePath = "content/Static/Obj_CylinderKingBall.params");
    bool loadParamsSmall(const char* filePath = "content/Static/Obj_CylinderKingBallSmall.params");
    bool loadParamsBig(const char* filePath = "content/Static/Obj_CylinderKingBallBig.params");

    CylinderBallType getType() const { return mType; }
    CylinderBallState getState() const { return mState; }
    const Params& getParams() const { return mParams; }
    const std::string& getName() const { return mName; }

    float getHealth() const { return mHealth; }
    bool isAlive() const { return mState != CylinderBallState::Popped && mHealth > 0.0f; }
    float getCurrentSpeed() const { return mCurrentSpeed; }

    void setRollingDirection(const sead::Vector3f& dir);
    void setOnPlayerInk(bool onPlayerInk) { mIsOnPlayerInk = onPlayerInk; }

    bool applyDamage(float damage);
    bool checkPlayerContact(const sead::Vector3f& playerPos, float& outDamage, sead::Vector3f& outKnockback);

    bool shouldDropInkTrail() const { return mTrailDroppedThisFrame; }
    int getTotalTrailsDropped() const { return mTotalTrailsDropped; }

private:
    std::string mName = "Obj_CylinderKingBall";
    CylinderBallType mType = CylinderBallType::Normal;
    CylinderBallState mState = CylinderBallState::Rolling;
    Params mParams;

    float mHealth = 1.2f;
    float mCurrentSpeed = 2.5f;
    sead::Vector3f mDirection{0.0f, 0.0f, 1.0f};
    bool mIsOnPlayerInk = false;
    int mFrameCounter = 0;
    bool mTrailDroppedThisFrame = false;
    int mTotalTrailsDropped = 0;
};

} // namespace Game
