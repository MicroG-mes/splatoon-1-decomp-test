#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class PlayerClimbState : u32 {
    cDetached  = 0,
    cAttached  = 1,
    cSwimming  = 2,
    cSlipDown  = 3,
    cVaultLedge = 4
};

/**
 * GamePlayerClimb
 * Handles squid vertical wall swimming physics, ink adhesion, gravity resistance,
 * and ledge mantling transitions.
 */
class GamePlayerClimb : public GambitActor {
public:
    static constexpr f32 cClimbSpeedUp   = 0.32f;
    static constexpr f32 cClimbSpeedDown = 0.38f;
    static constexpr f32 cClimbSpeedSide = 0.22f;
    static constexpr f32 cSlipGravity    = -0.15f;

    GamePlayerClimb();
    virtual ~GamePlayerClimb() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    bool attachToWall(const sead::Vector3f& wallNormal, f32 wallTopY);
    void detachFromWall();
    void updateClimbMotion(f32 stickY, f32 stickX, bool hasFriendlyInk);

    PlayerClimbState getState() const { return mState; }
    bool isClimbing() const { return mState == PlayerClimbState::cAttached || mState == PlayerClimbState::cSwimming; }
    const sead::Vector3f& getWallNormal() const { return mWallNormal; }
    f32 getClimbRootOffsetY() const { return mClimbRootOffsetY; }
    f32 getClimbHideOffsetY() const { return mClimbHideOffsetY; }

protected:
    PlayerClimbState mState;
    sead::Vector3f mWallNormal;
    f32 mWallTopY;
    f32 mClimbOffsetY;
    f32 mClimbRootOffsetY;
    f32 mClimbHideOffsetY;
    f32 mCameraFrontAngle;
    s32 mTimer;
};

} // namespace Game
