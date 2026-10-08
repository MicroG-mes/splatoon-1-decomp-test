#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * Obj_PlazaPost (Inkopolis Plaza Miiverse Post Box / Mailbox Actor)
 * Decompiled from PowerPC retail executable Gambit.elf:
 * Vtable @ 0x100C81F0
 *
 * Authentic PowerPC Methods:
 *   vfunc_11 @ 0x02580778: 3x4 affine transform matrix synchronization
 *   vfunc_47 @ 0x02582260: Stage collision mesh registration
 */
class Obj_PlazaPost : public GambitActor {
public:
    Obj_PlazaPost();
    virtual ~Obj_PlazaPost() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic Ghidra Decompiled vfuncs
    virtual void vfunc_11(); // 0x02580778: Matrix sync
    virtual void vfunc_47(); // 0x02582260: Collision register

    bool checkPlayerProximity(const sead::Vector3f& playerPos, f32 interactRadius = 2.5f);
    void triggerOpenMailbox();
    void closeMailbox();

    bool isNearby() const { return mIsPlayerNearby; }
    bool isOpen() const { return mIsOpen; }
    const sead::Vector3f& getPosition() const { return mPosition; }

protected:
    sead::Vector3f mPosition;
    f32 mTransformMatrix[3][4]; // 0x54 - 0x80: 3x4 affine transform matrix
    f32 mRenderMatrix[3][4];    // 0x1F8 - 0x220: duplicated render matrix

    f32 mInteractRadius;
    bool mIsPlayerNearby;
    bool mIsOpen;

    u8 mReserved[0x38];
};

} // namespace Game
