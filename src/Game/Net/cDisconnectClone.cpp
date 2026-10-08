#include "Game/Net/cDisconnectClone.h"

namespace Game {

cDisconnectClone::cDisconnectClone()
    : mActiveMask(0),
      mDisconnectedMask(0) {
    for (u32 i = 0; i < 8; ++i) {
        mHeartbeatTimers[i] = 0;
        mPlayerTeams[i] = 0;
    }
}

cDisconnectClone::~cDisconnectClone() {
}

void cDisconnectClone::init() {
    GambitActor::init();
    mActiveMask = 0;
    mDisconnectedMask = 0;
    for (u32 i = 0; i < 8; ++i) {
        mHeartbeatTimers[i] = 0;
        mPlayerTeams[i] = 0;
    }
}

void cDisconnectClone::registerPlayer(u32 playerIndex, u32 teamId) {
    if (playerIndex < 8) {
        mActiveMask |= (1 << playerIndex);
        mDisconnectedMask &= ~(1 << playerIndex);
        mHeartbeatTimers[playerIndex] = 0;
        mPlayerTeams[playerIndex] = teamId;
    }
}

void cDisconnectClone::reportHeartbeat(u32 playerIndex) {
    if (playerIndex < 8 && (mActiveMask & (1 << playerIndex))) {
        mHeartbeatTimers[playerIndex] = 0; // Reset timeout clock
    }
}

void cDisconnectClone::handlePlayerDrop(u32 playerIndex, DisconnectReason reason) {
    mDisconnectedMask |= (1 << playerIndex);
    mActiveMask &= ~(1 << playerIndex);
    // Disconnect event dispatched to match HUD to render crossed-out squid icon
}

void cDisconnectClone::checkHeartbeats() {
    for (u32 i = 0; i < 8; ++i) {
        if (mActiveMask & (1 << i)) {
            mHeartbeatTimers[i]++;
            if (mHeartbeatTimers[i] >= cTimeoutThresholdFrames) {
                handlePlayerDrop(i, DisconnectReason::cHeartbeatTimeout);
            }
        }
    }
}

bool cDisconnectClone::isPlayerDisconnected(u32 playerIndex) const {
    if (playerIndex < 8) {
        return (mDisconnectedMask & (1 << playerIndex)) != 0;
    }
    return false;
}

void cDisconnectClone::update() {
    checkHeartbeats();
}

void cDisconnectClone::draw() {
}

} // namespace Game
