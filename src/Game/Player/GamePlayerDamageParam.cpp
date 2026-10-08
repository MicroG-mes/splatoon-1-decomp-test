#include "Game/Player/GamePlayerDamageParam.h"

namespace Game {

GamePlayerDamageParam::GamePlayerDamageParam() {
    resetToDefaults();
}

/**
 * FUN_02546838
 * Authentic parameter initialization from Wii U retail executable.
 */
void GamePlayerDamageParam::resetToDefaults() {
    mDamageEffect_AlphaAtDamageRateZ = 1.0f;
    mBombCoreDamageK = 1.0f;
    mRollerSplashDamageK = 1.0f;
    mRollerCoreDamageK = 1.0f;
    mChargeBulletDamageK = 1.0f;
    mSuperShotDamageK = 1.0f;

    mBarrierRadiusMaxFlicker = 1.0f;
    mBarrierRadiusFlickerKd = 0.85f;
    mBarrierRadiusFlickerCycleFrame = 12; // 0x138: 12 frames
    mBarrierRadiusFlickerCycleGap = 1.0f;
    mBarrierRadiusFlickerCancelFrame = 5;  // 0x168: 5 frames
}

} // namespace Game
