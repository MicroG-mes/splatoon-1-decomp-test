#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class ArmorState : u32 {
    cState_Wait = 0,
    cState_Got  = 1
};

/**
 * Obj_Armor / ItemArmor
 * Address: vtable @ 0x1009d060
 * Hero Armor suit power-up collectible in Octo Valley single-player campaign.
 * Grants extra armor layers that absorb damage before shattering.
 *
 * Real PowerPC methods:
 *   vfunc_3  @ 0x02467208 - Model & resource loading (Obj_Armor.szs)
 *   vfunc_5  @ 0x02468FD8 - Initialization and bobbing parameters
 *   vfunc_14 @ 0x02467278 - Touch collision & collection event
 *   vfunc_47 @ 0x0246771C - Floating animation sync
 */
class Obj_Armor : public GambitActor {
public:
    static constexpr f32 cTouchRadius             = 2.0f;
    static constexpr f32 cArmorDurabilityPerTier  = 100.0f;
    static constexpr u32 cMaxTier                 = 3;
    static constexpr s32 cBreakInvulnFrames       = 30;
    static constexpr f32 cBobAmplitude            = 0.4f;
    static constexpr f32 cRotateSpeed             = 0.03f;

    Obj_Armor();
    virtual ~Obj_Armor() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC vtable slots matching Ghidra
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_14();
    virtual void vfunc_47();

    // Collection & player armor management
    bool checkPlayerTouch(const sead::Vector3f& playerPos);
    static bool applyArmorPickup(u32& currentTier, f32& currentArmorHp);
    static bool applyDamageToArmor(f32 damage, u32& currentTier, f32& currentArmorHp, bool& outShattered);

    ArmorState getState() const { return mState; }
    bool isCollected() const { return mState == ArmorState::cState_Got; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    f32 getRotationAngle() const { return mRotAngle; }

    void setPosition(const sead::Vector3f& pos);

protected:
    sead::Vector3f mBasePosition;
    sead::Vector3f mPosition;      // 0x140

    ArmorState mState;
    f32 mRotAngle;
    s32 mTimer;

    undefined mReserved[0x38];
};

// Internal binary alias
using ItemArmor = Obj_Armor;

} // namespace Game
