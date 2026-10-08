#include "Game/Shop/LytShopHandler.h"
#include "Game/System/SaveDataMgr.h"

namespace Game {

LytShopHandler::LytShopHandler()
    : mCurrentShop(KindOfShop::cWeapon),
      mUiMode(ShopUiMode::Browsing),
      mSelectedIndex(0),
      mCursorAnimTimer(0),
      mShowConfirmationDialog(false) {
}

LytShopHandler::~LytShopHandler() {
}

void LytShopHandler::init() {
    GambitActor::init();
    mNpcMgr.init();
    mPlayerMgr.init();
    openShop(mCurrentShop);
}

void LytShopHandler::openShop(KindOfShop kind) {
    mCurrentShop = kind;
    mUiMode = ShopUiMode::Browsing;
    mSelectedIndex = 0;
    mShowConfirmationDialog = false;

    mNpcMgr.changeNpc(mCurrentShop);
    mPlayerMgr.setShopKind(mCurrentShop);

    populateStock();

    if (!mStock.empty()) {
        mPlayerMgr.previewItem(mStock[mSelectedIndex]);
        mNpcMgr.onItemInspected(mStock[mSelectedIndex]);
    }
}

void LytShopHandler::closeShop() {
    mUiMode = ShopUiMode::Leaving;
}

void LytShopHandler::switchShop(s32 direction) {
    s32 current = static_cast<s32>(mCurrentShop);
    current = (current + direction + 4) % 4;
    openShop(static_cast<KindOfShop>(current));
}

void LytShopHandler::populateStock() {
    mStock.clear();

    switch (mCurrentShop) {
        case KindOfShop::cWeapon: {
            // Splattershot, Splat Roller, Splat Charger, etc.
            ShopItem item1 = { 1, KindOfShop::cWeapon, BrandId::cSquidForce, 2, AbilityId::None, {AbilityId::None, AbilityId::None, AbilityId::None}, 500, 2, false, 1, 1, 50.0f, 45.0f, 55.0f };
            ShopItem item2 = { 2, KindOfShop::cWeapon, BrandId::cKrakOn,     2, AbilityId::None, {AbilityId::None, AbilityId::None, AbilityId::None}, 1000, 3, false, 2, 2, 48.0f, 60.0f, 40.0f };
            ShopItem item3 = { 3, KindOfShop::cWeapon, BrandId::cFirefin,    3, AbilityId::None, {AbilityId::None, AbilityId::None, AbilityId::None}, 1500, 4, false, 3, 3, 85.0f, 70.0f, 20.0f };
            mStock.push_back(item1);
            mStock.push_back(item2);
            mStock.push_back(item3);
            break;
        }
        case KindOfShop::cHead: {
            ShopItem item1 = { 101, KindOfShop::cHead, BrandId::cTentatek,   2, AbilityId::InkRecoveryUp, {AbilityId::None, AbilityId::None, AbilityId::None}, 800, 2, false, 0, 0, 0, 0, 0 };
            ShopItem item2 = { 102, KindOfShop::cHead, BrandId::cRockenberg, 3, AbilityId::RunSpeedUp,    {AbilityId::None, AbilityId::None, AbilityId::None}, 1600, 4, false, 0, 0, 0, 0, 0 };
            mStock.push_back(item1);
            mStock.push_back(item2);
            break;
        }
        case KindOfShop::cClothes: {
            ShopItem item1 = { 201, KindOfShop::cClothes, BrandId::cZekko,     2, AbilityId::SpecialSaver,   {AbilityId::None, AbilityId::None, AbilityId::None}, 900, 2, false, 0, 0, 0, 0, 0 };
            ShopItem item2 = { 202, KindOfShop::cClothes, BrandId::cSplashMob, 3, AbilityId::InkSaverMain,   {AbilityId::None, AbilityId::None, AbilityId::None}, 1800, 4, false, 0, 0, 0, 0, 0 };
            mStock.push_back(item1);
            mStock.push_back(item2);
            break;
        }
        case KindOfShop::cShoes: {
            ShopItem item1 = { 301, KindOfShop::cShoes, BrandId::cTakoroka, 2, AbilityId::SpecialChargeUp, {AbilityId::None, AbilityId::None, AbilityId::None}, 850, 2, false, 0, 0, 0, 0, 0 };
            ShopItem item2 = { 302, KindOfShop::cShoes, BrandId::cKrakOn,   3, AbilityId::SwimSpeedUp,     {AbilityId::None, AbilityId::None, AbilityId::None}, 1750, 4, false, 0, 0, 0, 0, 0 };
            mStock.push_back(item1);
            mStock.push_back(item2);
            break;
        }
    }
}

void LytShopHandler::handleInput(const VPADStatus& vpad) {
    if (mUiMode == ShopUiMode::Leaving) {
        return;
    }

    // Right stick rotates character preview
    if (vpad.rightStick.x > 0.2f || vpad.rightStick.x < -0.2f) {
        mPlayerMgr.rotatePreview(vpad.rightStick.x * 3.0f);
    }

    // Shoulder buttons switch shop
    if (vpad.trigger & VPAD_BUTTON_L) {
        switchShop(-1);
        return;
    } else if (vpad.trigger & VPAD_BUTTON_R) {
        switchShop(1);
        return;
    }

    // Mode-specific handling
    switch (mUiMode) {
        case ShopUiMode::Browsing: {
            if (vpad.trigger & VPAD_BUTTON_LEFT) {
                if (mSelectedIndex > 0) {
                    mSelectedIndex--;
                    mPlayerMgr.previewItem(mStock[mSelectedIndex]);
                    mNpcMgr.onItemInspected(mStock[mSelectedIndex]);
                }
            } else if (vpad.trigger & VPAD_BUTTON_RIGHT) {
                if (mSelectedIndex + 1 < static_cast<s32>(mStock.size())) {
                    mSelectedIndex++;
                    mPlayerMgr.previewItem(mStock[mSelectedIndex]);
                    mNpcMgr.onItemInspected(mStock[mSelectedIndex]);
                }
            } else if (vpad.trigger & VPAD_BUTTON_A) {
                tryBuyCurrentItem();
            } else if (vpad.trigger & VPAD_BUTTON_B) {
                closeShop();
            }
            break;
        }

        case ShopUiMode::ConfirmBuy: {
            if (vpad.trigger & VPAD_BUTTON_A) {
                confirmPurchase();
            } else if (vpad.trigger & VPAD_BUTTON_B) {
                mUiMode = ShopUiMode::Browsing;
            }
            break;
        }

        case ShopUiMode::EquipPrompt: {
            if (vpad.trigger & VPAD_BUTTON_A) {
                confirmEquip();
                mUiMode = ShopUiMode::Browsing;
            } else if (vpad.trigger & VPAD_BUTTON_B) {
                mPlayerMgr.resetToEquipped();
                mUiMode = ShopUiMode::Browsing;
            }
            break;
        }

        default:
            break;
    }
}

void LytShopHandler::tryBuyCurrentItem() {
    if (mStock.empty() || mSelectedIndex >= static_cast<s32>(mStock.size())) {
        return;
    }

    const ShopItem& item = mStock[mSelectedIndex];
    if (item.isOwned) {
        // Already owned, just equip preview
        mUiMode = ShopUiMode::EquipPrompt;
        return;
    }

    SaveDataMgr* save = SaveDataMgr::instance();
    if (!save) {
        return;
    }

    const PlayerStats& stats = save->getStats();
    if (stats.level < item.unlockLevel) {
        mNpcMgr.onItemRejected(true, false);
        return;
    }

    if (stats.money < item.price) {
        mNpcMgr.onItemRejected(false, true);
        return;
    }

    // Prompt user to buy
    mUiMode = ShopUiMode::ConfirmBuy;
}

void LytShopHandler::confirmPurchase() {
    if (mSelectedIndex >= static_cast<s32>(mStock.size())) {
        return;
    }

    ShopItem& item = mStock[mSelectedIndex];
    SaveDataMgr* save = SaveDataMgr::instance();
    if (save) {
        PlayerStats stats = save->getStats();
        if (stats.money >= item.price) {
            save->addMoney(-static_cast<u32>(item.price));
            item.isOwned = true;
            mNpcMgr.onItemPurchased(item);
            mUiMode = ShopUiMode::EquipPrompt;
        }
    }
}

void LytShopHandler::confirmEquip() {
    if (mSelectedIndex >= static_cast<s32>(mStock.size())) {
        return;
    }
    mPlayerMgr.confirmEquip(mStock[mSelectedIndex]);
}

void LytShopHandler::update() {
    mCursorAnimTimer++;
    mNpcMgr.update();
    mPlayerMgr.update();
}

void LytShopHandler::draw() {
    mNpcMgr.draw();
    mPlayerMgr.draw();
}

} // namespace Game
