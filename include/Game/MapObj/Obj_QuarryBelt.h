#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * Obj_QuarryBelt
 * Industrial conveyor belt mechanism featured in Piranha Pit and Octo Valley.
 * Scrolls painted surfaces and imparts continuous linear conveyor velocity to standing players and objects.
 */
class Obj_QuarryBelt : public GambitActor {
public:
    static constexpr f32 cDefaultBeltSpeed = 0.08f;

    Obj_QuarryBelt();
    virtual ~Obj_QuarryBelt() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setupBelt(const sead::Vector3f& center, const sead::Vector3f& direction, f32 length, f32 width, f32 speed = cDefaultBeltSpeed);
    bool getBeltVelocityAt(const sead::Vector3f& queryPos, sead::Vector3f* outVel) const;

    const sead::Vector3f& getCenter() const { return mCenter; }
    const sead::Vector3f& getDirection() const { return mDirection; }
    f32 getSpeed() const { return mSpeed; }
    f32 getUvOffset() const { return mUvOffset; }
    bool isActive() const { return mIsActive; }

    void setActive(bool active) { mIsActive = active; }

protected:
    sead::Vector3f mCenter;
    sead::Vector3f mDirection; // Normalized conveyor movement direction
    f32 mLength;
    f32 mWidth;
    f32 mSpeed;
    f32 mUvOffset;
    bool mIsActive;
    s32 mTimer;
};

} // namespace Game
