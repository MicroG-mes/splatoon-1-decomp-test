#include "Game/Result/DuelListenerPoser.h"

namespace Game {

DuelListenerPoser::DuelListenerPoser()
    : mTimer(0) {
    for (u32 i = 0; i < 8; ++i) {
        mPlayerPoses[i] = MatchOutcomePose::cWinWeaponPump;
    }
}

DuelListenerPoser::~DuelListenerPoser() {
}

void DuelListenerPoser::init() {
    GambitActor::init();
    mTimer = 0;
    for (u32 i = 0; i < 8; ++i) {
        mPlayerPoses[i] = MatchOutcomePose::cWinWeaponPump;
    }
}

void DuelListenerPoser::setPlayerOutcome(u32 playerIndex, bool isWinner, u32 weaponCategory) {
    if (playerIndex >= 8) {
        return;
    }

    if (isWinner) {
        // Diverse victory celebrations based on weapon type and slot
        if (weaponCategory == 1) { // Roller
            mPlayerPoses[playerIndex] = MatchOutcomePose::cWinProudArms;
        } else if (weaponCategory == 2) { // Charger
            mPlayerPoses[playerIndex] = MatchOutcomePose::cWinJumpCheer;
        } else { // Shooter / Slosher / Splatling
            mPlayerPoses[playerIndex] = MatchOutcomePose::cWinWeaponPump;
        }
    } else {
        // Disappointed animations
        if (playerIndex % 2 == 0) {
            mPlayerPoses[playerIndex] = MatchOutcomePose::cLoseSadClap;
        } else {
            mPlayerPoses[playerIndex] = MatchOutcomePose::cLoseHeadShake;
        }
    }
}

MatchOutcomePose DuelListenerPoser::getPlayerPose(u32 playerIndex) const {
    if (playerIndex < 8) {
        return mPlayerPoses[playerIndex];
    }
    return MatchOutcomePose::cLoseSlump;
}

void DuelListenerPoser::update() {
    mTimer++;
}

void DuelListenerPoser::draw() {
}

} // namespace Game
