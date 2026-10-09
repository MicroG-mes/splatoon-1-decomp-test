#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class ColorConeState : u32 {
    cState_Upright = 0,
    cState_Tilted  = 1,
    cState_Toppled = 2
};

/**
 * Obj_ColorCone
 * Interactive traffic cone prop in multiplayer and single-player stages.
 * Tilts and wobbles elastically when brushed or hit with ink; topples on heavy impacts.
 */
class Obj_ColorCone : public GambitActor {
public:
    static constexpr f32 cBaseRadius             = 0.40f;
    static constexpr f32 cHeight                 = 0.85f;
    static constexpr f32 cTiltSpringKp           = 0.12f;
    static constexpr f32 cTiltDamperKd           = 0.20f;
    static constexpr f32 cToppleThreshold        = 6.0f;
    static constexpr f32 cMaxTiltAngle           = 1.45f; // ~83 degrees

    Obj_ColorCone();
    virtual ~Obj_ColorCone() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC vtable slots matching Ghidra
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_11();

    // Interaction physics
    void applyImpulse(const sead::Vector3f& force);
    bool checkPlayerBrush(const sead::Vector3f& playerPos, const sead::Vector3f& playerVel, f32 playerRadius = 0.6f);
    bool checkBulletImpact(const sead::Vector3f& bulletPos, const sead::Vector3f& bulletVel, f32 damage);

    // Status inspection
    ColorConeState getState() const { return mState; }
    f32 getTiltAngle() const { return mTiltAngle; }
    const sead::Vector3f& getTiltAxis() const { return mTiltAxis; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    bool isUpright() const { return mState == ColorConeState::cState_Upright; }
    bool isToppled() const { return mState == ColorConeState::cState_Toppled; }

    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mPosition;      // 0x140
    sead::Vector3f mTiltAxis;

    ColorConeState mState;
    f32 mTiltAngle;
    f32 mTiltVelocity;
    s32 mStateTimer;

    undefined mReserved[0x38];
};

} // namespace Game
