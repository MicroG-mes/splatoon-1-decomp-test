#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "cafe/vpad.h"

namespace Game {

enum class TitleViewState : u32 {
    Init           = 0,
    WaitInput      = 1, // "Press ZL + ZR to Start"
    StartTriggered = 2, // Ink splatter confirmation animation
    FadeOut        = 3, // Transitioning to Inkopolis Studio News
    Done           = 4,
};

class TitleView : public GambitActor {
public:
    TitleView();
    virtual ~TitleView() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void handleInput(const VPADStatus& vpad);
    void changeState(TitleViewState state);

    TitleViewState getState() const { return mCurrentState; }
    bool isStartConfirmed() const { return mCurrentState == TitleViewState::Done; }

protected:
    TitleViewState mCurrentState;
    s32 mTimer;
    f32 mLogoScale;
    bool mPromptBlink;
    undefined mReserved[0x30];
};

} // namespace Game
