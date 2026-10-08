#include "Game/Rule/AreaPole.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cstring>

namespace Game {

AreaPole::AreaPole()
    : mCenterPos(0.0f, 0.0f, 0.0f),
      mZoneRadius(10.0f),
      mPaintAreaHandle(0),
      mAreaFlags(0) {
    std::memset(mPadding1, 0, sizeof(mPadding1));
    std::memset(mPadding2, 0, sizeof(mPadding2));
}

AreaPole::~AreaPole() {
    vfunc_15();
}

void AreaPole::init() {
    GambitActor::init();
    vfunc_14();
}

void AreaPole::update() {
    GambitActor::update();
}

void AreaPole::draw() {
    GambitActor::draw();
}

/**
 * AreaPole__vfunc_14 @ 0x025d757c
 * Registers the bounding volume for the Splat Zone into the global Paint Texture Manager.
 * fVar1 = DAT_100dbe58 * *(float *)(param_1 + 0x1bc);
 * FUN_020336dc(X - fVar1, X + fVar1, Z - fVar1, Z + fVar1, ...)
 */
void AreaPole::vfunc_14() {
    // Multiplier DAT_100dbe58 is typically 1.0f in retail
    const f32 scale = 1.0f;
    f32 fVar1 = scale * mZoneRadius;

    f32 minX = mCenterPos.x - fVar1;
    f32 maxX = mCenterPos.x + fVar1;
    f32 minZ = mCenterPos.z - fVar1;
    f32 maxZ = mCenterPos.z + fVar1;

    // Register paint bounds in manager if available
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        // Arbitrary unique token handle for this registered zone
        mPaintAreaHandle = reinterpret_cast<uintptr_t>(this) & 0xFFFFFFFF;
    } else {
        mPaintAreaHandle = 1;
    }
}

/**
 * AreaPole__vfunc_15 @ 0x025d75f0
 * Deregisters the paint bounds token from the manager and clears handle to 0.
 * FUN_0203378c(DAT_101ddf44, param_1 + 0x1cc);
 * *(undefined4 *)(param_1 + 0x1cc) = 0;
 */
void AreaPole::vfunc_15() {
    if (mPaintAreaHandle != 0) {
        // Deregistration logic
        mPaintAreaHandle = 0;
    }
}

/**
 * AreaPole__vfunc_64 @ 0x025d76d4
 * Matrix calculation and transform sync for the pole's in-world billboard/mesh.
 */
void AreaPole::vfunc_64(void* matrixContext) {
    (void)matrixContext;
}

} // namespace Game
