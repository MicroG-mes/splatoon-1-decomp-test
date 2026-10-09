#pragma once

#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include <string>

namespace Game {

enum class TreeState {
    cState_Wait = 0,            // Idle wind sway
    cState_DamageShot = 1,       // Trunk vibration shock
    cState_DamageShotBend = 2    // Canopy bending away from blast
};

class Obj_Tree00 : public GambitActor {
public:
    struct Params {
        float mTrunkRadius = 3.5f;        // Base trunk collision radius
        float mTrunkHeight = 25.0f;       // Trunk vertical height
        float mCanopyRadius = 12.0f;      // Foliage radius
        float mSpringKp = 0.03f;          // Sway spring restoration
        float mSpringKd = 0.15f;          // Sway damping
        float mMaxSwayAngle = 0.20f;      // Peak bend angle in radians (~11.5 deg)
        int mRecoveryFrames = 45;         // Frame duration to settle back to idle
    };

    Obj_Tree00(int variant = 0);
    virtual ~Obj_Tree00() = default;

    virtual void init() override;
    virtual void update() override;

    bool loadParams(const char* filePath = "content/Static/Obj_Tree00_AnmItp.params");

    TreeState getState() const { return mState; }
    const Params& getParams() const { return mParams; }
    const std::string& getName() const { return mName; }

    float getSwayAngle() const { return mSwayAngle; }
    const sead::Vector3f& getSwayDirection() const { return mSwayDir; }
    bool isShaking() const { return mState != TreeState::cState_Wait; }

    bool applyShotDamage(const sead::Vector3f& hitPos, const sead::Vector3f& shotDir);

private:
    std::string mName = "Obj_Tree00";
    TreeState mState = TreeState::cState_Wait;
    Params mParams;

    int mVariant = 0;
    float mSwayAngle = 0.0f;
    float mSwayVelocity = 0.0f;
    sead::Vector3f mSwayDir = sead::Vector3f(0.0f, 0.0f, 1.0f);
    int mTimer = 0;
    float mWindPhase = 0.0f;
};

} // namespace Game
