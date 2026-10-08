#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "Game/Shop/ShopDef.h"
#include "Game/Shop/NpcMgrShop.h"
#include "Game/Shop/PlayerMgrShop.h"
#include "cafe/vpad.h"
#include <vector>

namespace Game {

enum class ShopUiMode : u32 {
    Browsing       = 0,
    ConfirmBuy     = 1,
    EquipPrompt    = 2,
    TestFirePrompt = 3,
    Leaving        = 4
};

class LytShopHandler : public GambitActor {
public:
    LytShopHandler();
    virtual ~LytShopHandler() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void handleInput(const VPADStatus& vpad);
    void openShop(KindOfShop kind);
    void closeShop();

    void switchShop(s32 direction); // -1 = Prev (L), +1 = Next (R)

    bool isLeaving() const { return mUiMode == ShopUiMode::Leaving; }

protected:
    void populateStock();
    void tryBuyCurrentItem();
    void confirmPurchase();
    void confirmEquip();

    KindOfShop mCurrentShop;
    ShopUiMode mUiMode;

    NpcMgrShop mNpcMgr;
    PlayerMgrShop mPlayerMgr;

    std::vector<ShopItem> mStock;
    s32 mSelectedIndex;

    s32 mCursorAnimTimer;
    bool mShowConfirmationDialog;
    undefined mReserved[0x38];
};

} // namespace Game
