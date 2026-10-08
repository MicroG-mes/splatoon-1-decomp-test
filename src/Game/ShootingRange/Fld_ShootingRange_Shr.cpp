#include "Game/ShootingRange/Fld_ShootingRange_Shr.h"
#include "Game/Paint/PaintTextureMgr.h"

namespace Game {

Fld_ShootingRange_Shr::Fld_ShootingRange_Shr()
    : mState(ShootingRangeState::cWaitSceneFade),
      mStateTimer(0) {
}

Fld_ShootingRange_Shr::~Fld_ShootingRange_Shr() {
}

void Fld_ShootingRange_Shr::init() {
    GambitActor::init();

    // 0: Short range dummy (2.0 lines)
    mTargets[0].init();
    mTargets[0].setup(sead::Vector3f(0.0f, 0.0f, 10.0f), 100.0f, 0, false);

    // 1: Medium range dummy (3.5 lines)
    mTargets[1].init();
    mTargets[1].setup(sead::Vector3f(-6.0f, 0.0f, 17.5f), 100.0f, 0, false);

    // 2: Long range dummy (5.0 lines)
    mTargets[2].init();
    mTargets[2].setup(sead::Vector3f(6.0f, 0.0f, 25.0f), 100.0f, 0, false);

    // 3: Moving target dummy
    mTargets[3].init();
    mTargets[3].setup(sead::Vector3f(0.0f, 1.5f, 20.0f), 100.0f, 0, true);

    // 4: Fortified dummy (3 Defense Up)
    mTargets[4].init();
    mTargets[4].setup(sead::Vector3f(0.0f, 0.0f, 15.0f), 100.0f, 3, false);

    mState = ShootingRangeState::cProgress;
}

void Fld_ShootingRange_Shr::resetAllInk() {
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        paint->clearBuffer();
    }
}

void Fld_ShootingRange_Shr::exitToShop() {
    mState = ShootingRangeState::cExitToShop;
}

SighterTarget* Fld_ShootingRange_Shr::getTarget(u32 index) {
    if (index < 5) {
        return &mTargets[index];
    }
    return nullptr;
}

void Fld_ShootingRange_Shr::update() {
    mStateTimer++;

    if (mState == ShootingRangeState::cProgress) {
        for (int i = 0; i < 5; ++i) {
            mTargets[i].update();
        }
    }
}

void Fld_ShootingRange_Shr::draw() {
    GambitActor::draw();
    if (mState == ShootingRangeState::cProgress) {
        for (int i = 0; i < 5; ++i) {
            mTargets[i].draw();
        }
    }
}

} // namespace Game
