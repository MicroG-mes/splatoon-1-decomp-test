#include "Game/Weapon/GameWeaponRoller.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <algorithm>

namespace Game {

GameWeaponRoller::GameWeaponRoller()
    : mRollerType(RollerType::cNormal),
      mState(RollerState::cIdle),
      mStateTimer(0),
      mPaintWidth(1.8f),
      mRollSpeed(1.08f),
      mSquishDamage(125.0f),
      mFlingDamage(125.0f),
      mWindupFrames(20),
      mFlingRange(14.0f),
      mInkCostFling(0.09f),
      mInkCostRollPerFrame(0.0012f),
      mHasInk(true),
      mIsPainting(false),
      mSwingSpeedX(0.0f),
      mSwingSpeedY(0.0f),
      mPaintTimer(0.0f),
      mPaintOffFlag(0) {
}

GameWeaponRoller::~GameWeaponRoller() = default;

void GameWeaponRoller::init() {
    GambitActor::init();
    mState = RollerState::cIdle;
    mStateTimer = 0;
    vfunc_14();
}

/**
 * TankEmptyRoller__vfunc_14 @ 0x026D8908
 * Resets swing velocity components to zero.
 */
void GameWeaponRoller::vfunc_14() {
    mSwingSpeedX = 0.0f; // DAT_100f0624
    mSwingSpeedY = 0.0f;
    mPaintTimer = 0.0f;
    mPaintOffFlag = 0;
}

/**
 * TankEmptyRoller__vfunc_70 @ 0x026D95AC
 * Handles empty tank state transition during rolling.
 */
void GameWeaponRoller::vfunc_70(s32* pOutState) {
    if (!pOutState) {
        return;
    }

    s32 cur = static_cast<s32>(mState);
    if (cur == static_cast<s32>(RollerState::cRolling) && !mHasInk) {
        // Switch to empty tank / dry tap state
        *pOutState = static_cast<s32>(RollerState::cEmptyTank);
        mState = RollerState::cEmptyTank;
        return;
    }
    *pOutState = cur;
}

/**
 * FUN_026D8C38
 * Updates the paint timer, decrements by 1.0f (DAT_100f0640),
 * and triggers PaintOff when exhausted.
 */
void GameWeaponRoller::updatePaintTimer(u32 param2, u8 param3) {
    (void)param2;
    mSwingSpeedY = 0.0f; // Cleared in FUN_026d8c38 line 21

    f32 nextTimer = mPaintTimer - 1.0f; // DAT_100f0640
    if (nextTimer < 0.0f) {
        mPaintTimer = 0.0f; // Clamped to DAT_100f0624
        mIsPainting = false;
    } else {
        mPaintTimer = nextTimer;
        mIsPainting = true;
    }

    u8 invertedParam = param3 ^ 1;
    if (invertedParam != mPaintOffFlag) {
        mPaintOffFlag = invertedParam;
        if (invertedParam != 0) {
            // "PaintOff" event triggered at 0x026D8F80
            mIsPainting = false;
        }
    }
}

void GameWeaponRoller::setRollerType(RollerType type) {
    mRollerType = type;
    switch (type) {
        case RollerType::cCompact: // Carbon Roller
            mWindupFrames = 10;
            mRollSpeed = 1.34f;
            mSquishDamage = 70.0f;
            mFlingDamage = 100.0f;
            mFlingRange = 12.0f;
            mPaintWidth = 1.4f;
            mInkCostFling = 0.05f;
            mInkCostRollPerFrame = 0.0008f;
            break;

        case RollerType::cNormal: // Splat Roller
            mWindupFrames = 20;
            mRollSpeed = 1.08f;
            mSquishDamage = 125.0f;
            mFlingDamage = 125.0f;
            mFlingRange = 14.0f;
            mPaintWidth = 1.8f;
            mInkCostFling = 0.09f;
            mInkCostRollPerFrame = 0.0012f;
            break;

        case RollerType::cHeavy: // Dynamo Roller
            mWindupFrames = 48;
            mRollSpeed = 0.88f;
            mSquishDamage = 160.0f;
            mFlingDamage = 180.0f;
            mFlingRange = 22.0f;
            mPaintWidth = 2.4f;
            mInkCostFling = 0.18f;
            mInkCostRollPerFrame = 0.0020f;
            break;

        case RollerType::cBrushMini: // Inkbrush
            mWindupFrames = 6;
            mRollSpeed = 1.68f;
            mSquishDamage = 28.0f;
            mFlingDamage = 30.0f;
            mFlingRange = 8.0f;
            mPaintWidth = 0.8f;
            mInkCostFling = 0.02f;
            mInkCostRollPerFrame = 0.0004f;
            break;

        case RollerType::cBrushNormal: // Octobrush
            mWindupFrames = 10;
            mRollSpeed = 1.45f;
            mSquishDamage = 37.0f;
            mFlingDamage = 40.0f;
            mFlingRange = 11.0f;
            mPaintWidth = 1.1f;
            mInkCostFling = 0.035f;
            mInkCostRollPerFrame = 0.0006f;
            break;
    }
}

void GameWeaponRoller::updateInkConsumption(f32 currentInk) {
    mHasInk = (currentInk > 0.005f);
    if (!mHasInk && mState == RollerState::cRolling) {
        mState = RollerState::cEmptyTank;
        mRollSpeed = 0.35f;       // Drag penalty
        mSquishDamage = 25.0f;    // Non-lethal dry tap
    } else if (mHasInk && mState == RollerState::cEmptyTank) {
        setRollerType(mRollerType); // Restore base stats
        mState = RollerState::cRolling;
    }
}

void GameWeaponRoller::startFling() {
    if (mState == RollerState::cIdle) {
        mState = RollerState::cSwingWindup;
        mStateTimer = 0;
        mPaintTimer = static_cast<f32>(mWindupFrames);
    }
}

void GameWeaponRoller::startRolling() {
    if (mState == RollerState::cIdle) {
        mState = mHasInk ? RollerState::cRolling : RollerState::cEmptyTank;
        mStateTimer = 0;
        mIsPainting = mHasInk;
    }
}

void GameWeaponRoller::stopRolling() {
    if (mState == RollerState::cRolling || mState == RollerState::cEmptyTank) {
        mState = RollerState::cIdle;
        mStateTimer = 0;
        mIsPainting = false;
    }
}

void GameWeaponRoller::flingInkWave() {
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint && mHasInk) {
        paint->splatInk(sead::Vector3f(0.0f, 0.0f, mFlingRange * 0.4f), mPaintWidth * 1.2f, 0);
        paint->splatInk(sead::Vector3f(0.0f, 0.0f, mFlingRange * 0.8f), mPaintWidth * 0.9f, 0);
    }
}

void GameWeaponRoller::update() {
    mStateTimer++;

    switch (mState) {
        case RollerState::cSwingWindup:
            updatePaintTimer(0, 0);
            if (mStateTimer >= mWindupFrames) {
                mState = RollerState::cSwingRelease;
                mStateTimer = 0;
                flingInkWave();
            }
            break;

        case RollerState::cSwingRelease:
            if (mStateTimer >= 15) {
                mState = RollerState::cIdle;
            }
            break;

        case RollerState::cRolling: {
            mIsPainting = true;
            PaintTextureMgr* paint = PaintTextureMgr::instance();
            if (paint) {
                paint->splatInk(sead::Vector3f(0.0f, 0.0f, 0.0f), mPaintWidth * 0.5f, 0);
            }
            break;
        }

        case RollerState::cEmptyTank:
            mIsPainting = false;
            break;

        default:
            break;
    }
}

void GameWeaponRoller::draw() {
    GambitActor::draw();
}

} // namespace Game
