#include "Game/Mission/PlayerCustomPartMission.h"

namespace Game {

PlayerCustomPartMission::PlayerCustomPartMission()
    : mStateTimer(0) {
    mUpgrades.heroShotLevel = 1;
    mUpgrades.inkTankLevel = 1;
    mUpgrades.hasBurstBomb = false;
    mUpgrades.hasSuctionBomb = false;
    mUpgrades.powerEggsBank = 0;
}

PlayerCustomPartMission::~PlayerCustomPartMission() {
}

void PlayerCustomPartMission::init() {
    GambitActor::init();
}

void PlayerCustomPartMission::addPowerEggs(u32 count) {
    mUpgrades.powerEggsBank += count;
}

bool PlayerCustomPartMission::upgradeHeroShot() {
    if (mUpgrades.heroShotLevel == 1 && mUpgrades.powerEggsBank >= 500) {
        mUpgrades.powerEggsBank -= 500;
        mUpgrades.heroShotLevel = 2;
        return true;
    } else if (mUpgrades.heroShotLevel == 2 && mUpgrades.powerEggsBank >= 1500) {
        mUpgrades.powerEggsBank -= 1500;
        mUpgrades.heroShotLevel = 3;
        return true;
    }
    return false;
}

bool PlayerCustomPartMission::upgradeInkTank() {
    if (mUpgrades.inkTankLevel == 1 && mUpgrades.powerEggsBank >= 300) {
        mUpgrades.powerEggsBank -= 300;
        mUpgrades.inkTankLevel = 2;
        return true;
    } else if (mUpgrades.inkTankLevel == 2 && mUpgrades.powerEggsBank >= 800) {
        mUpgrades.powerEggsBank -= 800;
        mUpgrades.inkTankLevel = 3;
        return true;
    }
    return false;
}

bool PlayerCustomPartMission::unlockBurstBomb() {
    if (!mUpgrades.hasBurstBomb && mUpgrades.powerEggsBank >= 200) {
        mUpgrades.powerEggsBank -= 200;
        mUpgrades.hasBurstBomb = true;
        return true;
    }
    return false;
}

bool PlayerCustomPartMission::unlockSuctionBomb() {
    if (!mUpgrades.hasSuctionBomb && mUpgrades.powerEggsBank >= 400) {
        mUpgrades.powerEggsBank -= 400;
        mUpgrades.hasSuctionBomb = true;
        return true;
    }
    return false;
}

f32 PlayerCustomPartMission::getHeroShotFireRate() const {
    if (mUpgrades.heroShotLevel == 3) return 3.5f;
    if (mUpgrades.heroShotLevel == 2) return 4.0f;
    return 4.5f;
}

f32 PlayerCustomPartMission::getInkTankMultiplier() const {
    if (mUpgrades.inkTankLevel == 3) return 1.5f;
    if (mUpgrades.inkTankLevel == 2) return 1.25f;
    return 1.0f;
}

void PlayerCustomPartMission::update() {
    mStateTimer++;
}

void PlayerCustomPartMission::draw() {
    GambitActor::draw();
}

} // namespace Game
