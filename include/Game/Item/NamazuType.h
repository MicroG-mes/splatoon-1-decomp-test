#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class ZapfishState : u32 {
    cCaptiveOctoDome = 0,
    cRescueFlight    = 1,
    cSleepingTower   = 2,
    cFestivalDancing = 3
};

class NamazuType : public GambitActor {
public:
    static constexpr s32 cSparkIntervalFrames = 45;

    NamazuType();
    virtual ~NamazuType() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setPlazaTowerPerch(const sead::Vector3f& towerApexPos);
    void triggerRescueCelebration();
    void setFestiveMode(bool isSplatfestActive);

    ZapfishState getState() const { return mState; }
    f32 getElectricityCharge() const { return mElectricCharge; }
    const sead::Vector3f& getPosition() const { return mPosition; }

protected:
    void emitElectricSparks();

    sead::Vector3f mPosition;
    ZapfishState mState;
    s32 mTimer;
    s32 mSparkTimer;
    f32 mBreathingPhase;
    f32 mElectricCharge;

    undefined mReserved[0x38];
};

} // namespace Game
