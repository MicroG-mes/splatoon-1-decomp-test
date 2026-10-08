#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class DevilBallState : u32 {
    cAirborne  = 0,
    cDetonated = 1,
    cFinished  = 2
};

struct DisruptedDebuff {
    bool active;
    s32 remainingFrames;
    f32 speedFactor;       // 0.50x
    f32 inkRecoveryFactor; // 0.40x
    f32 jumpFactor;        // 0.60x

    void reset() {
        active = false;
        remainingFrames = 0;
        speedFactor = 1.0f;
        inkRecoveryFactor = 1.0f;
        jumpFactor = 1.0f;
    }

    void apply(bool hasColdBlooded = false) {
        active = true;
        remainingFrames = hasColdBlooded ? 75 : 300; // 1.25s vs 5.0s
        speedFactor = 0.50f;
        inkRecoveryFactor = 0.40f;
        jumpFactor = 0.60f;
    }

    void update() {
        if (active && remainingFrames > 0) {
            remainingFrames--;
            if (remainingFrames == 0) {
                reset();
            }
        }
    }
};

/**
 * DevilBall (Wsb_DevilBall - Disruptor Sub Weapon)
 * Reverse engineered from Splatoon 1 retail binary (0x0220a4e8).
 * Thrown poison flask that bursts on impact, applying speed/ink debuff.
 */
class DevilBall : public GambitActor {
public:
    static constexpr f32 cSplashRadius = 4.5f;
    static constexpr f32 cGravity = 0.038f;

    DevilBall();
    virtual ~DevilBall() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void throwBall(const sead::Vector3f& startPos, const sead::Vector3f& initVel, u32 teamId, u32 ownerPlayerId);
    void detonate(const sead::Vector3f& impactPos);

    // Apply debuff to target player within blast radius
    bool checkHitAndDebuff(const sead::Vector3f& targetPos, u32 targetTeam, DisruptedDebuff& targetDebuff, bool hasColdBlooded = false);

    DevilBallState getState() const { return mState; }
    bool isDetonated() const { return mState == DevilBallState::cDetonated; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    u32 getTeam() const { return mTeamId; }

private:
    DevilBallState mState;
    sead::Vector3f mPosition;
    sead::Vector3f mVelocity;
    u32 mTeamId;
    u32 mOwnerPlayerId;
    s32 mLifeFrames;
};

} // namespace Game
