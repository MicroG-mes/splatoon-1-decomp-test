#include "Game/Mission/Mission.h"

namespace Game {

Mission::Mission()
    : mMissionState(MissionState::cInit),
      mStateTimer(0),
      mMissionId(1),
      mPowerEggs(0),
      mHasScroll(false),
      mLives(3),
      mHasArmor(true),
      mLastCheckpointPos(0.0f, 0.0f, 0.0f) {
}

Mission::~Mission() {
}

void Mission::init() {
    GambitActor::init();
    mMissionState = MissionState::cInit;
}

void Mission::startMission(u32 missionId) {
    mMissionId = missionId;
    mPowerEggs = 0;
    mHasScroll = false;
    mLives = 3;
    mHasArmor = true;
    mLastCheckpointPos = sead::Vector3f(0.0f, 0.0f, 0.0f);
    mMissionState = MissionState::cPlay;
    mStateTimer = 0;
}

void Mission::setCheckpoint(const sead::Vector3f& checkpointPos) {
    mLastCheckpointPos = checkpointPos;
    mMissionState = MissionState::cCheckpointReached;
    mStateTimer = 0;
}

void Mission::collectPowerEgg(u32 count) {
    mPowerEggs += count;
}

void Mission::collectSunkenScroll() {
    mHasScroll = true;
}

bool Mission::takePlayerDamage() {
    if (mHasArmor) {
        // Armor shatters, saving player from death
        mHasArmor = false;
        return true;
    }

    // Lethal hit without armor
    mLives--;
    if (mLives <= 0) {
        mMissionState = MissionState::cGameOver;
        mStateTimer = 0;
    } else {
        mMissionState = MissionState::cPlayerRespawn;
        mStateTimer = 0;
    }
    return false;
}

void Mission::pickupArmor() {
    mHasArmor = true;
}

void Mission::triggerGoal() {
    mMissionState = MissionState::cGoalRescued;
    mStateTimer = 0;
}

void Mission::update() {
    mStateTimer++;

    switch (mMissionState) {
        case MissionState::cCheckpointReached:
            if (mStateTimer > 45) {
                mMissionState = MissionState::cPlay;
            }
            break;

        case MissionState::cPlayerRespawn:
            if (mStateTimer > 60) { // Respawn delay
                mHasArmor = true;   // Regains base armor on respawn
                mMissionState = MissionState::cPlay;
            }
            break;

        default:
            break;
    }
}

void Mission::draw() {
    GambitActor::draw();
}

} // namespace Game
