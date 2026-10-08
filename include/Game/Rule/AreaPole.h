#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * AreaPole
 * In-game Splat Zone objective marker pole actor.
 * Manages zone ink area bounds registration and capture status visual pole.
 *
 * Address: vtable @ 0x100D5D60
 * Real PowerPC methods:
 *   vfunc_3  @ 0x025d74f4
 *   vfunc_14 @ 0x025d757c - Register zone bounds in paint texture manager
 *   vfunc_15 @ 0x025d75f0 - Unregister zone bounds
 *   vfunc_64 @ 0x025d76d4 - Matrix orientation update
 */
class AreaPole : public GambitActor {
public:
    AreaPole();
    virtual ~AreaPole() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Vtable slot overrides directly matched with Ghidra
    virtual void vfunc_14();
    virtual void vfunc_15();
    virtual void vfunc_64(void* matrixContext);

    void setZoneRadius(f32 radius) { mZoneRadius = radius; }
    f32 getZoneRadius() const { return mZoneRadius; }
    u32 getPaintAreaHandle() const { return mPaintAreaHandle; }
    bool isRegistered() const { return mPaintAreaHandle != 0; }

protected:
    // Struct layout aligned to Espresso PowerPC offsets:
    // +0x78: Center X
    // +0x7C: Center Y
    // +0x80: Center Z
    sead::Vector3f mCenterPos;     // 0x78 - 0x84

    undefined mPadding1[0x198];    // 0x84 - 0x1BC

    f32 mZoneRadius;               // 0x1BC - Zone capture radius
    undefined mPadding2[0xC];      // 0x1C0 - 0x1CC
    u32 mPaintAreaHandle;          // 0x1CC - Paint registry token/handle
    u32 mAreaFlags;                // 0x1D0 - Capture flags
};

} // namespace Game
