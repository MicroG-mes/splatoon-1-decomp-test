#pragma once

#include "types.h"
#include "sead/math/seadVector.h"
#include <string>

namespace Game {

enum class RankedModeType {
    cSplatZones = 0, // GachiArea
    cTowerControl = 1, // GachiYagura
    cRainmaker = 2     // GachiHoko
};

enum class SplatZoneOwner {
    cNeutral = -1,
    cTeamAlpha = 0,
    cTeamBravo = 1
};

// Splat Zones (GachiArea) Timer & Penalty Rule Engine
class GachiAreaTimer {
public:
    static constexpr s32 cMaxCount = 100;
    static constexpr f32 cPenaltyMultiplier = 0.75f;

    GachiAreaTimer();
    ~GachiAreaTimer();

    void reset();

    // Updates timer state based on which team currently holds the zones
    void update(SplatZoneOwner currentControl, f32 deltaTime);

    // Score & Penalty accessors
    s32 getCount(u32 team) const { return (team == 0) ? mAlphaCount : mBravoCount; }
    s32 getPenalty(u32 team) const { return (team == 0) ? mAlphaPenalty : mBravoPenalty; }
    s32 getEffectiveScore(u32 team) const { return getCount(team); }

    bool isKnockout() const { return (mAlphaCount <= 0 || mBravoCount <= 0); }
    u32 getWinnerTeam() const;

    // Overtime evaluation at match timer 0:00
    bool checkOvertimeNeeded(f32 matchTimeRemaining, SplatZoneOwner currentControl) const;
    bool isOvertimeActive() const { return mIsOvertime; }

private:
    s32 mAlphaCount;
    s32 mBravoCount;
    s32 mAlphaPenalty;
    s32 mBravoPenalty;

    f32 mAlphaAccumSec;
    f32 mBravoAccumSec;

    SplatZoneOwner mLastControl;
    bool mIsOvertime;
};

// Rainmaker Goal Pedestal (GachiHokoPedestal)
class GachiHokoPedestal {
public:
    GachiHokoPedestal();
    ~GachiHokoPedestal();

    void init(const sead::Vector3f& alphaPedestalPos, const sead::Vector3f& bravoPedestalPos);
    void reset();

    // Distance calculation from current carrier position to goal
    s32 updateCarrierDistance(const sead::Vector3f& carrierPos, u32 carrierTeam);

    // Check if carrier has climbed the goal pedestal for knockout
    bool checkGoalTouchdown(const sead::Vector3f& carrierPos, u32 carrierTeam) const;

    s32 getBestDistance(u32 team) const { return (team == 0) ? mAlphaBestDist : mBravoBestDist; }
    bool isKnockout() const { return (mAlphaBestDist <= 0 || mBravoBestDist <= 0); }
    u32 getWinnerTeam() const;

    const sead::Vector3f& getPedestalPos(u32 team) const {
        return (team == 0) ? mAlphaPedestal : mBravoPedestal;
    }

private:
    sead::Vector3f mAlphaPedestal; // Alpha team defends this (Bravo attacks)
    sead::Vector3f mBravoPedestal; // Bravo team defends this (Alpha attacks)

    s32 mAlphaBestDist; // Best count achieved by Team Alpha (toward Bravo pedestal, 100 to 0)
    s32 mBravoBestDist; // Best count achieved by Team Bravo (toward Alpha pedestal, 100 to 0)
    f32 mInitialDistance;
};

} // namespace Game
