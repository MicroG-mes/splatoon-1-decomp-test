#include "Game/Shop/NpcMgrShop.h"

namespace Game {

NpcMgrShop::NpcMgrShop()
    : mShopKind(KindOfShop::cWeapon),
      mState(ShopNpcState::cState_Greeting),
      mStateTimer(0),
      mAnimFrame(0),
      mEyeBlinkTimer(0.0f),
      mDialogueText(nullptr),
      mIsTalkFinished(false) {
}

NpcMgrShop::~NpcMgrShop() {
}

void NpcMgrShop::init() {
    GambitActor::init();
    changeNpc(mShopKind);
}

void NpcMgrShop::changeNpc(KindOfShop kind) {
    mShopKind = kind;
    mState = ShopNpcState::cState_Greeting;
    mStateTimer = 0;
    mAnimFrame = 0;
    mIsTalkFinished = false;

    switch (mShopKind) {
        case KindOfShop::cWeapon:
            mDialogueText = "Welcome to Ammo Knights! Looking to upgrade your arsenal?";
            break;
        case KindOfShop::cHead:
            mDialogueText = "U-um... hi. Welcome to Cooler Heads. (Whaddaya want, squirt?!)";
            break;
        case KindOfShop::cClothes:
            mDialogueText = "Bonjour, fresh kiddo! Jelly Fresh has all the finest threads!";
            break;
        case KindOfShop::cShoes:
            mDialogueText = "Yo! Welcome to Shrimp Kicks! Let's get some fresh kicks on your feet!";
            break;
    }
}

const char* NpcMgrShop::getNpcName() const {
    switch (mShopKind) {
        case KindOfShop::cWeapon:
            return "Sheldon";
        case KindOfShop::cHead:
            return "Annie & Moe";
        case KindOfShop::cClothes:
            return "Jelonzo";
        case KindOfShop::cShoes:
            return "Crusty Sean";
    }
    return "Shopkeeper";
}

void NpcMgrShop::onItemInspected(const ShopItem& item) {
    mState = ShopNpcState::cState_ExplainItem;
    mStateTimer = 0;

    switch (mShopKind) {
        case KindOfShop::cWeapon:
            mDialogueText = "Ah, excellent choice! This weapon packs precision and punch!";
            break;
        case KindOfShop::cHead:
            mDialogueText = "T-that hat looks nice on you... (Hurry up and buy it already!)";
            break;
        case KindOfShop::cClothes:
            mDialogueText = "Très chic! This piece is super popular with all the coolest squids!";
            break;
        case KindOfShop::cShoes:
            mDialogueText = "Those kicks are totally sizzling! Pure style right there!";
            break;
    }
}

void NpcMgrShop::onItemPurchased(const ShopItem& item) {
    mState = ShopNpcState::cState_PurchaseReaction;
    mStateTimer = 0;

    switch (mShopKind) {
        case KindOfShop::cWeapon:
            mDialogueText = "Thanks for your purchase! Treat that weapon with care on the battlefield!";
            break;
        case KindOfShop::cHead:
            mDialogueText = "Thank you so much! (Yeah yeah, now get lost and ink some turf!)";
            break;
        case KindOfShop::cClothes:
            mDialogueText = "Merci beaucoup! Wear it with maximum coolness!";
            break;
        case KindOfShop::cShoes:
            mDialogueText = "Thanks a bunch! Those sneakers are gonna tear up the turf!";
            break;
    }
}

void NpcMgrShop::onItemRejected(bool levelTooLow, bool notEnoughCash) {
    mStateTimer = 0;
    if (levelTooLow) {
        mState = ShopNpcState::cState_LevelTooLow;
        switch (mShopKind) {
            case KindOfShop::cWeapon:
                mDialogueText = "Sorry kiddo, you're not high enough level to wield this weapon yet!";
                break;
            case KindOfShop::cHead:
                mDialogueText = "Um, you need a higher level to wear that... (Come back when you're fresher!)";
                break;
            case KindOfShop::cClothes:
                mDialogueText = "Non non! Your freshness level is not yet high enough for this!";
                break;
            case KindOfShop::cShoes:
                mDialogueText = "Sorry buddy, you gotta level up some more before rocking these!";
                break;
        }
    } else if (notEnoughCash) {
        mState = ShopNpcState::cState_NotEnoughCash;
        switch (mShopKind) {
            case KindOfShop::cWeapon:
                mDialogueText = "Looks like your wallet's a bit light! Win some battles and come back!";
                break;
            case KindOfShop::cHead:
                mDialogueText = "You don't have enough cash... (No money, no gear, pal!)";
                break;
            case KindOfShop::cClothes:
                mDialogueText = "Oh la la! You don't have enough coins for this exquisite gear!";
                break;
            case KindOfShop::cShoes:
                mDialogueText = "Short on cash, huh? Go splash some ink and earn some coins!";
                break;
        }
    }
}

void NpcMgrShop::update() {
    mStateTimer++;
    mAnimFrame++;

    // Blink timer
    mEyeBlinkTimer += 0.016f;
    if (mEyeBlinkTimer > 3.0f) {
        mEyeBlinkTimer = 0.0f;
    }

    // Auto-return to idle after reaction messages
    if (mState == ShopNpcState::cState_PurchaseReaction ||
        mState == ShopNpcState::cState_NotEnoughCash ||
        mState == ShopNpcState::cState_LevelTooLow) {
        if (mStateTimer > 180) { // 3 seconds
            mState = ShopNpcState::cState_Idle;
            mStateTimer = 0;
            mDialogueText = "...";
        }
    }
}

void NpcMgrShop::draw() {
    GambitActor::draw();
}

} // namespace Game
