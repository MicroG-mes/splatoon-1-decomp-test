#include "Game/Fest/FestResult.h"

namespace Game {

FestResult::FestResult()
    : mPopAlpha(50.0f),
      mPopBravo(50.0f),
      mWinAlpha(50.0f),
      mWinBravo(50.0f),
      mTotalAlpha(350.0f),
      mTotalBravo(350.0f),
      mRevealAnimTimer(0) {
}

FestResult::~FestResult() {
}

void FestResult::init() {
    GambitActor::init();
    mRevealAnimTimer = 0;
}

void FestResult::calculateResults(f32 popAlpha, f32 popBravo, f32 winAlpha, f32 winBravo) {
    mPopAlpha = popAlpha;
    mPopBravo = popBravo;
    mWinAlpha = winAlpha;
    mWinBravo = winBravo;

    // Splatoon formula (v2.0.0+): Final = Popularity + (Win% * 6)
    mTotalAlpha = mPopAlpha + (mWinAlpha * 6.0f);
    mTotalBravo = mPopBravo + (mWinBravo * 6.0f);
}

u32 FestResult::calculateSnailPayout(SplatfestRank rank, bool isWinningTeam) const {
    switch (rank) {
        case SplatfestRank::Fan:
            return isWinningTeam ? 2 : 1;
        case SplatfestRank::Fiend:
            return isWinningTeam ? 4 : 2;
        case SplatfestRank::Defender:
            return isWinningTeam ? 8 : 4;
        case SplatfestRank::Champion:
            return isWinningTeam ? 14 : 7;
        case SplatfestRank::King:
            return isWinningTeam ? 24 : 12;
    }
    return 0;
}

void FestResult::update() {
    mRevealAnimTimer++;
}

void FestResult::draw() {
    GambitActor::draw();
}

} // namespace Game
