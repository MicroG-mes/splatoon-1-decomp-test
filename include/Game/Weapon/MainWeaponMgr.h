#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"

namespace Game {

/**
 * MainWeaponMgr
 * Central manager for all 5 main weapon categories across all 8 players in a match.
 *
 * Address: vtable @ 0x100FB6A4
 * Real PowerPC methods:
 *   vfunc_3  @ 0x0276ebec - Load & initialize 5 weapon pools (Shooters, Rollers, Chargers, Sloshers, Splatlings)
 *   vfunc_4  @ 0x0276eee0 - Pre-reset
 *   vfunc_5  @ 0x0276ec94 - Reset
 *   vfunc_6  @ 0x0276ef3c - Enter scene
 *   vfunc_7  @ 0x0276ada0 - Tick / Update
 *   vfunc_8  @ 0x0276ef98 - Post-calc
 *   vfunc_9  @ 0x0276eff4 - Render / Draw
 *   vfunc_10 @ 0x0276f050 - Splatted event handler
 *   vfunc_11 @ 0x0276f0ac - Respawn event handler
 *   vfunc_12 @ 0x0276f108 - SuperJump start event handler
 *   vfunc_13 @ 0x0276f168 - SuperJump land event handler
 *   vfunc_33 @ 0x0276f1c8 - Terminate / Unload
 */
class MainWeaponMgr : public GambitActor {
public:
    static MainWeaponMgr* instance() { return sInstance; }

    MainWeaponMgr();
    virtual ~MainWeaponMgr() override;

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
    static MainWeaponMgr* sInstance;

    undefined mPaddingMgr[0x230 - sizeof(GambitActor)];

    // +0x230: Lifecycle and state bitmask
    u32 mStateFlags; // 0x230
};

} // namespace Game
