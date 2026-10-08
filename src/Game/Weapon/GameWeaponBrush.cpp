#include "Game/Weapon/GameWeaponBrush.h"
#include "Game/Paint/PaintTextureMgr.h"

namespace Game {

GameWeaponBrush::GameWeaponBrush()
    : mState(BrushState::cIdle),
      mStateTimer(0),
      mSwipeCounter(0),
      mSprintSpeed(1.92f),
      mSwipeDamage(28.0f),
      mInkCostSwipe(0.02f),
      mInkCostSprintPerFrame(0.001f) {
}

GameWeaponBrush::~GameWeaponBrush() {
}

void GameWeaponBrush::init() {
    GambitActor::init();
    mState = BrushState::cIdle;
}

void GameWeaponBrush::triggerSwipe() {
    mState = BrushState::cSwiping;
    mStateTimer = 0;
    mSwipeCounter++;

    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        paint->splatInk(sead::Vector3f(0.0f, 0.0f, 3.5f), 1.2f, 0);
    }
}

void GameWeaponBrush::startSprint() {
    mState = BrushState::cSprint;
    mStateTimer = 0;
}

void GameWeaponBrush::stopSprint() {
    if (mState == BrushState::cSprint) {
        mState = BrushState::cIdle;
        mStateTimer = 0;
    }
}

void GameWeaponBrush::update() {
    mStateTimer++;

    switch (mState) {
        case BrushState::cSwiping:
            if (mStateTimer >= 6) { // Fast 6-frame recovery
                mState = BrushState::cIdle;
            }
            break;

        case BrushState::cSprint: {
            PaintTextureMgr* paint = PaintTextureMgr::instance();
            if (paint) {
                // Narrow sprint ink line
                paint->splatInk(sead::Vector3f(0.0f, 0.0f, 0.0f), 0.4f, 0);
            }
            break;
        }

        default:
            break;
    }
}

void GameWeaponBrush::draw() {
    GambitActor::draw();
}

} // namespace Game
