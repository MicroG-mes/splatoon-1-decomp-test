#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "cafe/vpad.h"

namespace Game {

enum class AmiiboFigureType : u32 {
    cNone        = 0xFF,
    InklingGirl  = 0,
    InklingBoy   = 1,
    InklingSquid = 2,
    Callie       = 3,
    Marie        = 4,
    Unknown      = 0xFF
};

enum class AmiiboScreenState : u32 {
    cWaitingNfcTouch     = 0,
    cChallengeGridSelect = 1,
    cChallengeBriefing   = 2,
    cRewardClaimed       = 3,
    cExiting             = 4
};

struct AmiiboChallengeMission {
    u32 missionIndex; // 0 to 19
    u32 stageId;
    u32 requiredWeapon;
    bool isCompleted;
    u32 cashReward;
};

class LytAmiiboHandler : public GambitActor {
public:
    LytAmiiboHandler();
    virtual ~LytAmiiboHandler() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void handleInput(const VPADStatus& vpad);

    void onAmiiboScanned(AmiiboFigureType figure);
    void completeChallenge(u32 missionIndex);
    bool isRewardUnlocked(u32 tierIndex) const; // 0 = Head, 1 = Clothes, 2 = Shoes, 3 = Weapon, 4 = MiniGame

    AmiiboScreenState getState() const { return mState; }
    AmiiboFigureType getActiveFigure() const { return mActiveFigure; }
    u32 getCompletedCount() const;

    bool isExitRequested() const { return mState == AmiiboScreenState::cExiting; }

protected:
    void populateChallenges();

    AmiiboScreenState mState;
    AmiiboFigureType mActiveFigure;
    s32 mStateTimer;

    s32 mSelectedMissionIndex;
    AmiiboChallengeMission mMissions[20];

    const char* mDialogueText;
    undefined mReserved[0x38];
};

} // namespace Game
