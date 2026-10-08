#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class ToothArmorType : u32 {
    cNormal     = 0,
    cReinforced = 1,
    cGold       = 2
};

/**
 * Enm_MouthKingTooth (Octomaw Dental Armor & Gold Tooth Actor)
 * Decompiled from PowerPC retail executable Gambit.elf:
 * Vtable @ 0x1007A4C8
 *
 * Authentic PowerPC Methods:
 *   vfunc_7  @ 0x02357470: Durability tick, gold tooth evaluation (0x297), broken state (0x29B)
 *   vfunc_11 @ 0x02357700: Jaw transform matrix sync
 *   vfunc_47 @ 0x0235E168: Stage collision mesh registration
 *   vfunc_52 @ 0x0235780C: Tooth break animation & dental shatter particle trigger
 */
class Enm_MouthKingTooth : public GambitActor {
public:
    Enm_MouthKingTooth();
    virtual ~Enm_MouthKingTooth() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic Ghidra Decompiled vfuncs
    virtual void vfunc_7();  // Durability & timer tick
    virtual void vfunc_11(); // Transform sync
    virtual void vfunc_47(); // Collision register
    virtual void vfunc_52(); // Tooth shatter trigger

    void setupTooth(u32 index, ToothArmorType armor);
    void applyDamage(f32 damage);
    void breakTooth();
    void resetTooth();

    bool isBroken() const { return mIsBroken; }
    f32 getHp() const { return mHp; }
    ToothArmorType getArmorType() const { return mArmorType; }
    u32 getIndex() const { return mIndex; }
    bool isGoldTooth() const { return mIsGoldTooth; }

protected:
    u32 mIndex;
    ToothArmorType mArmorType;
    f32 mHp;
    f32 mMaxHp;
    bool mIsBroken;       // 0x29B
    bool mIsGoldTooth;    // 0x297
    s32 mShakeTimer;      // 0x288
    s32 mDurabilityId;    // 0x28C

    u8 mReserved[0x20];
};

} // namespace Game
