#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include "sead/math/seadMatrix.h"

namespace Game {

/**
 * GameTurnPlate / Obj_TurnPlate
 * Address: vtable @ 0x100E26EC
 * Authentic Nintendo path: D:/home/Cafe/Gambit/App/Program/Game/MapObj/GameTurnPlate.cpp
 * Rotating stage turntable platform with angular velocity, rider friction,
 * and 3D affine transformation matrix.
 *
 * Real PowerPC methods:
 *   vfunc_3  @ 0x025e4ef8 - Turntable geometry and collision setup
 *   vfunc_5  @ 0x025e4fd0 - Reset rotation angle
 *   vfunc_7  @ 0x025e5bf0 - Matrix transform update & rider velocity propagation
 *   vfunc_11 @ 0x025e68dc - Surface contact collision check
 */
class GameTurnPlate : public GambitActor {
public:
    static constexpr f32 cDefaultAngularSpeed = 0.015f; // ~0.86 deg/frame
    static constexpr f32 cDefaultRadius = 6.0f;

    GameTurnPlate();
    virtual ~GameTurnPlate() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic PPC methods
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_11();

    void setupPlatform(const sead::Vector3f& center, f32 radius, f32 angularSpeed = cDefaultAngularSpeed);
    sead::Vector3f computeLinearVelocityAtPoint(const sead::Vector3f& worldPos) const;
    bool isPointOnPlatform(const sead::Vector3f& worldPos) const;

    f32 getCurrentAngle() const { return mCurrentAngle; }
    f32 getAngularSpeed() const { return mAngularSpeed; }
    f32 getRadius() const { return mRadius; }
    const sead::Vector3f& getCenter() const { return mCenter; }

protected:
    sead::Vector3f mCenter;
    f32 mRadius;
    f32 mCurrentAngle;
    f32 mAngularSpeed;
    s32 mRotationTimer; // 0x29C

    // 4x3 affine transform matrix for platform geometry (0x1F8 - 0x220)
    sead::Matrix34f mTransformMatrix;
};

} // namespace Game
