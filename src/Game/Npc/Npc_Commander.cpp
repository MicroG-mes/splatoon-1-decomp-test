#include "Game/Npc/Npc_Commander.h"
#include <cmath>
#include <algorithm>

namespace Game {

Npc_Commander::Npc_Commander()
    : mPosition(0.0f, 0.0f, 0.0f)
    , mState(CommanderState::cState_Wait)
    , mStateTimer(0)
    , mTalkCount(0)
    , mPlayerNearby(false)
{
    std::fill(mReserved, mReserved + sizeof(mReserved), 0);
}

Npc_Commander::~Npc_Commander() {
}

void Npc_Commander::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mState = CommanderState::cState_Wait;
    mStateTimer = 0;
    mTalkCount = 0;
    mPlayerNearby = false;
}

void Npc_Commander::vfunc_3() {
    // 0x0260d940: Model loading (Npc_Commander.szs, head, M_Eye, tex_mtx0)
}

void Npc_Commander::vfunc_5() {
    // 0x0260dc10: Parameter initialization from Npc_Commander_AnmItp.params
    mState = CommanderState::cState_Wait;
}

void Npc_Commander::vfunc_7() {
    // 0x0260e2c0: Dialogue state & animation update
    update();
}

void Npc_Commander::vfunc_9() {
    // 0x0260e524: Player proximity check & dialogue prompt trigger
}

bool Npc_Commander::checkPlayerProximity(const sead::Vector3f& playerPos) {
    f32 dx = playerPos.x - mPosition.x;
    f32 dy = playerPos.y - mPosition.y;
    f32 dz = playerPos.z - mPosition.z;
    f32 distSq = dx * dx + dy * dy + dz * dz;

    mPlayerNearby = (distSq <= cTalkRadius * cTalkRadius);
    return mPlayerNearby;
}

void Npc_Commander::startTalking() {
    mState = CommanderState::cState_Talk_St;
    mStateTimer = 0;
    mTalkCount++;
}

void Npc_Commander::endTalking() {
    mState = CommanderState::cState_Talk_Ed;
    mStateTimer = 0;
}

void Npc_Commander::triggerCaneFlick() {
    mState = CommanderState::cState_Flick;
    mStateTimer = 0;
}

void Npc_Commander::triggerHeroPose() {
    mState = CommanderState::cState_Pose_A;
    mStateTimer = 0;
}

void Npc_Commander::update() {
    mStateTimer++;

    switch (mState) {
        case CommanderState::cState_Talk_St: {
            if (mStateTimer >= cTalkStFrames) {
                mState = CommanderState::cState_Talk_A;
                mStateTimer = 0;
            }
            break;
        }

        case CommanderState::cState_Talk_Ed: {
            if (mStateTimer >= cTalkEdFrames) {
                mState = CommanderState::cState_Wait;
                mStateTimer = 0;
            }
            break;
        }

        case CommanderState::cState_Flick: {
            if (mStateTimer >= cFlickFrames) {
                mState = CommanderState::cState_Wait;
                mStateTimer = 0;
            }
            break;
        }

        case CommanderState::cState_Pose_A: {
            if (mStateTimer >= cPoseAFrames) {
                mState = CommanderState::cState_Wait;
                mStateTimer = 0;
            }
            break;
        }

        case CommanderState::cState_Wait:
        default:
            break;
    }
}

void Npc_Commander::draw() {
    // Model rendered via ModelSceneMgr
}

} // namespace Game
