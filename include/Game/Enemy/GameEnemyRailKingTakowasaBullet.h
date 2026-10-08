#pragma once

#include "types.h"
#include "Game/Bullet/GameBullet.h"

namespace Game {

class GameEnemyRailKingTakowasaBullet : public GameBullet {
public:
    GameEnemyRailKingTakowasaBullet();
    virtual ~GameEnemyRailKingTakowasaBullet() override;

    virtual void init() override;
    virtual void update() override;

    // Deflection mechanics: player shoots it back at DJ Octavio
    void onHitByPlayerShot(f32 shotPower);
    void reflectTowardsBoss(const sead::Vector3f& bossPos);

    bool isReflected() const { return mIsReflected; }

protected:
    bool mIsReflected;
    s32 mHitsToReflect;
    s32 mCurrentHits;
    sead::Vector3f mBossTargetPos;
    f32 mRotationAngle;
};

} // namespace Game
