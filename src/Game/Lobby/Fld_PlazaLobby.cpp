#include "Game/Lobby/Fld_PlazaLobby.h"

namespace Game {

Fld_PlazaLobby::Fld_PlazaLobby()
    : mDoorPosition(0.0f, 0.0f, -28.0f),
      mTriggerRadius(3.5f),
      mDoorState(LobbyDoorState::cClosed),
      mStateTimer(0),
      mDoorSlideProgress(0.0f) {
}

Fld_PlazaLobby::~Fld_PlazaLobby() {
}

void Fld_PlazaLobby::init() {
    GambitActor::init();
    resetDoor();
}

void Fld_PlazaLobby::resetDoor() {
    mDoorState = LobbyDoorState::cClosed;
    mStateTimer = 0;
    mDoorSlideProgress = 0.0f;
}

bool Fld_PlazaLobby::checkEntranceTrigger(const sead::Vector3f& playerPos) {
    f32 dx = playerPos.x - mDoorPosition.x;
    f32 dy = playerPos.y - mDoorPosition.y;
    f32 dz = playerPos.z - mDoorPosition.z;
    f32 distSq = dx * dx + dy * dy + dz * dz;

    if (distSq <= mTriggerRadius * mTriggerRadius) {
        if (mDoorState == LobbyDoorState::cClosed) {
            mDoorState = LobbyDoorState::cOpening;
            mStateTimer = 0;
        }
        return true;
    } else {
        if (mDoorState == LobbyDoorState::cOpened && mStateTimer > 60) {
            // Close door when player walks away
            mDoorState = LobbyDoorState::cClosed;
            mDoorSlideProgress = 0.0f;
        }
    }
    return false;
}

void Fld_PlazaLobby::enterLobby() {
    mDoorState = LobbyDoorState::cPlayerEntering;
    mStateTimer = 0;
}

void Fld_PlazaLobby::update() {
    mStateTimer++;

    switch (mDoorState) {
        case LobbyDoorState::cOpening:
            mDoorSlideProgress += 0.08f;
            if (mDoorSlideProgress >= 1.0f) {
                mDoorSlideProgress = 1.0f;
                mDoorState = LobbyDoorState::cOpened;
                mStateTimer = 0;
            }
            break;

        case LobbyDoorState::cPlayerEntering:
            // Walk into elevator transition
            if (mStateTimer > 45) {
                mDoorState = LobbyDoorState::cTransitionDone;
            }
            break;

        default:
            break;
    }
}

void Fld_PlazaLobby::draw() {
    GambitActor::draw();
}

} // namespace Game
