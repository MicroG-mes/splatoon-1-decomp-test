#include "Game/MapObj/Obj_Geyser.h"
#include "Game/Paint/PaintTextureMgr.h"

namespace Game {

Obj_Geyser::Obj_Geyser()
    : mPosition(0.0f, 0.0f, 0.0f),
      mState(GeyserState::cClosed),
      mStateTimer(0),
      mActiveTeam(-1),
      mInkAccumulated(0.0f),
      mCurrentHeight(0.0f),
      mMaxHeight(cDefaultMaxHeight) {
}

Obj_Geyser::~Obj_Geyser() {
}

void Obj_Geyser::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mState = GeyserState::cClosed;
    mStateTimer = 0;
    mActiveTeam = -1;
    mInkAccumulated = 0.0f;
    mCurrentHeight = 0.0f;
    mMaxHeight = cDefaultMaxHeight;
}

void Obj_Geyser::applyInkHit(s32 teamId, f32 amount) {
    if (mState != GeyserState::cClosed) {
        return;
    }

    mInkAccumulated += amount;
    if (mInkAccumulated >= cActivationThreshold) {
        triggerEruption(teamId);
    }
}

void Obj_Geyser::triggerEruption(s32 teamId) {
    mState = GeyserState::cOpening;
    mStateTimer = 0;
    mActiveTeam = teamId;
    mInkAccumulated = 0.0f;

    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        paint->splatInk(mPosition, 4.0f, teamId >= 0 ? static_cast<u32>(teamId) : 0);
    }
}

void Obj_Geyser::update() {
    mStateTimer++;

    switch (mState) {
        case GeyserState::cClosed:
            // Decay accumulated ink over time if not continuously shot
            if (mInkAccumulated > 0.0f) {
                mInkAccumulated -= 0.1f;
                if (mInkAccumulated < 0.0f) {
                    mInkAccumulated = 0.0f;
                }
            }
            mCurrentHeight = 0.0f;
            break;

        case GeyserState::cOpening:
            // Fountain shoots rapidly upward
            mCurrentHeight += 0.4f;
            if (mCurrentHeight >= mMaxHeight) {
                mCurrentHeight = mMaxHeight;
                mState = GeyserState::cActive;
                mStateTimer = 0;
            }
            break;

        case GeyserState::cActive:
            mCurrentHeight = mMaxHeight;
            if (mStateTimer >= cActiveDuration) {
                mState = GeyserState::cClosing;
                mStateTimer = 0;
            }
            break;

        case GeyserState::cClosing:
            mCurrentHeight -= 0.2f;
            if (mCurrentHeight <= 0.0f) {
                mCurrentHeight = 0.0f;
                mState = GeyserState::cCooldown;
                mStateTimer = 0;
            }
            break;

        case GeyserState::cCooldown:
            if (mStateTimer >= cCooldownDuration) {
                mState = GeyserState::cClosed;
                mActiveTeam = -1;
                mStateTimer = 0;
            }
            break;
    }
}

void Obj_Geyser::draw() {
    GambitActor::draw();
}

} // namespace Game
