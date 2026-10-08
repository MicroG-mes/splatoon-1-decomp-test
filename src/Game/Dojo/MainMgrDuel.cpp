#include "Game/Dojo/MainMgrDuel.h"

namespace Game {

MainMgrDuel::MainMgrDuel()
    : mState(DuelState::cInit),
      mTimeRemaining(cDefaultMatchDuration),
      mScoreP1(0),
      mScoreP2(0),
      mWinnerPlayerId(-1),
      mSpawnTimer(0) {
}

MainMgrDuel::~MainMgrDuel() {
}

void MainMgrDuel::init() {
    GambitActor::init();
    mState = DuelState::cReady;
    mTimeRemaining = cDefaultMatchDuration;
    mScoreP1 = 0;
    mScoreP2 = 0;
    mWinnerPlayerId = -1;
    mSpawnTimer = 0;

    for (u32 i = 0; i < cMaxBalloons; ++i) {
        mBalloons[i].init();
    }
}

void MainMgrDuel::startMatch() {
    mState = DuelState::cBattle;
    mTimeRemaining = cDefaultMatchDuration;
    mScoreP1 = 0;
    mScoreP2 = 0;
    mWinnerPlayerId = -1;
    mSpawnTimer = 0;

    // Spawn initial wave of balloons
    for (u32 i = 0; i < 6; ++i) {
        f32 angle = (static_cast<f32>(i) / 6.0f) * 6.283185f;
        sead::Vector3f pos(15.0f * (angle > 3.14f ? -1.0f : 1.0f), 3.0f, 10.0f * (i % 2 == 0 ? 1.0f : -1.0f));
        mBalloons[i].spawn(pos, (i == 0 || i == 3) ? BalloonType::cGold : BalloonType::cRegular);
    }
}

void MainMgrDuel::addPlayerScore(u32 playerId, u32 points) {
    if (mState != DuelState::cBattle) {
        return;
    }

    if (playerId == 0) {
        mScoreP1 += points;
    } else {
        mScoreP2 += points;
    }

    checkWinCondition();
}

u32 MainMgrDuel::getPlayerScore(u32 playerId) const {
    if (playerId == 0) {
        return mScoreP1;
    }
    return mScoreP2;
}

void MainMgrDuel::checkWinCondition() {
    if (mScoreP1 >= cWinningScore) {
        mWinnerPlayerId = 0;
        mState = DuelState::cFinished;
    } else if (mScoreP2 >= cWinningScore) {
        mWinnerPlayerId = 1;
        mState = DuelState::cFinished;
    }
}

void MainMgrDuel::updateBalloons() {
    mSpawnTimer++;
    if (mSpawnTimer >= 180) { // Every 3 seconds check for respawns
        mSpawnTimer = 0;
        for (u32 i = 0; i < cMaxBalloons; ++i) {
            if (mBalloons[i].getState() == BalloonState::cInactive) {
                f32 x = (static_cast<f32>(i % 4) - 1.5f) * 8.0f;
                f32 z = (static_cast<f32>(i / 4) - 1.5f) * 8.0f;
                sead::Vector3f pos(x, 2.5f, z);
                mBalloons[i].spawn(pos, (i % 5 == 0) ? BalloonType::cGold : BalloonType::cRegular);
                break;
            }
        }
    }

    for (u32 i = 0; i < cMaxBalloons; ++i) {
        mBalloons[i].update();
    }
}

void MainMgrDuel::update() {
    if (mState == DuelState::cBattle) {
        if (mTimeRemaining > 0) {
            mTimeRemaining--;
            if (mTimeRemaining == 0) {
                mState = DuelState::cTimeUp;
                if (mScoreP1 > mScoreP2) {
                    mWinnerPlayerId = 0;
                } else if (mScoreP2 > mScoreP1) {
                    mWinnerPlayerId = 1;
                } else {
                    mWinnerPlayerId = -1; // Draw
                }
            }
        }

        updateBalloons();
    }
}

void MainMgrDuel::draw() {
    for (u32 i = 0; i < cMaxBalloons; ++i) {
        mBalloons[i].draw();
    }
}

} // namespace Game
