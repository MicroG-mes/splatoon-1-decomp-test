#include "Game/Fest/LytPlazaFestChoiceMgr.h"
#include "Game/System/SaveDataMgr.h"

namespace Game {

LytPlazaFestChoiceMgr::LytPlazaFestChoiceMgr()
    : mState(FestVoteState::cSleep),
      mStateTimer(0),
      mTeamAName("Team Alpha"),
      mTeamBName("Team Bravo"),
      mHasVoted(false),
      mChosenTeam(0),
      mCursorIndex(0) {
}

LytPlazaFestChoiceMgr::~LytPlazaFestChoiceMgr() {
}

void LytPlazaFestChoiceMgr::init() {
    GambitActor::init();
    mState = FestVoteState::cSleep;
    mHasVoted = false;
}

void LytPlazaFestChoiceMgr::openVotingBooth(const char* teamAName, const char* teamBName) {
    mTeamAName = teamAName;
    mTeamBName = teamBName;
    mCursorIndex = 0;
    mState = FestVoteState::cInFestChoice;
    mStateTimer = 0;
}

void LytPlazaFestChoiceMgr::castVote(u32 teamChoice) {
    mChosenTeam = teamChoice;
    mHasVoted = true;
    mState = FestVoteState::cReceiveTee;
    mStateTimer = 0;

    // Equip temporary Splatfest Tee
    SaveDataMgr* save = SaveDataMgr::instance();
    if (save) {
        InklingCustomization custom = save->getCustomization();
        custom.clothesId = 999; // Splatfest Tee ID
        save->setCustomization(custom);
    }
}

void LytPlazaFestChoiceMgr::handleInput(const VPADStatus& vpad) {
    switch (mState) {
        case FestVoteState::cInFestChoice:
            if (vpad.trigger & VPAD_BUTTON_LEFT) {
                mCursorIndex = 0;
            } else if (vpad.trigger & VPAD_BUTTON_RIGHT) {
                mCursorIndex = 1;
            } else if (vpad.trigger & VPAD_BUTTON_A) {
                mState = FestVoteState::cVoteConfirm;
            } else if (vpad.trigger & VPAD_BUTTON_B) {
                mState = FestVoteState::cSleep;
            }
            break;

        case FestVoteState::cVoteConfirm:
            if (vpad.trigger & VPAD_BUTTON_A) {
                castVote(mCursorIndex);
            } else if (vpad.trigger & VPAD_BUTTON_B) {
                mState = FestVoteState::cInFestChoice;
            }
            break;

        case FestVoteState::cReceiveTee:
            if (vpad.trigger & (VPAD_BUTTON_A | VPAD_BUTTON_B)) {
                mState = FestVoteState::cFinished;
            }
            break;

        default:
            break;
    }
}

void LytPlazaFestChoiceMgr::update() {
    mStateTimer++;
}

void LytPlazaFestChoiceMgr::draw() {
    GambitActor::draw();
}

} // namespace Game
