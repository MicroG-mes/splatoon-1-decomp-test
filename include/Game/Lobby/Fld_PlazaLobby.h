#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class LobbyDoorState : u32 {
    cClosed         = 0,
    cOpening        = 1,
    cOpened         = 2,
    cPlayerEntering = 3,
    cTransitionDone = 4
};

class Fld_PlazaLobby : public GambitActor {
public:
    Fld_PlazaLobby();
    virtual ~Fld_PlazaLobby() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    bool checkEntranceTrigger(const sead::Vector3f& playerPos);
    void enterLobby();

    bool isTransitionReady() const { return mDoorState == LobbyDoorState::cTransitionDone; }
    void resetDoor();

protected:
    sead::Vector3f mDoorPosition;
    f32 mTriggerRadius;
    LobbyDoorState mDoorState;
    s32 mStateTimer;
    f32 mDoorSlideProgress; // 0.0 = Closed, 1.0 = Fully opened

    undefined mReserved[0x38];
};

} // namespace Game
