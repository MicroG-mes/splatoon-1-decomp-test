#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"

namespace Game {

enum class MatchOutcomePose : u32 {
    cWinWeaponPump = 0,
    cWinJumpCheer  = 1,
    cWinProudArms  = 2,
    cLoseSadClap   = 3,
    cLoseHeadShake = 4,
    cLoseSlump     = 5
};

class DuelListenerPoser : public GambitActor {
public:
    DuelListenerPoser();
    virtual ~DuelListenerPoser() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setPlayerOutcome(u32 playerIndex, bool isWinner, u32 weaponCategory);
    MatchOutcomePose getPlayerPose(u32 playerIndex) const;

protected:
    MatchOutcomePose mPlayerPoses[8];
    s32 mTimer;

    undefined mReserved[0x38];
};

} // namespace Game
