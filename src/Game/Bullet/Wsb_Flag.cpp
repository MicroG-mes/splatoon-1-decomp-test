#include "Game/Bullet/Wsb_Flag.h"

namespace Game {

Wsb_Flag::Wsb_Flag()
    : mPosition(0.0f, 0.0f, 0.0f),
      mTeamId(0),
      mOwnerPlayerId(0),
      mHp(cMaxHp),
      mState(BeakonState::cDestroyed),
      mTimer(0),
      mPingTimer(0) {
}

Wsb_Flag::~Wsb_Flag() {
}

void Wsb_Flag::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mTeamId = 0;
    mOwnerPlayerId = 0;
    mHp = cMaxHp;
    mState = BeakonState::cDestroyed;
    mTimer = 0;
    mPingTimer = 0;
}

void Wsb_Flag::deploy(const sead::Vector3f& pos, u32 teamId, u32 ownerPlayerId) {
    mPosition = pos;
    mTeamId = teamId;
    mOwnerPlayerId = ownerPlayerId;
    mHp = cMaxHp;
    mState = BeakonState::cPlacing;
    mTimer = 0;
    mPingTimer = 0;
}

void Wsb_Flag::applyDamage(f32 damage) {
    if (mState != BeakonState::cActive) {
        return;
    }

    mHp -= damage;
    if (mHp <= 0.0f) {
        mHp = 0.0f;
        mState = BeakonState::cDestroyed;
        mTimer = 0;
    }
}

void Wsb_Flag::consumeOnJumpLanding() {
    if (mState == BeakonState::cActive) {
        // Disintegrates after landing safely
        mState = BeakonState::cDestroyed;
        mTimer = 0;
    }
}

void Wsb_Flag::update() {
    mTimer++;

    switch (mState) {
        case BeakonState::cPlacing:
            if (mTimer >= 20) {
                mState = BeakonState::cActive;
                mTimer = 0;
            }
            break;

        case BeakonState::cActive:
            mPingTimer++;
            if (mPingTimer >= cPingIntervalFrames) {
                mPingTimer = 0;
                // Periodic radar ping emission
            }
            break;

        case BeakonState::cDestroyed:
        default:
            break;
    }
}

void Wsb_Flag::draw() {
    if (mState == BeakonState::cPlacing || mState == BeakonState::cActive) {
        GambitActor::draw();
    }
}

} // namespace Game
