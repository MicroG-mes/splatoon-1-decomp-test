#include "Game/Player/PlayerMgr.h"
#include <cstring>

namespace Game {

PlayerMgr* PlayerMgr::sInstance = nullptr;

PlayerMgr::PlayerMgr()
    : mManagerFlags(0),
      mPlayerCount(0),
      mDefaultSpawnPos(0.0f, 0.0f, 0.0f) {
    sInstance = this;
    std::memset(mReserved0_0x8, 0, sizeof(mReserved0_0x8));
    std::memset(mReserved1_0x234, 0, sizeof(mReserved1_0x234));
    std::memset(mReserved2_0x324, 0, sizeof(mReserved2_0x324));
    for (int i = 0; i < 8; ++i) {
        mPlayers[i] = nullptr;
    }
}

PlayerMgr::~PlayerMgr() {
    if (sInstance == this) {
        sInstance = nullptr;
    }
}

void PlayerMgr::vfunc_1() {
    mManagerFlags = 0;
}

// 0x026B9FFC: Decompiled vfunc_3 ("PlayerMgr::Load")
void PlayerMgr::init() {
    mManagerFlags |= 1; // Mark initialized
}

// 0x026BA2C0: Decompiled vfunc_5 (Reset 8 players and spawn positions)
void PlayerMgr::resetMatch() {
    for (int i = 0; i < 8; ++i) {
        mPlayers[i] = nullptr;
    }
    mDefaultSpawnPos.set(0.0f, 0.0f, 0.0f);
    mManagerFlags |= 4; // Mark reset
}

// 0x026BA3C8: Decompiled vfunc_7 (Tick / Update)
void PlayerMgr::update() {
    for (u32 i = 0; i < mPlayerCount && i < 8; ++i) {
        if (mPlayers[i]) {
            mPlayers[i]->update();
        }
    }
}

// 0x026BAE0C: Decompiled vfunc_10 (Event 0x17: Player splatted)
void PlayerMgr::onPlayerSplatted(u32 playerIdx) {
    mManagerFlags |= 0x200;
}

// 0x026BA4E8: Decompiled vfunc_11 (Event 0x18: Player respawn with coordinates)
void PlayerMgr::onPlayerRespawn(u32 playerIdx, const sead::Vector3f& respawnPos) {
    mDefaultSpawnPos = respawnPos;
    mManagerFlags |= 0x400;

    if (playerIdx < 8 && mPlayers[playerIdx]) {
        mPlayers[playerIdx]->mPosition = respawnPos;
        mPlayers[playerIdx]->changeState(PlayerStateID::Human_Wait);
    }
}

// 0x026BAE68: Decompiled vfunc_12 (Event 0x19: Super jump start)
void PlayerMgr::onSuperJumpStart(u32 playerIdx) {
    mManagerFlags |= 0x800;
}

// 0x026BAEC8: Decompiled vfunc_13 (Event 0x1A: Super jump land)
void PlayerMgr::onSuperJumpLand(u32 playerIdx) {
    mManagerFlags |= 0x1000;
}

} // namespace Game
