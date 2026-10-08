#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"

namespace Game {

enum class NetSessionState : u32 {
    cDisconnected    = 0,
    cSearching       = 1,
    cGatheringLobby  = 2,
    cHandshakeSync   = 3,
    cLoadingStage    = 4,
    cInGameMatch     = 5,
    cMatchFinish     = 6
};

struct NetPlayerSlot {
    bool isConnected;
    u32 teamId;       // 0 = Alpha, 1 = Bravo
    u32 weaponId;
    u32 pingMs;
    char playerName[32];
};

/**
 * NetSessionMgr
 * Manages peer-to-peer 8-player lobby coordination, host election,
 * team balancing (Alpha vs Bravo), and match state lifecycle.
 */
class NetSessionMgr : public GambitActor {
public:
    static constexpr u32 cMaxPlayers = 8;

    NetSessionMgr();
    virtual ~NetSessionMgr() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void startLobbySession(u32 sessionId, bool isHost);
    bool registerPlayer(u32 slot, u32 teamId, u32 weaponId, const char* name);
    void unregisterPlayer(u32 slot);
    void setHostSlot(u32 slot);
    void transitionTo(NetSessionState newState);

    NetSessionState getState() const { return mState; }
    u32 getSessionId() const { return mSessionId; }
    u32 getLocalSlot() const { return mLocalSlot; }
    u32 getHostSlot() const { return mHostSlot; }
    bool isHost() const { return mIsHost; }
    u32 getConnectedCount() const;
    u32 getTeamPlayerCount(u32 teamId) const;
    const NetPlayerSlot& getPlayer(u32 slot) const { return mSlots[slot < cMaxPlayers ? slot : 0]; }

protected:
    NetSessionState mState;
    u32 mSessionId;
    u32 mLocalSlot;
    u32 mHostSlot;
    bool mIsHost;
    NetPlayerSlot mSlots[cMaxPlayers];
    s32 mStateTimer;
};

} // namespace Game
