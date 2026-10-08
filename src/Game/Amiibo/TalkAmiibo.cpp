#include "Game/Amiibo/TalkAmiibo.h"

namespace Game {

TalkAmiibo::TalkAmiibo()
    : mActiveFigure(AmiiboFigureType::cNone),
      mState(AmiiboTalkState::cPromptScan),
      mSelectedChallenge(0),
      mCompletedMask(0),
      mTimer(0) {
}

TalkAmiibo::~TalkAmiibo() {
}

void TalkAmiibo::init() {
    GambitActor::init();
    mActiveFigure = AmiiboFigureType::cNone;
    mState = AmiiboTalkState::cPromptScan;
    mSelectedChallenge = 0;
    mCompletedMask = 0;
    mTimer = 0;
}

void TalkAmiibo::startInteraction(AmiiboFigureType figureType) {
    mActiveFigure = figureType;
    mState = AmiiboTalkState::cGreetingTalk;
    mSelectedChallenge = 0;
    mTimer = 0;
}

void TalkAmiibo::selectChallengeIndex(u32 challengeIndex) {
    if (challengeIndex < 20) {
        mSelectedChallenge = challengeIndex;
        mState = AmiiboTalkState::cSelectChallenge;
        mTimer = 0;
    }
}

bool TalkAmiibo::claimTierReward(u32 tierIndex) {
    if (tierIndex >= 5) {
        return false;
    }

    u32 tierMask = (0xF << (tierIndex * 4));
    if ((mCompletedMask & tierMask) == tierMask) {
        mState = AmiiboTalkState::cRewardGrant;
        mTimer = 0;
        return true;
    }

    return false;
}

bool TalkAmiibo::isCompletedTier(u32 tierIndex) const {
    if (tierIndex >= 5) {
        return false;
    }
    u32 tierMask = (0xF << (tierIndex * 4));
    return (mCompletedMask & tierMask) == tierMask;
}

void TalkAmiibo::endInteraction() {
    mState = AmiiboTalkState::cFarewell;
    mTimer = 0;
}

void TalkAmiibo::update() {
    mTimer++;

    switch (mState) {
        case AmiiboTalkState::cGreetingTalk:
            if (mTimer >= 60) {
                mState = AmiiboTalkState::cSelectChallenge;
                mTimer = 0;
            }
            break;

        case AmiiboTalkState::cRewardGrant:
            if (mTimer >= 90) {
                mState = AmiiboTalkState::cSelectChallenge;
                mTimer = 0;
            }
            break;

        case AmiiboTalkState::cFarewell:
            if (mTimer >= 45) {
                mState = AmiiboTalkState::cPromptScan;
                mActiveFigure = AmiiboFigureType::cNone;
                mTimer = 0;
            }
            break;

        case AmiiboTalkState::cPromptScan:
        case AmiiboTalkState::cSelectChallenge:
        default:
            break;
    }
}

void TalkAmiibo::draw() {
    GambitActor::draw();
}

} // namespace Game
