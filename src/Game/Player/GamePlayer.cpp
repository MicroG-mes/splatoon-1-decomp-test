#include "Game/GamePlayer.h"
#include "Game/Paint/PaintTextureMgr.h"
#include "Game/Bullet/GameBullet.h"
#include <cmath>
#include <cstring>

namespace Game {

Player::Player()
    : mPosition(0.0f, 0.0f, 0.0f),
      mCollisionOffset(0.0f, 0.0f, 0.0f),
      mVelocity(0.0f, 0.0f, 0.0f),
      mTargetVelocity(0.0f),
      mCurrentVelocity(0.0f),
      mAccelDelta(0.0f),
      mAccelTimer(0),
      mIsSlopeGrounded(1),
      deathFrm(0),
      mIsAiming(0),
      mIsSuperSpeed(0),
      mIsInInk(0),
      mSubmergedState(0),
      mInkDepth(0.0f),
      mGroundNormal(0.0f, 1.0f, 0.0f),
      mDistanceToFloor(0.0f),
      mGearSkillMgr(nullptr),
      mJumpController(nullptr),
      mAnimController(nullptr),
      playerInkAction(nullptr),
      playerModel(nullptr),
      playerEffect(nullptr),
      mSoundController(nullptr),
      mFormStateMachine(nullptr),
      mWeaponController(nullptr),
      playerNetControl(nullptr),
      mCurrentState(PlayerStateID::Human_Wait),
      mStateSubFlag(0),
      swimSpeed(1.0f),
      playerBehindCamera(nullptr),
      mInkTankAmount(1.0f),
      mIsGrounded(true),
      mIsInFriendlyInk(false),
      mIsInEnemyInk(false) {
    std::memset(field0_0x0, 0, sizeof(field0_0x0));
    std::memset(field2_0x30, 0, sizeof(field2_0x30));
    std::memset(field7_0x62, 0, sizeof(field7_0x62));
    std::memset(field11_0x84, 0, sizeof(field11_0x84));
    std::memset(field13_0xac, 0, sizeof(field13_0xac));
    std::memset(field16_0xde, 0, sizeof(field16_0xde));
    std::memset(field19_0xfe, 0, sizeof(field19_0xfe));
    std::memset(field20_0x254, 0, sizeof(field20_0x254));
    std::memset(field21_0x278, 0, sizeof(field21_0x278));
    std::memset(field22_0x324, 0, sizeof(field22_0x324));
    std::memset(field23_0x430, 0, sizeof(field23_0x430));
    std::memset(field24_0x48D, 0, sizeof(field24_0x48D));
    std::memset(field25_0x4D8, 0, sizeof(field25_0x4D8));
    std::memset(field26_0x586, 0, sizeof(field26_0x586));
    std::memset(field27_0x59D, 0, sizeof(field27_0x59D));
    std::memset(field28_0x5B4, 0, sizeof(field28_0x5B4));
    std::memset(field29_0x5F0, 0, sizeof(field29_0x5F0));
    std::memset(field30_0x600, 0, sizeof(field30_0x600));
    std::memset(field31_0x62C, 0, sizeof(field31_0x62C));
    std::memset(field32_0x77C, 0, sizeof(field32_0x77C));
    std::memset(field33_0x788, 0, sizeof(field33_0x788));
    field34_0x7B8 = 0;
    std::memset(field35_0x7BA, 0, sizeof(field35_0x7BA));
    std::memset(field36_0x894, 0, sizeof(field36_0x894));

    teamId = 0;
    playerId = 0;
    playerIndex = 0;
    isOtherPlayer = 0;
    isLocalPlayer = 1;
    shotBulletsNum = 0;
    weaponId = 0;
    subWeaponId = 0;
    specialWeaponId = 0;
    playerActorPtr = this;
    matchMgrPtr = nullptr;
    isShooting = 0;
    isSubWeaponHeld = 0;
    useSubDelayFrm = 0;
    isSwimming = 0;
}

Player::~Player() = default;

// 0x0265A688: Decompiled vfunc_36 (Actor model name)
const char* Player::getActorName() {
    if (playerIndex == 0) return "PlayerV0";
    if (playerIndex == 1) return "PlayerV1";
    return "Player";
}

// 0x0265A730: Decompiled vfunc_37 (Controller name)
const char* Player::getControllerName() {
    if (isOtherPlayer != 0) return "PlayerOther";
    if (playerIndex == 0) return "DuelPlayerCtrlV0";
    if (playerIndex == 1) return "DuelPlayerCtrlV1";
    return "PlayerCtrl";
}

// 0x02659F7C: Decompiled vfunc_52 (Enter Human form)
void Player::enterHumanForm() {
    isSwimming = 0;
    mSubmergedState = 0;
    mInkDepth = 0.0f;
    changeState(PlayerStateID::Human_Move);
}

// 0x02659FD0: Decompiled vfunc_53 (Enter Squid form)
void Player::enterSquidForm() {
    isSwimming = 1;
    if (mIsInFriendlyInk) {
        mSubmergedState = 1;
        mInkDepth = 1.0f;
    }
    changeState(PlayerStateID::Squid_Move);
}

// 0x02641C1C: Decompiled vfunc_7 (Physics step and speed integration)
void Player::updatePhysicsStep() {
    // Check inactive / respawning states
    if (static_cast<u32>(mCurrentState) >= 1 && static_cast<u32>(mCurrentState) <= 3) {
        return;
    }

    // 1. Quadratic momentum acceleration curve (PowerPC float smoothing)
    if (mAccelTimer > 0) {
        f32 remTimer = static_cast<f32>(mAccelTimer);
        f32 velDiff = mTargetVelocity - mCurrentVelocity;
        f32 accelAdjust = -(mAccelDelta * remTimer - velDiff);
        f32 newAccel = mAccelDelta + (accelAdjust * 2.0f) / (remTimer * remTimer + remTimer);
        mCurrentVelocity += newAccel;
        mAccelDelta = newAccel;
        mAccelTimer++;
        if (mCurrentVelocity > mTargetVelocity) {
            mCurrentVelocity = mTargetVelocity;
        }
    } else {
        mCurrentVelocity = mTargetVelocity;
    }

    // 2. Ink submerge depth state integration (recovered from 0x0267B18C)
    if (mIsInInk && isSquidState()) {
        if (mInkDepth < 1.0f) {
            mInkDepth += 0.15f;
            if (mInkDepth > 1.0f) mInkDepth = 1.0f;
        }
        mSubmergedState = (mInkDepth > 0.40f) ? 1 : 0;
    } else {
        if (mInkDepth > 0.0f) {
            mInkDepth -= 0.15f;
            if (mInkDepth < 0.0f) mInkDepth = 0.0f;
        }
        mSubmergedState = 0;
    }

    // 3. Super Speed threshold (0x02641C1C offset 0x585)
    f32 horizSpeedSq = (mVelocity.x * mVelocity.x) + (mVelocity.z * mVelocity.z);
    mIsSuperSpeed = (horizSpeedSq > (0.28f * 0.28f)) ? 1 : 0;
}

// 0x0264FF04: Decompiled vfunc_11 (Ground slope and collision update)
void Player::updateGroundSlope() {
    f32 slopeCos = mGroundNormal.y;
    // Slope threshold: 0.707 (45 degrees)
    if (slopeCos > 0.707f) {
        mIsSlopeGrounded = 1;
        mDistanceToFloor = 0.0f;
    } else if (slopeCos > 0.1f) {
        // Steep slope: apply gravity tangent slip
        mIsSlopeGrounded = 0;
        mVelocity.x += mGroundNormal.x * 0.04f;
        mVelocity.z += mGroundNormal.z * 0.04f;
    } else {
        // Vertical wall: climbable in squid form if painted
        mIsSlopeGrounded = 0;
    }
}

void Player::init() {
    GambitActor::init();
    changeState(PlayerStateID::Human_Wait);
}

void Player::update() {
    updateInkSwimming();
    updateState(mCurrentState);
    updatePhysics();
    updatePhysicsStep();
    updateGroundSlope();
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

    // Apply movement velocity based on form and ink
    updateInkSwimming();
    f32 moveSpeed = isSquidState() ? (0.28f * swimSpeed) : 0.16f;
    mVelocity.x = stickX * moveSpeed;
    mVelocity.z = stickY * moveSpeed;

    // Update facing yaw rotation from movement or gyro/stick
    if (stickMagSq > 0.05f) {
        mRotation.y = std::atan2(stickX, stickY);
    }

    // ZL button switches between Human and Squid form
    bool wantsSquid = (vpad.hold & VPAD_BUTTON_ZL) != 0;

    if (wantsSquid && isHumanState()) {
        changeState(stickMagSq > 0.05f ? PlayerStateID::Squid_Move : PlayerStateID::Squid_Wait);
    } else if (!wantsSquid && isSquidState()) {
        changeState(stickMagSq > 0.05f ? PlayerStateID::Human_Move : PlayerStateID::Human_Wait);
    }

    // B button jumps
    if ((vpad.trigger & VPAD_BUTTON_B) != 0 && mIsGrounded) {
        mVelocity.y = 0.40f;
        mIsGrounded = false;
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

    // Ground floor collision clamp at y = 0
    if (mPosition.y <= 0.0f) {
        mPosition.y = 0.0f;
        mVelocity.y = 0.0f;
        mIsGrounded = true;
    }
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

        // Splat ink on ground in front of player
        if (PaintTextureMgr::instance()) {
            sead::Vector3f splatPos = mPosition;
            splatPos.x += std::sin(mRotation.y) * 3.5f;
            splatPos.z += std::cos(mRotation.y) * 3.5f;
            PaintTextureMgr::instance()->paintSplat(splatPos, 2.2f, PaintColor::TeamAlpha);
        }
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
