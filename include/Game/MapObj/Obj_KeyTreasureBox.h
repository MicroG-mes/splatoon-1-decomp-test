#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include <string>

namespace Game {

enum class TreasureBoxState : u32 {
    cState_Locked = 0,
    cState_Unlocking,
    cState_Opening,
    cState_Opened
};

struct KeyTreasureBoxParams {
    s32 colOffFrame; // mColOffFrame (15)
    s32 openStFrame; // mOpenStFrame (30)

    bool load(const char* paramsPath);
};

/**
 * Obj_KeyTreasureBox
 * Authentic Octo Valley locked vault and treasure chest actor.
 * Opened using Obj_DoorKey00, with timed collision cutoff and reward ejection.
 */
class Obj_KeyTreasureBox : public GambitActor {
public:
    static constexpr f32 cInteractionRadius = 2.5f;

    Obj_KeyTreasureBox();
    virtual ~Obj_KeyTreasureBox() override;

    virtual void init() override;
    void init(const sead::Vector3f& pos, bool requiresKey = true, const std::string& rewardType = "SunkenScroll");
    virtual void update() override;

    // Key interaction
    bool tryUnlock(bool hasKey);

    // Getters
    TreasureBoxState getState() const { return mState; }
    bool isLocked() const { return mState == TreasureBoxState::cState_Locked; }
    bool isOpened() const { return mState == TreasureBoxState::cState_Opened; }
    bool isCollisionActive() const { return mIsCollisionActive; }
    bool isRewardSpawned() const { return mRewardSpawned; }
    const std::string& getRewardType() const { return mRewardType; }
    const KeyTreasureBoxParams& getParams() const { return mParams; }

private:
    TreasureBoxState mState;
    KeyTreasureBoxParams mParams;
    bool mRequiresKey;
    bool mIsCollisionActive;
    bool mRewardSpawned;
    std::string mRewardType;
    s32 mOpenTimer;
};

} // namespace Game
