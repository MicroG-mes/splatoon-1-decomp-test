#include "Game/Enemy/Enm_Tentacle.h"
#include <cmath>

namespace Game {

Enm_Tentacle::Enm_Tentacle()
    : mPosition(0.0f, 0.0f, 0.0f),
      mHp(cMaxHp),
      mEmergeHeight(0.0f),
      mMaxHeight(3.5f),
      mWhipAngle(0.0f),
      mState(TentacleState::cSubmergedIdle),
      mStateTimer(0) {
}

Enm_Tentacle::~Enm_Tentacle() {
}

void Enm_Tentacle::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mHp = cMaxHp;
    mEmergeHeight = 0.0f;
    mMaxHeight = 3.5f;
    mWhipAngle = 0.0f;
    mState = TentacleState::cSubmergedIdle;
    mStateTimer = 0;
}

void Enm_Tentacle::applyDamage(f32 damage) {
    if (mState == TentacleState::cSplatted) {
        return;
    }

    mHp -= damage;
    if (mHp <= 0.0f) {
        mHp = 0.0f;
        mState = TentacleState::cSplatted;
        mStateTimer = 0;
    }
}

void Enm_Tentacle::updateAi(const sead::Vector3f& playerPos) {
    if (mState == TentacleState::cSplatted) {
        return;
    }

    f32 dx = playerPos.x - mPosition.x;
    f32 dz = playerPos.z - mPosition.z;
    f32 distSq = dx * dx + dz * dz;

    switch (mState) {
        case TentacleState::cSubmergedIdle: {
            if (distSq < (cTriggerDistance * cTriggerDistance)) {
                mState = TentacleState::cEmergeLash;
                mStateTimer = 0;
            }
            break;
        }

        case TentacleState::cEmergeLash: {
            mStateTimer++;
            mEmergeHeight += 0.25f;
            if (mEmergeHeight >= mMaxHeight) {
                mEmergeHeight = mMaxHeight;
                mState = TentacleState::cWhipAttack;
                mStateTimer = 0;
            }
            break;
        }

        case TentacleState::cWhipAttack: {
            mStateTimer++;
            mWhipAngle += 0.2f;
            if (mStateTimer > 60) {
                mState = TentacleState::cRetreat;
                mStateTimer = 0;
            }
            break;
        }

        case TentacleState::cRetreat: {
            mStateTimer++;
            mEmergeHeight -= 0.15f;
            if (mEmergeHeight <= 0.0f) {
                mEmergeHeight = 0.0f;
                mState = TentacleState::cSubmergedIdle;
                mStateTimer = 0;
            }
            break;
        }

        default:
            break;
    }
}

void Enm_Tentacle::update() {
    if (mState == TentacleState::cSplatted) {
        mStateTimer++;
        if (mEmergeHeight > 0.0f) {
            mEmergeHeight -= 0.2f;
        }
    }
}

void Enm_Tentacle::draw() {
    // Inverse kinematics tentacle chain and slime rendering handled by ModelSceneMgr
}

} // namespace Game
