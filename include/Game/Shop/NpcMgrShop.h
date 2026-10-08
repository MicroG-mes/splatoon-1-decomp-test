#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "Game/Shop/ShopDef.h"

namespace Game {

enum class ShopNpcState : u32 {
    cState_Idle            = 0, // Wait_Shop_Glng
    cState_Greeting        = 1,
    cState_ExplainItem     = 2,
    cState_PurchaseReaction = 3,
    cState_NotEnoughCash   = 4,
    cState_LevelTooLow     = 5,
    cState_Exit            = 6
};

class NpcMgrShop : public GambitActor {
public:
    NpcMgrShop();
    virtual ~NpcMgrShop() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void changeNpc(KindOfShop kind);
    void onItemInspected(const ShopItem& item);
    void onItemPurchased(const ShopItem& item);
    void onItemRejected(bool levelTooLow, bool notEnoughCash);

    ShopNpcState getState() const { return mState; }
    KindOfShop getShopKind() const { return mShopKind; }
    const char* getNpcName() const;
    const char* getCurrentDialogue() const { return mDialogueText; }

protected:
    KindOfShop mShopKind;
    ShopNpcState mState;
    s32 mStateTimer;
    s32 mAnimFrame;
    f32 mEyeBlinkTimer;

    const char* mDialogueText;
    bool mIsTalkFinished;
    undefined mReserved[0x38];
};

} // namespace Game
