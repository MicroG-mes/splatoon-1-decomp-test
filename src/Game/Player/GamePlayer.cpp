#include "Game/GamePlayer.h"

namespace Game {

Player::~Player() = default;

void Player::changeState(PlayerStateID stateId) {
    // State machine transition logic
}

bool Player::isSquidState() const {
    // Squid states: Squid_Wait (7) through Squid_ObjAim (16)
    return isSwimming != 0;
}

bool Player::isHumanState() const {
    return isSwimming == 0;
}

// Network event handlers
void Player::receiveDie_Net() {}
void Player::receiveAirFall_Net() {}
void Player::receiveWaterFall_Net() {}
void Player::receiveRevival_Net() {}
void Player::receiveUnk_Net() {}
void Player::receiveStartDokanWarp_Net() {}
void Player::receiveUnk2_Net() {}
void Player::receiveEndDokanWarp_Net() {}

} // namespace Game
