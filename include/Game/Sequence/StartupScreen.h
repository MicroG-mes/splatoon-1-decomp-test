#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "cafe/vpad.h"

namespace Game {

enum class StartupState : u32 {
    Sleep   = 0, // State::cSleep (waiting for assets to load)
    FadeIn  = 1, // State::cFadeIn
    Show    = 2, // State::cIn (Controller & safety reminder screen)
    FadeOut = 3, // State::cFadeOut (transitioning to Title Screen)
    Done    = 4,
};

class StartupScreen : public GambitActor {
public:
    StartupScreen();
    virtual ~StartupScreen() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void changeState(StartupState state);
    StartupState getState() const { return mCurrentState; }
    bool isFinished() const { return mCurrentState == StartupState::Done; }

protected:
    StartupState mCurrentState;
    s32 mTimer;
    f32 mAlpha; // Fade opacity
};

} // namespace Game
