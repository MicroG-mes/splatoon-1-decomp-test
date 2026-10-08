#include "Game/Weapon/MainWeaponMgr.h"
#include <cstring>

namespace Game {

MainWeaponMgr* MainWeaponMgr::sInstance = nullptr;

MainWeaponMgr::MainWeaponMgr()
    : mStateFlags(0) {
    sInstance = this;
    std::memset(mPaddingMgr, 0, sizeof(mPaddingMgr));
}

MainWeaponMgr::~MainWeaponMgr() {
    if (sInstance == this) {
        sInstance = nullptr;
    }
}

void MainWeaponMgr::init() {
    GambitActor::init();
    vfunc_3();
}

void MainWeaponMgr::update() {
    GambitActor::update();
    vfunc_7();
}

void MainWeaponMgr::draw() {
    GambitActor::draw();
    vfunc_9();
}

/**
 * MainWeaponMgr__vfunc_3 @ 0x0276ebec
 * Initializes 5 weapon pools:
 *   FUN_0276cd48(); (Shooters)
 *   FUN_0276d654(param_1); (Rollers)
 *   FUN_0276db14(param_1); (Chargers)
 *   FUN_0276dfd4(param_1); (Sloshers)
 *   FUN_0276e494(param_1); (Splatlings)
 * Sets state flag 0x1 on *(param_1 + 0x230).
 */
void MainWeaponMgr::vfunc_3() {
    // 5 weapon categories initialized
    mStateFlags |= 0x1;
}

/**
 * MainWeaponMgr__vfunc_4 @ 0x0276eee0
 * Sets state flag 0x2 on *(param_1 + 0x230).
 */
void MainWeaponMgr::vfunc_4() {
    mStateFlags |= 0x2;
}

/**
 * MainWeaponMgr__vfunc_5 @ 0x0276ec94
 * Reset weapon manager for match start, sets state flag 0x4.
 */
void MainWeaponMgr::vfunc_5() {
    mStateFlags |= 0x4;
}

/**
 * MainWeaponMgr__vfunc_6 @ 0x0276ef3c
 * Scene transition, sets state flag 0x8.
 */
void MainWeaponMgr::vfunc_6() {
    mStateFlags |= 0x8;
}

/**
 * MainWeaponMgr__vfunc_7 @ 0x0276ada0
 * Main update tick, sets state flag 0x40.
 */
void MainWeaponMgr::vfunc_7() {
    mStateFlags |= 0x40;
}

/**
 * MainWeaponMgr__vfunc_8 @ 0x0276ef98
 * Post-calc tick, sets state flag 0x80.
 */
void MainWeaponMgr::vfunc_8() {
    mStateFlags |= 0x80;
}

/**
 * MainWeaponMgr__vfunc_9 @ 0x0276eff4
 * Draw tick, sets state flag 0x100.
 */
void MainWeaponMgr::vfunc_9() {
    mStateFlags |= 0x100;
}

/**
 * MainWeaponMgr__vfunc_10 @ 0x0276f050
 * Player splatted event handler, sets state flag 0x200.
 */
void MainWeaponMgr::vfunc_10(u32 playerIndex) {
    (void)playerIndex;
    mStateFlags |= 0x200;
}

/**
 * MainWeaponMgr__vfunc_11 @ 0x0276f0ac
 * Player respawn event handler, sets state flag 0x400.
 */
void MainWeaponMgr::vfunc_11(u32 playerIndex) {
    (void)playerIndex;
    mStateFlags |= 0x400;
}

/**
 * MainWeaponMgr__vfunc_12 @ 0x0276f108
 * SuperJump start event handler, sets state flag 0x800.
 */
void MainWeaponMgr::vfunc_12(u32 playerIndex) {
    (void)playerIndex;
    mStateFlags |= 0x800;
}

/**
 * MainWeaponMgr__vfunc_13 @ 0x0276f168
 * SuperJump land event handler, sets state flag 0x1000.
 */
void MainWeaponMgr::vfunc_13(u32 playerIndex) {
    (void)playerIndex;
    mStateFlags |= 0x1000;
}

/**
 * MainWeaponMgr__vfunc_33 @ 0x0276f1c8
 * Terminate & cleanup.
 */
void MainWeaponMgr::vfunc_33() {
    mStateFlags = 0;
}

} // namespace Game
