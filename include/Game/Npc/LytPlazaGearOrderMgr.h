#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "Game/Shop/ShopDef.h"
#include "cafe/vpad.h"
#include <vector>

namespace Game {

enum class SpykeState : u32 {
    cSleep          = 0, // State::cSleep (listening to headphones, resting)
    cInNPCStatus    = 1, // State::cInNPCStatus (talking to player)
    cCheckOrder     = 2, // Checking pending delivery
    cRerollMenu     = 3, // Selecting gear to reroll or add slot
    cRerollRollAnim = 4, // Roulette slot rolling animation
    cPurchaseOrder  = 5, // Buying delivered gear
    cGoodBye        = 6  // Leaving conversation
};

struct GearOrder {
    u32 gearId;
    KindOfShop kind;
    BrandId brand;
    AbilityId mainAbility;
    AbilityId subAbilities[3];
    u32 price;
    bool isReadyForPickup;
};

class LytPlazaGearOrderMgr : public GambitActor {
public:
    LytPlazaGearOrderMgr();
    virtual ~LytPlazaGearOrderMgr() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void handleInput(const VPADStatus& vpad);

    void talkToSpyke();
    void leaveSpyke();

    bool placeOrder(const GearOrder& order);
    bool cancelOrder(u32 orderIndex);

    bool canRerollGear(u32 starCount) const;
    void executeReroll(u32 gearId, KindOfShop kind, BrandId brand, AbilityId subAbilitiesOut[3]);
    void executeAddSlot(u32 gearId, KindOfShop kind, BrandId brand, AbilityId& newAbilityOut);

    SpykeState getState() const { return mState; }
    const char* getDialogue() const { return mDialogueText; }
    u32 getOrderCount() const { return static_cast<u32>(mOrders.size()); }

protected:
    AbilityId rollSubAbilityWithBrandWeight(BrandId brand);

    SpykeState mState;
    s32 mStateTimer;
    s32 mRerollAnimFrame;

    std::vector<GearOrder> mOrders; // Up to 3 pending orders
    u32 mSelectedOrderIndex;

    const char* mDialogueText;
    bool mHasSnails;
    u32 mRerollCostCash;

    undefined mReserved[0x38];
};

} // namespace Game
