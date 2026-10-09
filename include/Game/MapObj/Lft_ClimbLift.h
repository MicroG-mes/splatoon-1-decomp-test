#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include "sead/resource/BfresParser.h"

namespace Game {

enum class ClimbLiftState : u32 {
    cIdle = 0,
    cMovingUp = 1,
    cMovingDown = 2,
    cPausedAtTop = 3,
    cPausedAtBottom = 4
};

/**
 * Lft_ClimbLift
 * Vertical inkable climb surface lift platform.
 *
 * Retail Wii U binary:
 *   vtable @ 0x100CBEE0
 *   Model: content/Model/Lft_ClimbLift.szs
 *   Mesh: 520 authentic BFRES vertices.
 */
class Lft_ClimbLift : public GambitActor {
public:
    Lft_ClimbLift();
    virtual ~Lft_ClimbLift() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC Ghidra vtable methods
    virtual void vfunc_3();  // 0x02574E10 - Model & track setup
    virtual void vfunc_5();  // 0x02574F80 - Reset position to bottom
    virtual void vfunc_7();  // 0x025750C0 - Vertical translation & ink sync

    void setTravelBounds(f32 bottomY, f32 topY);
    void setSpeed(f32 speed);

    ClimbLiftState getState() const { return mState; }
    f32 getCurrentHeight() const { return mCurrentY; }
    bool isInkCovered() const { return mIsInkCovered; }
    void setInkCovered(bool covered) { mIsInkCovered = covered; }

    u32 getModelVertexCount() const { return static_cast<u32>(mModel.getTotalVertexCount()); }

private:
    ClimbLiftState mState;
    f32 mCurrentY;
    f32 mBottomY;
    f32 mTopY;
    f32 mSpeed;              // m/s (default 1.5)
    u32 mPauseTimer;
    bool mIsInkCovered;

    sead::BfresModel mModel;
};

} // namespace Game
