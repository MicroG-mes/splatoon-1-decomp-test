#include "Game/Lobby/Lobby.h"
#include "Game/System/SaveDataMgr.h"
#include <cstring>

namespace Game {

Lobby::Lobby()
    : mBattleMode(BattleMode::RegularMatch),
      mRankedRule(RankedRule::SplatZones),
      mState(LobbyState::SelectMode),
      mStateTimer(0),
      mCountdownFrames(120 * 60), // 120 seconds
      mChosenStageId(0) {
    mStageRotation[0] = 0; // Urchin Underpass
    mStageRotation[1] = 1; // Saltspray Rig
    for (int i = 0; i < 8; ++i) {
        mSlots[i].isOccupied = false;
        mSlots[i].playerId = 0;
        mSlots[i].playerName[0] = '\0';
        mSlots[i].level = 1;
        mSlots[i].udemaeRank = 0;
        mSlots[i].weaponId = 0;
        mSlots[i].team = 0;
    }
}

Lobby::~Lobby() {
}

void Lobby::init() {
    GambitActor::init();
    mState = LobbyState::SelectMode;
}

void Lobby::selectMode(BattleMode mode) {
    mBattleMode = mode;
}

void Lobby::startMatchmaking() {
    mState = LobbyState::SearchingMatch;
    mStateTimer = 0;
    mCountdownFrames = 120 * 60;

    // Reset slots
    for (int i = 0; i < 8; ++i) {
        mSlots[i].isOccupied = false;
    }

    // Add local player in slot 0
    SaveDataMgr* save = SaveDataMgr::instance();
    u32 myLevel = 1;
    s32 myRank = 0;
    u32 myWeapon = 0;
    if (save) {
        myLevel = save->getStats().level;
        myRank = save->getStats().udemaeRank;
        myWeapon = save->getCustomization().weaponSetId;
    }
    addPlayer(0, 10001, "Player", myLevel, myRank, myWeapon);
}

void Lobby::cancelMatchmaking() {
    mState = LobbyState::Canceled;
}

void Lobby::addPlayer(u32 slotIndex, u32 playerId, const char* name, u32 level, s32 rank, u32 weaponId) {
    if (slotIndex < 8) {
        mSlots[slotIndex].isOccupied = true;
        mSlots[slotIndex].playerId = playerId;
        std::strncpy(mSlots[slotIndex].playerName, name, sizeof(mSlots[slotIndex].playerName) - 1);
        mSlots[slotIndex].playerName[sizeof(mSlots[slotIndex].playerName) - 1] = '\0';
        mSlots[slotIndex].level = level;
        mSlots[slotIndex].udemaeRank = rank;
        mSlots[slotIndex].weaponId = weaponId;
    }
}

void Lobby::removePlayer(u32 slotIndex) {
    if (slotIndex < 8) {
        mSlots[slotIndex].isOccupied = false;
    }
}

u32 Lobby::getPlayerCount() const {
    u32 count = 0;
    for (int i = 0; i < 8; ++i) {
        if (mSlots[i].isOccupied) {
            count++;
        }
    }
    return count;
}

void Lobby::assignTeams() {
    // 4 players in Alpha (team 0), 4 players in Bravo (team 1)
    u32 alphaCount = 0;
    u32 bravoCount = 0;
    for (int i = 0; i < 8; ++i) {
        if (mSlots[i].isOccupied) {
            if (alphaCount < 4) {
                mSlots[i].team = 0;
                alphaCount++;
            } else {
                mSlots[i].team = 1;
                bravoCount++;
            }
        }
    }
}

void Lobby::pickStage() {
    // Random selection between the two rotation stages
    static u32 sRng = 42;
    sRng = sRng * 1664525 + 1013904223;
    mChosenStageId = mStageRotation[(sRng >> 16) % 2];
}

void Lobby::handleInput(const VPADStatus& vpad) {
    switch (mState) {
        case LobbyState::SelectMode:
            if (vpad.trigger & VPAD_BUTTON_A) {
                startMatchmaking();
            } else if (vpad.trigger & VPAD_BUTTON_B) {
                mState = LobbyState::Canceled;
            }
            break;

        case LobbyState::SearchingMatch:
            if (vpad.trigger & VPAD_BUTTON_B) {
                cancelMatchmaking();
            }
            break;

        default:
            break;
    }
}

void Lobby::update() {
    mStateTimer++;

    switch (mState) {
        case LobbyState::SearchingMatch: {
            if (mCountdownFrames > 0) {
                mCountdownFrames--;
            }

            // Simulate incoming player connections
            if (mStateTimer % 60 == 0) {
                u32 count = getPlayerCount();
                if (count < 8) {
                    char nameBuf[32];
                    nameBuf[0] = 'I'; nameBuf[1] = 'n'; nameBuf[2] = 'k'; nameBuf[3] = 'l'; nameBuf[4] = 'i'; nameBuf[5] = 'n'; nameBuf[6] = 'g';
                    nameBuf[7] = '0' + static_cast<char>(count);
                    nameBuf[8] = '\0';
                    addPlayer(count, 10002 + count, nameBuf, 10 + count * 2, 2, count);
                }
            }

            if (getPlayerCount() == 8) {
                mState = LobbyState::LobbyFull;
                mStateTimer = 0;
            }
            break;
        }

        case LobbyState::LobbyFull: {
            if (mStateTimer > 90) { // 1.5 seconds wait
                assignTeams();
                pickStage();
                mState = LobbyState::SelectingStage;
                mStateTimer = 0;
            }
            break;
        }

        case LobbyState::SelectingStage: {
            if (mStateTimer > 120) { // 2.0 seconds stage reveal
                mState = LobbyState::StartingBattle;
            }
            break;
        }

        case LobbyState::StartingBattle: {
            // Transitions into map loading (Urchin Underpass, etc.)
            break;
        }

        default:
            break;
    }
}

void Lobby::draw() {
    GambitActor::draw();
}

} // namespace Game
