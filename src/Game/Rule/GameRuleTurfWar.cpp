#include "Game/Rule/GameRuleTurfWar.h"
#include <cstdio>
#include <cmath>

namespace Game {

GameRuleTurfWar::GameRuleTurfWar()
    : mState(MatchState::cIntro)
    , mRemainingFrames(cTotalMatchFrames)
    , mStateTimer(0)
    , mAlphaPercent(0.0f)
    , mBravoPercent(0.0f)
{
    mResult.winnerTeam = 2;
    mResult.alphaTurfPercent = 0.0f;
    mResult.bravoTurfPercent = 0.0f;
    mResult.alphaTurfPoints = 0;
    mResult.bravoTurfPoints = 0;
    mResult.isKnockout = false;
}

GameRuleTurfWar::~GameRuleTurfWar() {
}

void GameRuleTurfWar::reset() {
    mState = MatchState::cIntro;
    mRemainingFrames = cTotalMatchFrames;
    mStateTimer = 0;
    mAlphaPercent = 0.0f;
    mBravoPercent = 0.0f;
    mResult.winnerTeam = 2;
    mResult.alphaTurfPercent = 0.0f;
    mResult.bravoTurfPercent = 0.0f;
    mResult.alphaTurfPoints = 0;
    mResult.bravoTurfPoints = 0;
    mResult.isKnockout = false;
}

void GameRuleTurfWar::startMatch() {
    mState = MatchState::cPlaying;
    mRemainingFrames = cTotalMatchFrames;
    mStateTimer = 0;
}

void GameRuleTurfWar::forceFinish() {
    mRemainingFrames = 0;
    mState = MatchState::cFinish;
    mStateTimer = 0;
}

void GameRuleTurfWar::update(f32 currentAlphaPercent, f32 currentBravoPercent) {
    mAlphaPercent = currentAlphaPercent;
    mBravoPercent = currentBravoPercent;

    switch (mState) {
    case MatchState::cIntro:
        mStateTimer++;
        if (mStateTimer >= cIntroFrames) {
            mState = MatchState::cReadyGo;
            mStateTimer = 0;
        }
        break;

    case MatchState::cReadyGo:
        mStateTimer++;
        if (mStateTimer >= cReadyGoFrames) {
            mState = MatchState::cPlaying;
            mStateTimer = 0;
        }
        break;

    case MatchState::cPlaying:
        if (mRemainingFrames > 0) {
            mRemainingFrames--;
            if (mRemainingFrames <= cOneMinuteFrames) {
                mState = MatchState::cOneMinute;
            }
        } else {
            mState = MatchState::cFinish;
            mStateTimer = 0;
        }
        break;

    case MatchState::cOneMinute:
        if (mRemainingFrames > 0) {
            mRemainingFrames--;
        } else {
            mState = MatchState::cFinish;
            mStateTimer = 0;
        }
        break;

    case MatchState::cFinish:
        mStateTimer++;
        if (mStateTimer >= cFinishBannerFrames) {
            mState = MatchState::cJudgement;
            mStateTimer = 0;
            computeJudgement();
        }
        break;

    case MatchState::cJudgement:
        mStateTimer++;
        break;
    }
}

void GameRuleTurfWar::computeJudgement() {
    mResult.alphaTurfPercent = mAlphaPercent;
    mResult.bravoTurfPercent = mBravoPercent;

    // Standard Walleye Warehouse / Inkopolis ~1,600 paintable points
    mResult.alphaTurfPoints = static_cast<u32>(mAlphaPercent * 16.0f + 0.5f);
    mResult.bravoTurfPoints = static_cast<u32>(mBravoPercent * 16.0f + 0.5f);

    const f32 eps = 0.05f; // Precision threshold
    if (mAlphaPercent > mBravoPercent + eps) {
        mResult.winnerTeam = 0; // Team Alpha Victory
    } else if (mBravoPercent > mAlphaPercent + eps) {
        mResult.winnerTeam = 1; // Team Bravo Victory
    } else {
        mResult.winnerTeam = 2; // Dead Heat Tie / Draw
    }

    mResult.isKnockout = (std::abs(mAlphaPercent - mBravoPercent) >= 40.0f);
}

std::string GameRuleTurfWar::getFormattedTime() const {
    u32 totalSec = mRemainingFrames / 60;
    u32 minutes = totalSec / 60;
    u32 seconds = totalSec % 60;
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%u:%02u", minutes, seconds);
    return std::string(buf);
}

} // namespace Game
