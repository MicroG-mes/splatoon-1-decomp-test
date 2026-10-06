#include "Game/GamePlayer.h"
#include "Game/Paint/PaintTextureMgr.h"
#include "Game/Bullet/GameBullet.h"

namespace Game {

Player::Player()
    : mCurrentState(PlayerStateID::Human_Wait),
      mInkTankAmount(1.0f),
      mVelocity(0.0f, 0.0f, 0.0f),
      mIsGrounded(true),
      mIsInFriendlyInk(false),
      mIsInEnemyInk(false) {
    teamId = 0;
    playerId = 0;
    isLocalPlayer = 1;
    isSwimming = 0;
    isShooting = 0;
    swimSpeed = 1.0f;
}

Player::~Player() = default;

void Player::init() {
    GambitActor::init();
    changeState(PlayerStateID::Human_Wait);
}

void Player::update() {
    updateInkSwimming();
    updateState(mCurrentState);
    updatePhysics();
}

void Player::postUpdate() {
    GambitActor::postUpdate();
}

void Player::draw() {
    GambitActor::draw();
}

void Player::changeState(PlayerStateID stateId) {
    if (mCurrentState == stateId) return;

    exitState(mCurrentState);
    mCurrentState = stateId;
    enterState(mCurrentState);
}

bool Player::isSquidState() const {
    return static_cast<u32>(mCurrentState) >= static_cast<u32>(PlayerStateID::Squid_Wait);
}

bool Player::isHumanState() const {
    return !isSquidState();
}

void Player::enterState(PlayerStateID state) {
    switch (state) {
        case PlayerStateID::Squid_Wait:
        case PlayerStateID::Squid_Move:
        case PlayerStateID::Squid_Jump:
            isSwimming = 1;
            break;
        default:
            isSwimming = 0;
            break;
    }
}

void Player::updateState(PlayerStateID state) {
    switch (state) {
        case PlayerStateID::Human_Wait:
            // Recover ink slowly when upright
            if (mInkTankAmount < 1.0f) {
                mInkTankAmount += 0.003f;
                if (mInkTankAmount > 1.0f) mInkTankAmount = 1.0f;
            }
            break;

        case PlayerStateID::Human_Move:
            // Footstep paint checks & friction
            break;

        case PlayerStateID::Squid_Wait:
        case PlayerStateID::Squid_Move:
            // Fast ink tank recovery when submerged in friendly ink
            if (mIsInFriendlyInk && mInkTankAmount < 1.0f) {
                mInkTankAmount += 0.015f;
                if (mInkTankAmount > 1.0f) mInkTankAmount = 1.0f;
            }
            break;

        case PlayerStateID::Human_Jet:
        case PlayerStateID::Squid_Jet:
            // Super Jump trajectory calculation
            break;

        default:
            break;
    }
}

void Player::exitState(PlayerStateID state) {
    // Cleanup state timers, sound effects, or visual trails
}

void Player::handleInput(const VPADStatus& vpad) {
    if (!isLocalPlayer) return;

    // Left analog stick drives movement
    f32 stickX = vpad.lStick.x;
    f32 stickY = vpad.lStick.y;
    f32 stickMagSq = (stickX * stickX) + (stickY * stickY);

    // ZL button switches between Human and Squid form
    bool wantsSquid = (vpad.hold & VPAD_BUTTON_ZL) != 0;

    if (wantsSquid && isHumanState() && mIsInFriendlyInk) {
        changeState(stickMagSq > 0.05f ? PlayerStateID::Squid_Move : PlayerStateID::Squid_Wait);
    } else if (!wantsSquid && isSquidState()) {
        changeState(stickMagSq > 0.05f ? PlayerStateID::Human_Move : PlayerStateID::Human_Wait);
    }

    // B button jumps
    if ((vpad.trigger & VPAD_BUTTON_B) != 0 && mIsGrounded) {
        changeState(isSquidState() ? PlayerStateID::Squid_JumpSt : PlayerStateID::Human_JumpSt);
    }

    // ZR button fires weapon (only possible in Human form)
    if ((vpad.hold & VPAD_BUTTON_ZR) != 0 && isHumanState()) {
        tryShoot();
    }

    // R button throws sub-weapon (Bombs, Sprinklers, Beakons)
    if ((vpad.trigger & VPAD_BUTTON_R) != 0 && isHumanState()) {
        trySubWeapon();
    }
}

void Player::updatePhysics() {
    // Gravity and velocity integration
    if (!mIsGrounded) {
        mVelocity.y -= 0.035f; // Gravity constant
    }
    mPosition.x += mVelocity.x;
    mPosition.y += mVelocity.y;
    mPosition.z += mVelocity.z;
}

void Player::updateInkSwimming() {
    if (PaintTextureMgr::instance()) {
        mIsInFriendlyInk = PaintTextureMgr::instance()->isFriendlyInk(mPosition, teamId);
        mIsInEnemyInk = PaintTextureMgr::instance()->isEnemyInk(mPosition, teamId);
    }

    if (isSquidState() && mIsInFriendlyInk) {
        swimSpeed = 1.95f; // Fast swim speed in friendly ink
    } else if (mIsInEnemyInk) {
        swimSpeed = 0.35f; // Trapped / slowed down in enemy ink
    } else {
        swimSpeed = 1.0f;  // Normal speed on neutral ground
    }
}

void Player::tryShoot() {
    // Consume ink and spawn GameBullet
    const f32 inkCost = 0.009f;
    if (mInkTankAmount >= inkCost) {
        mInkTankAmount -= inkCost;
        isShooting = 1;
        shotBulletsNum++;
    }
}

void Player::trySubWeapon() {
    const f32 subInkCost = 0.70f; // Typical bomb cost: 70% of tank
    if (mInkTankAmount >= subInkCost) {
        mInkTankAmount -= subInkCost;
    }
}

// Network event handlers
void Player::receiveDie_Net() {
    changeState(PlayerStateID::Human_Wait);
}

void Player::receiveAirFall_Net() {}
void Player::receiveWaterFall_Net() {}
void Player::receiveRevival_Net() {
    mInkTankAmount = 1.0f;
    changeState(PlayerStateID::Human_Wait);
}
void Player::receiveUnk_Net() {}
void Player::receiveStartDokanWarp_Net() {}
void Player::receiveUnk2_Net() {}
void Player::receiveEndDokanWarp_Net() {}

} // namespace Game
