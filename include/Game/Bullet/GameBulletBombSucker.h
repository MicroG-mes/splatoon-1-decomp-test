#pragma once

#include "Game/Bullet/GameBullet.h"

namespace Game {

class GameBulletBombSucker : public GameBullet {
public:
    GameBulletBombSucker();
    virtual ~GameBulletBombSucker() override;

    virtual void update() override;

    // Contact sticking mechanics
    virtual void onHitGround(const sead::Vector3f& hitPos, const sead::Vector3f& normal) override;
    virtual void onHitWall(const sead::Vector3f& hitPos, const sead::Vector3f& normal) override;

    void explode();

    bool isStuck() const { return mIsStuck; }
    bool isDetonated() const { return mFuseFrames <= 0; }
    s32 getFuseFrames() const { return mFuseFrames; }
    f32 getExplosionRadius() const { return mExplosionRadius; }

protected:
    bool mIsStuck;
    sead::Vector3f mStickNormal;
    s32 mFuseFrames;       // 120 frames (~2 seconds countdown)
    f32 mExplosionRadius;  // Lethal splat radius
};

} // namespace Game
