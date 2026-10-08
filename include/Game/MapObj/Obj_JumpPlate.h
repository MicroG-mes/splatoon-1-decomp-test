#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class JumpPlateState : u32 {
    cIdle     = 0,
    cCoiling  = 1,
    cLaunch   = 2,
    cCooldown = 3
};

// Stage Gizmo: Launch Pad (Obj_JumpPlate)
// Decompiled from PPC: Obj_JumpPlate @ 0x100CB600, 0x0255F6B8, 0x0255F8E0
class Obj_JumpPlate : public GambitActor {
public:
    Obj_JumpPlate();
    virtual ~Obj_JumpPlate() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setupPad(const sead::Vector3f& padPos, const sead::Vector3f& destPos);
    bool stepOnPad(u32 playerId);

    sead::Vector3f computeBallisticVelocity(f32 arcHeight = 25.0f, s32 flightFrames = 90) const;

    JumpPlateState getState() const { return mState; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    const sead::Vector3f& getDestination() const { return mDestinationPos; }
    bool isLaunching() const { return mState == JumpPlateState::cLaunch; }

private:
    JumpPlateState mState;
    s32 mStateTimer;
    sead::Vector3f mPosition;
    sead::Vector3f mDestinationPos;
    u32 mRiderPlayerId;
};

} // namespace Game
