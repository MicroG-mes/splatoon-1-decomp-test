#pragma once

#include "types.h"

namespace Game {

/**
 * GamePlayerDamageParam
 * Address: constructor @ 0x02546838, vtable @ 0x100C6ED0
 * Contains authentic retail damage scaling constants and Bubbler/Barrier flicker configuration.
 */
class GamePlayerDamageParam {
public:
    // Authentic defaults initialized in FUN_02546838
    GamePlayerDamageParam();
    ~GamePlayerDamageParam() = default;

    void resetToDefaults();

    f32 getDamageEffectAlpha() const { return mDamageEffect_AlphaAtDamageRateZ; }
    f32 getBombCoreDamageK() const { return mBombCoreDamageK; }
    f32 getRollerSplashDamageK() const { return mRollerSplashDamageK; }
    f32 getRollerCoreDamageK() const { return mRollerCoreDamageK; }
    f32 getChargeBulletDamageK() const { return mChargeBulletDamageK; }
    f32 getSuperShotDamageK() const { return mSuperShotDamageK; }

    f32 getBarrierRadiusMaxFlicker() const { return mBarrierRadiusMaxFlicker; }
    f32 getBarrierRadiusFlickerKd() const { return mBarrierRadiusFlickerKd; }
    s32 getBarrierRadiusFlickerCycleFrame() const { return mBarrierRadiusFlickerCycleFrame; }
    f32 getBarrierRadiusFlickerCycleGap() const { return mBarrierRadiusFlickerCycleGap; }
    s32 getBarrierRadiusFlickerCancelFrame() const { return mBarrierRadiusFlickerCancelFrame; }

protected:
    f32 mDamageEffect_AlphaAtDamageRateZ; // 0x78
    f32 mBombCoreDamageK;                 // 0x90
    f32 mRollerSplashDamageK;             // 0xA8
    f32 mRollerCoreDamageK;               // 0xC0
    f32 mChargeBulletDamageK;             // 0xD8
    f32 mSuperShotDamageK;                // 0xF0

    f32 mBarrierRadiusMaxFlicker;         // 0x108
    f32 mBarrierRadiusFlickerKd;          // 0x120
    s32 mBarrierRadiusFlickerCycleFrame;  // 0x138 (12 frames)
    f32 mBarrierRadiusFlickerCycleGap;    // 0x150
    s32 mBarrierRadiusFlickerCancelFrame; // 0x168 (5 frames)
};

} // namespace Game
