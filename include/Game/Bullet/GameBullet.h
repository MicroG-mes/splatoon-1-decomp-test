#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

class GameBullet : public GambitActor {
public:
    GameBullet();
    virtual ~GameBullet() override;

    // Actor overrides
    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Projectile setup
    void launch(const sead::Vector3f& startPos, const sead::Vector3f& direction, f32 speed, u32 teamId);

    // Collision & Paint interaction
    virtual void onHitGround(const sead::Vector3f& hitPos, const sead::Vector3f& normal);
    virtual void onHitWall(const sead::Vector3f& hitPos, const sead::Vector3f& normal);
    virtual void onHitActor(GambitActor* target);

    f32 getDamage() const { return mDamage; }
    f32 getPaintRadius() const { return mPaintRadius; }
    u32 getTeamId() const { return mTeamId; }

protected:
    sead::Vector3f mVelocity;
    f32 mGravity;
    f32 mDamage;
    f32 mPaintRadius;
    s32 mLifeSpanFrames;
    u32 mTeamId;
    u32 mOwnerPlayerId;
    bool mHasCollided;
};

} // namespace Game
