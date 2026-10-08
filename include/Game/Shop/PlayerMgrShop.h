#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "Game/Shop/ShopDef.h"
#include "Game/System/SaveDataMgr.h"

namespace Game {

enum class PlayerShopState : u32 {
    cState_Idle         = 0,
    cState_ChangeGear   = 1, // Wait_Shop_Change_Shs_M
    cState_PoseSuccess  = 2,
    cState_ReturnNormal = 3
};

class PlayerMgrShop : public GambitActor {
public:
    PlayerMgrShop();
    virtual ~PlayerMgrShop() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setShopKind(KindOfShop kind);
    void previewItem(const ShopItem& item);
    void resetToEquipped();
    void confirmEquip(const ShopItem& item);

    void rotatePreview(f32 deltaYaw);
    f32 getPreviewYaw() const { return mPreviewYaw; }

    PlayerShopState getState() const { return mState; }
    const InklingCustomization& getPreviewCustomization() const { return mPreviewCustom; }

protected:
    KindOfShop mShopKind;
    PlayerShopState mState;
    s32 mStateTimer;

    f32 mPreviewYaw;
    f32 mTargetCameraDistance;
    f32 mCurrentCameraDistance;
    f32 mCameraHeight;

    InklingCustomization mEquippedCustom;
    InklingCustomization mPreviewCustom;

    bool mIsPreviewing;
    undefined mReserved[0x30];
};

} // namespace Game
