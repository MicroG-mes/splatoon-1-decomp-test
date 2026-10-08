#include "Game/MiniGame/LytMiniGameHandler.h"

namespace Game {

LytMiniGameHandler::LytMiniGameHandler()
    : mScreenState(MiniGameScreenState::cMenuSelect),
      mStateTimer(0),
      mSelectedMenuIndex(0),
      mWasJumpButtonPressed(false) {
}

LytMiniGameHandler::~LytMiniGameHandler() {
}

void LytMiniGameHandler::init() {
    GambitActor::init();
    mMiniGame.init();
    mScreenState = MiniGameScreenState::cMenuSelect;
}

void LytMiniGameHandler::open() {
    mScreenState = MiniGameScreenState::cMenuSelect;
    mStateTimer = 0;
    mSelectedMenuIndex = 0;
}

void LytMiniGameHandler::close() {
    mScreenState = MiniGameScreenState::cExiting;
}

void LytMiniGameHandler::handleInput(const VPADStatus& vpad) {
    switch (mScreenState) {
        case MiniGameScreenState::cMenuSelect: {
            if (vpad.trigger & VPAD_BUTTON_UP) {
                if (mSelectedMenuIndex > 0) mSelectedMenuIndex--;
            } else if (vpad.trigger & VPAD_BUTTON_DOWN) {
                if (mSelectedMenuIndex < 3) mSelectedMenuIndex++;
            } else if (vpad.trigger & VPAD_BUTTON_A) {
                mMiniGame.selectGame(static_cast<MiniGameType>(mSelectedMenuIndex));
                mScreenState = MiniGameScreenState::cPlaying;
            } else if (vpad.trigger & VPAD_BUTTON_B) {
                close();
            }
            break;
        }

        case MiniGameScreenState::cPlaying: {
            // Horizontal steering
            f32 steerX = 0.0f;
            if (vpad.hold & VPAD_BUTTON_LEFT) steerX -= 1.0f;
            if (vpad.hold & VPAD_BUTTON_RIGHT) steerX += 1.0f;
            if (vpad.leftStick.x > 0.2f || vpad.leftStick.x < -0.2f) {
                steerX = vpad.leftStick.x;
            }
            mMiniGame.steerHorizontal(steerX);

            // Jump charging / releasing
            bool isJumpHeld = (vpad.hold & (VPAD_BUTTON_A | VPAD_BUTTON_B | VPAD_BUTTON_X | VPAD_BUTTON_Y)) || vpad.tpData.touched;
            if (isJumpHeld) {
                mMiniGame.startChargeJump();
            } else if (mWasJumpButtonPressed && !isJumpHeld) {
                mMiniGame.releaseJump();
            }
            mWasJumpButtonPressed = isJumpHeld;

            // Pause
            if (vpad.trigger & VPAD_BUTTON_PLUS) {
                mScreenState = MiniGameScreenState::cPaused;
            }
            break;
        }

        case MiniGameScreenState::cPaused: {
            if (vpad.trigger & VPAD_BUTTON_PLUS) {
                mScreenState = MiniGameScreenState::cPlaying;
            } else if (vpad.trigger & VPAD_BUTTON_B) {
                mScreenState = MiniGameScreenState::cMenuSelect;
            }
            break;
        }

        case MiniGameScreenState::cGameOverView: {
            if (vpad.trigger & (VPAD_BUTTON_A | VPAD_BUTTON_B)) {
                mScreenState = MiniGameScreenState::cMenuSelect;
            }
            break;
        }

        default:
            break;
    }
}

void LytMiniGameHandler::update() {
    mStateTimer++;

    if (mScreenState == MiniGameScreenState::cPlaying) {
        mMiniGame.update();
        if (mMiniGame.isGameOver()) {
            mScreenState = MiniGameScreenState::cGameOverView;
        }
    }
}

void LytMiniGameHandler::draw() {
    GambitActor::draw();
    if (mScreenState == MiniGameScreenState::cPlaying) {
        mMiniGame.draw();
    }
}

} // namespace Game
