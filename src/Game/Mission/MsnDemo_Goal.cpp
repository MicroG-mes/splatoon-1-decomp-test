#include "Game/Mission/MsnDemo_Goal.h"

namespace Game {

MsnDemo_Goal::MsnDemo_Goal()
    : mPhase(GoalDemoPhase::cWaitingBulbHit),
      mPhaseTimer(0),
      mBulbHp(100.0f),
      mZapfishPos(0.0f, 1.0f, 0.0f),
      mZapfishSpinAngle(0.0f) {
}

MsnDemo_Goal::~MsnDemo_Goal() {
}

void MsnDemo_Goal::init() {
    GambitActor::init();
    mPhase = GoalDemoPhase::cWaitingBulbHit;
    mBulbHp = 100.0f;
    mPhaseTimer = 0;
}

void MsnDemo_Goal::triggerBulbBreak() {
    if (mPhase == GoalDemoPhase::cWaitingBulbHit) {
        mBulbHp = 0.0f;
        mPhase = GoalDemoPhase::cBulbExploding;
        mPhaseTimer = 0;
    }
}

void MsnDemo_Goal::update() {
    mPhaseTimer++;

    switch (mPhase) {
        case GoalDemoPhase::cBulbExploding:
            if (mPhaseTimer >= 30) {
                mPhase = GoalDemoPhase::cZapfishDance;
                mPhaseTimer = 0;
            }
            break;

        case GoalDemoPhase::cZapfishDance:
            mZapfishSpinAngle += 0.25f;
            mZapfishPos.y = 1.0f + 0.5f * __builtin_sinf(mZapfishSpinAngle);
            if (mPhaseTimer >= 90) { // 1.5s flip dance
                mPhase = GoalDemoPhase::cPlayerVictory;
                mPhaseTimer = 0;
            }
            break;

        case GoalDemoPhase::cPlayerVictory:
            // Player poses with Zapfish
            if (mPhaseTimer >= 90) {
                mPhase = GoalDemoPhase::cResultsTally;
                mPhaseTimer = 0;
            }
            break;

        case GoalDemoPhase::cResultsTally:
            // Score and Power Egg count presentation
            if (mPhaseTimer >= 120) {
                mPhase = GoalDemoPhase::cComplete;
            }
            break;

        default:
            break;
    }
}

void MsnDemo_Goal::draw() {
    GambitActor::draw();
}

} // namespace Game
