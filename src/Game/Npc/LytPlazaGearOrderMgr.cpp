#include "Game/Npc/LytPlazaGearOrderMgr.h"
#include "Game/System/SaveDataMgr.h"

namespace Game {

LytPlazaGearOrderMgr::LytPlazaGearOrderMgr()
    : mState(SpykeState::cSleep),
      mStateTimer(0),
      mRerollAnimFrame(0),
      mSelectedOrderIndex(0),
      mDialogueText("...Hmm? Whaddaya want, squirt?"),
      mHasSnails(false),
      mRerollCostCash(30000) {
}

LytPlazaGearOrderMgr::~LytPlazaGearOrderMgr() {
}

void LytPlazaGearOrderMgr::init() {
    GambitActor::init();
    mState = SpykeState::cSleep;
    mDialogueText = "...";
}

void LytPlazaGearOrderMgr::talkToSpyke() {
    mState = SpykeState::cInNPCStatus;
    mStateTimer = 0;

    SaveDataMgr* save = SaveDataMgr::instance();
    if (save) {
        mHasSnails = (save->getStats().superSeaSnails > 0);
    }

    if (!mOrders.empty() && mOrders[0].isReadyForPickup) {
        mDialogueText = "Oi, that order ya asked for came in. Ya got the cash ready?";
        mState = SpykeState::cCheckOrder;
    } else {
        mDialogueText = "Looking for something special? I can track down gear or tweak your abilities for a price.";
    }
}

void LytPlazaGearOrderMgr::leaveSpyke() {
    mState = SpykeState::cSleep;
    mDialogueText = "...Keep it shady.";
}

bool LytPlazaGearOrderMgr::placeOrder(const GearOrder& order) {
    if (mOrders.size() >= 3) {
        return false; // Queue full
    }
    mOrders.push_back(order);
    return true;
}

bool LytPlazaGearOrderMgr::cancelOrder(u32 orderIndex) {
    if (orderIndex < mOrders.size()) {
        mOrders.erase(mOrders.begin() + orderIndex);
        return true;
    }
    return false;
}

bool LytPlazaGearOrderMgr::canRerollGear(u32 starCount) const {
    SaveDataMgr* save = SaveDataMgr::instance();
    if (!save) return false;

    // Must be level 20 or higher to access rerolling
    if (save->getStats().level < 20) {
        return false;
    }

    // Must have at least 1 Super Sea Snail or 30,000 cash
    if (save->getStats().superSeaSnails == 0 && save->getStats().money < mRerollCostCash) {
        return false;
    }

    return true;
}

AbilityId LytPlazaGearOrderMgr::rollSubAbilityWithBrandWeight(BrandId brand) {
    // Brand affinity tables: favoured ability has ~5x weight, unfavoured has ~0.5x weight
    AbilityId favored = AbilityId::None;
    switch (brand) {
        case BrandId::cSquidForce: favored = AbilityId::DamageUp; break;
        case BrandId::cTentatek:    favored = AbilityId::InkRecoveryUp; break;
        case BrandId::cRockenberg: favored = AbilityId::RunSpeedUp; break;
        case BrandId::cKrakOn:     favored = AbilityId::SwimSpeedUp; break;
        case BrandId::cSplashMob:  favored = AbilityId::InkSaverMain; break;
        case BrandId::cFirefin:    favored = AbilityId::InkSaverSub; break;
        case BrandId::cTakoroka:   favored = AbilityId::SpecialChargeUp; break;
        case BrandId::cZekko:      favored = AbilityId::SpecialSaver; break;
        default:                   favored = AbilityId::DamageUp; break;
    }

    // Pseudo-random selection simulation
    static u32 sSeed = 1337;
    sSeed = sSeed * 1103515245 + 12345;
    u32 roll = (sSeed >> 16) % 100;

    if (roll < 28) {
        return favored; // 28% chance for favoured brand ability
    } else {
        u32 genericIndex = roll % 13;
        return static_cast<AbilityId>(genericIndex);
    }
}

void LytPlazaGearOrderMgr::executeReroll(u32 gearId, KindOfShop kind, BrandId brand, AbilityId subAbilitiesOut[3]) {
    SaveDataMgr* save = SaveDataMgr::instance();
    if (!save) return;

    if (save->getStats().superSeaSnails > 0) {
        // Deduct 1 Super Sea Snail
        // SaveDataMgr handles stats
    } else if (save->getStats().money >= mRerollCostCash) {
        save->addMoney(-mRerollCostCash);
    } else {
        return;
    }

    mState = SpykeState::cRerollRollAnim;
    mStateTimer = 0;

    for (int i = 0; i < 3; ++i) {
        subAbilitiesOut[i] = rollSubAbilityWithBrandWeight(brand);
    }
}

void LytPlazaGearOrderMgr::executeAddSlot(u32 gearId, KindOfShop kind, BrandId brand, AbilityId& newAbilityOut) {
    SaveDataMgr* save = SaveDataMgr::instance();
    if (!save) return;

    if (save->getStats().superSeaSnails > 0) {
        // Deduct 1 Snail
    } else if (save->getStats().money >= mRerollCostCash) {
        save->addMoney(-mRerollCostCash);
    } else {
        return;
    }

    newAbilityOut = rollSubAbilityWithBrandWeight(brand);
}

void LytPlazaGearOrderMgr::handleInput(const VPADStatus& vpad) {
    switch (mState) {
        case SpykeState::cInNPCStatus:
            if (vpad.trigger & VPAD_BUTTON_B) {
                leaveSpyke();
            } else if (vpad.trigger & VPAD_BUTTON_A) {
                mState = SpykeState::cRerollMenu;
            }
            break;

        case SpykeState::cRerollMenu:
            if (vpad.trigger & VPAD_BUTTON_B) {
                mState = SpykeState::cInNPCStatus;
            }
            break;

        default:
            break;
    }
}

void LytPlazaGearOrderMgr::update() {
    mStateTimer++;

    if (mState == SpykeState::cRerollRollAnim) {
        mRerollAnimFrame++;
        if (mRerollAnimFrame > 90) { // 1.5 seconds rolling animation
            mRerollAnimFrame = 0;
            mState = SpykeState::cInNPCStatus;
            mDialogueText = "Heh, there ya go! Fresh new abilities just for you.";
        }
    }
}

void LytPlazaGearOrderMgr::draw() {
    GambitActor::draw();
}

} // namespace Game
