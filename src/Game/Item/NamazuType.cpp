#include "Game/Item/NamazuType.h"
#include <cmath>

namespace Game {

NamazuType::NamazuType()
    : mPosition(0.0f, 35.0f, 0.0f),
      mState(ZapfishState::cSleepingTower),
      mTimer(0),
      mSparkTimer(0),
      mBreathingPhase(0.0f),
      mElectricCharge(1.0f) {
}

NamazuType::~NamazuType() {
}

void NamazuType::init() {
    GambitActor::init();
    mPosition.set(0.0f, 35.0f, 0.0f);
    mState = ZapfishState::cSleepingTower;
    mTimer = 0;
    mSparkTimer = 0;
    mBreathingPhase = 0.0f;
    mElectricCharge = 1.0f;
}

void NamazuType::setPlazaTowerPerch(const sead::Vector3f& towerApexPos) {
    mPosition = towerApexPos;
    mState = ZapfishState::cSleepingTower;
}

void NamazuType::triggerRescueCelebration() {
    mState = ZapfishState::cRescueFlight;
    mTimer = 0;
    mElectricCharge = 1.0f;
}

void NamazuType::setFestiveMode(bool isSplatfestActive) {
    if (isSplatfestActive) {
        mState = ZapfishState::cFestivalDancing;
    } else {
        mState = ZapfishState::cSleepingTower;
    }
}

void NamazuType::emitElectricSparks() {
    mSparkTimer++;
    if (mSparkTimer >= cSparkIntervalFrames) {
        mSparkTimer = 0;
        // Spawns crackling electric arcs around whiskers and dorsal spine
    }
}

void NamazuType::update() {
    mTimer++;

    // Breathing scale animation
    mBreathingPhase += 0.025f;
    if (mBreathingPhase > 6.283185f) {
        mBreathingPhase -= 6.283185f;
    }

    emitElectricSparks();

    switch (mState) {
        case ZapfishState::cRescueFlight:
            // Fly up into the sky towards Inkopolis
            mPosition.y += 0.15f;
            if (mTimer >= 180) {
                mState = ZapfishState::cSleepingTower;
                mTimer = 0;
            }
            break;

        case ZapfishState::cFestivalDancing:
            // Swings tail to Splatfest beat
            break;

        case ZapfishState::cSleepingTower:
        case ZapfishState::cCaptiveOctoDome:
        default:
            break;
    }
}

void NamazuType::draw() {
    GambitActor::draw();
}

} // namespace Game
