#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class PlazaMode : u32 {
    cDaytimeStandard = 0,
    cSplatfestNight  = 1,
    cEndingSequence  = 2
};

enum class PlazaEventState : u32 {
    cIdle       = 0,
    cIntroNews  = 1,
    cActive     = 2,
    cExiting    = 3
};

/**
 * Fld_Plaza00_Plz (Inkopolis Plaza Stage Manager / Field Controller)
 * Decompiled from PowerPC retail executable Gambit.elf:
 * Vtable @ 0x10065050
 *
 * Authentic PowerPC Methods:
 *   vfunc_3  @ 0x027E02F0: Model scene setup & EndingPlaza initialization
 *   vfunc_4  @ 0x027E178C: Splatfest Night mode activation (DefaultNight/CommonNight)
 *   vfunc_5  @ 0x027E18C8: Plaza actor loading & stage lifecycle reset
 *   vfunc_6  @ 0x027E0248: Camera transition stage trigger (flag | 0x8)
 *   vfunc_7  @ 0x027E1B54: Event transition state 6 (flag | 0x40)
 *   vfunc_8  @ 0x027E2AB4: Event transition state 7 (flag | 0x80)
 *   vfunc_9  @ 0x027E1BB0: Event transition state 8 (flag | 0x100)
 *   vfunc_10 @ 0x027E2B10: Event transition state 9 (flag | 0x200)
 *   vfunc_11 @ 0x027E21F8: Active Plaza simulation tick, Jumbotron TV dummy refresh (30f)
 *   vfunc_12 @ 0x027E249C: Event state 0xB (flag | 0x800)
 *   vfunc_13 @ 0x027E2B6C: Event state 0xC (flag | 0x1000)
 *   vfunc_33 @ 0x027E2BCC: Plaza exit sequence trigger
 */
class Fld_Plaza00_Plz : public GambitActor {
public:
    static constexpr u32 cJumbotronRefreshRate = 30; // 2Hz refresh (every 30 frames)

    Fld_Plaza00_Plz();
    virtual ~Fld_Plaza00_Plz() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic Ghidra Decompiled vfuncs
    virtual void vfunc_3(); // Model scene & EndingPlaza init
    virtual void vfunc_4(); // Splatfest Night setup
    virtual void vfunc_5(); // Stage actor lifecycle reset
    virtual void vfunc_6(); // State transition trigger
    virtual void vfunc_7(); // Event flag 0x40
    virtual void vfunc_8(); // Event flag 0x80
    virtual void vfunc_9(); // Event flag 0x100
    virtual void vfunc_10(); // Event flag 0x200
    virtual void vfunc_11(); // Active Plaza simulation tick
    virtual void vfunc_12(u32 param); // Event flag 0x800
    virtual void vfunc_13(u32 param); // Event flag 0x1000
    virtual void vfunc_33(); // Plaza exit sequence

    void setSplatfestNight(bool enable);
    void triggerPlazaExit();

    PlazaMode getMode() const { return mMode; }
    PlazaEventState getEventState() const { return mEventState; }
    u32 getPlazaFlags() const { return mFlags; }
    u32 getFrameCounter() const { return mFrameCounter; }
    u32 getJumbotronUpdateCount() const { return mJumbotronUpdateCount; }
    bool isSplatfestNight() const { return (mFlags & 0x2) != 0; }
    bool isJumbotronActive() const { return (mFlags & 0x400) != 0; }

protected:
    u32 mFlags;               // 0x230: Plaza state bitmask
    u32 mFrameCounter;         // 0x298: Plaza frame counter
    s32 mInternalState;        // 0x1C: Internal state id
    PlazaMode mMode;
    PlazaEventState mEventState;
    u32 mJumbotronUpdateCount;

    // Plaza model scene assets
    const char* mTvDummyName;
    const char* mTopicDummyName;
    const char* mDefaultNightName;
    const char* mCommonNightName;
    const char* mModelSceneName;
    const char* mEndingPlazaName;

    u8 mReserved[0x60];
};

} // namespace Game
