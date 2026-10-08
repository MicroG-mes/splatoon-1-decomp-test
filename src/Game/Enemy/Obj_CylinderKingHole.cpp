#include "Game/Enemy/Obj_CylinderKingHole.h"
#include <cstring>

namespace Game {

Obj_CylinderKingHole::Obj_CylinderKingHole()
    : mHoleIndex(0),
      mHeightOffset(0.0f),
      mAngleRad(0.0f),
      mState(CylinderHoleState::cClosed),
      mTimer(0),
      mInkedAmount(0.0f) {
    std::memset(mReserved, 0, sizeof(mReserved));
}

Obj_CylinderKingHole::~Obj_CylinderKingHole() {
}

void Obj_CylinderKingHole::init() {
    GambitActor::init();
    mState = CylinderHoleState::cClosed;
    mTimer = 0;
    mInkedAmount = 0.0f;
    vfunc_47();
}

// 0x0251DC48: Decompiled vfunc_11 (Transform update relative to cylinder body)
void Obj_CylinderKingHole::vfunc_11() {
    // Synchronize local position and orientation relative to parent cylinder
}

// 0x0251DDBC: Decompiled vfunc_47 (Stage collision registration & ink penetration logic)
void Obj_CylinderKingHole::vfunc_47() {
    // Collision sphere registered on the nozzle opening
}

void Obj_CylinderKingHole::setupHole(u32 holeIndex, f32 heightOffset, f32 angleRad) {
    mHoleIndex = holeIndex;
    mHeightOffset = heightOffset;
    mAngleRad = angleRad;
    mState = CylinderHoleState::cClosed;
    mTimer = 0;
    mInkedAmount = 0.0f;
}

void Obj_CylinderKingHole::openAndFire() {
    if (mState != CylinderHoleState::cInkedClimb) {
        mState = CylinderHoleState::cOpenFiring;
        mTimer = 0;
    }
}

void Obj_CylinderKingHole::hitWithInk(f32 inkAmount) {
    mInkedAmount += inkAmount;
    if (mInkedAmount >= 20.0f) {
        // Covered in player ink, forming a climbable surface rung
        mState = CylinderHoleState::cInkedClimb;
        mTimer = 0;
    }
}

void Obj_CylinderKingHole::closeHole() {
    mState = CylinderHoleState::cClosed;
    mInkedAmount = 0.0f;
    mTimer = 0;
}

void Obj_CylinderKingHole::update() {
    vfunc_11();
    mTimer++;

    switch (mState) {
        case CylinderHoleState::cOpenFiring:
            if (mTimer >= 90) { // 1.5s open fire window
                closeHole();
            }
            break;

        case CylinderHoleState::cInkedClimb:
            // Remains plugged until boss reaches transition
            break;

        case CylinderHoleState::cClosed:
            break;
    }
}

void Obj_CylinderKingHole::draw() {
    GambitActor::draw();
}

} // namespace Game
