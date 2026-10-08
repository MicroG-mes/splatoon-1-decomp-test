#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class OctowhirlPhase : u32 {
    cPhase1 = 1,
    cPhase2 = 2,
    cPhase3 = 3
};

enum class OctowhirlState : u32 {
    cRevUpEngine     = 0,
    cRollingDash     = 1,
    cSpinoutSkid     = 2,
    cFlippedExposed  = 3,
    cRecoverRighting = 4,
    cDefeated        = 5
};

/**
 * GameEnemyBallKing / Enm_BallKing (The Dread Roll / Octowhirl Boss)
 * Address: vtable @ 0x1005EFB8
 * Authentic Nintendo path: D:/home/Cafe/Gambit/App/Program/Game/Enemy/GameEnemyBallKing.cpp
 * Giant rolling clam armor boss.
 *
 * Real PowerPC methods:
 *   vfunc_7  @ 0x022af2dc - AI tick, vulnerability countdown (+0x39C), stun recovery (+0x3A0)
 *   vfunc_11 @ 0x022af5b4 - Hit reaction, ink skid spinout, and tentacle exposure
 */
class GameEnemyBallKing : public GambitActor {
public:
    static constexpr f32 cMaxRollSpeedPhase1 = 0.5f;
    static constexpr f32 cMaxRollSpeedPhase2 = 0.7f;
    static constexpr f32 cMaxRollSpeedPhase3 = 0.95f;
    static constexpr f32 cTentacleMaxHp = 100.0f;

    GameEnemyBallKing();
    virtual ~GameEnemyBallKing() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    virtual void vfunc_7();
    virtual void vfunc_11();

    void updateBossAi(const sead::Vector3f& playerPos, bool isRollingOnPlayerInk);
    void applyDamage(f32 damage, bool hitExposedTentacle);

    OctowhirlState getState() const { return mState; }
    void setState(OctowhirlState state) { mState = state; }
    OctowhirlPhase getPhase() const { return mPhase; }
    f32 getTentacleHp() const { return mTentacleHp; }
    bool isTentacleExposed() const { return mState == OctowhirlState::cFlippedExposed; }
    bool isShieldActive() const { return mIsShieldActive != 0; }
    const sead::Vector3f& getPosition() const { return mPosition; }

protected:
    void triggerSpinout();
    void advancePhase();

    OctowhirlState mState;
    OctowhirlPhase mPhase;
    s32 mStateTimer;

    sead::Vector3f mPosition;
    sead::Vector3f mVelocity;
    sead::Vector3f mRollDirection;

    f32 mCurrentSpeed;
    f32 mTentacleHp;

    u8 mIsShieldActive;       // 0xAC
    s32 mVulnerabilityTimer;  // 0x39C
    s32 mStunRecoveryTimer;   // 0x3A0
};

} // namespace Game
