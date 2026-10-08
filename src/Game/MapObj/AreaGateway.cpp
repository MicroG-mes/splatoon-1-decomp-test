#include "Game/MapObj/AreaGateway.h"

namespace Game {

AreaGateway::AreaGateway()
    : mPosition(0.0f, 0.0f, 0.0f),
      mSectionId(0),
      mState(GatewayState::cLocked),
      mTimer(0),
      mDoorCurrentHeight(0.0f),
      mHasPlayerPassed(false) {
}

AreaGateway::~AreaGateway() {
}

void AreaGateway::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mSectionId = 0;
    mState = GatewayState::cLocked;
    mTimer = 0;
    mDoorCurrentHeight = 0.0f;
    mHasPlayerPassed = false;
}

void AreaGateway::setupGateway(const sead::Vector3f& pos, u32 sectionId) {
    mPosition = pos;
    mSectionId = sectionId;
    mState = GatewayState::cLocked;
    mDoorCurrentHeight = 0.0f;
    mHasPlayerPassed = false;
}

void AreaGateway::unlockGate() {
    if (mState == GatewayState::cLocked) {
        mState = GatewayState::cUnlocking;
        mTimer = 0;
    }
}

void AreaGateway::checkPlayerPassage(const sead::Vector3f& playerPos) {
    if (mState != GatewayState::cOpenPassage || mHasPlayerPassed) {
        return;
    }

    // Check if player passed through gate
    f32 dz = playerPos.z - mPosition.z;
    if (dz > 2.0f) {
        mHasPlayerPassed = true;
        mState = GatewayState::cLockedBehind;
        mTimer = 0;
    }
}

void AreaGateway::update() {
    mTimer++;

    switch (mState) {
        case GatewayState::cUnlocking:
            mDoorCurrentHeight += 0.10f;
            if (mDoorCurrentHeight >= cDoorMaxHeight) {
                mDoorCurrentHeight = cDoorMaxHeight;
                mState = GatewayState::cOpenPassage;
                mTimer = 0;
            }
            break;

        case GatewayState::cLockedBehind:
            mDoorCurrentHeight -= 0.15f;
            if (mDoorCurrentHeight <= 0.0f) {
                mDoorCurrentHeight = 0.0f;
            }
            break;

        case GatewayState::cOpenPassage:
        case GatewayState::cLocked:
        default:
            break;
    }
}

void AreaGateway::draw() {
    GambitActor::draw();
}

} // namespace Game
