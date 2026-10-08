#include "Game/Enemy/RailKingPilotHouse.h"

namespace Game {

RailKingPilotHouse::RailKingPilotHouse()
    : mPosition(0.0f, 0.0f, 0.0f),
      mState(PilotHouseState::cIntactCockpit),
      mTimer(0),
      mDazeDuration(0),
      mShieldHp(cMaxShieldHp) {
}

RailKingPilotHouse::~RailKingPilotHouse() {
}

void RailKingPilotHouse::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mState = PilotHouseState::cIntactCockpit;
    mTimer = 0;
    mDazeDuration = 0;
    mShieldHp = cMaxShieldHp;
}

void RailKingPilotHouse::applyDirectHit(f32 damage) {
    if (mState == PilotHouseState::cEjectedDefeat) {
        return;
    }

    mShieldHp -= damage;
    if (mShieldHp <= 50.0f && mState == PilotHouseState::cIntactCockpit) {
        mState = PilotHouseState::cGlassCracked;
    }

    if (mShieldHp <= 0.0f) {
        triggerDaze(300);
    }
}

void RailKingPilotHouse::triggerDaze(s32 durationFrames) {
    mState = PilotHouseState::cVulnerableDazed;
    mTimer = 0;
    mDazeDuration = durationFrames;
}

void RailKingPilotHouse::resetCockpit() {
    mShieldHp = cMaxShieldHp;
    mState = PilotHouseState::cIntactCockpit;
    mTimer = 0;
}

void RailKingPilotHouse::update() {
    mTimer++;

    switch (mState) {
        case PilotHouseState::cVulnerableDazed:
            if (mTimer >= mDazeDuration) {
                resetCockpit();
            }
            break;

        case PilotHouseState::cIntactCockpit:
        case PilotHouseState::cGlassCracked:
        case PilotHouseState::cEjectedDefeat:
        default:
            break;
    }
}

void RailKingPilotHouse::draw() {
    GambitActor::draw();
}

} // namespace Game
