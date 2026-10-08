#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * PlayerWeaponBase
 * Base class for all player weapons (Main, Sub, Special) in Splatoon.
 *
 * Real PowerPC methods verified across PlayerWeaponBigLaser, BigShot, Charge, Tornado:
 *   vfunc_35 - Sub-action controller invocation (offset 0x1A8)
 *   vfunc_70 - Export weapon parameter context pointer (offset 0x18C)
 */
class PlayerWeaponBase : public GambitActor {
public:
    PlayerWeaponBase();
    virtual ~PlayerWeaponBase() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Vtable slot overrides directly matched with Ghidra
    virtual u32 vfunc_35();
    virtual void vfunc_70(u32* outWeaponContext);

    void* getWeaponParamContext() const { return mWeaponContextPtr; }
    void setWeaponParamContext(void* ctx) { mWeaponContextPtr = ctx; }

protected:
    // Struct layout aligned to Espresso PowerPC offsets:
    undefined mPaddingBase[0x18C - sizeof(GambitActor)];

    // +0x18C: Pointer to active weapon/ammo context
    void* mWeaponContextPtr;           // 0x18C

    undefined mPaddingAction[0x18];    // 0x190 - 0x1A8

    // +0x1A8: Sub-action / trigger controller structure
    u32 mActionState;                  // 0x1A8
    void* mActionVtable;               // 0x1AC
};

} // namespace Game
