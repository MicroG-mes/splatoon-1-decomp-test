#include "Game/MapObj/Lft_Propeller00.h"
#include <cmath>
#include <algorithm>

namespace Game {

Lft_Propeller00::Lft_Propeller00()
    : mStartPos(0.0f, 0.0f, 0.0f),
      mEndPos(0.0f, 0.0f, 0.0f),
      mPosition(0.0f, 0.0f, 0.0f),
      mState(PropellerLiftState::cWait),
      mProgress(0.0f),
      mPropellerRpm(0.0f),
      mPropellerAngle(0.0f),
      mTargetProgress(0.0f),
      mTimer(0),
      mLastHitTeam(0) {
}

Lft_Propeller00::~Lft_Propeller00() {
}

void Lft_Propeller00::init() {
    GambitActor::init();
    mPosition = mStartPos;
    mState = PropellerLiftState::cWait;
    mProgress = 0.0f;
    mPropellerRpm = 0.0f;
    mPropellerAngle = 0.0f;
    mTargetProgress = 0.0f;
    mTimer = 0;
    mLastHitTeam = 0;
}

void Lft_Propeller00::vfunc_1() {
    // Teardown
}

void Lft_Propeller00::vfunc_3() {
    // Model & resource load (Lft_Propeller00.szs, Lft_Propeller01.szs)
}

void Lft_Propeller00::vfunc_5() {
    // Light & bone setup (Screw, Slave, neck_root, vernier00, propeller_root)
}

void Lft_Propeller00::vfunc_7() {
    // Winch movement update
    update();
}

void Lft_Propeller00::vfunc_9() {
    // Linear track evaluation
}

void Lft_Propeller00::vfunc_11() {
    // KCL collision transform update
}

void Lft_Propeller00::vfunc_14() {
    // Ink impulse receiver
}

void Lft_Propeller00::vfunc_15() {
    // Player ride attachment
}

void Lft_Propeller00::vfunc_35() {
    // Switch event broadcast
}

void Lft_Propeller00::vfunc_39() {
    // Sound effect trigger (FastUp, Vernier)
}

void Lft_Propeller00::vfunc_47() {
    // Animation & rotation sync
}

void Lft_Propeller00::vfunc_56() {
    // Audio pitch modulation
}

void Lft_Propeller00::setTrack(const sead::Vector3f& startPos, const sead::Vector3f& endPos) {
    mStartPos = startPos;
    mEndPos = endPos;
    mPosition = startPos;
    mProgress = 0.0f;
    mState = PropellerLiftState::cStartPoint;
}

void Lft_Propeller00::hitPropeller(f32 inkPower, u32 teamId) {
    mPropellerRpm += inkPower * cInkImpulseMult;
    if (mPropellerRpm > cMaxRpm) {
        mPropellerRpm = cMaxRpm;
    }
    mLastHitTeam = teamId;
}

void Lft_Propeller00::update() {
    // Rotate propeller according to RPM
    f32 angleDelta = (mPropellerRpm / 60.0f) * 6.2831853f;
    mPropellerAngle += angleDelta;
    if (mPropellerAngle > 6.2831853f) {
        mPropellerAngle -= 6.2831853f;
    }

    // RPM decay friction
    mPropellerRpm *= cRpmDecay;
    if (mPropellerRpm < 0.05f) {
        mPropellerRpm = 0.0f;
    }

    // Lift progress logic driven by propeller spin
    if (mPropellerRpm > cMinMoveRpm) {
        mProgress += mPropellerRpm * cMoveSpeedFactor;
        if (mProgress >= 1.0f) {
            mProgress = 1.0f;
            mState = PropellerLiftState::cEndPoint;
        } else {
            mState = PropellerLiftState::cMove;
        }
    } else if (mProgress > 0.0f) {
        mState = PropellerLiftState::cMoveReturn;
        mProgress -= cReturnSpeed;
        if (mProgress <= 0.0f) {
            mProgress = 0.0f;
            mState = PropellerLiftState::cStartPoint;
        }
    } else {
        mState = PropellerLiftState::cWait;
    }

    // Interpolate world position along track
    mPosition.x = mStartPos.x + (mEndPos.x - mStartPos.x) * mProgress;
    mPosition.y = mStartPos.y + (mEndPos.y - mStartPos.y) * mProgress;
    mPosition.z = mStartPos.z + (mEndPos.z - mStartPos.z) * mProgress;

    mTimer++;
}

void Lft_Propeller00::draw() {
    // Model rendering handled by ModelSceneMgr
}

} // namespace Game
