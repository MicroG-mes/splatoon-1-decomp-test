#include "Game/MapObj/Trap.h"
#include "Game/Paint/PaintTextureMgr.h"

namespace Game {

Trap::Trap()
    : mPosition(0.0f, 0.0f, 0.0f),
      mTeamId(0),
      mOwnerPlayerId(0),
      mState(TrapState::cDefunct),
      mTimer(0) {
}

Trap::~Trap() {
}

void Trap::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mTeamId = 0;
    mOwnerPlayerId = 0;
    mState = TrapState::cDefunct;
    mTimer = 0;
}

void Trap::place(const sead::Vector3f& pos, u32 teamId, u32 ownerPlayerId) {
    mPosition = pos;
    mTeamId = teamId;
    mOwnerPlayerId = ownerPlayerId;
    mState = TrapState::cPlacing;
    mTimer = 0;
}

bool Trap::checkProximityTrigger(const sead::Vector3f& enemyPos, u32 enemyTeamId) {
    if (mState != TrapState::cArmed || enemyTeamId == mTeamId) {
        return false;
    }

    f32 dx = enemyPos.x - mPosition.x;
    f32 dz = enemyPos.z - mPosition.z;
    f32 distSq = dx * dx + dz * dz;

    if (distSq <= cTriggerRadius * cTriggerRadius) {
        triggerDetonation();
        return true;
    }

    return false;
}

void Trap::triggerDetonation() {
    if (mState == TrapState::cArmed) {
        mState = TrapState::cWarning;
        mTimer = 0;
    }
}

void Trap::update() {
    mTimer++;

    switch (mState) {
        case TrapState::cPlacing:
            if (mTimer >= cArmDelayFrames) {
                mState = TrapState::cArmed;
                mTimer = 0;
            }
            break;

        case TrapState::cArmed:
            // Mine remains dormant under ink
            break;

        case TrapState::cWarning:
            if (mTimer >= cWarningFrames) {
                mState = TrapState::cDetonating;
                mTimer = 0;

                // Ink detonation burst
                PaintTextureMgr* paint = PaintTextureMgr::instance();
                if (paint) {
                    paint->splatInk(mPosition, cExplosionRadius, mTeamId);
                }
            }
            break;

        case TrapState::cDetonating:
            if (mTimer >= 20) {
                mState = TrapState::cDefunct;
                mTimer = 0;
            }
            break;

        case TrapState::cDefunct:
        default:
            break;
    }
}

void Trap::draw() {
    if (mState == TrapState::cPlacing || mState == TrapState::cWarning) {
        GambitActor::draw();
    }
}

} // namespace Game
