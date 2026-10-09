#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include "sead/resource/BfresParser.h"

namespace Game {

enum class AirDancerState : u32 {
    cDeflated = 0,
    cStarting = 1,
    cBillowing = 2,
    cWaving   = 3,
    cLimpHit  = 4
};

/**
 * Obj_AirDancer
 * Inflatable tube dancer prop & kinetic stage hazard.
 *
 * Retail Wii U binary:
 *   vtable @ 0x100D8F18
 *   Model: content/Model/Obj_AirDancer.szs
 *   Meshes:
 *     - Airdancer_m01__Airdancer (716 vertices)
 *     - Airdancer_m01__AirdancerMachine (982 vertices)
 *   Total Vertices: 1,698 authentic BFRES vertices.
 */
class Obj_AirDancer : public GambitActor {
public:
    Obj_AirDancer();
    virtual ~Obj_AirDancer() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC Ghidra vtable methods
    virtual void vfunc_3();  // 0x025C2140 - Model & physics initialization
    virtual void vfunc_5();  // 0x025C2320 - Reset / spawn state
    virtual void vfunc_7();  // 0x025C2460 - Pneumatic aerodynamic update

    void setBlowerActive(bool active);
    bool isBlowerActive() const { return mBlowerActive; }

    void applyImpact(const sead::Vector3f& force);

    AirDancerState getState() const { return mState; }
    f32 getInflationRatio() const { return mInflationRatio; }
    f32 getWobbleAngle() const { return mWobbleAngle; }
    const sead::Vector3f& getTipOffset() const { return mTipOffset; }

    u32 getModelVertexCount() const { return static_cast<u32>(mModel.getTotalVertexCount()); }

private:
    AirDancerState mState;
    bool mBlowerActive;
    f32 mInflationRatio;   // 0.0 (deflated) to 1.0 (fully inflated)
    f32 mOscillationPhase; // Phase angle for sine wave flutter
    f32 mWobbleAngle;      // Deflection angle in degrees (up to ~35 deg)
    f32 mBlowerWindSpeed;  // Nominal 12.5 m/s
    sead::Vector3f mTipOffset;
    u32 mHitRecoverTimer;

    sead::BfresModel mModel;
};

} // namespace Game
