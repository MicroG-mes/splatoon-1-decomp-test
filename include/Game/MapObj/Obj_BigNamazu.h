#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class BigNamazuState : u32 {
    cState_Wait     = 0,
    cState_WaitB    = 1,
    cState_Start    = 2,
    cState_Missing  = 3
};

/**
 * Obj_BigNamazu / BigNamazu
 * Retail Address: vtable @ 0x100bb024
 * The Great Zapfish (オオデンチナマズ) coiled atop the Inkopolis Tower spire.
 * Powers the entire city of Inkopolis with 100,000 MW of high-voltage bio-electricity.
 *
 * Real PowerPC methods:
 *   vfunc_3 @ 0x02507840 - Resource loading (Obj_BigNamazu.szs, 7,563 vertices)
 *   vfunc_5 @ 0x025074ec - Tower apex locator setup (Locater_BigNamazu)
 *   vfunc_7 @ 0x025070a4 - Harmonic breathing motion & electrical spark generation
 *   vfunc_9 @ 0x025074f0 - Restoration sequence when campaign is cleared
 */
class Obj_BigNamazu : public GambitActor {
public:
    static constexpr f32 cFullPowerMW       = 100000.0f; // 100,000 MW
    static constexpr s32 cSparkInterval     = 60;        // Spark every 60 frames

    Obj_BigNamazu();
    virtual ~Obj_BigNamazu() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC vtable slots matching Ghidra
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_9();

    // Tower grid & zapfish events
    void restoreToTower();
    void setToMissing();

    // Queries
    BigNamazuState getState() const { return mState; }
    f32 getPowerOutputMW() const { return mPowerOutputMW; }
    f32 getBreathingScale() const { return mBreathingScale; }
    bool isSparksActive() const { return mSparksActive; }

    const sead::Vector3f& getPosition() const { return mPosition; }
    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mPosition;      // 0x140
    BigNamazuState mState;
    f32 mPowerOutputMW;
    f32 mBreathingScale;
    s32 mTimer;
    bool mSparksActive;

    undefined mReserved[0x38];
};

using BigNamazu = Obj_BigNamazu;

} // namespace Game
