#include "Game/MapObj/RollingLift.h"

namespace Game {

RollingLift::RollingLift()
    : mStartPoint(0.0f, 0.0f, 0.0f),
      mEndPoint(0.0f, 0.0f, 0.0f),
      mPosition(0.0f, 0.0f, 0.0f),
      mVelocity(0.0f, 0.0f, 0.0f),
      mProgress(0.0f),
      mSpeed(cDefaultTravelSpeed),
      mState(LiftTravelState::cMovingForward),
      mTimer(0),
      mIsPropellerDriven(false) {
}

RollingLift::~RollingLift() {
}

void RollingLift::init() {
    GambitActor::init();
    mPosition = mStartPoint;
    mVelocity.set(0.0f, 0.0f, 0.0f);
    mProgress = 0.0f;
    mSpeed = cDefaultTravelSpeed;
    mState = LiftTravelState::cMovingForward;
    mTimer = 0;
    mIsPropellerDriven = false;
}

void RollingLift::setTrackWaypoints(const sead::Vector3f& startPoint, const sead::Vector3f& endPoint) {
    mStartPoint = startPoint;
    mEndPoint = endPoint;
    mPosition = startPoint;
    mProgress = 0.0f;
}

void RollingLift::applyPropellerInk(f32 inkAmount) {
    mIsPropellerDriven = true;
    mProgress += inkAmount * 0.01f;
    if (mProgress > 1.0f) {
        mProgress = 1.0f;
    }
}

void RollingLift::stepMovement() {
    if (mIsPropellerDriven) {
        // Slowly drift back if not inked continuously
        if (mProgress > 0.0f) {
            mProgress -= 0.002f;
            if (mProgress < 0.0f) mProgress = 0.0f;
        }
    } else {
        // Automatic periodic motion
        switch (mState) {
            case LiftTravelState::cMovingForward:
                mProgress += mSpeed * 0.01f;
                if (mProgress >= 1.0f) {
                    mProgress = 1.0f;
                    mState = LiftTravelState::cPauseAtEnd;
                    mTimer = 0;
                }
                break;

            case LiftTravelState::cPauseAtEnd:
                mTimer++;
                if (mTimer >= cDefaultPauseFrames) {
                    mState = LiftTravelState::cMovingReverse;
                    mTimer = 0;
                }
                break;

            case LiftTravelState::cMovingReverse:
                mProgress -= mSpeed * 0.01f;
                if (mProgress <= 0.0f) {
                    mProgress = 0.0f;
                    mState = LiftTravelState::cPauseAtStart;
                    mTimer = 0;
                }
                break;

            case LiftTravelState::cPauseAtStart:
                mTimer++;
                if (mTimer >= cDefaultPauseFrames) {
                    mState = LiftTravelState::cMovingForward;
                    mTimer = 0;
                }
                break;
        }
    }

    sead::Vector3f oldPos = mPosition;

    // Linear interpolation between start and end
    mPosition.x = mStartPoint.x + (mEndPoint.x - mStartPoint.x) * mProgress;
    mPosition.y = mStartPoint.y + (mEndPoint.y - mStartPoint.y) * mProgress;
    mPosition.z = mStartPoint.z + (mEndPoint.z - mStartPoint.z) * mProgress;

    // Delta velocity to impart onto riders
    mVelocity.x = mPosition.x - oldPos.x;
    mVelocity.y = mPosition.y - oldPos.y;
    mVelocity.z = mPosition.z - oldPos.z;
}

void RollingLift::update() {
    stepMovement();
}

void RollingLift::draw() {
    GambitActor::draw();
}

} // namespace Game
