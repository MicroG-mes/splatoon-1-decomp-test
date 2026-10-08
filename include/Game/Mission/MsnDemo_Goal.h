#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class GoalDemoPhase : u32 {
    cWaitingBulbHit = 0,
    cBulbExploding  = 1,
    cZapfishDance   = 2,
    cPlayerVictory  = 3,
    cResultsTally   = 4,
    cComplete       = 5
};

class MsnDemo_Goal : public GambitActor {
public:
    MsnDemo_Goal();
    virtual ~MsnDemo_Goal() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void triggerBulbBreak();

    GoalDemoPhase getPhase() const { return mPhase; }
    bool isComplete() const { return mPhase == GoalDemoPhase::cComplete; }
    f32 getBulbHealth() const { return mBulbHp; }

protected:
    GoalDemoPhase mPhase;
    s32 mPhaseTimer;
    f32 mBulbHp;

    sead::Vector3f mZapfishPos;
    f32 mZapfishSpinAngle;

    undefined mReserved[0x38];
};

} // namespace Game
