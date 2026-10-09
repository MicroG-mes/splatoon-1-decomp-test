#include "Game/Npc/Npc_JudgeSleep.h"
#include <cmath>
#include <algorithm>

namespace Game {

Npc_JudgeSleep::Npc_JudgeSleep()
    : mState(JudgeSleepState::cDeepSleep),
      mAnimFrame(0),
      mBreathCycle(0.0f),
      mChestExpansion(1.0f),
      mSnoreBubbleSize(0.0f),
      mTwitchTimer(0),
      mLookAtPos(0.8f, 16.8f, 0.0f),
      mCameraPos(-1.1f, 21.3f, 36.9f) {
}

Npc_JudgeSleep::~Npc_JudgeSleep() {
}

void Npc_JudgeSleep::init() {
    GambitActor::init();
    vfunc_3();
    vfunc_5();
}

void Npc_JudgeSleep::vfunc_3() {
    mModel = sead::BfresParser::createJudgeSleepModel("Npc_JudgeSleep");
}

void Npc_JudgeSleep::vfunc_5() {
    mState = JudgeSleepState::cDeepSleep;
    mAnimFrame = 0;
    mBreathCycle = 0.0f;
    mChestExpansion = 1.0f;
    mSnoreBubbleSize = 0.0f;
    mTwitchTimer = 0;
}

void Npc_JudgeSleep::vfunc_7() {
    mAnimFrame++;

    if (mState == JudgeSleepState::cStartleWake) {
        mSnoreBubbleSize = 0.0f;
        mChestExpansion = 1.0f;
        return;
    }

    // 90-frame rhythmic breathing cycle (1.5 seconds per breath)
    const f32 kBreathPeriod = 90.0f;
    f32 breathPhase = static_cast<f32>(mAnimFrame % static_cast<u32>(kBreathPeriod)) / kBreathPeriod;
    mBreathCycle = breathPhase;

    // Smooth sinusoidal chest expansion: 1.00 at exhale, 1.06 at inhale
    f32 sineBreath = (std::sin(breathPhase * 6.2831853f - 1.5707963f) + 1.0f) * 0.5f;
    mChestExpansion = 1.0f + 0.06f * sineBreath;

    // Snore bubble expands during inhale, slowly deflates during exhale
    mSnoreBubbleSize = sineBreath * 0.85f;

    // Random ear twitch / tail waggle every 240 frames
    if ((mAnimFrame % 240) < 20) {
        mState = JudgeSleepState::cTwitch;
    } else {
        mState = JudgeSleepState::cDeepSleep;
    }
}

void Npc_JudgeSleep::update() {
    vfunc_7();
}

void Npc_JudgeSleep::draw() {
}

void Npc_JudgeSleep::wakeUp() {
    mState = JudgeSleepState::cStartleWake;
    mSnoreBubbleSize = 0.0f;
}

void Npc_JudgeSleep::fallAsleep() {
    mState = JudgeSleepState::cDeepSleep;
}

} // namespace Game
