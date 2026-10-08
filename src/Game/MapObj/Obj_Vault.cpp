#include "Game/MapObj/Obj_Vault.h"

namespace Game {

Obj_Vault::Obj_Vault()
    : mPosition(0.0f, 0.0f, 0.0f),
      mRequiredKeyId(0),
      mState(VaultState::cLocked),
      mLidAngle(0.0f),
      mTimer(0) {
}

Obj_Vault::~Obj_Vault() {
}

void Obj_Vault::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mRequiredKeyId = 0;
    mState = VaultState::cLocked;
    mLidAngle = 0.0f;
    mTimer = 0;
}

void Obj_Vault::setupVault(u32 keyId, const sead::Vector3f& pos) {
    mRequiredKeyId = keyId;
    mPosition = pos;
    mState = VaultState::cLocked;
    mLidAngle = 0.0f;
    mTimer = 0;
}

bool Obj_Vault::tryUnlockWithKey(u32 keyId) {
    if (mState != VaultState::cLocked) {
        return false;
    }

    if (keyId == mRequiredKeyId) {
        mState = VaultState::cUnlocking;
        mTimer = 0;
        return true;
    }

    return false;
}

void Obj_Vault::update() {
    switch (mState) {
        case VaultState::cUnlocking: {
            mTimer++;
            // Chain shattering delay
            if (mTimer > 30) {
                // Lid starts swinging open
                mLidAngle += 0.06f;
                if (mLidAngle >= 1.5707963f) { // 90 degrees open
                    mLidAngle = 1.5707963f;
                    mState = VaultState::cOpened;
                }
            }
            break;
        }

        case VaultState::cOpened:
        case VaultState::cLocked:
        default:
            break;
    }
}

void Obj_Vault::draw() {
    // Model, chain mesh break and lid bone rotation handled by ModelSceneMgr
}

} // namespace Game
