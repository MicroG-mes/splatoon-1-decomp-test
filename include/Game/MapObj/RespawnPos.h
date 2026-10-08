#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

class RespawnPos : public GambitActor {
public:
    static constexpr f32 cBarrierRadius = 4.2f;
    static constexpr f32 cBarrierHeight = 3.5f;

    RespawnPos();
    virtual ~RespawnPos() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setupSpawn(const sead::Vector3f& pos, u32 teamId);
    bool checkBarrierCollision(const sead::Vector3f& testPos, u32 testTeam) const;
    bool isInsideBarrier(const sead::Vector3f& playerPos) const;

    u32 getTeamId() const { return mTeamId; }
    const sead::Vector3f& getPosition() const { return mPosition; }

protected:
    sead::Vector3f mPosition;
    u32 mTeamId;
    f32 mBarrierPulsePhase;

    undefined mReserved[0x38];
};

} // namespace Game
