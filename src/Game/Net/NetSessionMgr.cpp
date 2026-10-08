#include "Game/Net/NetSessionMgr.h"
#include <cstring>

namespace Game {

NetSessionMgr::NetSessionMgr()
    : mState(NetSessionState::cDisconnected),
      mSessionId(0),
      mLocalSlot(0),
      mHostSlot(0),
      mIsHost(false),
      mStateTimer(0) {
    for (u32 i = 0; i < cMaxPlayers; ++i) {
        mSlots[i].isConnected = false;
        mSlots[i].teamId = (i < 4) ? 0 : 1;
        mSlots[i].weaponId = 0;
        mSlots[i].pingMs = 0;
        mSlots[i].playerName[0] = '\0';
    }
}

NetSessionMgr::~NetSessionMgr() {
}

void NetSessionMgr::init() {
    GambitActor::init();
    mState = NetSessionState::cDisconnected;
    mSessionId = 0;
    mLocalSlot = 0;
    mHostSlot = 0;
    mIsHost = false;
    mStateTimer = 0;
    for (u32 i = 0; i < cMaxPlayers; ++i) {
        mSlots[i].isConnected = false;
        mSlots[i].teamId = (i < 4) ? 0 : 1;
        mSlots[i].weaponId = 0;
        mSlots[i].pingMs = 0;
        mSlots[i].playerName[0] = '\0';
    }
}

void NetSessionMgr::startLobbySession(u32 sessionId, bool isHost) {
    mSessionId = sessionId;
    mIsHost = isHost;
    mHostSlot = isHost ? mLocalSlot : 0;
    mState = NetSessionState::cGatheringLobby;
    mStateTimer = 0;
}

bool NetSessionMgr::registerPlayer(u32 slot, u32 teamId, u32 weaponId, const char* name) {
    if (slot >= cMaxPlayers) {
        return false;
    }

    mSlots[slot].isConnected = true;
    mSlots[slot].teamId = teamId;
    mSlots[slot].weaponId = weaponId;
    mSlots[slot].pingMs = 30;

    if (name) {
        std::strncpy(mSlots[slot].playerName, name, sizeof(mSlots[slot].playerName) - 1);
        mSlots[slot].playerName[sizeof(mSlots[slot].playerName) - 1] = '\0';
    } else {
        mSlots[slot].playerName[0] = '\0';
    }

    return true;
}

void NetSessionMgr::unregisterPlayer(u32 slot) {
    if (slot < cMaxPlayers) {
        mSlots[slot].isConnected = false;
        mSlots[slot].playerName[0] = '\0';
    }
}

void NetSessionMgr::setHostSlot(u32 slot) {
    if (slot < cMaxPlayers) {
        mHostSlot = slot;
        mIsHost = (mLocalSlot == slot);
    }
}

void NetSessionMgr::transitionTo(NetSessionState newState) {
    mState = newState;
    mStateTimer = 0;
}

u32 NetSessionMgr::getConnectedCount() const {
    u32 count = 0;
    for (u32 i = 0; i < cMaxPlayers; ++i) {
        if (mSlots[i].isConnected) {
            count++;
        }
    }
    return count;
}

u32 NetSessionMgr::getTeamPlayerCount(u32 teamId) const {
    u32 count = 0;
    for (u32 i = 0; i < cMaxPlayers; ++i) {
        if (mSlots[i].isConnected && mSlots[i].teamId == teamId) {
            count++;
        }
    }
    return count;
}

void NetSessionMgr::update() {
    mStateTimer++;

    switch (mState) {
        case NetSessionState::cGatheringLobby: {
            // Once 8 players gather, proceed to handshake sync
            if (getConnectedCount() >= cMaxPlayers) {
                transitionTo(NetSessionState::cHandshakeSync);
            }
            break;
        }

        case NetSessionState::cHandshakeSync: {
            if (mStateTimer > 60) {
                transitionTo(NetSessionState::cLoadingStage);
            }
            break;
        }

        case NetSessionState::cLoadingStage: {
            if (mStateTimer > 180) { // 3 seconds loading
                transitionTo(NetSessionState::cInGameMatch);
            }
            break;
        }

        default:
            break;
    }
}

void NetSessionMgr::draw() {
    // Network lobby HUD and connection status icons handled by LobbyHUD
}

} // namespace Game
