#include "Game/Weapon/PlayerKingSquid.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cstring>

namespace Game {

PlayerKingSquid::PlayerKingSquid()
    : mPlayerTeamIndex(-1),
      mIsInvulnerable(0),
      mIsActive(0),
      mHasContactTarget(0),
      mSplashPos(0.0f, 0.0f, 0.0f),
      mHitboxPos(0.0f, 0.0f, 0.0f),
      mAttackTimer(0),
      mPosition(0.0f, 0.0f, 0.0f),
      mTeamId(0),
      mState(KingSquidState::cInactive),
      mDurationTimer(0),
      mStateTimer(0),
      mScaleFactor(1.0f),
      mAttackMask(0),
      mCollisionFlags(0),
      mSpinAttackCounter(0) {
    std::memset(mReserved0_0x8, 0, sizeof(mReserved0_0x8));
    std::memset(mReserved_0x0D, 0, sizeof(mReserved_0x0D));
    std::memset(mReserved_0x12, 0, sizeof(mReserved_0x12));
    std::memset(mReserved_0x52, 0, sizeof(mReserved_0x52));
}

PlayerKingSquid::~PlayerKingSquid() {
}

// 0x02267E08: Decompiled vfunc_1 (State reset & cleanup)
void PlayerKingSquid::vfunc_1() {
    mIsInvulnerable = 0;
    mIsActive = 0;
    mState = KingSquidState::cInactive;
    mAttackTimer = 0;
}

// 0x02267F7C: Decompiled vfunc_2 (Deactivate Kraken & revert vulnerability)
void PlayerKingSquid::vfunc_2() {
    mIsActive = 0;
    if (mIsInvulnerable != 0 && mPlayerTeamIndex == -1) {
        // Trigger onKrakenFinished callback
    }
    mIsInvulnerable = 0;
    mState = KingSquidState::cInactive;
}

// 0x02268074: Decompiled vfunc_3 (Activate Kraken special)
void PlayerKingSquid::vfunc_3(s32 duration) {
    mIsActive = 1;
    mIsInvulnerable = 1;
    mDurationTimer = duration;
    mState = KingSquidState::cActive;
}

// 0x022647A8: Decompiled vfunc_11 (Spin attack hit splash & damage mask calculation)
void PlayerKingSquid::vfunc_11() {
    if (mAttackTimer == 0) {
        mAttackTimer = 2;
        if (mHasContactTarget == 0) {
            mSplashPos = mPosition;
            mHitboxPos = mPosition;
        }
        mAttackTimer--;
        u32 uVar1 = ((mAttackMask >> 0x1E & 1) << 0x1D) | (mAttackMask & 0x9FFFFFFF);
        mAttackMask = uVar1;
        mCollisionFlags = (((mCollisionFlags & 0xC000) != 0) ? 0x1000 : 0) | (mCollisionFlags & 0xFFFFCFFF);
    } else {
        mAttackTimer--;
        u32 uVar1 = ((mAttackMask >> 0x1E & 1) << 0x1D) | (mAttackMask & 0x9FFFFFFF);
        mAttackMask = uVar1;
        mCollisionFlags = (((mCollisionFlags & 0xC000) != 0) ? 0x1000 : 0) | (mCollisionFlags & 0xFFFFCFFF);
    }

    if (mSpinAttackCounter > 1) {
        mCollisionFlags &= 0xFFFF3FFF;
    }
    if (mSpinAttackCounter < 0xFE) {
        mSpinAttackCounter++;
    }

    // Apply lethal splat radius
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        paint->splatInk(mPosition, cSpinAttackRadius, mTeamId);
    }
}

// 0x02266810: Decompiled vfunc_88 (Kraken damage calculation)
f64 PlayerKingSquid::vfunc_88(s32 param2, const s32* pTargetOwnerId) {
    // DAT_10055294: 0.0f default / self-check
    if (pTargetOwnerId && *pTargetOwnerId == static_cast<s32>(mTeamId)) {
        return 0.0;
    }

    // Base contact damage: 45.714f * 1.0f (or direct 45.7f)
    // Spin leap attack (param2 == 0xC or in cSpinAttack state): * 3.5f (DAT_10055528) = 160.0f
    f32 baseDamage = 45.714285f;
    f32 multiplier = 1.0f; // DAT_10055290

    if (param2 == 0xC || mState == KingSquidState::cSpinAttack) {
        multiplier = 3.5f; // DAT_10055528
    }

    return static_cast<f64>(baseDamage * multiplier);
}


void PlayerKingSquid::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mTeamId = 0;
    mState = KingSquidState::cInactive;
    mDurationTimer = 0;
    mStateTimer = 0;
    mScaleFactor = 1.0f;
    vfunc_1();
}

void PlayerKingSquid::activate(const sead::Vector3f& startPos, u32 teamId, s32 durationFrames) {
    mPosition = startPos;
    mTeamId = teamId;
    vfunc_3(durationFrames);
    mStateTimer = 0;
    mScaleFactor = 1.0f;
}

bool PlayerKingSquid::triggerSpinAttack() {
    if (mState != KingSquidState::cActive) {
        return false;
    }

    mState = KingSquidState::cSpinAttack;
    mStateTimer = 0;

    // Splat wide ink footprint on leap attack
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        paint->splatInk(mPosition, cSpinAttackRadius, mTeamId);
    }

    return true;
}

void PlayerKingSquid::updateMovement(const sead::Vector3f& moveDir, f32 speed) {
    if (mState != KingSquidState::cActive && mState != KingSquidState::cSpinAttack) {
        return;
    }

    mPosition.x += moveDir.x * speed;
    mPosition.z += moveDir.z * speed;

    // Continuous wide ink trail while sliding
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint && (mStateTimer % 4 == 0)) {
        paint->splatInk(mPosition, 1.8f, mTeamId);
    }
}

void PlayerKingSquid::update() {
    mStateTimer++;

    switch (mState) {
        case KingSquidState::cTransforming:
            mScaleFactor += 0.1f;
            if (mScaleFactor >= 2.5f) {
                mScaleFactor = 2.5f;
                mState = KingSquidState::cActive;
                mStateTimer = 0;
            }
            break;

        case KingSquidState::cActive:
            mDurationTimer--;
            if (mDurationTimer <= 0) {
                mState = KingSquidState::cReverting;
                mStateTimer = 0;
            }
            break;

        case KingSquidState::cSpinAttack:
            if (mStateTimer >= cSpinAttackDuration) {
                mState = KingSquidState::cActive;
                mStateTimer = 0;
            }
            mDurationTimer--;
            if (mDurationTimer <= 0) {
                mState = KingSquidState::cReverting;
                mStateTimer = 0;
            }
            break;

        case KingSquidState::cReverting:
            mScaleFactor -= 0.1f;
            if (mScaleFactor <= 1.0f) {
                mScaleFactor = 1.0f;
                mState = KingSquidState::cInactive;
                mStateTimer = 0;
            }
            break;

        case KingSquidState::cInactive:
        default:
            break;
    }
}

void PlayerKingSquid::draw() {
    if (mState != KingSquidState::cInactive) {
        GambitActor::draw();
    }
}

} // namespace Game
