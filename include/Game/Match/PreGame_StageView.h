#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class PreGamePhase : u32 {
    cStageFlyby        = 0,
    cTeamFriendIntro   = 1, // PreGame_PlayerView_Default_Friend (Alpha)
    cTeamOppositeIntro = 2, // PreGame_PlayerView_Default_Opposite (Bravo)
    cSpawnPointFocus   = 3,
    cFinished          = 4
};

class PreGame_StageView : public GambitActor {
public:
    PreGame_StageView();
    virtual ~PreGame_StageView() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void startSequence();
    bool isFinished() const { return mPhase == PreGamePhase::cFinished; }
    PreGamePhase getPhase() const { return mPhase; }

    const sead::Vector3f& getCameraPos() const { return mCameraPos; }
    const sead::Vector3f& getCameraLookAt() const { return mCameraLookAt; }

protected:
    PreGamePhase mPhase;
    s32 mPhaseTimer;
    sead::Vector3f mCameraPos;
    sead::Vector3f mCameraLookAt;

    undefined mReserved[0x38];
};

} // namespace Game
