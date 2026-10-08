#include "Game/Weapon/PlayerWeaponBase.h"
#include <cstring>
#include <cassert>

namespace Game {

PlayerWeaponBase::PlayerWeaponBase()
    : mWeaponContextPtr(nullptr),
      mActionState(0),
      mActionVtable(nullptr) {
    std::memset(mPaddingBase, 0, sizeof(mPaddingBase));
    std::memset(mPaddingAction, 0, sizeof(mPaddingAction));
}

PlayerWeaponBase::~PlayerWeaponBase() {
}

void PlayerWeaponBase::init() {
    GambitActor::init();
    mWeaponContextPtr = nullptr;
    mActionState = 0;
    mActionVtable = nullptr;
}

void PlayerWeaponBase::update() {
    GambitActor::update();
}

void PlayerWeaponBase::draw() {
    GambitActor::draw();
}

/**
 * PlayerWeapon__vfunc_35
 * Address: 0x026d73e4 / 0x026d7774 / 0x026d7b04 / 0x026da730
 * PowerPC instructions:
 *   lwz r12, 0x1ac(r31)
 *   lwz r0,  0x14(r12)
 *   mtspr CTR, r0
 *   addi r3, r31, 0x1a8
 *   bctrl
 *   lwz r3, 0x1a8(r31)
 *   blr
 */
u32 PlayerWeaponBase::vfunc_35() {
    if (mActionVtable) {
        typedef void (*ActionFunc)(void*);
        // Call virtual function at offset +0x14
        ActionFunc* vtable = reinterpret_cast<ActionFunc*>(mActionVtable);
        ActionFunc func = vtable[5]; // +0x14 / 4 = 5
        if (func) {
            func(&mActionState);
        }
    }
    return mActionState;
}

/**
 * PlayerWeapon__vfunc_70
 * Address: 0x026d7424 / 0x026d77b4 / 0x026d7b44 / 0x026da770
 * PowerPC instructions:
 *   cmpwi cr0, r4, 0
 *   bne .valid
 *   li r3, 4
 *   bl FUN_028cd9c0 ; assert
 * .valid:
 *   lwz r5, 0x18c(r3)
 *   stw r5, 0x0(r4)
 *   blr
 */
void PlayerWeaponBase::vfunc_70(u32* outWeaponContext) {
    if (!outWeaponContext) {
        // Assert handler at 0x028cd9c0 with code 4
        assert(outWeaponContext != nullptr);
        return;
    }
    *outWeaponContext = reinterpret_cast<uintptr_t>(mWeaponContextPtr) & 0xFFFFFFFF;
}

} // namespace Game
