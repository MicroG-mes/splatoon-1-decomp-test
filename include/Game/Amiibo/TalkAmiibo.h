#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "Game/Amiibo/LytAmiiboHandler.h"

namespace Game {

enum class AmiiboTalkState : u32 {
    cPromptScan      = 0,
    cGreetingTalk    = 1,
    cSelectChallenge = 2,
    cRewardGrant     = 3,
    cFarewell        = 4
};

class TalkAmiibo : public GambitActor {
public:
    TalkAmiibo();
    virtual ~TalkAmiibo() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void startInteraction(AmiiboFigureType figureType);
    void selectChallengeIndex(u32 challengeIndex);
    bool claimTierReward(u32 tierIndex);
    void endInteraction();

    AmiiboTalkState getState() const { return mState; }
    AmiiboFigureType getActiveFigure() const { return mActiveFigure; }
    u32 getSelectedChallenge() const { return mSelectedChallenge; }
    bool isCompletedTier(u32 tierIndex) const;

protected:
    AmiiboFigureType mActiveFigure;
    AmiiboTalkState mState;
    u32 mSelectedChallenge;
    u32 mCompletedMask;
    s32 mTimer;

    undefined mReserved[0x38];
};

} // namespace Game
