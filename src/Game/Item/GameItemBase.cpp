#include "Game/Item/GameItemBase.h"

namespace Game {

GameItemBase::GameItemBase() = default;

GameItemBase::~GameItemBase() = default;

void GameItemBase::init() {
    GambitActor::init();
}

void GameItemBase::update() {
    GambitActor::update();
}

void GameItemBase::draw() {
    GambitActor::draw();
}

} // namespace Game
