#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"

namespace Game {

/**
 * SuperWeaponMgr
 * Central manager for all Special / Super Weapons across all 8 players in a match.
 * Controls special duration timers, charging states, and super weapon activation events.
 *
 * Address: vtable @ 0x100FC5BC
 * Real PowerPC methods:
 *   vfunc_3  @ 0x02773d70 - Load & initialize 7 special weapon types
 *   vfunc_4  @ 0x02773fb4 - Pre-reset
 *   vfunc_5  @ 0x02773dd8 - Reset
 *   vfunc_6  @ 0x02774010 - Enter scene
 *   vfunc_7  @ 0x02773cf4 - Tick / Update & coordination with PlayerMgr
 *   vfunc_8  @ 0x0277406c - Post-calc
 *   vfunc_9  @ 0x027740c8 - Render / Draw
 *   vfunc_10 @ 0x02774124 - Splatted event handler (cancels active special)
 *   vfunc_11 @ 0x02774180 - Respawn event handler (resets special gauge)
 *   vfunc_12 @ 0x027741dc - SuperJump start event handler
 *   vfunc_13 @ 0x0277423c - SuperJump land event handler
 *   vfunc_33 @ 0x0277429c - Terminate / Unload
 */
class SuperWeaponMgr : public GambitActor {
public:
    static SuperWeaponMgr* instance() { return sInstance; }

    SuperWeaponMgr();
    virtual ~SuperWeaponMgr() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Vtable lifecycle overrides directly from Espresso PPC
    virtual void vfunc_3();
    virtual void vfunc_4();
    virtual void vfunc_5();
    virtual void vfunc_6();
    virtual void vfunc_7();
    virtual void vfunc_8();
    virtual void vfunc_9();
    virtual void vfunc_10(u32 playerIndex);
    virtual void vfunc_11(u32 playerIndex);
    virtual void vfunc_12(u32 playerIndex);
    virtual void vfunc_13(u32 playerIndex);
    virtual void vfunc_33();

    u32 getStateFlags() const { return mStateFlags; }

protected:
    static SuperWeaponMgr* sInstance;

    undefined mPaddingMgr[0x230 - sizeof(GambitActor)];

    // +0x230: Lifecycle and state bitmask
    u32 mStateFlags; // 0x230
};

} // namespace Game
