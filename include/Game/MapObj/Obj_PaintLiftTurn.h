#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class TurnLiftState : u32 {
    cState_Idle         = 0,
    cState_Rotating     = 1,
    cState_DetentPause  = 2
};

/**
 * Obj_PaintLiftTurn / Obj_PaintLiftTurn12M
 * Retail Address: vtable @ 0x100d0ffc / 0x100d1230
 * Revolving paintable turntable platform (回転リフト) featured in Octo Valley Stage 19
 * (Snake_19TurnLift) and multiplayer stages (Flounder Heights / Camp Triggerfish).
 * Rotates around its central axis, accelerating when inked and locking into 90-degree
 * detent alignments.
 *
 * Real PowerPC methods:
 *   vfunc_3  @ 0x0258fef4 - Model & paint canvas binding (Lft_TurnLift00.szs, 1,468 vertices)
 *   vfunc_5  @ 0x02590054 - Rotary inertia and motor speed setup (DAT_100d1264, DAT_100d1268)
 *   vfunc_7  @ 0x0259119c - Rotation update & player passenger binding
 *   vfunc_11 @ 0x02591320 - Ink projectile torque impulse reception
 *   vfunc_30 @ 0x02591368 - Detent snap alignment (0, 90, 180, 270 deg)
 */
class Obj_PaintLiftTurn : public GambitActor {
public:
    static constexpr f32 cDefaultRotSpeed = 1.5f;   // degrees per frame
    static constexpr f32 cPlatformRadius  = 12.0f;  // 12m radius variant

    Obj_PaintLiftTurn();
    virtual ~Obj_PaintLiftTurn() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC vtable slots matching Ghidra
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_11();
    virtual void vfunc_30();

    // Mechanics
    void applyInkTorque(f32 torque);
    void setRotationSpeed(f32 speed) { mRotSpeed = speed; }
    bool checkPassenger(const sead::Vector3f& playerPos) const;

    // Queries
    TurnLiftState getState() const { return mState; }
    f32 getCurrentAngle() const { return mCurrentAngle; }
    f32 getAngularVelocity() const { return mAngularVelocity; }
    bool isRotating() const { return mState == TurnLiftState::cState_Rotating; }

    const sead::Vector3f& getPosition() const { return mPosition; }
    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mPosition;      // 0x140
    TurnLiftState mState;
    f32 mCurrentAngle;
    f32 mAngularVelocity;
    f32 mRotSpeed;
    s32 mPauseTimer;

    undefined mReserved[0x38];
};

} // namespace Game
