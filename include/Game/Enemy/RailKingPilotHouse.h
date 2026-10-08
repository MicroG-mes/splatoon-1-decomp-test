#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class PilotHouseState : u32 {
    cIntactCockpit   = 0,
    cGlassCracked    = 1,
    cVulnerableDazed = 2,
    cEjectedDefeat   = 3
};

class RailKingPilotHouse : public GambitActor {
public:
    static constexpr f32 cMaxShieldHp = 100.0f;

    RailKingPilotHouse();
    virtual ~RailKingPilotHouse() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void applyDirectHit(f32 damage);
    void triggerDaze(s32 durationFrames);
    void resetCockpit();

    PilotHouseState getState() const { return mState; }
    f32 getShieldHp() const { return mShieldHp; }
    bool isVulnerable() const { return mState == PilotHouseState::cVulnerableDazed; }
    const sead::Vector3f& getPosition() const { return mPosition; }

    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mPosition;
    PilotHouseState mState;
    s32 mTimer;
    s32 mDazeDuration;
    f32 mShieldHp;

    undefined mReserved[0x38];
};

} // namespace Game
