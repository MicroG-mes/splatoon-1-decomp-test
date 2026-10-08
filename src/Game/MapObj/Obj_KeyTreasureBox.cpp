#include "Game/MapObj/Obj_KeyTreasureBox.h"
#include "Game/System/AglParameter.h"

namespace Game {

bool KeyTreasureBoxParams::load(const char* paramsPath) {
    colOffFrame = 15;
    openStFrame = 30;

    if (paramsPath) {
        AglParameterObj obj;
        if (obj.loadFromFile(paramsPath)) {
            colOffFrame = obj.getInt("mColOffFrame", colOffFrame);
            openStFrame = obj.getInt("mOpenStFrame", openStFrame);
        }
    }

    return true;
}

Obj_KeyTreasureBox::Obj_KeyTreasureBox()
    : mState(TreasureBoxState::cState_Locked)
    , mRequiresKey(true)
    , mIsCollisionActive(true)
    , mRewardSpawned(false)
    , mRewardType("SunkenScroll")
    , mOpenTimer(0) {
    mPosition.set(0.0f, 0.0f, 0.0f);
    mParams.load("content/Static/Obj_KeyTreasureBox.params");
}

Obj_KeyTreasureBox::~Obj_KeyTreasureBox() {}

void Obj_KeyTreasureBox::init() {
    init(mPosition, true, "SunkenScroll");
}

void Obj_KeyTreasureBox::init(const sead::Vector3f& pos, bool requiresKey, const std::string& rewardType) {
    mPosition = pos;
    mRequiresKey = requiresKey;
    mRewardType = rewardType;
    mState = requiresKey ? TreasureBoxState::cState_Locked : TreasureBoxState::cState_Unlocking;
    mIsCollisionActive = true;
    mRewardSpawned = false;
    mOpenTimer = 0;
    mParams.load("content/Static/Obj_KeyTreasureBox.params");
}

bool Obj_KeyTreasureBox::tryUnlock(bool hasKey) {
    if (mState != TreasureBoxState::cState_Locked && mState != TreasureBoxState::cState_Unlocking) {
        return false;
    }

    if (mRequiresKey && !hasKey) {
        return false;
    }

    mState = TreasureBoxState::cState_Opening;
    mOpenTimer = 0;
    return true;
}

void Obj_KeyTreasureBox::update() {
    if (mState == TreasureBoxState::cState_Opening) {
        mOpenTimer++;

        // Timed collision cutoff
        if (mOpenTimer >= mParams.colOffFrame) {
            mIsCollisionActive = false;
        }

        // Timed open completion and reward spawning
        if (mOpenTimer >= mParams.openStFrame) {
            mState = TreasureBoxState::cState_Opened;
            mRewardSpawned = true;
        }
    }
}

} // namespace Game
