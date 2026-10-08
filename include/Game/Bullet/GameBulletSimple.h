#pragma once

#include "types.h"
#include "Game/Bullet/GameBullet.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * GameBulletSimple
 * Lightweight projectile with minimal physics
 */
class GameBulletSimple : public GameBullet {
public:
    GameBulletSimple();
    virtual ~GameBulletSimple() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

protected:
    undefined mModuleData[0x40];
};

} // namespace Game
