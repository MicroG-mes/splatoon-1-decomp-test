#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class MissionState : u32 {
    cInit              = 0,
    cPlay              = 1,
    cCheckpointReached = 2,
    cPlayerRespawn     = 3,
    cGoalRescued       = 4,
    cGameOver          = 5
};

class Mission : public GambitActor {
public:
    Mission();
    virtual ~Mission() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void startMission(u32 missionId);
    void setCheckpoint(const sead::Vector3f& checkpointPos);
    void collectPowerEgg(u32 count = 1);
    void collectSunkenScroll();

    bool takePlayerDamage(); // Returns true if survived (armor break), false if splatted
    void pickupArmor();

    void triggerGoal();

    MissionState getState() const { return mMissionState; }
    u32 getMissionId() const { return mMissionId; }
    u32 getPowerEggs() const { return mPowerEggs; }
    bool hasSunkenScroll() const { return mHasScroll; }
    s32 getLives() const { return mLives; }
    bool hasArmor() const { return mHasArmor; }

    const sead::Vector3f& getRespawnPos() const { return mLastCheckpointPos; }

protected:
    MissionState mMissionState;
    s32 mStateTimer;

    u32 mMissionId;
    u32 mPowerEggs;
    bool mHasScroll;
    s32 mLives;
    bool mHasArmor;

    sead::Vector3f mLastCheckpointPos;

    undefined mReserved[0x38];
};

} // namespace Game
