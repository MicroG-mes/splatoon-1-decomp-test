#include "Game/MapObj/Obj_BigNamazu.h"
#include <cmath>
#include <algorithm>

namespace Game {

Obj_BigNamazu::Obj_BigNamazu()
    : mPosition(0.0f, 0.0f, 0.0f)
    , mState(BigNamazuState::cState_Wait)
    , mPowerOutputMW(cFullPowerMW)
    , mBreathingScale(1.0f)
    , mTimer(0)
    , mSparksActive(false)
{
    std::fill(mReserved, mReserved + sizeof(mReserved), 0);
}

Obj_BigNamazu::~Obj_BigNamazu() {
}

void Obj_BigNamazu::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mState = BigNamazuState::cState_Wait;
    mPowerOutputMW = cFullPowerMW;
    mBreathingScale = 1.0f;
    mTimer = 0;
    mSparksActive = false;
}

void Obj_BigNamazu::vfunc_3() {
    // 0x02507840: Model loading (Obj_BigNamazu.szs)
}

void Obj_BigNamazu::vfunc_5() {
    // 0x025074ec: Tower apex locator setup (Locater_BigNamazu)
    mState = BigNamazuState::cState_Wait;
}

void Obj_BigNamazu::vfunc_7() {
    // 0x025070a4: Breathing motion & electrical discharge animation
    update();
}

void Obj_BigNamazu::vfunc_9() {
    // 0x025074f0: Restoration sequence when campaign is cleared
    restoreToTower();
}

void Obj_BigNamazu::restoreToTower() {
    mState = BigNamazuState::cState_Start;
    mPowerOutputMW = cFullPowerMW;
    mTimer = 0;
}

void Obj_BigNamazu::setToMissing() {
    mState = BigNamazuState::cState_Missing;
    mPowerOutputMW = 0.0f;
    mBreathingScale = 0.0f;
    mSparksActive = false;
}

void Obj_BigNamazu::update() {
    mTimer++;

    if (mState == BigNamazuState::cState_Missing) {
        return;
    }

    // Breathing harmonic oscillation (120-frame cycle)
    f32 breathPhase = mTimer * (6.2831853f / 120.0f);
    mBreathingScale = 1.0f + std::sin(breathPhase) * 0.04f;

    // Periodic electrical spark discharge
    mSparksActive = (mTimer % cSparkInterval < 8);

    if (mState == BigNamazuState::cState_Start && mTimer >= 30) {
        mState = BigNamazuState::cState_Wait;
    }
}

void Obj_BigNamazu::draw() {
    // Model rendered via ModelSceneMgr
}

} // namespace Game
