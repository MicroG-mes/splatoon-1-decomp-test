#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "Game/Result/VSGamePlayerNameResult.h"
#include "Game/Result/PostGame_StageView_Default_DRC.h"
#include "Game/Npc/Npc_Judge_Flag.h"
#include "Game/Lobby/Lobby.h"

namespace Game {

enum class FinalResultPhase : u32 {
    cCoverageRoll     = 0,
    cJuddVerdict      = 1,
    cPlayerScoreboard = 2,
    cExpMoneyLevelUp  = 3,
    cFinished         = 4
};

class Game_FinalResult : public GambitActor {
public:
    Game_FinalResult();
    virtual ~Game_FinalResult() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void startResultSequence(BattleMode mode, bool alphaWon, f32 alphaScore, f32 bravoScore);
    void setPlayerResult(u32 slot, const char* name, u32 level, s32 rank, u32 weaponId, u32 splats, u32 deaths, u32 turfInked, bool isLocal);

    FinalResultPhase getPhase() const { return mPhase; }
    bool isFinished() const { return mPhase == FinalResultPhase::cFinished; }

    f32 getDisplayScoreAlpha() const { return mDisplayScoreAlpha; }
    f32 getDisplayScoreBravo() const { return mDisplayScoreBravo; }
    bool didAlphaWin() const { return mAlphaWon; }

    u32 getAwardedExp() const { return mAwardedExp; }
    u32 getAwardedMoney() const { return mAwardedMoney; }
    s32 getRankPointsDelta() const { return mRankPointsDelta; }

protected:
    void calculateRewards();
    void applySaveProgress();

    FinalResultPhase mPhase;
    s32 mPhaseTimer;

    BattleMode mBattleMode;
    bool mAlphaWon;
    f32 mFinalScoreAlpha;
    f32 mFinalScoreBravo;

    f32 mDisplayScoreAlpha;
    f32 mDisplayScoreBravo;

    u32 mAwardedExp;
    u32 mAwardedMoney;
    s32 mRankPointsDelta;
    bool mIsLevelUp;
    bool mIsRankUp;

    Npc_Judge_Flag mJudd;
    PostGame_StageView_Default_DRC mDrcView;
    VSGamePlayerNameResult mPlayers[8];

    undefined mReserved[0x38];
};

} // namespace Game
