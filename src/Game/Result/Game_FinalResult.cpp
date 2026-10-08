#include "Game/Result/Game_FinalResult.h"
#include "Game/System/SaveDataMgr.h"

namespace Game {

Game_FinalResult::Game_FinalResult()
    : mPhase(FinalResultPhase::cCoverageRoll),
      mPhaseTimer(0),
      mBattleMode(BattleMode::RegularMatch),
      mAlphaWon(false),
      mFinalScoreAlpha(0.0f),
      mFinalScoreBravo(0.0f),
      mDisplayScoreAlpha(0.0f),
      mDisplayScoreBravo(0.0f),
      mAwardedExp(0),
      mAwardedMoney(0),
      mRankPointsDelta(0),
      mIsLevelUp(false),
      mIsRankUp(false) {
}

Game_FinalResult::~Game_FinalResult() {
}

void Game_FinalResult::init() {
    GambitActor::init();
    mJudd.init();
    mDrcView.init();
    mPhase = FinalResultPhase::cCoverageRoll;
}

void Game_FinalResult::startResultSequence(BattleMode mode, bool alphaWon, f32 alphaScore, f32 bravoScore) {
    mBattleMode = mode;
    mAlphaWon = alphaWon;
    mFinalScoreAlpha = alphaScore;
    mFinalScoreBravo = bravoScore;

    mDisplayScoreAlpha = 0.0f;
    mDisplayScoreBravo = 0.0f;

    mPhase = FinalResultPhase::cCoverageRoll;
    mPhaseTimer = 0;

    mDrcView.setCoverageResults(alphaScore, bravoScore);
    calculateRewards();
}

void Game_FinalResult::setPlayerResult(u32 slot, const char* name, u32 level, s32 rank, u32 weaponId, u32 splats, u32 deaths, u32 turfInked, bool isLocal) {
    if (slot < 8) {
        mPlayers[slot].setPlayerInfo(name, level, rank, weaponId, splats, deaths, turfInked, isLocal);
    }
}

void Game_FinalResult::calculateRewards() {
    // Local player stats
    u32 localTurfInked = mPlayers[0].getStats().turfInked;

    if (mBattleMode == BattleMode::RegularMatch) {
        // Turf War calculation:
        // Win bonus = 300 EXP, 300 Money.
        // Turf bonus = up to 200 EXP (100 if >= 200p, 200 if >= 500p)
        mAwardedExp = (localTurfInked >= 500) ? 200 : (localTurfInked >= 200 ? 100 : 0);
        mAwardedMoney = localTurfInked;

        if (mAlphaWon) { // Assuming player is on winning team
            mAwardedExp += 300;
            mAwardedMoney += 300;
        }
        mRankPointsDelta = 0; // No rank change in Regular match
    } else {
        // Ranked calculation:
        // Knockout win = 5000 EXP, 5000 Money.
        // Rank point delta = +10 on win, -10 on loss.
        if (mAlphaWon) {
            mAwardedExp = 5000;
            mAwardedMoney = 5000;
            mRankPointsDelta = 10;
        } else {
            mAwardedExp = 1500;
            mAwardedMoney = 1500;
            mRankPointsDelta = -10;
        }
    }
}

void Game_FinalResult::applySaveProgress() {
    SaveDataMgr* save = SaveDataMgr::instance();
    if (save) {
        save->addMoney(mAwardedMoney);
        save->addExp(mAwardedExp);
    }
}

void Game_FinalResult::update() {
    mPhaseTimer++;

    switch (mPhase) {
        case FinalResultPhase::cCoverageRoll: {
            // Numbers roll up smoothly over 90 frames (1.5s)
            mDisplayScoreAlpha += (mFinalScoreAlpha - mDisplayScoreAlpha) * 0.08f;
            mDisplayScoreBravo += (mFinalScoreBravo - mDisplayScoreBravo) * 0.08f;

            if (mPhaseTimer > 90) {
                mDisplayScoreAlpha = mFinalScoreAlpha;
                mDisplayScoreBravo = mFinalScoreBravo;
                mPhase = FinalResultPhase::cJuddVerdict;
                mPhaseTimer = 0;
                mJudd.triggerBattleResult(mAlphaWon);
            }
            break;
        }

        case FinalResultPhase::cJuddVerdict: {
            mJudd.update();
            if (mPhaseTimer > 120) { // 2.0s Judd flag animation
                mPhase = FinalResultPhase::cPlayerScoreboard;
                mPhaseTimer = 0;
            }
            break;
        }

        case FinalResultPhase::cPlayerScoreboard: {
            if (mPhaseTimer > 150) { // 2.5s scoreboard view
                mPhase = FinalResultPhase::cExpMoneyLevelUp;
                mPhaseTimer = 0;
                applySaveProgress();
            }
            break;
        }

        case FinalResultPhase::cExpMoneyLevelUp: {
            if (mPhaseTimer > 120) {
                mPhase = FinalResultPhase::cFinished;
            }
            break;
        }

        default:
            break;
    }
}

void Game_FinalResult::draw() {
    GambitActor::draw();
    if (mPhase == FinalResultPhase::cJuddVerdict) {
        mJudd.draw();
    }
}

} // namespace Game
