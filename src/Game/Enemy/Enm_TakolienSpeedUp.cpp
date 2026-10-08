#include "Game/Enemy/Enm_TakolienSpeedUp.h"
#include <cstring>
#include <cmath>

namespace Game {

Enm_TakolienSpeedUp::Enm_TakolienSpeedUp()
    : Enm_Takolien(),
      mHasKelp(true),
      mSpecialCharge(0.0f),
      mIsSpecialDeploying(false),
      mSpecialDurationFrames(0) {
    std::memset(mReservedSpeedUp, 0, sizeof(mReservedSpeedUp));
}

Enm_TakolienSpeedUp::~Enm_TakolienSpeedUp() {
}

void Enm_TakolienSpeedUp::init() {
    Enm_Takolien::init();
    mHasKelp = true;
    mSpecialCharge = 0.0f;
    mIsSpecialDeploying = false;
    mSpecialDurationFrames = 0;
    vfunc_1();
}

// 0x023CE8DC: Decompiled vfunc_1 (Kelp accessory attachment & speed buff init)
void Enm_TakolienSpeedUp::vfunc_1() {
    mHasKelp = true;
}

void Enm_TakolienSpeedUp::updateEliteAi(const sead::Vector3f& playerPos, bool isPlayerVisible) {
    if (mState == TakolienState::cDefeated) {
        return;
    }

    // Elite Octolings charge special while maneuvering
    if (!mIsSpecialDeploying && mSpecialCharge < cSpecialChargeThreshold) {
        mSpecialCharge += 0.5f;
    }

    if (mIsSpecialDeploying) {
        mSpecialDurationFrames++;
        if (mSpecialDurationFrames >= 180) { // 3 seconds Killer Wail column
            mIsSpecialDeploying = false;
            mSpecialCharge = 0.0f;
            mSpecialDurationFrames = 0;
        }
        return;
    }

    // Call base AI
    updateAi(playerPos, isPlayerVisible);

    // Apply speed multiplier to octopus swim moves
    if (mForm == TakolienForm::cOctopus) {
        mPosition.x += mMoveDirection.x * (cEliteSwimSpeed - cOctoSwimSpeed);
        mPosition.z += mMoveDirection.z * (cEliteSwimSpeed - cOctoSwimSpeed);
    }

    // Check special deployment
    if (isPlayerVisible && isSpecialReady()) {
        tryDeployKillerWail(playerPos);
    }
}

bool Enm_TakolienSpeedUp::tryDeployKillerWail(const sead::Vector3f& targetPos) {
    (void)targetPos;
    if (!isSpecialReady() || mIsSpecialDeploying) {
        return false;
    }

    mIsSpecialDeploying = true;
    mSpecialDurationFrames = 0;
    vfunc_52(); // Stand up in humanoid form to deploy Megaphone
    return true;
}

void Enm_TakolienSpeedUp::update() {
    Enm_Takolien::update();
}

} // namespace Game
