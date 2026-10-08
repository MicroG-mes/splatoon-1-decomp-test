#include "Game/Sequence/StartupScreen.h"

namespace Game {

StartupScreen::StartupScreen()
    : mCurrentState(StartupState::Sleep),
      mTimer(0),
      mAlpha(0.0f) {}

StartupScreen::~StartupScreen() = default;

void StartupScreen::init() {
    GambitActor::init();
    changeState(StartupState::Sleep);
}

void StartupScreen::changeState(StartupState state) {
    mCurrentState = state;
    mTimer = 0;

    switch (mCurrentState) {
        case StartupState::Sleep:
            mAlpha = 0.0f;
            break;
        case StartupState::FadeIn:
            mAlpha = 0.0f;
            break;
        case StartupState::Show:
            mAlpha = 1.0f;
            break;
        case StartupState::FadeOut:
            mAlpha = 1.0f;
            break;
        case StartupState::Done:
            mAlpha = 0.0f;
            break;
    }
}

void StartupScreen::update() {
    mTimer++;

    switch (mCurrentState) {
        case StartupState::Sleep:
            // Wait 15 frames for initialization
            if (mTimer > 15) {
                changeState(StartupState::FadeIn);
            }
            break;

        case StartupState::FadeIn:
            mAlpha += 0.05f;
            if (mAlpha >= 1.0f) {
                mAlpha = 1.0f;
                changeState(StartupState::Show);
            }
            break;

        case StartupState::Show:
            // Display controller layout / Nintendo splash for ~120 frames (2 seconds)
            if (mTimer > 120) {
                changeState(StartupState::FadeOut);
            }
            break;

        case StartupState::FadeOut:
            mAlpha -= 0.05f;
            if (mAlpha <= 0.0f) {
                mAlpha = 0.0f;
                changeState(StartupState::Done);
            }
            break;

        case StartupState::Done:
            break;
    }
}

void StartupScreen::draw() {
    // Render splash layout with mAlpha transparency
}

} // namespace Game
