#include "Game/Hud/LytVSGameHandler.h"

namespace Game {

LytVSGameHandler::LytVSGameHandler()
    : mScoreAlpha(0.0f),
      mScoreBravo(0.0f),
      mRemainingSeconds(180),
      mSelectedSuperJumpTarget(-1),
      mAnimTimer(0) {
    for (int i = 0; i < 4; ++i) {
        mTeamAlpha[i].isAlive = true;
        mTeamAlpha[i].respawnFrames = 0;
        mTeamAlpha[i].isSpecialActive = false;
        mTeamAlpha[i].weaponId = 0;
        mTeamAlpha[i].worldPos = sead::Vector3f(0.0f, 0.0f, 0.0f);

        mTeamBravo[i].isAlive = true;
        mTeamBravo[i].respawnFrames = 0;
        mTeamBravo[i].isSpecialActive = false;
        mTeamBravo[i].weaponId = 0;
        mTeamBravo[i].worldPos = sead::Vector3f(0.0f, 0.0f, 0.0f);
    }
}

LytVSGameHandler::~LytVSGameHandler() {
}

void LytVSGameHandler::init() {
    GambitActor::init();
    mSelectedSuperJumpTarget = -1;
}

void LytVSGameHandler::setPlayerStatus(u32 team, u32 slot, bool isAlive, s32 respawnFrames, bool isSpecialActive, const sead::Vector3f& pos) {
    if (slot >= 4) return;
    SquidStatusSlot& s = (team == 0) ? mTeamAlpha[slot] : mTeamBravo[slot];
    s.isAlive = isAlive;
    s.respawnFrames = respawnFrames;
    s.isSpecialActive = isSpecialActive;
    s.worldPos = pos;
}

void LytVSGameHandler::setScores(f32 scoreAlpha, f32 scoreBravo) {
    mScoreAlpha = scoreAlpha;
    mScoreBravo = scoreBravo;
}

void LytVSGameHandler::setTime(s32 remainingSeconds) {
    mRemainingSeconds = remainingSeconds;
}

const SquidStatusSlot& LytVSGameHandler::getSlot(u32 team, u32 slot) const {
    if (slot >= 4) slot = 3;
    return (team == 0) ? mTeamAlpha[slot] : mTeamBravo[slot];
}

void LytVSGameHandler::checkSuperJumpTouch(const VPADStatus& vpad) {
    // GamePad touch screen coordinates or D-pad fast-jump shortcuts
    if (vpad.trigger & VPAD_BUTTON_UP) {
        mSelectedSuperJumpTarget = 0; // Spawn point
    } else if (vpad.trigger & VPAD_BUTTON_RIGHT) {
        mSelectedSuperJumpTarget = 1; // Teammate 1
    } else if (vpad.trigger & VPAD_BUTTON_DOWN) {
        mSelectedSuperJumpTarget = 2; // Teammate 2
    } else if (vpad.trigger & VPAD_BUTTON_LEFT) {
        mSelectedSuperJumpTarget = 3; // Teammate 3
    } else if (vpad.tpData.touched) {
        // Touch on right-side icon column
        if (vpad.tpData.x > 700) {
            if (vpad.tpData.y < 150) mSelectedSuperJumpTarget = 0;
            else if (vpad.tpData.y < 280) mSelectedSuperJumpTarget = 1;
            else if (vpad.tpData.y < 410) mSelectedSuperJumpTarget = 2;
            else mSelectedSuperJumpTarget = 3;
        }
    }
}

void LytVSGameHandler::handleInput(const VPADStatus& vpad) {
    checkSuperJumpTouch(vpad);
}

void LytVSGameHandler::update() {
    mAnimTimer++;

    // Decrement respawn timers
    for (int i = 0; i < 4; ++i) {
        if (!mTeamAlpha[i].isAlive && mTeamAlpha[i].respawnFrames > 0) {
            mTeamAlpha[i].respawnFrames--;
            if (mTeamAlpha[i].respawnFrames == 0) {
                mTeamAlpha[i].isAlive = true;
            }
        }
        if (!mTeamBravo[i].isAlive && mTeamBravo[i].respawnFrames > 0) {
            mTeamBravo[i].respawnFrames--;
            if (mTeamBravo[i].respawnFrames == 0) {
                mTeamBravo[i].isAlive = true;
            }
        }
    }
}

void LytVSGameHandler::draw() {
    GambitActor::draw();
}

} // namespace Game
