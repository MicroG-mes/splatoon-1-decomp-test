#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "Game/Match/VSGameTime.h"
#include "Game/Match/PreGame_StageView.h"
#include "Game/Lobby/Lobby.h"

namespace Game {

enum class VSGameState : u32 {
    cState_Loading       = 0,
    cState_IntroStage    = 1,
    cState_ReadyGo       = 2,
    cState_Battle        = 3,
    cState_Overtime      = 4,
    cState_GameSet       = 5,
    cState_Result        = 6,
    cState_ReturnToLobby = 7
};

class VSGame : public GambitActor {
public:
    VSGame();
    virtual ~VSGame() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void startMatch(BattleMode mode, RankedRule rule, u32 stageId);
    void finishMatch(bool alphaWon, f32 scoreAlpha, f32 scoreBravo);

    VSGameState getState() const { return mState; }
    const VSGameTime& getGameTime() const { return mGameTime; }
    bool isMatchActive() const { return mState == VSGameState::cState_Battle || mState == VSGameState::cState_Overtime; }

    f32 getScoreAlpha() const { return mScoreAlpha; }
    f32 getScoreBravo() const { return mScoreBravo; }
    bool didAlphaWin() const { return mAlphaWon; }

protected:
    void updateReadyGo();
    void updateGameSet();

    VSGameState mState;
    s32 mStateTimer;

    BattleMode mBattleMode;
    RankedRule mRankedRule;
    u32 mStageId;

    VSGameTime mGameTime;
    PreGame_StageView mPreGameView;

    f32 mScoreAlpha;
    f32 mScoreBravo;
    bool mAlphaWon;

    undefined mReserved[0x38];
};

} // namespace Game
