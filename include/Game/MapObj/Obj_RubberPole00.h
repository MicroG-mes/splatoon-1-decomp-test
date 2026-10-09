#pragma once

#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include <string>

namespace Game {

enum class RubberPoleState {
    cState_Idle = 0,
    cState_Deflected = 1
};

class Obj_RubberPole00 : public GambitActor {
public:
    struct Params {
        float mRadius = 1.5f;             // Pole collision cylinder radius
        float mHeight = 18.0f;            // Pole vertical height
        float mReboundSpeed = 2.5f;       // Player knockback rebound speed (m/s)
        float mSpringKp = 0.12f;          // Restoration stiffness
        float mSpringKd = 0.18f;          // Damping coefficient
        float mMaxDeflection = 0.40f;     // Max bend angle (~23 deg)
    };

    Obj_RubberPole00();
    virtual ~Obj_RubberPole00() = default;

    virtual void init() override;
    virtual void update() override;

    RubberPoleState getState() const { return mState; }
    const Params& getParams() const { return mParams; }
    const std::string& getName() const { return mName; }

    float getDeflectionAngle() const { return mDeflectionAngle; }
    const sead::Vector3f& getDeflectionDirection() const { return mDeflectionDir; }
    bool isBending() const { return mState == RubberPoleState::cState_Deflected; }

    // Player elastic collision
    bool checkPlayerCollision(const sead::Vector3f& playerPos, float playerRadius, sead::Vector3f& outReboundVel);

    // Ink shot impact
    bool applyShotDamage(const sead::Vector3f& hitPos, const sead::Vector3f& shotDir);

private:
    std::string mName = "Obj_RubberPole00";
    RubberPoleState mState = RubberPoleState::cState_Idle;
    Params mParams;

    float mDeflectionAngle = 0.0f;
    float mDeflectionVelocity = 0.0f;
    sead::Vector3f mDeflectionDir = sead::Vector3f(0.0f, 0.0f, 1.0f);
    int mTimer = 0;
};

} // namespace Game
