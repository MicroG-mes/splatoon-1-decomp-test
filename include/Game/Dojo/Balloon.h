#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class BalloonType : u32 {
    cRegular = 0, // 1 point
    cGold    = 1  // 2 points
};

enum class BalloonState : u32 {
    cInactive = 0,
    cSpawning = 1,
    cFloating = 2,
    cPopped   = 3
};

class Balloon : public GambitActor {
public:
    Balloon();
    virtual ~Balloon() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void spawn(const sead::Vector3f& spawnPos, BalloonType type);
    bool pop(u32 poppingPlayerId);

    bool isPopped() const { return mState == BalloonState::cPopped; }
    BalloonState getState() const { return mState; }
    BalloonType getType() const { return mType; }
    u32 getPointValue() const { return (mType == BalloonType::cGold) ? 2 : 1; }
    u32 getLastPoppedBy() const { return mLastPoppedBy; }
    const sead::Vector3f& getPosition() const { return mPosition; }

protected:
    sead::Vector3f mPosition;
    sead::Vector3f mBasePosition;
    BalloonType mType;
    BalloonState mState;
    s32 mTimer;
    f32 mFloatBobPhase;
    u32 mLastPoppedBy;

    undefined mReserved[0x28];
};

} // namespace Game
