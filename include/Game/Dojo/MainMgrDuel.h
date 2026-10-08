#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "Game/Dojo/Balloon.h"

namespace Game {

enum class DuelState : u32 {
    cInit     = 0,
    cReady    = 1,
    cBattle   = 2,
    cTimeUp   = 3,
    cFinished = 4
};

class MainMgrDuel : public GambitActor {
public:
    static constexpr u32 cMaxBalloons = 16;
    static constexpr u32 cWinningScore = 30;
    static constexpr s32 cDefaultMatchDuration = 18000; // 5 minutes at 60fps

    MainMgrDuel();
    virtual ~MainMgrDuel() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void startMatch();
    void addPlayerScore(u32 playerId, u32 points);
    
    u32 getPlayerScore(u32 playerId) const;
    s32 getRemainingTimeFrames() const { return mTimeRemaining; }
    DuelState getState() const { return mState; }
    bool isMatchOver() const { return mState == DuelState::cFinished || mState == DuelState::cTimeUp; }
    s32 getWinnerPlayerId() const { return mWinnerPlayerId; }

    Balloon* getBalloon(u32 index) {
        if (index < cMaxBalloons) {
            return &mBalloons[index];
        }
        return nullptr;
    }

protected:
    void updateBalloons();
    void checkWinCondition();

    DuelState mState;
    s32 mTimeRemaining;
    u32 mScoreP1; // GamePad player
    u32 mScoreP2; // TV player
    s32 mWinnerPlayerId; // 0 = P1, 1 = P2, -1 = Draw

    Balloon mBalloons[cMaxBalloons];
    s32 mSpawnTimer;

    undefined mReserved[0x40];
};

} // namespace Game
