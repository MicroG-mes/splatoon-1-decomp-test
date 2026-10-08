#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class OctavioPhase : u32 {
    cPhase1 = 1, // Single rocket punch
    cPhase2 = 2, // Dual rocket punches
    cPhase3 = 3, // Megaphone (Killer Wail) sonic beam
    cPhase4 = 4, // Octoballs & spinning missiles
    cPhase5 = 5  // Calamari Inkantation finale with Rainmaker/Hero Shot
};

enum class OctavioState : u32 {
    cHoverArena         = 0,
    cLaunchRocketPunch  = 1,
    cFistReflected      = 2,
    cStunnedExposed     = 3,
    cKillerWailBarrage  = 4,
    cOctoballBarrage    = 5,
    cPhaseTransition    = 6,
    cDefeated           = 7
};

// Boss 5: DJ Octavio in the Octobot King (Enm_RailKing)
// Decompiled from PPC: Enm_RailKing @ 0x1007F804, RailKingPilotHouse @ 0x100D473C
class EnemyRailKing : public GambitActor {
public:
    static constexpr f32 cMaxHealth = 100.0f;
    static constexpr f32 cFistSwatHp = 50.0f; // Amount of ink needed to swat fist back

    EnemyRailKing();
    virtual ~EnemyRailKing() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void updateBossAi(const sead::Vector3f& playerPos);

    // Rocket fist swat-back deflection
    void applyDamageToFist(f32 damage);
    void applyTentacleDamage(f32 damage);

    OctavioState getState() const { return mState; }
    OctavioPhase getPhase() const { return mPhase; }
    f32 getTentacleHp() const { return mTentacleHp; }
    f32 getFistSwatHp() const { return mFistSwatRemaining; }
    bool isStunned() const { return mState == OctavioState::cStunnedExposed; }
    bool isAlive() const { return mState != OctavioState::cDefeated; }
    bool isKillerWailFiring() const { return mState == OctavioState::cKillerWailBarrage; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    const sead::Vector3f& getFistPosition() const { return mFistPos; }

    void setState(OctavioState state) { mState = state; }

protected:
    void advancePhase();

    OctavioState mState;
    OctavioPhase mPhase;
    s32 mStateTimer;

    sead::Vector3f mPosition;
    sead::Vector3f mTargetPlayerPos;
    sead::Vector3f mFistPos;
    sead::Vector3f mFistVelocity;

    f32 mTentacleHp;
    f32 mFistSwatRemaining;
    bool mFistActive;
    bool mFistReflected;
};

} // namespace Game
