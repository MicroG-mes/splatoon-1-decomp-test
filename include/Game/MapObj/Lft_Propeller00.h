#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class PropellerLiftState : u32 {
    cIdle      = 0,
    cMoving    = 1,
    cReturning = 2
};

/**
 * Lft_Propeller00
 * Propeller-driven moving platform in Octo Valley and Ancho-V Games.
 * Spinning the top pinwheel by inking it causes the lift to advance along its track.
 */
class Lft_Propeller00 : public GambitActor {
public:
    static constexpr f32 cMaxRpm = 120.0f;
    static constexpr f32 cRpmDecay = 0.94f;
    static constexpr f32 cReturnSpeed = 0.003f;
    static constexpr f32 cMoveSpeedFactor = 0.00025f;

    Lft_Propeller00();
    virtual ~Lft_Propeller00() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setTrack(const sead::Vector3f& startPos, const sead::Vector3f& endPos);
    void hitPropeller(f32 inkPower);

    PropellerLiftState getState() const { return mState; }
    f32 getProgress() const { return mProgress; }
    f32 getPropellerRpm() const { return mPropellerRpm; }
    f32 getPropellerAngle() const { return mPropellerAngle; }
    const sead::Vector3f& getPosition() const { return mPosition; }

protected:
    sead::Vector3f mStartPos;
    sead::Vector3f mEndPos;
    sead::Vector3f mPosition;

    PropellerLiftState mState;
    f32 mProgress;       // 0.0f to 1.0f
    f32 mPropellerRpm;
    f32 mPropellerAngle;
    s32 mTimer;
};

} // namespace Game
