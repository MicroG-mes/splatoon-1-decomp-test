#include "Game/Sequence/SeqMgrPlaza.h"

namespace Game {

SeqMgrPlaza* SeqMgrPlaza::sInstance = nullptr;

SeqMgrPlaza::SeqMgrPlaza()
    : mCurrentState(PlazaSeqState::StartupSplash),
      mStateTimer(0),
      mIsFirstBoot(false) {
    sInstance = this;
}

SeqMgrPlaza::~SeqMgrPlaza() {
    if (sInstance == this) {
        sInstance = nullptr;
    }
}

void SeqMgrPlaza::init() {
    GambitActor::init();
    mStartupScreen.init();
    mTitleView.init();
    mNewsBroadcast.init();
    changeState(PlazaSeqState::StartupSplash);
}

void SeqMgrPlaza::changeState(PlazaSeqState state) {
    mCurrentState = state;
    mStateTimer = 0;

    switch (mCurrentState) {
        case PlazaSeqState::StartupSplash:
            mStartupScreen.changeState(StartupState::Sleep);
            break;

        case PlazaSeqState::TitleView:
            mTitleView.changeState(TitleViewState::Init);
            break;

        case PlazaSeqState::NewsBroadcast:
            mNewsBroadcast.startBroadcast();
            break;

        case PlazaSeqState::StageInDemoPlaza:
            // Load Inkopolis Plaza level geometry (Model/Fld_Plaza00.szs)
            break;

        case PlazaSeqState::PlazaFreeRoam:
            break;

        default:
            break;
    }
}

void SeqMgrPlaza::handleInput(const VPADStatus& vpad) {
    switch (mCurrentState) {
        case PlazaSeqState::TitleView:
            mTitleView.handleInput(vpad);
            break;
        case PlazaSeqState::PlazaFreeRoam:
            // Input handled by GamePlayer in plaza
            break;
        default:
            break;
    }
}

void SeqMgrPlaza::update() {
    mStateTimer++;

    switch (mCurrentState) {
        case PlazaSeqState::StartupSplash:
            mStartupScreen.update();
            if (mStartupScreen.isFinished()) {
                changeState(PlazaSeqState::TitleView);
            }
            break;

        case PlazaSeqState::TitleView:
            mTitleView.update();
            if (mTitleView.isStartConfirmed()) {
                // If save data is valid, transition to Squid Sisters news broadcast
                changeState(PlazaSeqState::NewsBroadcast);
            }
            break;

        case PlazaSeqState::NewsBroadcast:
            mNewsBroadcast.update();
            // Once broadcast finishes (or skipped by player), load plaza
            if (mStateTimer > 400) {
                changeState(PlazaSeqState::StageInDemoPlaza);
            }
            break;

        case PlazaSeqState::StageInDemoPlaza:
            if (mStateTimer > 60) {
                changeState(PlazaSeqState::PlazaFreeRoam);
            }
            break;

        case PlazaSeqState::PlazaFreeRoam:
            // Free roam in Inkopolis Plaza with other Inklings, shops, and tower
            break;

        default:
            break;
    }
}

void SeqMgrPlaza::draw() {
    switch (mCurrentState) {
        case PlazaSeqState::StartupSplash:
            mStartupScreen.draw();
            break;
        case PlazaSeqState::TitleView:
            mTitleView.draw();
            break;
        case PlazaSeqState::NewsBroadcast:
            mNewsBroadcast.draw();
            break;
        default:
            GambitActor::draw();
            break;
    }
}

} // namespace Game
