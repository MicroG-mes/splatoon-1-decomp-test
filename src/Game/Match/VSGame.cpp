#include "Game/Match/VSGame.h"

namespace Game {

VSGame::VSGame()
    : mState(VSGameState::cState_Loading),
      mStateTimer(0),
      mBattleMode(BattleMode::RegularMatch),
      mRankedRule(RankedRule::SplatZones),
      mStageId(0),
      mScoreAlpha(0.0f),
      mScoreBravo(0.0f),
      mAlphaWon(false) {
}

VSGame::~VSGame() {
}

void VSGame::init() {
    GambitActor::init();
    mPreGameView.init();
    mState = VSGameState::cState_Loading;
}

void VSGame::startMatch(BattleMode mode, RankedRule rule, u32 stageId) {
    mBattleMode = mode;
    mRankedRule = rule;
    mStageId = stageId;

    u32 duration = (mode == BattleMode::RegularMatch) ? 180 : 300;
    mGameTime.init(duration);

    mPreGameView.startSequence();
    mState = VSGameState::cState_IntroStage;
    mStateTimer = 0;
}

void VSGame::finishMatch(bool alphaWon, f32 scoreAlpha, f32 scoreBravo) {
    mAlphaWon = alphaWon;
    mScoreAlpha = scoreAlpha;
    mScoreBravo = scoreBravo;
    mState = VSGameState::cState_GameSet;
    mStateTimer = 0;
}

void VSGame::updateReadyGo() {
    // 90 frames: 0-60 "Ready...", 60-90 "GO!"
    if (mStateTimer >= 90) {
        mState = VSGameState::cState_Battle;
        mStateTimer = 0;
    }
}

void VSGame::updateGameSet() {
    // 120 frames "GAME SET!" whistle banner
    if (mStateTimer >= 120) {
        mState = VSGameState::cState_Result;
        mStateTimer = 0;
    }
}

void VSGame::update() {
    mStateTimer++;

    switch (mState) {
        case VSGameState::cState_IntroStage:
            mPreGameView.update();
            if (mPreGameView.isFinished()) {
                mState = VSGameState::cState_ReadyGo;
                mStateTimer = 0;
            }
            break;

        case VSGameState::cState_ReadyGo:
            updateReadyGo();
            break;

        case VSGameState::cState_Battle:
        case VSGameState::cState_Overtime:
            mGameTime.update();
            if (mGameTime.isTimeUp()) {
                // If match ended without knockout, trigger finish
                finishMatch(mScoreAlpha >= mScoreBravo, mScoreAlpha, mScoreBravo);
            }
            break;

        case VSGameState::cState_GameSet:
            updateGameSet();
            break;

        case VSGameState::cState_Result:
            // Match result ceremony active
            break;

        default:
            break;
    }
}

void VSGame::draw() {
    GambitActor::draw();
    if (mState == VSGameState::cState_IntroStage) {
        mPreGameView.draw();
    }
}

} // namespace Game
