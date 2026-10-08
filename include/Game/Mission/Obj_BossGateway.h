#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

class Obj_BossGateway : public GambitActor {
public:
    Obj_BossGateway();
    virtual ~Obj_BossGateway() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setUnlockState(bool isUnlocked);
    bool checkPlayerProximity(const sead::Vector3f& playerPos, f32 interactRadius = 3.5f);
    void enterBossStage();

    bool isUnlocked() const { return mIsUnlocked; }
    bool isTransitioning() const { return mIsEntering; }

protected:
    sead::Vector3f mPosition;
    f32 mInteractRadius;
    bool mIsUnlocked;
    bool mIsEntering;
    s32 mStateTimer;

    undefined mReserved[0x38];
};

} // namespace Game
