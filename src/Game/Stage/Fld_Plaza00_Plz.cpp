#include "Game/Stage/Fld_Plaza00_Plz.h"
#include <cstring>

namespace Game {

Fld_Plaza00_Plz::Fld_Plaza00_Plz()
    : mFlags(0),
      mFrameCounter(0),
      mInternalState(0),
      mMode(PlazaMode::cDaytimeStandard),
      mEventState(PlazaEventState::cIdle),
      mJumbotronUpdateCount(0),
      mTvDummyName("PlazaTVDummy"),
      mTopicDummyName("PlazaTopicDummy"),
      mDefaultNightName("DefaultNight"),
      mCommonNightName("CommonNight"),
      mModelSceneName("ModelScene"),
      mEndingPlazaName("EndingPlaza") {
    std::memset(mReserved, 0, sizeof(mReserved));
}

Fld_Plaza00_Plz::~Fld_Plaza00_Plz() {
}

void Fld_Plaza00_Plz::init() {
    GambitActor::init();
    mFlags = 0;
    mFrameCounter = 0;
    mInternalState = 0;
    mMode = PlazaMode::cDaytimeStandard;
    mEventState = PlazaEventState::cIdle;
    mJumbotronUpdateCount = 0;

    vfunc_3();
    vfunc_5();
}

// 0x027E02F0: Decompiled vfunc_3 (Model scene & EndingPlaza init)
void Fld_Plaza00_Plz::vfunc_3() {
    mModelSceneName = "ModelScene";
    mEndingPlazaName = "EndingPlaza";
}

// 0x027E178C: Decompiled vfunc_4 (Splatfest Night setup)
void Fld_Plaza00_Plz::vfunc_4() {
    mFlags |= 0x2; // Bit 1: Splatfest Night active
    mMode = PlazaMode::cSplatfestNight;
    mDefaultNightName = "DefaultNight";
    mCommonNightName = "CommonNight";
}

// 0x027E18C8: Decompiled vfunc_5 (Plaza actor loading & stage lifecycle reset)
void Fld_Plaza00_Plz::vfunc_5() {
    mFlags |= 0x4; // Bit 2: Actors initialized
    mFrameCounter = 0;
    mEventState = PlazaEventState::cActive;
}

// 0x027E0248: Decompiled vfunc_6 (Stage transition trigger)
void Fld_Plaza00_Plz::vfunc_6() {
    mFlags |= 0x8; // Bit 3: Transition flag
}

// 0x027E1B54: Decompiled vfunc_7 (Event flag 0x40)
void Fld_Plaza00_Plz::vfunc_7() {
    mFlags |= 0x40;
}

// 0x027E2AB4: Decompiled vfunc_8 (Event flag 0x80)
void Fld_Plaza00_Plz::vfunc_8() {
    mFlags |= 0x80;
}

// 0x027E1BB0: Decompiled vfunc_9 (Event flag 0x100)
void Fld_Plaza00_Plz::vfunc_9() {
    mFlags |= 0x100;
}

// 0x027E2B10: Decompiled vfunc_10 (Event flag 0x200)
void Fld_Plaza00_Plz::vfunc_10() {
    mFlags |= 0x200;
}

// 0x027E21F8: Decompiled vfunc_11 (Active Plaza simulation tick)
void Fld_Plaza00_Plz::vfunc_11() {
    mFlags |= 0x400; // Bit 10: Active tick processing
    mTvDummyName = "PlazaTVDummy";
    mTopicDummyName = "PlazaTopicDummy";

    // Modulo 30 periodic refresh (2Hz Jumbotron broadcast update)
    if (mFrameCounter % cJumbotronRefreshRate == 0) {
        mJumbotronUpdateCount++;
    }

    mFrameCounter++;
}

// 0x027E249C: Decompiled vfunc_12 (Event state 0xB)
void Fld_Plaza00_Plz::vfunc_12(u32 param) {
    (void)param;
    mFlags |= 0x800;
}

// 0x027E2B6C: Decompiled vfunc_13 (Event state 0xC)
void Fld_Plaza00_Plz::vfunc_13(u32 param) {
    (void)param;
    mFlags |= 0x1000;
}

// 0x027E2BCC: Decompiled vfunc_33 (Plaza exit sequence)
void Fld_Plaza00_Plz::vfunc_33() {
    if (mInternalState == 3) {
        mInternalState = 2;
    }
    mEventState = PlazaEventState::cExiting;
}

void Fld_Plaza00_Plz::setSplatfestNight(bool enable) {
    if (enable) {
        vfunc_4();
    } else {
        mFlags &= ~0x2;
        mMode = PlazaMode::cDaytimeStandard;
    }
}

void Fld_Plaza00_Plz::triggerPlazaExit() {
    vfunc_33();
}

void Fld_Plaza00_Plz::update() {
    vfunc_11();
}

void Fld_Plaza00_Plz::draw() {
    GambitActor::draw();
}

} // namespace Game
