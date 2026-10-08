#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * Obj_Grate
 * Wire mesh metal catwalk / grating platform.
 * Solid to players in humanoid form, but permeable to squids (allowing squids to slip through)
 * and permeable to ink bullet trajectories.
 */
class Obj_Grate : public GambitActor {
public:
    Obj_Grate();
    virtual ~Obj_Grate() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setupGrate(const sead::Vector3f& center, f32 width, f32 length, f32 thickness = 0.2f);
    bool checkPlayerCollision(const sead::Vector3f& playerPos, bool isSquidForm, sead::Vector3f* outSupportPos) const;
    bool canBulletPassThrough() const { return true; }

    const sead::Vector3f& getCenter() const { return mCenter; }
    f32 getWidth() const { return mWidth; }
    f32 getLength() const { return mLength; }

protected:
    sead::Vector3f mCenter;
    f32 mWidth;
    f32 mLength;
    f32 mThickness;
    bool mIsEnabled;
};

} // namespace Game
