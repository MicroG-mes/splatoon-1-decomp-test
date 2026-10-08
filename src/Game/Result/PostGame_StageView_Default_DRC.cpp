#include "Game/Result/PostGame_StageView_Default_DRC.h"

namespace Game {

PostGame_StageView_Default_DRC::PostGame_StageView_Default_DRC()
    : mAlphaPercent(0.0f),
      mBravoPercent(0.0f),
      mIsContinueConfirmed(false),
      mAnimTimer(0) {
}

PostGame_StageView_Default_DRC::~PostGame_StageView_Default_DRC() {
}

void PostGame_StageView_Default_DRC::init() {
    GambitActor::init();
    mIsContinueConfirmed = false;
    mAnimTimer = 0;
}

void PostGame_StageView_Default_DRC::setCoverageResults(f32 alphaPercent, f32 bravoPercent) {
    mAlphaPercent = alphaPercent;
    mBravoPercent = bravoPercent;
}

void PostGame_StageView_Default_DRC::handleInput(const VPADStatus& vpad) {
    if (vpad.trigger & (VPAD_BUTTON_A | VPAD_BUTTON_B) || vpad.tpData.touched) {
        mIsContinueConfirmed = true;
    }
}

void PostGame_StageView_Default_DRC::update() {
    mAnimTimer++;
}

void PostGame_StageView_Default_DRC::draw() {
    GambitActor::draw();
}

} // namespace Game
