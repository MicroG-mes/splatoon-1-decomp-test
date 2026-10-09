#pragma once

#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include <string>

namespace Game {

enum class GateManholeState {
    cState_Hidden = 0,     // Invisible / ink-covered until inked by player
    cState_Revealed = 1,   // Discovered kettle, lid open, ready for entry
    cState_Warping = 2,    // Player squid submerging, 35f warp countdown
    cState_Cleared = 3     // Mission cleared, mini-zapfish rescued
};

class Obj_GateManhole : public GambitActor {
public:
    struct Params {
        float mWarpColRadius = 23.0f;       // Squid submerge warp trigger radius (23.0m solo, 13.0m boss)
        int mCommanderSearchProbability = 420;
        int mCommanderNoSearchFrame = 100;
        int mOpenDemoWaitFrame = 60;
        int mInWaitFrame = 35;              // Submerge-to-warp countdown (35 frames)
        float mStageIconOffsetY = 20.0f;
    };

    Obj_GateManhole(const std::string& destinationStage = "Fld_EasyClimb00_Msn", bool isBossKettle = false);
    virtual ~Obj_GateManhole() = default;

    virtual void init() override;
    virtual void update() override;

    bool loadParams(const char* filePath = "content/Static/Obj_GateManhole.params");
    bool loadBossParams(const char* filePath = "content/Static/Obj_BossGateway.params");

    GateManholeState getState() const { return mState; }
    const Params& getParams() const { return mParams; }
    const std::string& getName() const { return mName; }

    const std::string& getDestinationStage() const { return mDestinationStage; }
    bool isBossKettle() const { return mIsBossKettle; }

    bool isRevealed() const { return mState != GateManholeState::cState_Hidden; }
    bool isWarpTriggered() const { return mWarpTriggered; }
    int getWarpTimer() const { return mWarpTimer; }

    void reveal();
    void setCleared(bool cleared = true);

    bool tryEnterWarp(const sead::Vector3f& playerPos, bool isSquidForm);

private:
    std::string mName = "Obj_GateManhole";
    GateManholeState mState = GateManholeState::cState_Hidden;
    Params mParams;

    std::string mDestinationStage;
    bool mIsBossKettle = false;
    bool mWarpTriggered = false;
    int mWarpTimer = 0;
};

} // namespace Game
