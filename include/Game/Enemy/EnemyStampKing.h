#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class StampKingPhase : u32 {
    cPhase1 = 1,
    cPhase2 = 2,
    cPhase3 = 3
};

enum class StampKingState : u32 {
    cHoverStalk       = 0,
    cTelegraphRise    = 1,
    cFaceSlam         = 2,
    cStuckVulnerable  = 3,
    cRecoverRise      = 4,
    cDefeated         = 5
};

// Boss 1: The Mighty Octostomp (EnemyStampKing)
// Decompiled from PPC: EnemyStampKing @ 0x100867E4, Enm_Stamp @ 0x100860CC
class EnemyStampKing : public GambitActor {
public:
    static constexpr f32 cTentacleMaxHp = 100.0f;
    static constexpr f32 cSlamDamage = 180.0f; // Instant lethal crush
    static constexpr f32 cSlamShockwaveRadius = 6.0f;

    EnemyStampKing();
    virtual ~EnemyStampKing() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void updateBossAi(const sead::Vector3f& playerPos);
    void applyTentacleDamage(f32 damage);

    StampKingState getState() const { return mState; }
    StampKingPhase getPhase() const { return mPhase; }
    f32 getTentacleHp() const { return mTentacleHp; }
    bool isStuckFaceDown() const { return mState == StampKingState::cStuckVulnerable; }
    bool isAlive() const { return mState != StampKingState::cDefeated; }
    const sead::Vector3f& getPosition() const { return mPosition; }

    void setState(StampKingState state) { mState = state; }

protected:
    void triggerFaceSlamImpact();

    StampKingState mState;
    StampKingPhase mPhase;
    s32 mStateTimer;

    sead::Vector3f mPosition;
    sead::Vector3f mTargetPlayerPos;
    f32 mYawAngle;
    f32 mTentacleHp;

    bool mSideArmorLeft;
    bool mSideArmorRight;
};

} // namespace Game
