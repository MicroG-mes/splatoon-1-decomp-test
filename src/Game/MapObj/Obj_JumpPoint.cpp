#include "Game/MapObj/Obj_JumpPoint.h"
#include <cmath>
#include <algorithm>

namespace Game {

Obj_JumpPoint::Obj_JumpPoint()
    : mPosition(0.0f, 0.0f, 0.0f)
    , mState(JumpPointState::cState_Idle)
    , mDurability(cMaxLife)
    , mTeamId(0)
    , mTimer(0)
    , mJumpsCount(0)
    , mRadarActive(false)
{
    std::fill(mReserved, mReserved + sizeof(mReserved), 0);
}

Obj_JumpPoint::~Obj_JumpPoint() {
}

void Obj_JumpPoint::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mState = JumpPointState::cState_Idle;
    mDurability = cMaxLife;
    mTeamId = 0;
    mTimer = 0;
    mJumpsCount = 0;
    mRadarActive = false;
}

void Obj_JumpPoint::vfunc_3() {
    // 0x025f2a24: Model loading (Obj_JumpPoint.szs)
}

void Obj_JumpPoint::vfunc_5() {
    // 0x025f2b1c: Parameter initialization (Obj_JumpPoint.params)
    mDurability = cMaxLife;
}

void Obj_JumpPoint::vfunc_7() {
    // 0x025f2e64: Radar pulse update
    update();
}

void Obj_JumpPoint::vfunc_9() {
    // 0x025f2f2c: Signal attention beacon broadcast
}

void Obj_JumpPoint::vfunc_11() {
    // 0x025f30a8: Super jump landing callback
    onSuperJumpLanded();
}

void Obj_JumpPoint::deploy(const sead::Vector3f& pos, u32 teamId) {
    mPosition = pos;
    mTeamId = teamId;
    mDurability = cMaxLife;
    mState = JumpPointState::cState_Idle;
    mTimer = 0;
    mJumpsCount = 0;
    mRadarActive = false;
}

bool Obj_JumpPoint::onSuperJumpLanded() {
    if (!isActive()) {
        return false;
    }

    mJumpsCount++;
    mDurability -= 1.0f; // Each jump consumes 1 use

    if (mDurability <= 0.0f) {
        mDurability = 0.0f;
        mState = JumpPointState::cState_Break;
        mRadarActive = false;
    }

    return true;
}

bool Obj_JumpPoint::takeDamage(f32 damage) {
    if (!isActive()) {
        return false;
    }

    // Direct damage decreases durability
    mDurability -= (damage / 10.0f);

    if (mDurability <= 0.0f) {
        mDurability = 0.0f;
        mState = JumpPointState::cState_Break;
        mRadarActive = false;
        return true;
    }

    return false;
}

void Obj_JumpPoint::update() {
    mTimer++;

    if (!isActive()) {
        return;
    }

    // Periodic sonar radar pulse
    mRadarActive = (mTimer % cRadarInterval < 10);
}

void Obj_JumpPoint::draw() {
    // Model rendered via ModelSceneMgr
}

} // namespace Game
