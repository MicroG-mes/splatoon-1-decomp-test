#include "Game/Camera/CameraSequenceDirector.h"
#include <cmath>
#include <algorithm>

namespace Game {

CameraSequenceDirector::CameraSequenceDirector() {
    mParamEngine = &mDefaultParamEngine;
}

bool CameraSequenceDirector::init(CameraParamEngine* paramEngine) {
    if (paramEngine) {
        mParamEngine = paramEngine;
    } else {
        mDefaultParamEngine.loadDirectory("content/Static");
        mParamEngine = &mDefaultParamEngine;
    }
    return mParamEngine != nullptr && mParamEngine->getLoadedCount() > 0;
}

void CameraSequenceDirector::clearKeyframes() {
    mKeyframes.clear();
    mCurrentKeyframeIdx = 0;
    mCurrentFrame = 0;
    mTotalFrames = 0;
    mFrameInKeyframe = 0;
    mIsPlaying = false;
    mIsFinished = false;
}

void CameraSequenceDirector::addKeyframe(const CameraKeyframe& kf) {
    mKeyframes.push_back(kf);
    mTotalFrames += kf.durationFrames;
}

bool CameraSequenceDirector::startSequence(CameraSequenceType type, const std::string& stageName) {
    clearKeyframes();
    mCurrentType = type;

    switch (type) {
        case CameraSequenceType::MatchIntro:
            buildMatchIntroSequence();
            break;
        case CameraSequenceType::PostGameMatchSweep:
            buildPostGameSweepSequence(stageName);
            break;
        case CameraSequenceType::TutorialSuperJump:
            buildTutorialSuperJumpSequence();
            break;
        case CameraSequenceType::OctoValleyIntro:
            buildOctoValleyIntroSequence();
            break;
        case CameraSequenceType::BossIntroOctavio:
            buildBossIntroOctavioSequence();
            break;
        case CameraSequenceType::StaffRollCredits:
            buildStaffRollSequence();
            break;
        default:
            return false;
    }

    if (mKeyframes.empty()) {
        return false;
    }

    mIsPlaying = true;
    mIsFinished = false;
    mCurrentFrame = 0;
    mCurrentKeyframeIdx = 0;
    mFrameInKeyframe = 0;

    mActiveCameraTV = mKeyframes[0].params;
    updateInterpolation();
    return true;
}

void CameraSequenceDirector::stopSequence() {
    mIsPlaying = false;
    mIsFinished = true;
}

void CameraSequenceDirector::update(float frameDelta) {
    if (!mIsPlaying || mIsFinished) {
        return;
    }

    int delta = static_cast<int>(std::max(1.0f, frameDelta));
    mCurrentFrame += delta;
    mFrameInKeyframe += delta;

    if (mCurrentFrame >= mTotalFrames) {
        mCurrentFrame = mTotalFrames;
        mIsFinished = true;
        mIsPlaying = false;
        if (!mKeyframes.empty()) {
            mActiveCameraTV = mKeyframes.back().params;
        }
        return;
    }

    // Advance keyframe index if duration exceeded
    while (mCurrentKeyframeIdx < mKeyframes.size() &&
           mFrameInKeyframe >= mKeyframes[mCurrentKeyframeIdx].durationFrames) {
        mFrameInKeyframe -= mKeyframes[mCurrentKeyframeIdx].durationFrames;
        ++mCurrentKeyframeIdx;
    }

    if (mCurrentKeyframeIdx >= mKeyframes.size()) {
        mCurrentKeyframeIdx = mKeyframes.size() - 1;
    }

    updateInterpolation();
}

float CameraSequenceDirector::getProgress() const {
    if (mTotalFrames <= 0) return 1.0f;
    return std::clamp(static_cast<float>(mCurrentFrame) / static_cast<float>(mTotalFrames), 0.0f, 1.0f);
}

void CameraSequenceDirector::updateInterpolation() {
    if (mKeyframes.empty() || mCurrentKeyframeIdx >= mKeyframes.size()) {
        return;
    }

    const auto& currentKf = mKeyframes[mCurrentKeyframeIdx];

    // If within transition frames and have a previous keyframe, interpolate
    if (mCurrentKeyframeIdx > 0 && currentKf.transitionFrames > 0 && mFrameInKeyframe < currentKf.transitionFrames) {
        const auto& prevKf = mKeyframes[mCurrentKeyframeIdx - 1];
        float t = static_cast<float>(mFrameInKeyframe) / static_cast<float>(currentKf.transitionFrames);
        mActiveCameraTV = interpolateParams(prevKf.params, currentKf.params, t);
    } else {
        mActiveCameraTV = currentKf.params;
    }
}

CameraParams CameraSequenceDirector::interpolateParams(const CameraParams& from, const CameraParams& to, float t) {
    // Smoothstep interpolation
    t = std::clamp(t, 0.0f, 1.0f);
    float smoothT = t * t * (3.0f - 2.0f * t);

    CameraParams result;
    result.refType = to.refType;
    result.dirType = to.dirType;
    result.pos = from.pos * (1.0f - smoothT) + to.pos * smoothT;
    result.at = from.at * (1.0f - smoothT) + to.at * smoothT;

    sead::Vector3f blendedUp = from.up * (1.0f - smoothT) + to.up * smoothT;
    result.up = blendedUp.length() > 0.001f ? blendedUp.normalized() : sead::Vector3f(0.0f, 1.0f, 0.0f);

    result.fovy = from.fovy * (1.0f - smoothT) + to.fovy * smoothT;
    result.aspect = from.aspect * (1.0f - smoothT) + to.aspect * smoothT;
    result.nearPlane = from.nearPlane * (1.0f - smoothT) + to.nearPlane * smoothT;
    result.farPlane = from.farPlane * (1.0f - smoothT) + to.farPlane * smoothT;
    result.limit = to.limit;
    return result;
}

void CameraSequenceDirector::buildMatchIntroSequence() {
    const auto* p0 = mParamEngine ? mParamEngine->getCamera("PreGame_StageView") : nullptr;
    const auto* p1 = mParamEngine ? mParamEngine->getCamera("PreGame_PlayerView_Default_Friend") : nullptr;
    const auto* p2 = mParamEngine ? mParamEngine->getCamera("PreGame_PlayerView_Default_Opposite") : nullptr;

    CameraKeyframe kf0;
    kf0.presetName = "PreGame_StageView";
    kf0.params = p0 ? *p0 : CameraParams();
    kf0.durationFrames = 90;
    kf0.transitionFrames = 0;
    addKeyframe(kf0);

    CameraKeyframe kf1;
    kf1.presetName = "PreGame_PlayerView_Default_Friend";
    kf1.params = p1 ? *p1 : CameraParams();
    kf1.durationFrames = 60;
    kf1.transitionFrames = 15;
    addKeyframe(kf1);

    CameraKeyframe kf2;
    kf2.presetName = "PreGame_PlayerView_Default_Opposite";
    kf2.params = p2 ? *p2 : CameraParams();
    kf2.durationFrames = 60;
    kf2.transitionFrames = 15;
    addKeyframe(kf2);
}

void CameraSequenceDirector::buildPostGameSweepSequence(const std::string& stageName) {
    std::string tvName = "PostGame_StageView_Default_TV";
    std::string drcName = "PostGame_StageView_Default_DRC";

    if (!stageName.empty()) {
        std::string customTv = "PostGame_StageView_" + stageName + "_TV";
        std::string customDrc = "PostGame_StageView_" + stageName + "_DRC";
        if (mParamEngine && mParamEngine->getCamera(customTv)) {
            tvName = customTv;
        }
        if (mParamEngine && mParamEngine->getCamera(customDrc)) {
            drcName = customDrc;
        }
    }

    const auto* pTv = mParamEngine ? mParamEngine->getCamera(tvName) : nullptr;
    const auto* pDrc = mParamEngine ? mParamEngine->getCamera(drcName) : nullptr;
    const auto* pResult = mParamEngine ? mParamEngine->getCamera("Game_FinalResult") : nullptr;

    if (pDrc) {
        mActiveCameraDRC = *pDrc;
    }

    CameraKeyframe kfSweep;
    kfSweep.presetName = tvName;
    kfSweep.params = pTv ? *pTv : CameraParams();
    kfSweep.durationFrames = 120;
    kfSweep.transitionFrames = 0;
    addKeyframe(kfSweep);

    CameraKeyframe kfResult;
    kfResult.presetName = "Game_FinalResult";
    kfResult.params = pResult ? *pResult : CameraParams();
    kfResult.durationFrames = 90;
    kfResult.transitionFrames = 20;
    addKeyframe(kfResult);
}

void CameraSequenceDirector::buildTutorialSuperJumpSequence() {
    const auto* p = mParamEngine ? mParamEngine->getCamera("Tutorial_SuperJumpToPlaza") : nullptr;

    CameraKeyframe kf;
    kf.presetName = "Tutorial_SuperJumpToPlaza";
    kf.params = p ? *p : CameraParams();
    kf.durationFrames = 180;
    kf.transitionFrames = 0;
    addKeyframe(kf);
}

void CameraSequenceDirector::buildOctoValleyIntroSequence() {
    const auto* p0 = mParamEngine ? mParamEngine->getCamera("World_FirstEntryDemo_Commander") : nullptr;
    const auto* p1 = mParamEngine ? mParamEngine->getCamera("World_FirstEntryDemo_HeroSuit") : nullptr;
    const auto* p2 = mParamEngine ? mParamEngine->getCamera("World_FirstEntryDemo_World") : nullptr;

    CameraKeyframe kf0;
    kf0.presetName = "World_FirstEntryDemo_Commander";
    kf0.params = p0 ? *p0 : CameraParams();
    kf0.durationFrames = 90;
    kf0.transitionFrames = 0;
    addKeyframe(kf0);

    CameraKeyframe kf1;
    kf1.presetName = "World_FirstEntryDemo_HeroSuit";
    kf1.params = p1 ? *p1 : CameraParams();
    kf1.durationFrames = 90;
    kf1.transitionFrames = 20;
    addKeyframe(kf1);

    CameraKeyframe kf2;
    kf2.presetName = "World_FirstEntryDemo_World";
    kf2.params = p2 ? *p2 : CameraParams();
    kf2.durationFrames = 120;
    kf2.transitionFrames = 30;
    addKeyframe(kf2);
}

void CameraSequenceDirector::buildBossIntroOctavioSequence() {
    const auto* p0 = mParamEngine ? mParamEngine->getCamera("LastBossDemo_PlayerFront") : nullptr;
    const auto* p1 = mParamEngine ? mParamEngine->getCamera("LastBossDemo_CommanderFar") : nullptr;
    const auto* p2 = mParamEngine ? mParamEngine->getCamera("LastBossDemo_LastBossCloseUp") : nullptr;
    const auto* p3 = mParamEngine ? mParamEngine->getCamera("LastBossDemo_CommanderCloseUp") : nullptr;

    CameraKeyframe kf0;
    kf0.presetName = "LastBossDemo_PlayerFront";
    kf0.params = p0 ? *p0 : CameraParams();
    kf0.durationFrames = 60;
    kf0.transitionFrames = 0;
    addKeyframe(kf0);

    CameraKeyframe kf1;
    kf1.presetName = "LastBossDemo_CommanderFar";
    kf1.params = p1 ? *p1 : CameraParams();
    kf1.durationFrames = 60;
    kf1.transitionFrames = 15;
    addKeyframe(kf1);

    CameraKeyframe kf2;
    kf2.presetName = "LastBossDemo_LastBossCloseUp";
    kf2.params = p2 ? *p2 : CameraParams();
    kf2.durationFrames = 90;
    kf2.transitionFrames = 20;
    addKeyframe(kf2);

    CameraKeyframe kf3;
    kf3.presetName = "LastBossDemo_CommanderCloseUp";
    kf3.params = p3 ? *p3 : CameraParams();
    kf3.durationFrames = 60;
    kf3.transitionFrames = 15;
    addKeyframe(kf3);
}

void CameraSequenceDirector::buildStaffRollSequence() {
    const auto* p = mParamEngine ? mParamEngine->getCamera("StaffRoll") : nullptr;

    CameraKeyframe kf;
    kf.presetName = "StaffRoll";
    kf.params = p ? *p : CameraParams();
    kf.durationFrames = 300;
    kf.transitionFrames = 0;
    addKeyframe(kf);
}

} // namespace Game
