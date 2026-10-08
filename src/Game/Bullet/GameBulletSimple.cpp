#include "Game/Bullet/GameBulletSimple.h"

namespace Game {

GameBulletSimple::GameBulletSimple() = default;

GameBulletSimple::~GameBulletSimple() = default;

void GameBulletSimple::init() {
    GameBullet::init();
}

void GameBulletSimple::update() {
    GameBullet::update();
}

void GameBulletSimple::draw() {
    GameBullet::draw();
}

} // namespace Game
