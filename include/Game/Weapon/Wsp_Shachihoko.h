#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class ShachihokoShieldState : u32 {
    cShieldCharging = 0,
    cShieldBursting = 1,
    cFreePickup     = 2,
    cCarried        = 3,
    cResetting      = 4
};

class Wsp_Shachihoko : public GambitActor {
public:
    static constexpr f32 cBurstThreshold = 500.0f;
    static constexpr f32 cBurstDamage = 300.0f;
    static constexpr f32 cBurstRadius = 7.0f;
    static constexpr s32 cCarrierTimeLimit = 3600; // 60 seconds at 60fps

    Wsp_Shachihoko();
    virtual ~Wsp_Shachihoko() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void applyInkToShield(s32 teamId, f32 amount);
    bool pickup(u32 playerId, s32 teamId);
    void drop(const sead::Vector3f& dropPos);

    ShachihokoShieldState getShieldState() const { return mState; }
    s32 getCarrierPlayerId() const { return mCarrierPlayerId; }
    s32 getCarrierTeamId() const { return mCarrierTeamId; }
    s32 getRemainingCarrierFrames() const { return mCarrierTimer; }
    f32 getShieldAlphaHp() const { return mShieldAlphaHp; }
    f32 getShieldBravoHp() const { return mShieldBravoHp; }
    const sead::Vector3f& getPosition() const { return mPosition; }

    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    void triggerShieldBurst(s32 winningTeam);

    sead::Vector3f mPosition;
    ShachihokoShieldState mState;
    s32 mCarrierPlayerId; // -1 if not carried
    s32 mCarrierTeamId;   // -1 if not carried
    s32 mCarrierTimer;

    f32 mShieldAlphaHp;
    f32 mShieldBravoHp;
    s32 mBurstTimer;

    undefined mReserved[0x38];
};

} // namespace Game
