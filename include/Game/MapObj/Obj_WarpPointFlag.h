#pragma once

#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include <string>

namespace Game {

enum class WarpFlagState {
    Unchecked = 0,    // Flag down, awaiting player touch
    Raised = 1,       // Active checkpoint spawn point
    Exhausted = 2     // All respawn charges consumed
};

class Obj_WarpPointFlag : public GambitActor {
public:
    struct Params {
        float mLife = 3.0f; // Max respawns / beacon durability (3.0f)
    };

    Obj_WarpPointFlag(int checkpointIndex = 0);
    virtual ~Obj_WarpPointFlag() = default;

    virtual void init() override;
    virtual void update() override;

    bool loadParams(const char* filePath = "content/Static/Obj_WarpPointFlag.params");

    WarpFlagState getState() const { return mState; }
    const Params& getParams() const { return mParams; }
    const std::string& getName() const { return mName; }

    int getCheckpointIndex() const { return mCheckpointIndex; }
    int getRemainingCharges() const { return mRemainingCharges; }
    bool isActive() const { return mState == WarpFlagState::Raised; }

    bool tryActivate(const sead::Vector3f& playerPos, float touchRadius = 15.0f);
    bool consumeRespawnCharge(sead::Vector3f& outRespawnPos);

private:
    std::string mName = "Obj_WarpPointFlag";
    WarpFlagState mState = WarpFlagState::Unchecked;
    Params mParams;

    int mCheckpointIndex = 0;
    int mRemainingCharges = 3;
    float mFlagRiseProgress = 0.0f;
};

} // namespace Game
