#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class TrapState : u32 {
    cPlacing    = 0,
    cArmed      = 1,
    cWarning    = 2,
    cDetonating = 3,
    cDefunct    = 4
};

class Trap : public GambitActor {
public:
    static constexpr f32 cTriggerRadius = 3.5f;
    static constexpr f32 cExplosionRadius = 5.0f;
    static constexpr f32 cMaxDamage = 180.0f;
    static constexpr s32 cArmDelayFrames = 30;
    static constexpr s32 cWarningFrames = 15;

    Trap();
    virtual ~Trap() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void place(const sead::Vector3f& pos, u32 teamId, u32 ownerPlayerId);
    bool checkProximityTrigger(const sead::Vector3f& enemyPos, u32 enemyTeamId);
    void triggerDetonation();

    TrapState getState() const { return mState; }
    u32 getTeamId() const { return mTeamId; }
    u32 getOwnerPlayerId() const { return mOwnerPlayerId; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    bool isArmed() const { return mState == TrapState::cArmed; }

protected:
    sead::Vector3f mPosition;
    u32 mTeamId;
    u32 mOwnerPlayerId;
    TrapState mState;
    s32 mTimer;

    undefined mReserved[0x38];
};

} // namespace Game
