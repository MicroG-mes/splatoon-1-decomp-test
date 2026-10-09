#include "Game/Npc/Npc_CommanderBind.h"
#include <cmath>
#include <algorithm>

namespace Game {

Npc_CommanderBind::Npc_CommanderBind()
    : mPosition(0.0f, 0.0f, 0.0f)
    , mState(CommanderBindState::cState_Wait)
    , mStateTimer(0)
    , mDancePhase(0.0f)
{
    std::fill(mReserved, mReserved + sizeof(mReserved), 0);
}

Npc_CommanderBind::~Npc_CommanderBind() {
}

void Npc_CommanderBind::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mState = CommanderBindState::cState_Wait;
    mStateTimer = 0;
    mDancePhase = 0.0f;
}

void Npc_CommanderBind::vfunc_3() {
    // 0x0260f964: Model loading (Npc_CommanderBind.szs)
}

void Npc_CommanderBind::vfunc_5() {
    // 0x0260fcc4: Bound state initialization (Npc_CommanderBind_AnmItp.params)
    mState = CommanderBindState::cState_Wait;
}

void Npc_CommanderBind::vfunc_7() {
    // 0x0260fd6c: State update & dancing cheer
    update();
}

void Npc_CommanderBind::vfunc_9() {
    // 0x0260fda0: Bullet touch & rescue collision
}

void Npc_CommanderBind::vfunc_11() {
    // 0x0260fda4: Break / release event
    triggerRescueBreak();
}

void Npc_CommanderBind::startCheeringDance() {
    mState = CommanderBindState::cState_LastBossDance;
    mStateTimer = 0;
    mDancePhase = 0.0f;
}

void Npc_CommanderBind::triggerRescueBreak() {
    mState = CommanderBindState::cState_Break;
    mStateTimer = 0;
}

void Npc_CommanderBind::update() {
    mStateTimer++;

    switch (mState) {
        case CommanderBindState::cState_LastBossDance: {
            // Grooving to Calamari Inkantation (6 frames per cycle)
            mDancePhase += 0.2f;
            break;
        }

        case CommanderBindState::cState_Break: {
            if (mStateTimer >= cBreakFrames) {
                mState = CommanderBindState::cState_ReleaseCommander;
                mStateTimer = 0;
            }
            break;
        }

        case CommanderBindState::cState_ReleaseCommander: {
            if (mStateTimer >= 10) {
                mState = CommanderBindState::cState_Landing;
                mStateTimer = 0;
            }
            break;
        }

        default:
            break;
    }
}

void Npc_CommanderBind::draw() {
    // Model rendered via ModelSceneMgr
}

} // namespace Game
