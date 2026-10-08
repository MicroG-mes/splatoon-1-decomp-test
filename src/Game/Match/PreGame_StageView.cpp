#include "Game/Match/PreGame_StageView.h"

namespace Game {

PreGame_StageView::PreGame_StageView()
    : mPhase(PreGamePhase::cStageFlyby),
      mPhaseTimer(0),
      mCameraPos(0.0f, 25.0f, -40.0f),
      mCameraLookAt(0.0f, 0.0f, 0.0f) {
}

PreGame_StageView::~PreGame_StageView() {
}

void PreGame_StageView::init() {
    GambitActor::init();
    mPhase = PreGamePhase::cStageFlyby;
    mPhaseTimer = 0;
}

void PreGame_StageView::startSequence() {
    mPhase = PreGamePhase::cStageFlyby;
    mPhaseTimer = 0;
    mCameraPos = sead::Vector3f(0.0f, 25.0f, -40.0f);
    mCameraLookAt = sead::Vector3f(0.0f, 0.0f, 0.0f);
}

void PreGame_StageView::update() {
    mPhaseTimer++;

    switch (mPhase) {
        case PreGamePhase::cStageFlyby:
            // Fly through stage center for 120 frames (2.0s)
            mCameraPos.z += 0.3f;
            if (mPhaseTimer > 120) {
                mPhase = PreGamePhase::cTeamFriendIntro;
                mPhaseTimer = 0;
                mCameraPos = sead::Vector3f(-15.0f, 2.0f, -30.0f);
                mCameraLookAt = sead::Vector3f(0.0f, 1.0f, -35.0f);
            }
            break;

        case PreGamePhase::cTeamFriendIntro:
            // Friendly team pan across 4 players (120 frames)
            mCameraPos.x += 0.25f;
            if (mPhaseTimer > 120) {
                mPhase = PreGamePhase::cTeamOppositeIntro;
                mPhaseTimer = 0;
                mCameraPos = sead::Vector3f(15.0f, 2.0f, 30.0f);
                mCameraLookAt = sead::Vector3f(0.0f, 1.0f, 35.0f);
            }
            break;

        case PreGamePhase::cTeamOppositeIntro:
            // Enemy team pan across 4 players (120 frames)
            mCameraPos.x -= 0.25f;
            if (mPhaseTimer > 120) {
                mPhase = PreGamePhase::cSpawnPointFocus;
                mPhaseTimer = 0;
            }
            break;

        case PreGamePhase::cSpawnPointFocus:
            // Snap camera directly behind local player on spawn kettle
            if (mPhaseTimer > 60) {
                mPhase = PreGamePhase::cFinished;
            }
            break;

        default:
            break;
    }
}

void PreGame_StageView::draw() {
    GambitActor::draw();
}

} // namespace Game
