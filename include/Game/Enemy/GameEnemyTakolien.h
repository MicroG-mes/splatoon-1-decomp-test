#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class TakolienState : u32 {
    Patrol = 0,
    SearchPlayer = 1,
    Pursue = 2,
    Shoot = 3,
    ThrowBomb = 4,
    SwimDodge = 5,
    SuperJump = 6,
    Damage = 7,
    Die = 8,
};

class GameEnemyTakolien : public GambitActor {
public:
    GameEnemyTakolien();
    virtual ~GameEnemyTakolien() override;

    // Actor lifecycle
    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // AI state machine
    void changeState(TakolienState state);
    void updateAI();
    void updateCombatMovement();

    // Combat & Actions
    void tryShoot();
    void tryThrowBomb();
    void takeDamage(f32 damage, u32 attackerTeam);

    bool isAlive() const { return mHealth > 0.0f; }

protected:
    TakolienState mCurrentState;
    s32 mStateTimer;
    f32 mHealth;            // 100 HP base
    f32 mTargetDistance;
    sead::Vector3f mTargetPlayerPos;
    bool mCanSeePlayer;
    bool mIsSwimming;

    // Combat timers
    s32 mAttackCooldown;
    s32 mBombCooldown;
    u32 mOctolingVariant;   // 0 = Normal, 1 = Elite / SpeedUp
};

} // namespace Game
