#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class TentacleState : u32 {
    cSubmergedIdle = 0,
    cEmergeLash    = 1,
    cWhipAttack    = 2,
    cRetreat       = 3,
    cSplatted      = 4
};

/**
 * Enm_Tentacle
 * Ambush tentacle submerged beneath Octarian purple ink pools.
 * Breaches surface to lash and whip at passing players, creating ink shockwaves.
 */
class Enm_Tentacle : public GambitActor {
public:
    static constexpr f32 cMaxHp = 60.0f;
    static constexpr f32 cTriggerDistance = 4.0f;
    static constexpr f32 cWhipDamage = 50.0f;

    Enm_Tentacle();
    virtual ~Enm_Tentacle() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void updateAi(const sead::Vector3f& playerPos);
    void applyDamage(f32 damage);

    TentacleState getState() const { return mState; }
    f32 getHp() const { return mHp; }
    f32 getEmergeHeight() const { return mEmergeHeight; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    bool isSplatted() const { return mState == TentacleState::cSplatted; }

    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mPosition;
    f32 mHp;
    f32 mEmergeHeight;
    f32 mMaxHeight;
    f32 mWhipAngle;

    TentacleState mState;
    s32 mStateTimer;
};

} // namespace Game
