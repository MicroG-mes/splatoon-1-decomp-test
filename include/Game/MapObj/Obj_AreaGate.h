#pragma once

#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include <string>

namespace Game {

enum class AreaGateState {
    cState_Locked = 0,    // High-voltage barrier active, blocks player
    cState_Shaking = 1,   // Field fluctuates / cancels after power condition met
    cState_Opening = 2,   // Barrier dematerializes
    cState_Opened = 3     // Completely open and passable
};

class Obj_AreaGate : public GambitActor {
public:
    struct Params {
        float mBarrierWidth = 60.0f;        // Collision barrier width (60.0m)
        float mBarrierHeight = 300.0f;      // Electric wall height (300.0m)
        float mBarrierBoundVelLen = 3.0f;   // Horizontal rebound knockback velocity (3.0 m/s)
        float mBarrierBoundVelY = 0.3f;     // Vertical rebound bounce impulse
        int mShakeCancelFrame = 20;         // Shake and de-energize frame duration (20f)
    };

    Obj_AreaGate(int areaId = 1, int requiredPowerMW = 100);
    virtual ~Obj_AreaGate() = default;

    virtual void init() override;
    virtual void update() override;

    bool loadParams(const char* filePath = "content/Static/Obj_AreaGate.params");

    AreaGateState getState() const { return mState; }
    const Params& getParams() const { return mParams; }
    const std::string& getName() const { return mName; }

    int getAreaId() const { return mAreaId; }
    int getRequiredPowerMW() const { return mRequiredPowerMW; }
    bool isLocked() const { return mState == AreaGateState::cState_Locked; }
    bool isPassable() const { return mState == AreaGateState::cState_Opened; }

    bool tryUnlock(int currentPowerMW);
    bool checkPlayerCollision(const sead::Vector3f& playerPos, sead::Vector3f& outReboundVel);

private:
    std::string mName = "Obj_AreaGate";
    AreaGateState mState = AreaGateState::cState_Locked;
    Params mParams;

    int mAreaId = 1;
    int mRequiredPowerMW = 100;
    int mShakeTimer = 0;
};

} // namespace Game
