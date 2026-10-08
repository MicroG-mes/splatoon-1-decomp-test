#include "Game/MapObj/Obj_Geyser.h"
#include "Game/System/AglParameter.h"
#include <cmath>
#include <algorithm>

namespace Game {

bool GeyserParams::load(const char* paramsPath) {
    hp = 0.30000001f;
    playerBindVel = 2.50000000f;
    playerBindLerpRate = 0.05000000f;
    bombCorePosOffsetY = 5.00000000f;
    bombCorePaintRadius = 45.00000000f;
    bombCoreDamageRadiusNear = 40.00000000f;
    bombCoreDamageNear = 0.80000001f;
    targetRadius = 15.00000000f;
    fountainHeight = 30.00000000f;

    if (paramsPath) {
        AglParameterObj obj;
        if (obj.loadFromFile(paramsPath)) {
            hp = obj.getFloat("mHp", hp);
            playerBindVel = obj.getFloat("mPlayerBindVel", playerBindVel);
            playerBindLerpRate = obj.getFloat("mPlayerBindLerpRate", playerBindLerpRate);
            bombCorePosOffsetY = obj.getFloat("mBombCorePosOffsetY", bombCorePosOffsetY);
            bombCorePaintRadius = obj.getFloat("mBombCorePaintRadius", bombCorePaintRadius);
            bombCoreDamageRadiusNear = obj.getFloat("mBombCoreDamageRadiusNear", bombCoreDamageRadiusNear);
            bombCoreDamageNear = obj.getFloat("mBombCoreDamageNear", bombCoreDamageNear);
            targetRadius = obj.getFloat("mTargetRadius", targetRadius);
            fountainHeight = obj.getFloat("mUmbrellaColOffHeight", fountainHeight);
        }
    }

    return true;
}

Obj_Geyser::Obj_Geyser()
    : mPosition(0.0f, 0.0f, 0.0f)
    , mState(GeyserState::cState_Closed)
    , mTeamId(0)
    , mCurrentDamage(0.0f)
    , mCurrentHeight(0.0f)
    , mEruptTimer(0) {
    mParams.load("content/Static/Obj_Geyser.params");
}

Obj_Geyser::~Obj_Geyser() {}

void Obj_Geyser::init(const sead::Vector3f& pos, u32 teamId) {
    mPosition = pos;
    mTeamId = teamId;
    mState = GeyserState::cState_Closed;
    mCurrentDamage = 0.0f;
    mCurrentHeight = 0.0f;
    mEruptTimer = 0;
    mParams.load("content/Static/Obj_Geyser.params");
}

bool Obj_Geyser::applyInkDamage(f32 damage, u32 teamId) {
    if (mState == GeyserState::cState_Closed) {
        mTeamId = teamId;
        mCurrentDamage += damage;
        if (mCurrentDamage >= mParams.hp) {
            mState = GeyserState::cState_Opening;
            mCurrentDamage = 0.0f;
            return true;
        }
    } else if (mState == GeyserState::cState_Erupting) {
        // Enemy ink can contest and force geyser closed
        if (teamId != mTeamId) {
            mCurrentDamage += damage;
            if (mCurrentDamage >= mParams.hp * 2.0f) {
                mState = GeyserState::cState_Closing;
                mCurrentDamage = 0.0f;
                return true;
            }
        } else {
            // Friendly ink replenishes eruption timer
            mEruptTimer = cDefaultEruptDuration;
        }
    }
    return false;
}

void Obj_Geyser::update() {
    switch (mState) {
        case GeyserState::cState_Closed:
            mCurrentHeight = 0.0f;
            break;

        case GeyserState::cState_Opening:
            mCurrentHeight += 1.5f; // Rapidly shoot upwards
            if (mCurrentHeight >= mParams.fountainHeight) {
                mCurrentHeight = mParams.fountainHeight;
                mState = GeyserState::cState_Erupting;
                mEruptTimer = cDefaultEruptDuration;
            }
            break;

        case GeyserState::cState_Erupting:
            mCurrentHeight = mParams.fountainHeight;
            if (mEruptTimer > 0) {
                mEruptTimer--;
            } else {
                mState = GeyserState::cState_Closing;
            }
            break;

        case GeyserState::cState_Closing:
            mCurrentHeight -= 1.0f;
            if (mCurrentHeight <= 0.0f) {
                mCurrentHeight = 0.0f;
                mState = GeyserState::cState_Closed;
            }
            break;
    }
}

bool Obj_Geyser::updatePlayerSwimAscent(sead::Vector3f& playerPos, f32& outVerticalVel) const {
    if (mState != GeyserState::cState_Erupting && mState != GeyserState::cState_Opening) {
        return false;
    }

    f32 dx = playerPos.x - mPosition.x;
    f32 dz = playerPos.z - mPosition.z;
    f32 distH = std::sqrt(dx * dx + dz * dz);

    if (distH <= mParams.targetRadius) {
        // Player is within geyser fountain cylinder
        if (playerPos.y >= mPosition.y && playerPos.y <= (mPosition.y + mCurrentHeight)) {
            // Apply upward swim velocity
            outVerticalVel = mParams.playerBindVel;
            playerPos.y += outVerticalVel;

            // Smoothly pull player toward cylinder center
            playerPos.x += (mPosition.x - playerPos.x) * mParams.playerBindLerpRate;
            playerPos.z += (mPosition.z - playerPos.z) * mParams.playerBindLerpRate;
            return true;
        }
    }

    return false;
}

} // namespace Game
