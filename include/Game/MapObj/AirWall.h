#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

class AirWall : public GambitActor {
public:
    AirWall();
    virtual ~AirWall() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setBoundaryPlane(const sead::Vector3f& planePoint, const sead::Vector3f& planeNormal, f32 width, f32 height);
    bool checkCollision(const sead::Vector3f& testPos, f32 radius, sead::Vector3f& outCorrection) const;

    const sead::Vector3f& getPlanePoint() const { return mPlanePoint; }
    const sead::Vector3f& getPlaneNormal() const { return mPlaneNormal; }

protected:
    sead::Vector3f mPlanePoint;
    sead::Vector3f mPlaneNormal;
    f32 mWidth;
    f32 mHeight;

    undefined mReserved[0x38];
};

} // namespace Game
