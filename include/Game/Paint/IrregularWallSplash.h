#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * IrregularWallSplash
 * Vertical wall ink splatter dynamics, drip trails, and surface contour deformation.
 * Handles gravity-driven ink downward seepage and non-planar projection.
 */
class IrregularWallSplash : public GambitActor {
public:
    static constexpr f32 cMaxDripLength = 2.4f;
    static constexpr f32 cDripSpeed = 0.04f;

    IrregularWallSplash();
    virtual ~IrregularWallSplash() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void applySplash(const sead::Vector3f& hitPos, const sead::Vector3f& normal, f32 radius, u32 teamId);

    const sead::Vector3f& getHitPosition() const { return mHitPos; }
    const sead::Vector3f& getSurfaceNormal() const { return mNormal; }
    f32 getRadius() const { return mRadius; }
    f32 getDripLength() const { return mDripLength; }
    u32 getTeamId() const { return mTeamId; }
    bool isComplete() const { return mIsComplete; }

protected:
    sead::Vector3f mHitPos;
    sead::Vector3f mNormal;
    f32 mRadius;
    f32 mDripLength;
    u32 mTeamId;
    bool mIsComplete;
    s32 mTimer;
};

} // namespace Game
