#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

// Stage Gizmo: Propeller Lift (PaintingLift)
// Decompiled from PPC: PaintingLift @ 0x100D1118, Obj_LiftFall @ 0x100CC254
class Obj_PaintingLift : public GambitActor {
public:
    Obj_PaintingLift();
    virtual ~Obj_PaintingLift() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setupLift(const sead::Vector3f& basePos, f32 maxElevation);
    void applyInkToPropeller(f32 inkPower);

    f32 getCurrentHeight() const { return mCurrentHeight; }
    f32 getMaxElevation() const { return mMaxElevation; }
    f32 getFanSpeed() const { return mFanAngularVelocity; }
    const sead::Vector3f& getBasePosition() const { return mBasePos; }
    sead::Vector3f getPlatformPosition() const {
        return sead::Vector3f(mBasePos.x, mBasePos.y + mCurrentHeight, mBasePos.z);
    }

private:
    sead::Vector3f mBasePos;
    f32 mMaxElevation;
    f32 mCurrentHeight;
    f32 mFanAngularVelocity;
    f32 mFanAngle;
};

} // namespace Game
