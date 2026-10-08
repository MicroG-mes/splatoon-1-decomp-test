#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"

namespace Game {

enum class BrushState : u32 {
    cIdle    = 0,
    cSwiping = 1,
    cSprint  = 2
};

class GameWeaponBrush : public GambitActor {
public:
    GameWeaponBrush();
    virtual ~GameWeaponBrush() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void triggerSwipe();
    void startSprint();
    void stopSprint();

    BrushState getState() const { return mState; }
    f32 getSprintSpeed() const { return mSprintSpeed; }
    f32 getSwipeDamage() const { return mSwipeDamage; }

protected:
    BrushState mState;
    s32 mStateTimer;
    s32 mSwipeCounter;

    f32 mSprintSpeed;
    f32 mSwipeDamage;
    f32 mInkCostSwipe;
    f32 mInkCostSprintPerFrame;

    undefined mReserved[0x38];
};

} // namespace Game
