#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class BeakonState : u32 {
    cPlacing   = 0,
    cActive    = 1,
    cDestroyed = 2
};

class Wsb_Flag : public GambitActor {
public:
    static constexpr f32 cMaxHp = 100.0f;
    static constexpr s32 cPingIntervalFrames = 90; // 1.5s sonar chirp

    Wsb_Flag();
    virtual ~Wsb_Flag() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void deploy(const sead::Vector3f& pos, u32 teamId, u32 ownerPlayerId);
    void applyDamage(f32 damage);
    void consumeOnJumpLanding();

    BeakonState getState() const { return mState; }
    bool isActive() const { return mState == BeakonState::cActive; }
    f32 getHp() const { return mHp; }
    u32 getTeamId() const { return mTeamId; }
    u32 getOwnerPlayerId() const { return mOwnerPlayerId; }
    const sead::Vector3f& getPosition() const { return mPosition; }

protected:
    sead::Vector3f mPosition;
    u32 mTeamId;
    u32 mOwnerPlayerId;
    f32 mHp;
    BeakonState mState;
    s32 mTimer;
    s32 mPingTimer;

    undefined mReserved[0x38];
};

} // namespace Game
