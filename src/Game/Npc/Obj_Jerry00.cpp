#include "Game/Npc/Obj_Jerry00.h"
#include <cmath>
#include <algorithm>

namespace Game {

Obj_Jerry00::Obj_Jerry00(JerryVariant variant)
    : mVariant(variant),
      mAnimState(JerryAnimState::cWaitRandom),
      mAnimFrame(0),
      mSquishRatio(1.0f),
      mBobbingOffset(0.0f),
      mCheerTimer(0) {
}

Obj_Jerry00::~Obj_Jerry00() {
}

void Obj_Jerry00::init() {
    GambitActor::init();
    vfunc_3();
    vfunc_5();
}

void Obj_Jerry00::vfunc_3() {
    mModel = sead::BfresParser::createJerryModel("Obj_Jerry00");
}

void Obj_Jerry00::vfunc_5() {
    mAnimFrame = 0;
    mSquishRatio = 1.0f;
    mBobbingOffset = 0.0f;
    mCheerTimer = 0;
}

void Obj_Jerry00::vfunc_7() {
    mAnimFrame++;

    if (mCheerTimer > 0) {
        mCheerTimer--;
        // Fast energetic cheer hop
        f32 hopPhase = static_cast<f32>(mAnimFrame % 20) / 20.0f;
        mBobbingOffset = std::sin(hopPhase * 3.14159265f) * 0.45f;
        mSquishRatio = 1.0f + std::sin(hopPhase * 6.2831853f) * 0.12f;
        return;
    }

    // Default spectator rhythmic ambient bob
    f32 phase = static_cast<f32>(mAnimFrame % 120) / 120.0f;
    mBobbingOffset = std::sin(phase * 6.2831853f) * 0.08f;

    // Gelatinous vertical elastic squish
    mSquishRatio = 1.0f + std::cos(phase * 6.2831853f) * 0.05f;
}

void Obj_Jerry00::update() {
    vfunc_7();
}

void Obj_Jerry00::draw() {
}

void Obj_Jerry00::setVariant(JerryVariant variant) {
    mVariant = variant;
}

void Obj_Jerry00::triggerCheer() {
    mCheerTimer = 90; // Cheer for 1.5 seconds (90 frames)
}

void Obj_Jerry00::setAnimState(JerryAnimState state) {
    mAnimState = state;
}

} // namespace Game
