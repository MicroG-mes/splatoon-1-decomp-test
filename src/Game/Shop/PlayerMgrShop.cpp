#include "Game/Shop/PlayerMgrShop.h"

namespace Game {

PlayerMgrShop::PlayerMgrShop()
    : mShopKind(KindOfShop::cWeapon),
      mState(PlayerShopState::cState_Idle),
      mStateTimer(0),
      mPreviewYaw(0.0f),
      mTargetCameraDistance(3.0f),
      mCurrentCameraDistance(3.0f),
      mCameraHeight(1.0f),
      mIsPreviewing(false) {
}

PlayerMgrShop::~PlayerMgrShop() {
}

void PlayerMgrShop::init() {
    GambitActor::init();
    if (SaveDataMgr::instance()) {
        mEquippedCustom = SaveDataMgr::instance()->getCustomization();
        mPreviewCustom = mEquippedCustom;
    }
    setShopKind(mShopKind);
}

void PlayerMgrShop::setShopKind(KindOfShop kind) {
    mShopKind = kind;
    mPreviewYaw = 0.0f;

    // Adjust camera focusing height and distance based on which gear part is being shopped
    switch (mShopKind) {
        case KindOfShop::cHead:
            mTargetCameraDistance = 1.8f;
            mCameraHeight = 1.4f;
            break;
        case KindOfShop::cClothes:
            mTargetCameraDistance = 2.4f;
            mCameraHeight = 1.0f;
            break;
        case KindOfShop::cShoes:
            mTargetCameraDistance = 1.6f;
            mCameraHeight = 0.35f;
            break;
        case KindOfShop::cWeapon:
            mTargetCameraDistance = 3.2f;
            mCameraHeight = 0.9f;
            break;
    }
    mCurrentCameraDistance = mTargetCameraDistance;
}

void PlayerMgrShop::previewItem(const ShopItem& item) {
    mState = PlayerShopState::cState_ChangeGear;
    mStateTimer = 0;
    mIsPreviewing = true;

    switch (item.kind) {
        case KindOfShop::cHead:
            mPreviewCustom.headGearId = item.id;
            break;
        case KindOfShop::cClothes:
            mPreviewCustom.clothesId = item.id;
            break;
        case KindOfShop::cShoes:
            mPreviewCustom.shoesId = item.id;
            break;
        case KindOfShop::cWeapon:
            mPreviewCustom.weaponSetId = item.id;
            break;
    }
}

void PlayerMgrShop::resetToEquipped() {
    mPreviewCustom = mEquippedCustom;
    mIsPreviewing = false;
    mState = PlayerShopState::cState_Idle;
}

void PlayerMgrShop::confirmEquip(const ShopItem& item) {
    mState = PlayerShopState::cState_PoseSuccess;
    mStateTimer = 0;
    mEquippedCustom = mPreviewCustom;

    if (SaveDataMgr::instance()) {
        SaveDataMgr::instance()->setCustomization(mEquippedCustom);
    }
}

void PlayerMgrShop::rotatePreview(f32 deltaYaw) {
    mPreviewYaw += deltaYaw;
    if (mPreviewYaw > 360.0f) {
        mPreviewYaw -= 360.0f;
    } else if (mPreviewYaw < 0.0f) {
        mPreviewYaw += 360.0f;
    }
}

void PlayerMgrShop::update() {
    mStateTimer++;

    // Smooth camera distance interpolation
    mCurrentCameraDistance += (mTargetCameraDistance - mCurrentCameraDistance) * 0.1f;

    switch (mState) {
        case PlayerShopState::cState_ChangeGear:
            // Spin animation frame duration
            if (mStateTimer > 20) {
                mState = PlayerShopState::cState_Idle;
            }
            break;

        case PlayerShopState::cState_PoseSuccess:
            // Victory posing celebration
            if (mStateTimer > 60) {
                mState = PlayerShopState::cState_Idle;
            }
            break;

        case PlayerShopState::cState_Idle:
        default:
            break;
    }
}

void PlayerMgrShop::draw() {
    GambitActor::draw();
}

} // namespace Game
