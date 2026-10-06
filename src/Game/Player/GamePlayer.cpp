#include "Game/GamePlayer.h"

namespace Game {

Player::~Player() = default;

// Network event stubs
void Player::receiveDie_Net() {}
void Player::receiveAirFall_Net() {}
void Player::receiveWaterFall_Net() {}
void Player::receiveRevival_Net() {}
void Player::receiveUnk_Net() {}
void Player::receiveStartDokanWarp_Net() {}
void Player::receiveUnk2_Net() {}
void Player::receiveEndDokanWarp_Net() {}

} // namespace Game
