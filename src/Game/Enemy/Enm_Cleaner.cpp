#include "Game/Enemy/Enm_Cleaner.h"
#include <cmath>
#include <algorithm>

namespace Game {

Enm_Cleaner::Enm_Cleaner()
    : mPosition(0.0f, 0.0f, 0.0f)
    , mMoveDirection(1.0f, 0.0f, 0.0f)
    , mState(CleanerState::cState_Patrol)
    , mHealth(cMaxHealth)
    , mTotalCleaned(0.0f)
    , mBrushAngle(0.0f)
    , mTurnTimer(0)
{
    std::fill(mReserved, mReserved + sizeof(mReserved), 0);
}

Enm_Cleaner::~Enm_Cleaner() {
}

void Enm_Cleaner::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mMoveDirection.set(1.0f, 0.0f, 0.0f);
    mState = CleanerState::cState_Patrol;
    mHealth = cMaxHealth;
    mTotalCleaned = 0.0f;
    mBrushAngle = 0.0f;
    mTurnTimer = 0;
}

void Enm_Cleaner::vfunc_3() {
    // 0x022d4ce0: Model loading (Enm_Cleaner.szs, 1,339 vertices)
}

void Enm_Cleaner::vfunc_5() {
    // 0x022d5168: Parameter loading
    mHealth = cMaxHealth;
}

void Enm_Cleaner::vfunc_7() {
    // 0x022d9670: Vacuum sweep physics
    update();
}

void Enm_Cleaner::vfunc_11() {
    // 0x022d97fc: Bump bounce & turnaround
    bumpWall();
}

void Enm_Cleaner::vfunc_37() {
    // 0x022d9cbc: Squeegee brush rotation
    mBrushAngle += 24.0f;
    if (mBrushAngle >= 360.0f) {
        mBrushAngle -= 360.0f;
    }
}

void Enm_Cleaner::vfunc_47() {
    // 0x022daff4: Armor deflection
}

void Enm_Cleaner::spawn(const sead::Vector3f& pos, const sead::Vector3f& moveDir) {
    mPosition = pos;
    mMoveDirection = moveDir;
    mState = CleanerState::cState_Patrol;
    mHealth = cMaxHealth;
    mTotalCleaned = 0.0f;
    mBrushAngle = 0.0f;
    mTurnTimer = 0;
}

f32 Enm_Cleaner::absorbInk(f32 amount) {
    mTotalCleaned += amount;
    mState = CleanerState::cState_Clean;
    vfunc_37();
    return amount;
}

void Enm_Cleaner::bumpWall() {
    mMoveDirection.x = -mMoveDirection.x;
    mMoveDirection.z = -mMoveDirection.z;
    mState = CleanerState::cState_Turn;
    mTurnTimer = 15; // 15 frames turnaround delay
}

bool Enm_Cleaner::hitWithInk(f32 damage, bool& outArmorDeflected) {
    (void)damage;
    // Squee-G has high armor plating, deflecting standard ink shots
    outArmorDeflected = true;
    vfunc_47();
    return false;
}

void Enm_Cleaner::update() {
    vfunc_37();

    if (mState == CleanerState::cState_Patrol || mState == CleanerState::cState_Clean) {
        mPosition.x += mMoveDirection.x * cMoveSpeed;
        mPosition.z += mMoveDirection.z * cMoveSpeed;

        if (mState == CleanerState::cState_Clean) {
            // Settle back to patrol after clearing ink
            mState = CleanerState::cState_Patrol;
        }
    } else if (mState == CleanerState::cState_Turn) {
        if (--mTurnTimer <= 0) {
            mState = CleanerState::cState_Patrol;
        }
    }
}

void Enm_Cleaner::draw() {
    // Model rendered via ModelSceneMgr
}

} // namespace Game
