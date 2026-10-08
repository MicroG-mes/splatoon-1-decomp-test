#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "cafe/vpad.h"

namespace Game {

enum class BattleMode : u32 {
    RegularMatch  = 0, // Turf War
    RankedMatch   = 1, // Gachi Match
    SquadBattle   = 2, // Tag Match
    PrivateBattle = 3, // Private Room
    BattleDojo    = 4  // Local 1v1
};

enum class RankedRule : u32 {
    SplatZones   = 0, // Area
    TowerControl = 1, // Yagura
    Rainmaker    = 2  // Hoko
};

enum class LobbyState : u32 {
    SelectMode     = 0,
    SearchingMatch = 1,
    LobbyFull      = 2,
    SelectingStage = 3,
    StartingBattle = 4,
    Canceled       = 5
};

struct LobbyPlayerSlot {
    bool isOccupied;
    u32 playerId;
    char playerName[32];
    u32 level;
    s32 udemaeRank;
    u32 weaponId;
    u32 team; // 0 = Alpha, 1 = Bravo
};

class Lobby : public GambitActor {
public:
    Lobby();
    virtual ~Lobby() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void handleInput(const VPADStatus& vpad);

    void selectMode(BattleMode mode);
    void startMatchmaking();
    void cancelMatchmaking();

    void addPlayer(u32 slotIndex, u32 playerId, const char* name, u32 level, s32 rank, u32 weaponId);
    void removePlayer(u32 slotIndex);

    LobbyState getState() const { return mState; }
    BattleMode getMode() const { return mBattleMode; }
    RankedRule getRankedRule() const { return mRankedRule; }

    u32 getPlayerCount() const;
    s32 getCountdownSeconds() const { return mCountdownFrames / 60; }
    u32 getChosenStageId() const { return mChosenStageId; }

protected:
    void assignTeams();
    void pickStage();

    BattleMode mBattleMode;
    RankedRule mRankedRule;
    LobbyState mState;
    s32 mStateTimer;
    s32 mCountdownFrames;

    LobbyPlayerSlot mSlots[8];
    u32 mStageRotation[2];
    u32 mChosenStageId;

    undefined mReserved[0x40];
};

} // namespace Game
