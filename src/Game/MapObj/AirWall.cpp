#include "Game/MapObj/AirWall.h"
#include <cmath>

namespace Game {

AirWall::AirWall()
    : mPlanePoint(0.0f, 0.0f, 0.0f),
      mPlaneNormal(0.0f, 0.0f, 1.0f),
      mWidth(50.0f),
      mHeight(30.0f) {
}

AirWall::~AirWall() {
}

void AirWall::init() {
    GambitActor::init();
    mPlanePoint.set(0.0f, 0.0f, 0.0f);
    mPlaneNormal.set(0.0f, 0.0f, 1.0f);
    mWidth = 50.0f;
    mHeight = 30.0f;
}

void AirWall::setBoundaryPlane(const sead::Vector3f& planePoint, const sead::Vector3f& planeNormal, f32 width, f32 height) {
    mPlanePoint = planePoint;
    f32 len = std::sqrt(planeNormal.x * planeNormal.x + planeNormal.y * planeNormal.y + planeNormal.z * planeNormal.z);
    if (len > 0.001f) {
        mPlaneNormal.set(planeNormal.x / len, planeNormal.y / len, planeNormal.z / len);
    } else {
        mPlaneNormal.set(0.0f, 0.0f, 1.0f);
    }
    mWidth = width;
    mHeight = height;
}

bool AirWall::checkCollision(const sead::Vector3f& testPos, f32 radius, sead::Vector3f& outCorrection) const {
    // Vector from plane point to test position
    f32 vx = testPos.x - mPlanePoint.x;
    f32 vy = testPos.y - mPlanePoint.y;
    f32 vz = testPos.z - mPlanePoint.z;

    // Signed distance to plane
    f32 dist = vx * mPlaneNormal.x + vy * mPlaneNormal.y + vz * mPlaneNormal.z;

    // If penetrating plane within sphere radius
    if (dist < radius && dist > -radius) {
        f32 penetration = radius - dist;
        outCorrection.set(
            mPlaneNormal.x * penetration,
            mPlaneNormal.y * penetration,
            mPlaneNormal.z * penetration
        );
        return true;
    }

    outCorrection.set(0.0f, 0.0f, 0.0f);
    return false;
}

void AirWall::update() {
}

void AirWall::draw() {
    // Invisible plane, no visual rendering
}

} // namespace Game
