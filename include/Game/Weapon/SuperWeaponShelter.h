#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class ShelterDeployState : u32 {
    cClosed    = 0,
    cOpening   = 1,
    cOpened    = 2,
    cRetracting= 3
};

/**
 * SuperWeaponShelter (Special Weapon Deployment Canopy / Protective Shield)
 * Address: vtable @ 0x10101CEC
 * Authentic path: D:/home/Cafe/Gambit/App/Program/Game/Weapon/SuperWeaponShelter.cpp
 *
 * Real PowerPC methods from Gambit.elf:
 *   vfunc_7  @ 0x027745B8 (size 384): Canopy extension animation, timer countdown (+0x66), height offset (+0x54/+0x58)
 *   vfunc_9  @ 0x0277474C (size 512): Draw canopy shield
 *   vfunc_15 @ 0x0277456C (size 76): Deflection & damage absorption
 *   vfunc_1  @ 0x02774C4C (size 188): Destructor & cleanup
 */
class SuperWeaponShelter : public GambitActor {
public:
    static constexpr f32 cMaxCanopyRadius = 3.2f;
    static constexpr f32 cMaxDurability = 500.0f;

    SuperWeaponShelter();
    virtual ~SuperWeaponShelter() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic PPC vfuncs
    virtual void vfunc_7();
    virtual void vfunc_9();
    virtual bool vfunc_15(f32 damage, const sead::Vector3f& hitPos);

    void deploy(const sead::Vector3f& anchorPos, u32 teamId);
    void close();

    bool isDeployed() const { return mState == ShelterDeployState::cOpened; }
    ShelterDeployState getState() const { return mState; }
    f32 getExtensionRatio() const { return mExtensionHeight; }
    f32 getDurability() const { return mDurability; }
    const sead::Vector3f& getPosition() const { return mPosition; }

protected:
    sead::Vector3f mPosition;
    f32 mExtensionHeight;   // +0x54
    f32 mCanopyRadius;      // +0x58
    u8 mDeployFlag;         // +0x60
    u8 mRetractFlag;        // +0x61
    s16 mTimer;             // +0x66
    s16 mCycleCounter;      // +0x64
    f32 mDurability;
    u32 mTeamId;
    ShelterDeployState mState;
};

} // namespace Game
