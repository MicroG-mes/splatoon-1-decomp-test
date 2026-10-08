#include "Game/System/SoundShapeMgr.h"
#include "Game/System/PcAudioDriver.h"
#include <cmath>

namespace Game {

SoundShapeMgr::SoundShapeMgr()
    : mListenerPos(0.0f, 0.0f, 0.0f),
      mListenerForward(0.0f, 0.0f, 1.0f),
      mIsSubmergedFilterActive(false),
      mLowpassCutoffHz(20000.0f),
      mMasterVolume(1.0f) {
}

SoundShapeMgr::~SoundShapeMgr() {
}

void SoundShapeMgr::init() {
    GambitActor::init();
    mListenerPos.set(0.0f, 0.0f, 0.0f);
    mListenerForward.set(0.0f, 0.0f, 1.0f);
    mIsSubmergedFilterActive = false;
    mLowpassCutoffHz = 20000.0f;
    mMasterVolume = 1.0f;
}

void SoundShapeMgr::setListenerTransform(const sead::Vector3f& listenerPos, const sead::Vector3f& forwardDir) {
    mListenerPos = listenerPos;
    mListenerForward = forwardDir;
}

void SoundShapeMgr::setInkImmersionFilter(bool isSubmergedInInk) {
    mIsSubmergedFilterActive = isSubmergedInInk;
    // Muffle audio when submerged inside squid ink
    mLowpassCutoffHz = isSubmergedInInk ? 950.0f : 20000.0f;
}

bool SoundShapeMgr::calculateSpatialAudio(const sead::Vector3f& emitterPos, f32 maxDist, f32& outVolume, f32& outPan) const {
    f32 dx = emitterPos.x - mListenerPos.x;
    f32 dy = emitterPos.y - mListenerPos.y;
    f32 dz = emitterPos.z - mListenerPos.z;
    f32 dist = std::sqrt(dx * dx + dy * dy + dz * dz);

    if (dist >= maxDist) {
        outVolume = 0.0f;
        outPan = 0.0f;
        return false;
    }

    // Linear distance volume rolloff
    f32 atten = 1.0f - (dist / maxDist);
    outVolume = atten * mMasterVolume;

    // Stereo panning based on cross product with listener forward direction
    // Listener right vector = (mListenerForward.z, 0, -mListenerForward.x)
    f32 rightX =  mListenerForward.z;
    f32 rightZ = -mListenerForward.x;

    f32 panDot = (dx * rightX + dz * rightZ) / (dist > 0.001f ? dist : 1.0f);
    // Clamp pan between -1.0 (Full Left) and 1.0 (Full Right)
    if (panDot < -1.0f) panDot = -1.0f;
    if (panDot >  1.0f) panDot =  1.0f;
    outPan = panDot;

    return true;
}

void SoundShapeMgr::play3dSound(u32 soundId, const sead::Vector3f& emitterPos, f32 volume, f32 maxDist) {
    f32 finalVol = 0.0f;
    f32 finalPan = 0.0f;
    if (calculateSpatialAudio(emitterPos, maxDist, finalVol, finalPan)) {
        finalVol *= volume;
        PcAudioDriver::instance().playSound(soundId, finalVol, finalPan, mIsSubmergedFilterActive);
    }
}

void SoundShapeMgr::update() {
}

void SoundShapeMgr::draw() {
}

} // namespace Game
