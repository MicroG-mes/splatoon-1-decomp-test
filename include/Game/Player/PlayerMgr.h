#pragma once

#include "types.h"
#include "sead/math/seadVector.h"
#include "Game/GamePlayer.h"

namespace Game {

class PlayerMgr {
public:
    PlayerMgr();
    virtual ~PlayerMgr();

    // Decompiled vtable 0x100EC0FC from Gambit.elf
    virtual void vfunc_1();
    virtual void init();             // vfunc_3 (0x026B9FFC): "PlayerMgr::Load"
    virtual void resetMatch();        // vfunc_5 (0x026BA2C0): Reset 8 players & spawn points
    virtual void update();           // vfunc_7 (0x026BA3C8)
    virtual void onPlayerSplatted(u32 playerIdx);    // vfunc_10 (0x026BAE0C): Event 0x17
    virtual void onPlayerRespawn(u32 playerIdx, const sead::Vector3f& respawnPos); // vfunc_11 (0x026BA4E8): Event 0x18
    virtual void onSuperJumpStart(u32 playerIdx);   // vfunc_12 (0x026BAE68): Event 0x19
    virtual void onSuperJumpLand(u32 playerIdx);    // vfunc_13 (0x026BAEC8): Event 0x1A

    u32 getPlayerCount() const { return mPlayerCount; }
    Player* getPlayer(u32 index) const {
        if (index < 8) return mPlayers[index];
        return nullptr;
    }
    void registerPlayer(u32 index, Player* player) {
        if (index < 8) {
            mPlayers[index] = player;
            if (index >= mPlayerCount) mPlayerCount = index + 1;
        }
    }

    static PlayerMgr* instance() { return sInstance; }
    static PlayerMgr* sInstance;

protected:
    u8 mReserved0_0x8[0x228];
    u32 mManagerFlags;               // 0x230: bit 0x1=init, 0x4=reset, 0x200=splat, 0x400=respawn, 0x800=jumpStart, 0x1000=jumpLand
    u8 mReserved1_0x234[0xA0];
    Player* mPlayers[8];             // 0x2D4: array of 8 player pointers
    u32 mPlayerCount;                // 0x320: registered player count (800 decimal)
    u8 mReserved2_0x324[4];
    sead::Vector3f mDefaultSpawnPos; // 0x328: default spawn coordinates (0x328, 0x32c, 0x330)
};

} // namespace Game
