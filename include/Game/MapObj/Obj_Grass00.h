#pragma once

#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include <string>

namespace Game {

enum class GrassState {
    cState_Wait_random = 0,    // Idle wind rustling with phase offset
    cState_Rub = 1,            // Gentle rustling from walking player
    cState_RubBend = 2,        // Deep deflection from squid dash
    cState_DamageShot = 3,     // Ink hit impulse shock
    cState_DamageShotBend = 4  // Bending reaction away from ink shot
};

class Obj_Grass00 : public GambitActor {
public:
    struct Params {
        float mTouchRadius = 4.0f;       // Player brush radius
        float mShotHitRadius = 3.0f;      // Ink bullet hit radius
        float mSpringKp = 0.08f;          // Restoration spring stiffness
        float mSpringKd = 0.20f;          // Restoration spring damping
        float mMaxBendAngle = 0.45f;      // Max bend in radians (~25 deg)
        int mRecoveryFrames = 30;         // Return to upright frames
    };

    Obj_Grass00(bool isRuins = false);
    virtual ~Obj_Grass00() = default;

    virtual void init() override;
    virtual void update() override;

    bool loadParams(const char* filePath = "content/Static/Obj_Grass00_AnmItp.params");

    GrassState getState() const { return mState; }
    const Params& getParams() const { return mParams; }
    const std::string& getName() const { return mName; }

    float getBendAngle() const { return mBendAngle; }
    const sead::Vector3f& getBendDirection() const { return mBendDir; }
    bool isBending() const { return mState != GrassState::cState_Wait_random; }

    // Player touch interaction (walking or squid rush)
    bool checkPlayerTouch(const sead::Vector3f& playerPos, const sead::Vector3f& playerVel, bool isSquid);

    // Ink projectile impact
    bool applyShotDamage(const sead::Vector3f& hitPos, const sead::Vector3f& shotDir);

private:
    std::string mName = "Obj_Grass00";
    GrassState mState = GrassState::cState_Wait_random;
    Params mParams;

    bool mIsRuins = false;
    float mBendAngle = 0.0f;
    float mBendVelocity = 0.0f;
    sead::Vector3f mBendDir = sead::Vector3f(0.0f, 0.0f, 1.0f);
    int mTimer = 0;
    float mWindPhase = 0.0f;
};

} // namespace Game
