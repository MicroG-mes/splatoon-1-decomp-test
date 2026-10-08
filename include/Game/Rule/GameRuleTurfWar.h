#pragma once

#include "types.h"
#include <string>

namespace Game {

enum class MatchState : u32 {
    cIntro = 0,       // Team fly-in / jumbotron intro (180 frames / 3.0 sec)
    cReadyGo = 1,     // Ready... GO! banner display (60 frames)
    cPlaying = 2,     // Active Turf War battle (3:00 / 10,800 frames)
    cOneMinute = 3,   // 1-minute remaining warning chime & tempo increase
    cFinish = 4,      // "GAME!" whistle banner (120 frames)
    cJudgement = 5    // Judd the Cat weigh-in & final victory tally
};

struct MatchResult {
    u32 winnerTeam;         // 0: Alpha, 1: Bravo, 2: Draw
    f32 alphaTurfPercent;   // e.g. 52.3%
    f32 bravoTurfPercent;   // e.g. 44.1%
    u32 alphaTurfPoints;    // e.g. 850p
    u32 bravoTurfPoints;    // e.g. 720p
    bool isKnockout;
};

class GameRuleTurfWar {
public:
    static constexpr u32 cTotalMatchFrames = 10800; // 3 minutes @ 60 FPS
    static constexpr u32 cOneMinuteFrames = 3600;   // 60 seconds @ 60 FPS
    static constexpr u32 cIntroFrames = 180;
    static constexpr u32 cReadyGoFrames = 60;
    static constexpr u32 cFinishBannerFrames = 120;

    GameRuleTurfWar();
    ~GameRuleTurfWar();

    void reset();
    void update(f32 currentAlphaPercent, f32 currentBravoPercent);

    // Match control
    void startMatch();
    void forceFinish();

    // Query state
    MatchState getState() const { return mState; }
    u32 getRemainingFrames() const { return mRemainingFrames; }
    f32 getRemainingSeconds() const { return static_cast<f32>(mRemainingFrames) / 60.0f; }
    bool isMatchActive() const { return mState == MatchState::cPlaying || mState == MatchState::cOneMinute; }
    bool isOneMinuteRemaining() const { return mState == MatchState::cOneMinute; }
    bool isGameOver() const { return mState == MatchState::cFinish || mState == MatchState::cJudgement; }

    // Judd judgment calculation
    const MatchResult& getResult() const { return mResult; }
    u32 getWinningTeam() const { return mResult.winnerTeam; }

    // Live match points
    f32 getAlphaPercent() const { return mAlphaPercent; }
    f32 getBravoPercent() const { return mBravoPercent; }

    // Timer format string: MM:SS
    std::string getFormattedTime() const;

private:
    MatchState mState;
    u32 mRemainingFrames;
    u32 mStateTimer;
    f32 mAlphaPercent;
    f32 mBravoPercent;
    MatchResult mResult;

    void computeJudgement();
};

} // namespace Game
