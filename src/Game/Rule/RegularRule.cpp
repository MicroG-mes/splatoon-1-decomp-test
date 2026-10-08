#include "Game/Rule/RegularRule.h"

namespace Game {

RegularRule::RegularRule()
    : mAlphaPercent(0.0f),
      mBravoPercent(0.0f),
      mNeutralPercent(100.0f),
      mAlphaPoints(0),
      mBravoPoints(0) {
}

RegularRule::~RegularRule() {
}

void RegularRule::init() {
    GambitActor::init();
    mAlphaPercent = 0.0f;
    mBravoPercent = 0.0f;
    mNeutralPercent = 100.0f;
}

void RegularRule::updatePaintCoverage(u32 alphaInkedPixels, u32 bravoInkedPixels, u32 totalPaintablePixels) {
    if (totalPaintablePixels == 0) {
        return;
    }

    f32 total = static_cast<f32>(totalPaintablePixels);
    mAlphaPercent = (static_cast<f32>(alphaInkedPixels) / total) * 100.0f;
    mBravoPercent = (static_cast<f32>(bravoInkedPixels) / total) * 100.0f;

    mNeutralPercent = 100.0f - (mAlphaPercent + mBravoPercent);
    if (mNeutralPercent < 0.0f) {
        mNeutralPercent = 0.0f;
    }

    mAlphaPoints = static_cast<u32>(alphaInkedPixels * 0.1f);
    mBravoPoints = static_cast<u32>(bravoInkedPixels * 0.1f);
}

bool RegularRule::didAlphaWin() const {
    if (mAlphaPercent > mBravoPercent) {
        return true;
    } else if (mBravoPercent > mAlphaPercent) {
        return false;
    }
    // Splatoon rule: Alpha wins exact 0.1% ties by default (represented by +0.1% bonus in game)
    return true;
}

void RegularRule::update() {
}

void RegularRule::draw() {
    GambitActor::draw();
}

} // namespace Game
