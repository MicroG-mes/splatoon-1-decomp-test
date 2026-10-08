#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "Game/MiniGame/MiniGame.h"
#include "cafe/vpad.h"

namespace Game {

enum class MiniGameScreenState : u32 {
    cMenuSelect   = 0,
    cPlaying      = 1,
    cPaused       = 2,
    cGameOverView = 3,
    cExiting      = 4
};

class LytMiniGameHandler : public GambitActor {
public:
    LytMiniGameHandler();
    virtual ~LytMiniGameHandler() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void handleInput(const VPADStatus& vpad);

    void open();
    void close();

    bool isClosed() const { return mScreenState == MiniGameScreenState::cExiting; }

    MiniGameScreenState getScreenState() const { return mScreenState; }
    const MiniGame& getMiniGame() const { return mMiniGame; }

protected:
    MiniGameScreenState mScreenState;
    MiniGame mMiniGame;
    s32 mStateTimer;
    s32 mSelectedMenuIndex;

    bool mWasJumpButtonPressed;
    undefined mReserved[0x38];
};

} // namespace Game
