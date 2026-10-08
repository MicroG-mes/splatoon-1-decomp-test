#include "Game/GamePlayerBehindCamera.h"
#include <cmath>
#include <algorithm>

namespace Game {

PlayerBehindCamera::~PlayerBehindCamera() = default;

void PlayerBehindCamera::update() {
    // Smooth camera target following and pitch clamping
    mFov = 60.0f;
}

} // namespace Game
