#pragma once

#include "types.h"
#include "Game/Enemy/Enm_Takolien.h"

namespace Game {

/**
 * Enm_TakolienSpeedUp (Elite Kelp Octoling / Speed-Up Takolien)
 * Decompiled from PowerPC retail executable Gambit.elf:
 * Vtable @ 0x1008D42C
 *
 * Authentic PowerPC Methods:
 *   vfunc_1 @ 0x023CE8DC: Kelp accessory attachment & speed buff initialization
 *   vfunc_7 @ 0x023B71EC: Inherited fast-patrol & aggressive aim calculation
 *   State::cPatrol @ 0x023CBC54: Dynamic patrol and rapid pursuit state logic
 */
class Enm_TakolienSpeedUp : public Enm_Takolien {
public:
    static constexpr f32 cSpeedMultiplier = 1.35f;
    static constexpr f32 cEliteSwimSpeed = cOctoSwimSpeed * cSpeedMultiplier;
    static constexpr f32 cSpecialChargeThreshold = 100.0f;

    Enm_TakolienSpeedUp();
    virtual ~Enm_TakolienSpeedUp() override;

    virtual void init() override;
    virtual void update() override;

    // Authentic Ghidra Decompiled vfuncs
    virtual void vfunc_1(); // 0x023CE8DC: Kelp accessory init

    void updateEliteAi(const sead::Vector3f& playerPos, bool isPlayerVisible);
    bool tryDeployKillerWail(const sead::Vector3f& targetPos);

    bool hasKelpEquipped() const { return mHasKelp; }
    f32 getSpecialCharge() const { return mSpecialCharge; }
    bool isSpecialReady() const { return mSpecialCharge >= cSpecialChargeThreshold; }
    bool isSpecialDeploying() const { return mIsSpecialDeploying; }

protected:
    bool mHasKelp;
    f32 mSpecialCharge;
    bool mIsSpecialDeploying;
    s32 mSpecialDurationFrames;

    u8 mReservedSpeedUp[0x20];
};

} // namespace Game
