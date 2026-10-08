#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include <vector>

namespace Game {

struct WeaponListing {
    u32 weaponId;
    const char* name;
    u32 requiredLevel;
    u32 priceCoins;
    bool isPurchased;
};

/**
 * Npc_WeaponsShop (Sheldon / Ammo Knights Shopkeeper)
 * Address: vtable @ 0x1011CD00
 * Authentic path: D:/home/Cafe/Gambit/App/Program/Game/Shop/Npc_WeaponsShop.cpp
 *
 * Real PowerPC methods from Gambit.elf:
 *   vfunc_1  @ 0x028384F0 (size 320): Destructor and actor cleanup
 *   vfunc_63 @ 0x02838080 (size 192): Purchase weapon transaction & coin deduction
 *   vfunc_64 @ 0x02838140 (size 56): Level requirement check
 *   vfunc_68 @ 0x028381B8 (size 104): Test fire range entry (Shooting Range)
 *   vfunc_72 @ 0x02838178 (size 64): Sheldon weapon monologue chatter
 */
class Npc_WeaponsShop : public GambitActor {
public:
    Npc_WeaponsShop();
    virtual ~Npc_WeaponsShop() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic PPC vfuncs
    virtual bool vfunc_63(u32 weaponId, u32* pPlayerCoins);
    virtual bool vfunc_64(u32 weaponId, u32 playerLevel);
    virtual bool vfunc_68(u32 weaponId);
    virtual const char* vfunc_72(u32 weaponId);

    void setupCatalog();
    bool purchaseWeapon(u32 weaponId, u32 playerLevel, u32& playerCoins);
    bool canTestFire(u32 weaponId) const;

    const std::vector<WeaponListing>& getCatalog() const { return mCatalog; }
    const WeaponListing* findWeapon(u32 weaponId) const;
    bool isTestFiring() const { return mIsTestFiring; }
    u32 getTestWeaponId() const { return mTestWeaponId; }

protected:
    std::vector<WeaponListing> mCatalog;
    sead::Vector3f mPosition;
    bool mIsTestFiring;
    u32 mTestWeaponId;
    s32 mDialogueTimer;
};

} // namespace Game
