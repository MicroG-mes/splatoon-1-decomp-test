#include "Game/Amiibo/LytAmiiboHandler.h"
#include "Game/System/SaveDataMgr.h"

namespace Game {

LytAmiiboHandler::LytAmiiboHandler()
    : mState(AmiiboScreenState::cWaitingNfcTouch),
      mActiveFigure(AmiiboFigureType::Unknown),
      mStateTimer(0),
      mSelectedMissionIndex(0),
      mDialogueText("Touch an amiibo to the NFC touchpoint on the Wii U GamePad.") {
    for (int i = 0; i < 20; ++i) {
        mMissions[i].missionIndex = i;
        mMissions[i].stageId = i + 1;
        mMissions[i].requiredWeapon = 0;
        mMissions[i].isCompleted = false;
        mMissions[i].cashReward = 1000 + (i * 200);
    }
}

LytAmiiboHandler::~LytAmiiboHandler() {
}

void LytAmiiboHandler::init() {
    GambitActor::init();
    mState = AmiiboScreenState::cWaitingNfcTouch;
}

void LytAmiiboHandler::populateChallenges() {
    u32 weaponId = 0;
    switch (mActiveFigure) {
        case AmiiboFigureType::InklingGirl:  weaponId = 2; break; // Hero Charger
        case AmiiboFigureType::InklingBoy:   weaponId = 1; break; // Hero Roller
        case AmiiboFigureType::InklingSquid: weaponId = 3; break; // Kraken Challenge
        default: break;
    }

    for (int i = 0; i < 20; ++i) {
        mMissions[i].requiredWeapon = weaponId;
    }
}

void LytAmiiboHandler::onAmiiboScanned(AmiiboFigureType figure) {
    mActiveFigure = figure;
    populateChallenges();
    mState = AmiiboScreenState::cChallengeGridSelect;
    mStateTimer = 0;
    mDialogueText = "Choose a challenge to test your skills!";
}

void LytAmiiboHandler::completeChallenge(u32 missionIndex) {
    if (missionIndex < 20) {
        mMissions[missionIndex].isCompleted = true;
        SaveDataMgr* save = SaveDataMgr::instance();
        if (save) {
            save->addMoney(mMissions[missionIndex].cashReward);
        }
    }
}

u32 LytAmiiboHandler::getCompletedCount() const {
    u32 count = 0;
    for (int i = 0; i < 20; ++i) {
        if (mMissions[i].isCompleted) count++;
    }
    return count;
}

bool LytAmiiboHandler::isRewardUnlocked(u32 tierIndex) const {
    u32 required = (tierIndex + 1) * 4;
    return getCompletedCount() >= required;
}

void LytAmiiboHandler::handleInput(const VPADStatus& vpad) {
    switch (mState) {
        case AmiiboScreenState::cWaitingNfcTouch:
            if (vpad.trigger & VPAD_BUTTON_B) {
                mState = AmiiboScreenState::cExiting;
            }
            break;

        case AmiiboScreenState::cChallengeGridSelect:
            if (vpad.trigger & VPAD_BUTTON_LEFT) {
                if (mSelectedMissionIndex > 0) mSelectedMissionIndex--;
            } else if (vpad.trigger & VPAD_BUTTON_RIGHT) {
                if (mSelectedMissionIndex < 19) mSelectedMissionIndex++;
            } else if (vpad.trigger & VPAD_BUTTON_A) {
                mState = AmiiboScreenState::cChallengeBriefing;
            } else if (vpad.trigger & VPAD_BUTTON_B) {
                mState = AmiiboScreenState::cExiting;
            }
            break;

        case AmiiboScreenState::cChallengeBriefing:
            if (vpad.trigger & VPAD_BUTTON_A) {
                // Launch into challenge stage
            } else if (vpad.trigger & VPAD_BUTTON_B) {
                mState = AmiiboScreenState::cChallengeGridSelect;
            }
            break;

        default:
            break;
    }
}

void LytAmiiboHandler::update() {
    mStateTimer++;
}

void LytAmiiboHandler::draw() {
    GambitActor::draw();
}

} // namespace Game
