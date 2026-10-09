#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class VehicleSubmarineState : u32 {
    cSubmerged  = 0, // Hidden beneath ink layer
    cPeriscope  = 1, // Periscope optics exposed, tracking target
    cBreachJump = 2, // Explosive vertical leap out of ink
    cSurfaced   = 3, // Hovering/floating on surface firing
    cBreak      = 4,
    cEject      = 5
};

/**
 * Enm_TakolienVehicleSubmarine
 * Decompiled from PowerPC retail executable Gambit.elf:
 * Vtable @ 0x1008B768
 * Model: content/Model/Enm_TakolienVehicleSubmarine.szs (8,172 vertices)
 * Sub-models: Enm_Break00, Enm_Break01, Enm_Break02, Enm_TakolienVehicleSubmarine
 *
 * Specialized Octoling submersible assault craft capable of traveling beneath
 * enemy ink pools and breaching into the air to perform surprise ambushes.
 */
class Enm_TakolienVehicleSubmarine : public GambitActor {
public:
    static constexpr f32 cMaxHealth       = 180.0f;
    static constexpr f32 cSubmergedDepth  = -3.0f;
    static constexpr f32 cPeriscopeDepth  = -0.5f;
    static constexpr f32 cBreachJumpVel   = 8.5f;
    static constexpr f32 cSwimSpeed       = 2.2f;

    Enm_TakolienVehicleSubmarine();
    virtual ~Enm_TakolienVehicleSubmarine() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic PowerPC vfuncs
    virtual void vfunc_3();  // Model loading (Enm_TakolienVehicleSubmarine.szs, 8,172 vertices)
    virtual void vfunc_5();  // Parameter initialization
    virtual void vfunc_7();  // Submersible fluid dynamics & breach update
    virtual void vfunc_47(); // Submarine ink eruption torpedo discharge
    virtual void vfunc_52(); // Hull rupture & pilot ejection

    void spawn(const sead::Vector3f& pos);
    void updateSubmarineAi(const sead::Vector3f& targetPos);
    void triggerBreach();
    bool takeDamage(f32 damage);

    VehicleSubmarineState getState() const { return mState; }
    f32 getHealth() const { return mHealth; }
    f32 getCurrentDepth() const { return mCurrentDepth; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    bool isSubmerged() const { return mState == VehicleSubmarineState::cSubmerged || mState == VehicleSubmarineState::cPeriscope; }
    bool isPilotEjected() const { return mIsPilotEjected; }
    bool isDestroyed() const { return mState == VehicleSubmarineState::cBreak || mState == VehicleSubmarineState::cEject; }
    u32 getBreachCount() const { return mBreachCount; }

protected:
    sead::Vector3f mPosition;
    sead::Vector3f mMoveDirection;
    f32 mCurrentDepth;
    f32 mVerticalVelocity;
    f32 mHealth;
    VehicleSubmarineState mState;
    s32 mStateTimer;
    u32 mBreachCount;
    bool mIsPilotEjected;
    u8 mReserved[0x20];
};

} // namespace Game
