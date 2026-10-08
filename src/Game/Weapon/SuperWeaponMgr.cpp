#include "Game/Weapon/SuperWeaponMgr.h"
#include <cstring>

namespace Game {

SuperWeaponMgr* SuperWeaponMgr::sInstance = nullptr;

SuperWeaponMgr::SuperWeaponMgr()
    : mStateFlags(0) {
    sInstance = this;
    std::memset(mPaddingMgr, 0, sizeof(mPaddingMgr));
}

SuperWeaponMgr::~SuperWeaponMgr() {
    if (sInstance == this) {
        sInstance = nullptr;
    }
}

void SuperWeaponMgr::init() {
    GambitActor::init();
    vfunc_3();
}

void SuperWeaponMgr::update() {
    GambitActor::update();
    vfunc_7();
}

void SuperWeaponMgr::draw() {
    GambitActor::draw();
    vfunc_9();
}

/**
 * SuperWeaponMgr__vfunc_3 @ 0x02773d70
 * Initializes the 7 special weapons:
 * Kraken, Bubbler, Inkstrike, Killer Wail, Inkzooka, Echolocator, Bomb Rush.
 * Sets state flag 0x1 on *(param_1 + 0x230).
 */
void SuperWeaponMgr::vfunc_3() {
    mStateFlags |= 0x1;
}

/**
 * SuperWeaponMgr__vfunc_4 @ 0x02773fb4
 * Pre-reset, sets state flag 0x2.
 */
void SuperWeaponMgr::vfunc_4() {
    mStateFlags |= 0x2;
}

/**
 * SuperWeaponMgr__vfunc_5 @ 0x02773dd8
 * Reset for match start, sets state flag 0x4.
 */
void SuperWeaponMgr::vfunc_5() {
    mStateFlags |= 0x4;
}

/**
 * SuperWeaponMgr__vfunc_6 @ 0x02774010
 * Enter scene, sets state flag 0x8.
 */
void SuperWeaponMgr::vfunc_6() {
    mStateFlags |= 0x8;
}

/**
 * SuperWeaponMgr__vfunc_7 @ 0x02773cf4
 * Main special weapon update tick.
 * Calls FUN_02773b0c which synchronizes special states with PlayerMgr.
 * Sets state flag 0x40.
 */
void SuperWeaponMgr::vfunc_7() {
    mStateFlags |= 0x40;
}

/**
 * SuperWeaponMgr__vfunc_8 @ 0x0277406c
 * Post-calc, sets state flag 0x80.
 */
void SuperWeaponMgr::vfunc_8() {
    mStateFlags |= 0x80;
}

/**
 * SuperWeaponMgr__vfunc_9 @ 0x027740c8
 * Draw tick, sets state flag 0x100.
 */
void SuperWeaponMgr::vfunc_9() {
    mStateFlags |= 0x100;
}

/**
 * SuperWeaponMgr__vfunc_10 @ 0x02774124
 * Player splatted event handler: cancels active special, sets state flag 0x200.
 */
void SuperWeaponMgr::vfunc_10(u32 playerIndex) {
    (void)playerIndex;
    mStateFlags |= 0x200;
}

/**
 * SuperWeaponMgr__vfunc_11 @ 0x02774180
 * Player respawn event handler: resets special charge meter, sets state flag 0x400.
 */
void SuperWeaponMgr::vfunc_11(u32 playerIndex) {
    (void)playerIndex;
    mStateFlags |= 0x400;
}

/**
 * SuperWeaponMgr__vfunc_12 @ 0x027741dc
 * SuperJump start event handler, sets state flag 0x800.
 */
void SuperWeaponMgr::vfunc_12(u32 playerIndex) {
    (void)playerIndex;
    mStateFlags |= 0x800;
}

/**
 * SuperWeaponMgr__vfunc_13 @ 0x0277423c
 * SuperJump land event handler, sets state flag 0x1000.
 */
void SuperWeaponMgr::vfunc_13(u32 playerIndex) {
    (void)playerIndex;
    mStateFlags |= 0x1000;
}

/**
 * SuperWeaponMgr__vfunc_33 @ 0x0277429c
 * Terminate & cleanup.
 */
void SuperWeaponMgr::vfunc_33() {
    mStateFlags = 0;
}

} // namespace Game
