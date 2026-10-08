#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"

namespace Game {

enum class SplatfestRank : u32 {
    Fan      = 0, // 0 - 9 pts
    Fiend    = 1, // 10 - 24 pts
    Defender = 2, // 25 - 49 pts
    Champion = 3, // 50 - 98 pts
    King     = 4  // 99 pts (Max title)
};

class FestResult : public GambitActor {
public:
    FestResult();
    virtual ~FestResult() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void calculateResults(f32 popAlpha, f32 popBravo, f32 winAlpha, f32 winBravo);
    u32 calculateSnailPayout(SplatfestRank rank, bool isWinningTeam) const;

    bool didAlphaWin() const { return mTotalAlpha > mTotalBravo; }
    f32 getPopAlpha() const { return mPopAlpha; }
    f32 getPopBravo() const { return mPopBravo; }
    f32 getWinAlpha() const { return mWinAlpha; }
    f32 getWinBravo() const { return mWinBravo; }
    f32 getTotalAlpha() const { return mTotalAlpha; }
    f32 getTotalBravo() const { return mTotalBravo; }

protected:
    f32 mPopAlpha;
    f32 mPopBravo;
    f32 mWinAlpha;
    f32 mWinBravo;
    f32 mTotalAlpha;
    f32 mTotalBravo;

    s32 mRevealAnimTimer;
    undefined mReserved[0x38];
};

} // namespace Game
