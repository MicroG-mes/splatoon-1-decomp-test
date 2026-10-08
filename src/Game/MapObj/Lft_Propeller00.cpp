#include "Game/MapObj/Lft_Propeller00.h"
#include <cmath>

namespace Game {

Lft_Propeller00::Lft_Propeller00()
    : mStartPos(0.0f, 0.0f, 0.0f),
      mEndPos(0.0f, 0.0f, 0.0f),
      mPosition(0.0f, 0.0f, 0.0f),
      mState(PropellerLiftState::cIdle),
      mProgress(0.0f),
      mPropellerRpm(0.0f),
      mPropellerAngle(0.0f),
      mTimer(0) {
}

Lft_Propeller00::~Lft_Propeller00() {
}

void Lft_Propeller00::init() {
    GambitActor::init();
    mStartPos.set(0.0f, 0.0f, 0.0f);
    mEndPos.set(0.0f, 0.0f, 0.0f);
    mPosition.set(0.0f, 0.0f, 0.0f);
    mState = PropellerLiftState::cIdle;
    mProgress = 0.0f;
    mPropellerRpm = 0.0f;
    mPropellerAngle = 0.0f;
    mTimer = 0;
}

void Lft_Propeller00::setTrack(const sead::Vector3f& startPos, const sead::Vector3f& endPos) {
    mStartPos = startPos;
    mEndPos = endPos;
    mPosition = startPos;
    mProgress = 0.0f;
    mState = PropellerLiftState::cIdle;
}

void Lft_Propeller00::hitPropeller(f32 inkPower) {
    mPropellerRpm += inkPower * 15.0f;
    if (mPropellerRpm > cMaxRpm) {
        mPropellerRpm = cMaxRpm;
    }
}

void Lft_Propeller00::update() {
    // Rotate propeller according to RPM
    f32 angleDelta = (mPropellerRpm / 60.0f) * 6.2831853f;
    mPropellerAngle += angleDelta;
    if (mPropellerAngle > 6.2831853f) {
        mPropellerAngle -= 6.2831853f;
    }

    // RPM decay
    mPropellerRpm *= cRpmDecay;
    if (mPropellerRpm < 0.05f) {
        mPropellerRpm = 0.0f;
    }

    // Lift progress logic
    if (mPropellerRpm > 1.0f) {
        mState = PropellerLiftState::cMoving;
        mProgress += mPropellerRpm * cMoveSpeedFactor;
        if (mProgress > 1.0f) {
            mProgress = 1.0f;
        }
    } else if (mProgress > 0.0f) {
        mState = PropellerLiftState::cReturning;
        mProgress -= cReturnSpeed;
        if (mProgress < 0.0f) {
            mProgress = 0.0f;
            mState = PropellerLiftState::cIdle;
        }
    } else {
        mState = PropellerLiftState::cIdle;
    }

    // Interpolate world position
    mPosition.x = mStartPos.x + (mEndPos.x - mStartPos.x) * mProgress;
    mPosition.y = mStartPos.y + (mEndPos.y - mStartPos.y) * mProgress;
    mPosition.z = mStartPos.z + (mEndPos.z - mStartPos.z) * mProgress;
}

void Lft_Propeller00::draw() {
    // Model rendering handled by ModelSceneMgr
}

} // namespace Game
