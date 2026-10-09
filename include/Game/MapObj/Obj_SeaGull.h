#pragma once

#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include <string>

namespace Game {

enum class SeaGullState {
    cState_Wait_random = 0,    // Perched idle looking around
    cState_TakeOff = 1,        // Flapping wings upward
    cState_Fly = 2             // Soaring away into sky
};

class Obj_SeaGull : public GambitActor {
public:
    struct Params {
        float mScareRadius = 12.0f;       // Player approach distance triggering take-off
        float mShotScareRadius = 8.0f;    // Ink impact splash alert radius
        float mFlySpeed = 5.0f;           // Horizontal cruising flight speed
        float mAscentSpeed = 3.5f;        // Climb speed during takeoff
        int mTakeOffFrames = 25;          // Flap duration to reach flight speed
    };

    Obj_SeaGull();
    virtual ~Obj_SeaGull() = default;

    virtual void init() override;
    virtual void update() override;

    bool loadParams(const char* filePath = "content/Static/Obj_SeaGull_AnmItp.params");

    SeaGullState getState() const { return mState; }
    const Params& getParams() const { return mParams; }
    const std::string& getName() const { return mName; }

    bool isPerched() const { return mState == SeaGullState::cState_Wait_random; }
    bool isAirborne() const { return mState != SeaGullState::cState_Wait_random; }
    float getAltitude() const { return mPosition.y; }

    // Alert triggers
    bool checkPlayerProximity(const sead::Vector3f& playerPos);
    bool applyDamage(const sead::Vector3f& hitPos);

    void triggerTakeOff(const sead::Vector3f& escapeDir);

private:
    std::string mName = "Obj_SeaGull";
    SeaGullState mState = SeaGullState::cState_Wait_random;
    Params mParams;

    sead::Vector3f mFlightVelocity = sead::Vector3f(0.0f, 0.0f, 0.0f);
    int mTimer = 0;
    float mPerchY = 0.0f;
};

} // namespace Game
