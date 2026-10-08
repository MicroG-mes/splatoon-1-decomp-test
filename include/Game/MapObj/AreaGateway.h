#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class GatewayState : u32 {
    cLocked       = 0,
    cUnlocking    = 1,
    cOpenPassage  = 2,
    cLockedBehind = 3
};

class AreaGateway : public GambitActor {
public:
    static constexpr f32 cDoorMaxHeight = 4.0f;
    static constexpr f32 cTriggerRadius = 3.5f;

    AreaGateway();
    virtual ~AreaGateway() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setupGateway(const sead::Vector3f& pos, u32 sectionId);
    void unlockGate();
    void checkPlayerPassage(const sead::Vector3f& playerPos);

    GatewayState getState() const { return mState; }
    f32 getDoorHeight() const { return mDoorCurrentHeight; }
    u32 getSectionId() const { return mSectionId; }
    const sead::Vector3f& getPosition() const { return mPosition; }

protected:
    sead::Vector3f mPosition;
    u32 mSectionId;
    GatewayState mState;
    s32 mTimer;
    f32 mDoorCurrentHeight;
    bool mHasPlayerPassed;

    undefined mReserved[0x38];
};

} // namespace Game
