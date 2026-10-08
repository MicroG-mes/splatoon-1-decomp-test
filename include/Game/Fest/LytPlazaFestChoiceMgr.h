#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "cafe/vpad.h"

namespace Game {

enum class FestVoteState : u32 {
    cSleep        = 0, // State::cSleep
    cInFestChoice = 1, // State::cInFestChoice
    cVoteConfirm  = 2,
    cReceiveTee   = 3,
    cFinished     = 4
};

class LytPlazaFestChoiceMgr : public GambitActor {
public:
    LytPlazaFestChoiceMgr();
    virtual ~LytPlazaFestChoiceMgr() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void handleInput(const VPADStatus& vpad);

    void openVotingBooth(const char* teamAName, const char* teamBName);
    void castVote(u32 teamChoice); // 0 = Team A (Alpha), 1 = Team B (Bravo)

    FestVoteState getState() const { return mState; }
    bool hasVoted() const { return mHasVoted; }
    u32 getChosenTeam() const { return mChosenTeam; }

    const char* getTeamAName() const { return mTeamAName; }
    const char* getTeamBName() const { return mTeamBName; }

protected:
    FestVoteState mState;
    s32 mStateTimer;

    const char* mTeamAName;
    const char* mTeamBName;

    bool mHasVoted;
    u32 mChosenTeam;
    s32 mCursorIndex; // 0 or 1

    undefined mReserved[0x38];
};

} // namespace Game
