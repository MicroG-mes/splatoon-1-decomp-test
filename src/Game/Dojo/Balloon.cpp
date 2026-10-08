#include "Game/Dojo/Balloon.h"
#include "Game/Paint/PaintTextureMgr.h"

namespace Game {

Balloon::Balloon()
    : mPosition(0.0f, 0.0f, 0.0f),
      mBasePosition(0.0f, 0.0f, 0.0f),
      mType(BalloonType::cRegular),
      mState(BalloonState::cInactive),
      mTimer(0),
      mFloatBobPhase(0.0f),
      mLastPoppedBy(0) {
}

Balloon::~Balloon() {
}

void Balloon::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mBasePosition.set(0.0f, 0.0f, 0.0f);
    mType = BalloonType::cRegular;
    mState = BalloonState::cInactive;
    mTimer = 0;
    mFloatBobPhase = 0.0f;
    mLastPoppedBy = 0;
}

void Balloon::spawn(const sead::Vector3f& spawnPos, BalloonType type) {
    mBasePosition = spawnPos;
    mPosition = spawnPos;
    mType = type;
    mState = BalloonState::cSpawning;
    mTimer = 0;
    mFloatBobPhase = 0.0f;
    mLastPoppedBy = 0;
}

bool Balloon::pop(u32 poppingPlayerId) {
    if (mState != BalloonState::cFloating && mState != BalloonState::cSpawning) {
        return false;
    }

    mState = BalloonState::cPopped;
    mLastPoppedBy = poppingPlayerId;
    mTimer = 0;

    // Splash small ink burst at balloon coordinates
    PaintTextureMgr* paintMgr = PaintTextureMgr::instance();
    if (paintMgr) {
        paintMgr->splatInk(mPosition, 2.5f, (poppingPlayerId == 0) ? 0 : 1);
    }

    return true;
}

void Balloon::update() {
    mTimer++;

    switch (mState) {
        case BalloonState::cSpawning:
            if (mTimer >= 30) {
                mState = BalloonState::cFloating;
                mTimer = 0;
            }
            break;

        case BalloonState::cFloating:
            // Bob up and down gently in the air
            mFloatBobPhase += 0.05f;
            mPosition.y = mBasePosition.y + (0.35f * ((mTimer % 60 < 30) ? 1.0f : -1.0f));
            break;

        case BalloonState::cPopped:
            if (mTimer >= 45) {
                mState = BalloonState::cInactive;
                mTimer = 0;
            }
            break;

        case BalloonState::cInactive:
        default:
            break;
    }
}

void Balloon::draw() {
    if (mState == BalloonState::cFloating || mState == BalloonState::cSpawning) {
        GambitActor::draw();
    }
}

} // namespace Game
