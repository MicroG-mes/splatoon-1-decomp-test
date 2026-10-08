#include "Game/PlazaEnv/PlazaSpeakerSpeaker.h"

namespace Game {

PlazaSpeakerSpeaker::PlazaSpeakerSpeaker()
    : mBpm(128.0f),
      mBeatPhase(0.0f),
      mPulseScale(1.0f),
      mAnimTimer(0) {
}

PlazaSpeakerSpeaker::~PlazaSpeakerSpeaker() {
}

void PlazaSpeakerSpeaker::init() {
    GambitActor::init();
    mPulseScale = 1.0f;
    mBeatPhase = 0.0f;
}

void PlazaSpeakerSpeaker::setBpm(f32 bpm) {
    mBpm = bpm > 0.0f ? bpm : 128.0f;
}

void PlazaSpeakerSpeaker::update() {
    mAnimTimer++;

    // Advance beat phase based on BPM
    f32 beatsPerSecond = mBpm / 60.0f;
    f32 phaseDelta = (beatsPerSecond / 60.0f) * 6.2831853f; // 60 FPS
    mBeatPhase += phaseDelta;
    if (mBeatPhase > 6.2831853f) {
        mBeatPhase -= 6.2831853f;
    }

    // Pulse speaker diaphragm on beat
    f32 sinVal = __builtin_sinf(mBeatPhase);
    if (sinVal > 0.0f) {
        mPulseScale = 1.0f + sinVal * 0.15f; // Expand up to 1.15x
    } else {
        mPulseScale = 1.0f;
    }
}

void PlazaSpeakerSpeaker::draw() {
    GambitActor::draw();
}

} // namespace Game
