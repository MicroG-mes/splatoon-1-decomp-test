#include "Game/Bullet/TimerTrap.h"
#include <cstring>
#include <cmath>

namespace Game {

TimerTrap::TimerTrap()
    : mPosition(0.0f, 0.0f, 0.0f),
      mTeamId(0),
      mOwnerPlayerId(0),
      mState(TrapState::cPlacing),
      mFuseTimer(0),
      mProximityTrip(false) {
    std::memset(mTransformMatrix, 0, sizeof(mTransformMatrix));
    std::memset(mReserved, 0, sizeof(mReserved));

    mTransformMatrix[0][0] = 1.0f;
    mTransformMatrix[1][1] = 1.0f;
    mTransformMatrix[2][2] = 1.0f;
}

TimerTrap::~TimerTrap() {
}

void TimerTrap::init() {
    GambitActor::init();
    mState = TrapState::cArmed;
    mFuseTimer = 0;
    mProximityTrip = false;
    vfunc_47();
}

// 0x025E15C8: Decompiled vfunc_7 (Proximity sensing & fuse countdown)
void TimerTrap::vfunc_7() {
    if (mState == TrapState::cTriggered) {
        mFuseTimer++;
        if (mFuseTimer >= cFuseFrames) {
            vfunc_15(); // Detonate!
        }
    }
}

// 0x025E1B1C: Decompiled vfunc_11 (Transform matrix sync)
void TimerTrap::vfunc_11() {
    mTransformMatrix[0][3] = mPosition.x;
    mTransformMatrix[1][3] = mPosition.y;
    mTransformMatrix[2][3] = mPosition.z;
}

// 0x025E1BF4: Decompiled vfunc_15 (Detonation & blast explosion)
void TimerTrap::vfunc_15() {
    mState = TrapState::cExploding;
    mFuseTimer = 0;
    mProximityTrip = false;
}

// 0x025E3880: Decompiled vfunc_47 (Surface collision mesh registration)
void TimerTrap::vfunc_47() {
    // Registered on terrain surface
}

void TimerTrap::plantTrap(const sead::Vector3f& pos, u32 teamId, u32 ownerPlayerId) {
    mPosition = pos;
    mTeamId = teamId;
    mOwnerPlayerId = ownerPlayerId;
    mState = TrapState::cArmed;
    mFuseTimer = 0;
    mProximityTrip = false;
    vfunc_11();
}

bool TimerTrap::checkEnemyProximity(const sead::Vector3f& enemyPos, u32 enemyTeamId) {
    if (mState != TrapState::cArmed || enemyTeamId == mTeamId) {
        return false;
    }

    f32 dx = enemyPos.x - mPosition.x;
    f32 dy = enemyPos.y - mPosition.y;
    f32 dz = enemyPos.z - mPosition.z;
    f32 distSq = dx * dx + dy * dy + dz * dz;

    if (distSq <= cTriggerRadius * cTriggerRadius) {
        triggerDetonation();
        return true;
    }
    return false;
}

void TimerTrap::triggerDetonation() {
    if (mState == TrapState::cArmed) {
        mState = TrapState::cTriggered;
        mProximityTrip = true;
        mFuseTimer = 0;
    }
}

void TimerTrap::update() {
    vfunc_11();
    vfunc_7();
}

void TimerTrap::draw() {
    GambitActor::draw();
}

} // namespace Game
