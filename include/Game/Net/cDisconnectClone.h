#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class DisconnectReason : u32 {
    cHeartbeatTimeout = 0,
    cPacketLoss       = 1,
    cManualExit       = 2,
    cHostMigrationFail = 3
};

class cDisconnectClone : public GambitActor {
public:
    static constexpr s32 cTimeoutThresholdFrames = 180; // 3 seconds timeout

    cDisconnectClone();
    virtual ~cDisconnectClone() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void registerPlayer(u32 playerIndex, u32 teamId);
    void reportHeartbeat(u32 playerIndex);
    void checkHeartbeats();
    bool isPlayerDisconnected(u32 playerIndex) const;

    u32 getDisconnectedCount() const { return mDisconnectedMask ? 1 : 0; }
    u32 getDisconnectedMask() const { return mDisconnectedMask; }

protected:
    void handlePlayerDrop(u32 playerIndex, DisconnectReason reason);

    s32 mHeartbeatTimers[8];
    u32 mPlayerTeams[8];
    u32 mActiveMask;
    u32 mDisconnectedMask;

    undefined mReserved[0x38];
};

} // namespace Game
