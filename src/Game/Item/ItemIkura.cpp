#include "Game/Item/ItemIkura.h"
#include <cmath>

namespace Game {

ItemIkura::ItemIkura()
    : mPosition(0.0f, 0.0f, 0.0f),
      mVelocity(0.0f, 0.0f, 0.0f),
      mState(IkuraState::cWait),
      mEggValue(1),
      mCollectorPlayerId(0),
      mTimer(0),
      mGroundY(0.0f),
      mIsGrounded(false) {
}

ItemIkura::~ItemIkura() {
}

void ItemIkura::init() {
    GameItemBase::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mVelocity.set(0.0f, 0.0f, 0.0f);
    mState = IkuraState::cWait;
    mEggValue = 1;
    mCollectorPlayerId = 0;
    mTimer = 0;
    mGroundY = 0.0f;
    mIsGrounded = false;
}

void ItemIkura::spawn(const sead::Vector3f& pos, u32 eggValue, bool withParachute) {
    mPosition = pos;
    mVelocity.set(0.0f, 0.0f, 0.0f);
    mEggValue = eggValue;
    mGroundY = pos.y;
    mIsGrounded = !withParachute;
    mState = withParachute ? IkuraState::cGotPara : IkuraState::cWait;
    mTimer = 0;
}

void ItemIkura::spawnDropWithVelocity(const sead::Vector3f& pos, const sead::Vector3f& vel, u32 eggValue) {
    mPosition = pos;
    mVelocity = vel;
    mEggValue = eggValue;
    mGroundY = pos.y - 1.0f;
    mIsGrounded = false;
    mState = IkuraState::cWait;
    mTimer = 0;
}

bool ItemIkura::checkPlayerPickup(const sead::Vector3f& playerPos, u32 playerId) {
    if (mState == IkuraState::cGot) {
        return false;
    }

    f32 dx = playerPos.x - mPosition.x;
    f32 dy = playerPos.y - mPosition.y;
    f32 dz = playerPos.z - mPosition.z;
    f32 distSq = dx * dx + dy * dy + dz * dz;

    // Direct collection
    if (distSq <= (cPickupRadius * cPickupRadius)) {
        mState = IkuraState::cGot;
        mCollectorPlayerId = playerId;
        mTimer = 0;
        return true;
    }

    // Vacuum attraction towards player
    if (distSq <= (cVacuumRadius * cVacuumRadius)) {
        f32 dist = std::sqrt(distSq);
        if (dist > 0.001f) {
            f32 pullSpeed = 0.12f;
            mPosition.x += (dx / dist) * pullSpeed;
            mPosition.y += (dy / dist) * pullSpeed;
            mPosition.z += (dz / dist) * pullSpeed;
        }
    }

    return false;
}

void ItemIkura::update() {
    switch (mState) {
        case IkuraState::cGotPara: {
            mTimer++;
            mPosition.y += cParachuteTerminalVelocity;
            // Slight drift
            mPosition.x += std::sin(mTimer * 0.08f) * 0.01f;

            if (mPosition.y <= mGroundY) {
                mPosition.y = mGroundY;
                mIsGrounded = true;
                mState = IkuraState::cWait;
            }
            break;
        }

        case IkuraState::cWait: {
            mTimer++;
            if (!mIsGrounded) {
                mVelocity.y += cGravity;
                if (mVelocity.y < cTerminalVelocity) {
                    mVelocity.y = cTerminalVelocity;
                }

                mPosition.x += mVelocity.x;
                mPosition.y += mVelocity.y;
                mPosition.z += mVelocity.z;

                mVelocity.x *= 0.96f;
                mVelocity.z *= 0.96f;

                if (mPosition.y <= mGroundY) {
                    mPosition.y = mGroundY;
                    mVelocity.set(0.0f, 0.0f, 0.0f);
                    mIsGrounded = true;
                }
            }
            break;
        }

        case IkuraState::cGot: {
            mTimer++;
            break;
        }

        default:
            break;
    }
}

void ItemIkura::draw() {
    // Model rendering handled by ModelSceneMgr
}

} // namespace Game
