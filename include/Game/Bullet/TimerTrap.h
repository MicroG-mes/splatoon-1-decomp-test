#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class TrapState : u32 {
    cPlacing   = 0,
    cArmed     = 1,
    cTriggered = 2,
    cExploding = 3,
    cInactive  = 4
};

/**
 * TimerTrap (Ink Mine Sub-Weapon / Proximity Detonator)
 * Decompiled from PowerPC retail executable Gambit.elf:
 * Vtable @ 0x100DE704
 *
 * Authentic PowerPC Methods:
 *   vfunc_7  @ 0x025E15C8: Proximity sensing, enemy detection & fuse countdown
 *   vfunc_11 @ 0x025E1B1C: Transform matrix synchronization
 *   vfunc_15 @ 0x025E1BF4: Detonation & blast explosion
 *   vfunc_47 @ 0x025E3880: Surface collision mesh registration
 */
class TimerTrap : public GambitActor {
public:
    static constexpr f32 cTriggerRadius = 2.5f;
    static constexpr f32 cInnerBlastDamage = 180.0f; // Lethal OHKO
    static constexpr f32 cOuterBlastDamage = 30.0f;
    static constexpr s32 cFuseFrames = 30; // 0.5s beep fuse before detonation

    TimerTrap();
    virtual ~TimerTrap() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic Ghidra Decompiled vfuncs
    virtual void vfunc_7();  // 0x025E15C8: Proximity tick
    virtual void vfunc_11(); // 0x025E1B1C: Transform sync
    virtual void vfunc_15(); // 0x025E1BF4: Detonation
    virtual void vfunc_47(); // 0x025E3880: Collision register

    void plantTrap(const sead::Vector3f& pos, u32 teamId, u32 ownerPlayerId);
    bool checkEnemyProximity(const sead::Vector3f& enemyPos, u32 enemyTeamId);
    void triggerDetonation();

    TrapState getState() const { return mState; }
    bool isArmed() const { return mState == TrapState::cArmed; }
    bool isExploded() const { return mState == TrapState::cExploding || mState == TrapState::cInactive; }
    u32 getTeamId() const { return mTeamId; }
    u32 getOwnerPlayerId() const { return mOwnerPlayerId; }
    const sead::Vector3f& getPosition() const { return mPosition; }

protected:
    sead::Vector3f mPosition;
    f32 mTransformMatrix[3][4];

    u32 mTeamId;
    u32 mOwnerPlayerId;
    TrapState mState;
    s32 mFuseTimer;      // 0x230
    bool mProximityTrip; // 0x248

    u8 mReserved[0x40];
};

} // namespace Game
