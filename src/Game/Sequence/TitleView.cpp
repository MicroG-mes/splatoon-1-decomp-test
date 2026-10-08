#include "Game/Sequence/TitleView.h"

namespace Game {

TitleView::TitleView()
    : mCurrentState(TitleViewState::Init),
      mTimer(0),
      mLogoScale(1.0f),
      mPromptBlink(false) {}

TitleView::~TitleView() = default;

void TitleView::init() {
    GambitActor::init();
    changeState(TitleViewState::Init);
}

void TitleView::changeState(TitleViewState state) {
    mCurrentState = state;
    mTimer = 0;
}

void TitleView::handleInput(const VPADStatus& vpad) {
    if (mCurrentState != TitleViewState::WaitInput) return;

    // Both ZL and ZR must be pressed together to start Splatoon!
    bool zlPressed = (vpad.hold & VPAD_BUTTON_ZL) != 0;
    bool zrPressed = (vpad.hold & VPAD_BUTTON_ZR) != 0;

    // Or press A / Start button as convenience fallback
    bool startPressed = (vpad.trigger & (VPAD_BUTTON_A | VPAD_BUTTON_PLUS)) != 0;

    if ((zlPressed && zrPressed) || startPressed) {
        changeState(TitleViewState::StartTriggered);
    }
}

void TitleView::update() {
    mTimer++;

    switch (mCurrentState) {
        case TitleViewState::Init:
            if (mTimer > 30) {
                changeState(TitleViewState::WaitInput);
            }
            break;

        case TitleViewState::WaitInput:
            // Blink "Press ZL + ZR" prompt every 30 frames
            mPromptBlink = (mTimer % 60) < 40;
            break;

        case TitleViewState::StartTriggered:
            // Play confirmation ink sound & splash animation for 45 frames
            mLogoScale += 0.01f;
            if (mTimer > 45) {
                changeState(TitleViewState::FadeOut);
            }
            break;

        case TitleViewState::FadeOut:
            if (mTimer > 30) {
                changeState(TitleViewState::Done);
            }
            break;

        case TitleViewState::Done:
            break;
    }
}

void TitleView::draw() {
    // Render Title Screen logo, ink splatters, and flashing ZL+ZR prompt
}

} // namespace Game
