#include "Game/MiniGame/Obj_PlazaGame.h"
#include <cmath>

namespace Game {

Obj_PlazaGame::Obj_PlazaGame()
    : mPosition(-12.5f, 0.0f, -4.2f),
      mInteractRadius(2.5f),
      mScreenGlow(1.0f),
      mAttractFrame(0),
      mIsPlayerNearby(false),
      mIsPlaying(false),
      mActiveGame(MiniGameType::cSquidJump) {
    for (int i = 0; i < 4; ++i) {
        mHighScores[i] = 0;
        mGameUnlocked[i] = (i == 0); // Squid Jump unlocked by default
    }
}

Obj_PlazaGame::~Obj_PlazaGame() = default;

void Obj_PlazaGame::init() {
    GambitActor::init();
    mScreenGlow = 1.0f;
    mAttractFrame = 0;
    mIsPlayerNearby = false;
    mIsPlaying = false;
    mActiveGame = MiniGameType::cSquidJump;
    for (int i = 0; i < 4; ++i) {
        mHighScores[i] = 0;
        mGameUnlocked[i] = (i == 0);
    }
}

void Obj_PlazaGame::setup(const sead::Vector3f& cabinetPos) {
    mPosition = cabinetPos;
    mScreenGlow = 1.0f;
    mAttractFrame = 0;
}

void Obj_PlazaGame::update() {
    GambitActor::update();
    vfunc_7();
}

void Obj_PlazaGame::draw() {
    GambitActor::draw();
}

void Obj_PlazaGame::vfunc_3() {
    mAttractFrame = 0;
    mScreenGlow = 1.0f;
}

/**
 * Obj_PlazaGame__vfunc_7 @ 0x0259CF58
 * Attract mode CRT flicker and screen animation.
 */
void Obj_PlazaGame::vfunc_7() {
    mAttractFrame++;
    // Subtle CRT phosphor flicker
    mScreenGlow = 0.90f + 0.10f * std::sin(mAttractFrame * 0.08f);
}

f32 Obj_PlazaGame::vfunc_11() {
    return mScreenGlow;
}

bool Obj_PlazaGame::vfunc_47() {
    if (mIsPlayerNearby && !mIsPlaying) {
        return launchGame(MiniGameType::cSquidJump);
    }
    return false;
}

bool Obj_PlazaGame::checkPlayerProximity(const sead::Vector3f& playerPos, f32 interactRadius) {
    updateProximity(playerPos, interactRadius);
    return mIsPlayerNearby;
}

void Obj_PlazaGame::updateProximity(const sead::Vector3f& playerPos, f32 interactRadius) {
    f32 dx = playerPos.x - mPosition.x;
    f32 dz = playerPos.z - mPosition.z;
    f32 dist = std::sqrt(dx * dx + dz * dz);
    mIsPlayerNearby = (dist <= interactRadius);
}

void Obj_PlazaGame::triggerGame() {
    if (mIsPlayerNearby) {
        launchGame(MiniGameType::cSquidJump);
    }
}

bool Obj_PlazaGame::launchGame(MiniGameType game) {
    u32 idx = static_cast<u32>(game);
    if (idx >= 4 || !mGameUnlocked[idx]) return false;

    mIsPlaying = true;
    mActiveGame = game;
    return true;
}

void Obj_PlazaGame::exitGame() {
    mIsPlaying = false;
}

void Obj_PlazaGame::submitHighScore(MiniGameType game, u32 score) {
    u32 idx = static_cast<u32>(game);
    if (idx < 4 && score > mHighScores[idx]) {
        mHighScores[idx] = score;
    }
}

u32 Obj_PlazaGame::getHighScore(MiniGameType game) const {
    u32 idx = static_cast<u32>(game);
    return (idx < 4) ? mHighScores[idx] : 0;
}

} // namespace Game
