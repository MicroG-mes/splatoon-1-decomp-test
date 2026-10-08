#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class SpongeState : u32 {
    cState_Neutral = 0,
    cState_Expanding,
    cState_Contracting,
    cState_MaxExpanded,
    cState_MinContracted
};

struct SpongeParams {
    f32 scaleDamageForMax;      // mScaleDamageForMax (2.4)
    f32 scaleBombCoreDamageK;   // mScaleBombCoreDamageK (2.0)
    f32 paintingLiftDamage;     // mPaintingLiftDamage (1.0)
    s32 enemyNoReactFrame;      // mEnemyNoReactFrame (12)

    bool load(const char* paramsPath);
};

/**
 * Obj_Sponge
 * Dynamic ink-reactive inflatable sponge obstacle / platform.
 * Expands dramatically when inked by friendly ink (up to 2.4x scale)
 * and contracts when sprayed with enemy ink.
 */
class Obj_Sponge : public GambitActor {
public:
    static constexpr f32 cBaseScale = 1.0f;
    static constexpr f32 cMinScale = 0.4f;
    static constexpr f32 cBaseRadius = 2.0f;
    static constexpr f32 cBaseHeight = 1.8f;

    Obj_Sponge();
    virtual ~Obj_Sponge() override;

    virtual void init() override;
    void init(const sead::Vector3f& pos, u32 initialTeam = 0);
    virtual void update() override;

    // Damage / Inking reactions
    bool applyFriendlyInk(f32 inkAmount);
    bool applyFriendlyBomb(f32 bombAmount);
    bool applyEnemyInk(f32 inkAmount);

    // Collision detection with dynamic scaling
    bool checkPlayerStanding(const sead::Vector3f& playerPos, f32& outGroundY) const;

    // Getters
    SpongeState getState() const { return mState; }
    f32 getCurrentScale() const { return mCurrentScale; }
    f32 getMaxScale() const { return mParams.scaleDamageForMax; }
    f32 getMinScale() const { return cMinScale; }
    u32 getTeamId() const { return mTeamId; }
    const SpongeParams& getParams() const { return mParams; }
    f32 getCurrentRadius() const { return cBaseRadius * mCurrentScale; }
    f32 getCurrentHeight() const { return cBaseHeight * mCurrentScale; }
    bool isFullyExpanded() const { return mCurrentScale >= (mParams.scaleDamageForMax - 0.05f); }

private:
    SpongeState mState;
    SpongeParams mParams;
    u32 mTeamId;
    f32 mCurrentScale;
    f32 mTargetScale;
    f32 mBreathingPhase;
    s32 mCooldownTimer;
};

} // namespace Game
