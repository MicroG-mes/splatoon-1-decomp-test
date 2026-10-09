#pragma once

#include "types.h"
#include "Game/Camera/CameraParamEngine.h"
#include "sead/math/seadVector.h"
#include "sead/math/seadMatrix.h"
#include <string>
#include <vector>

namespace Game {

enum class CameraSequenceType {
    None = 0,
    MatchIntro,          // PreGame_StageView -> Friend -> Opposite
    PostGameMatchSweep,  // PostGame_StageView (TV & DRC) -> Game_FinalResult
    TutorialSuperJump,   // Tutorial_SuperJumpToPlaza
    OctoValleyIntro,     // FirstEntry Commander -> HeroSuit -> World
    BossIntroOctavio,    // LastBoss PlayerFront -> CommanderFar -> LastBossCloseUp
    StaffRollCredits     // StaffRoll (16:9 widescreen panorama)
};

struct CameraKeyframe {
    std::string presetName;
    CameraParams params;
    int durationFrames = 60;
    int transitionFrames = 30; // Blend frames into this keyframe
};

class CameraSequenceDirector {
public:
    CameraSequenceDirector();
    ~CameraSequenceDirector() = default;

    bool init(CameraParamEngine* paramEngine = nullptr);

    // Sequence initiation
    bool startSequence(CameraSequenceType type, const std::string& stageName = "");
    void stopSequence();
    void update(float frameDelta = 1.0f);

    // Playback state
    bool isPlaying() const { return mIsPlaying; }
    bool isFinished() const { return mIsFinished; }
    CameraSequenceType getCurrentSequenceType() const { return mCurrentType; }
    int getCurrentFrame() const { return mCurrentFrame; }
    int getTotalFrames() const { return mTotalFrames; }
    float getProgress() const;
    size_t getCurrentKeyframeIndex() const { return mCurrentKeyframeIdx; }

    // Active interpolated camera parameters
    const CameraParams& getCurrentCameraTV() const { return mActiveCameraTV; }
    const CameraParams& getCurrentCameraDRC() const { return mActiveCameraDRC; }

    // Custom sequence construction
    void addKeyframe(const CameraKeyframe& kf);
    void clearKeyframes();

private:
    void buildMatchIntroSequence();
    void buildPostGameSweepSequence(const std::string& stageName);
    void buildTutorialSuperJumpSequence();
    void buildOctoValleyIntroSequence();
    void buildBossIntroOctavioSequence();
    void buildStaffRollSequence();

    void updateInterpolation();
    static CameraParams interpolateParams(const CameraParams& from, const CameraParams& to, float t);

    CameraParamEngine mDefaultParamEngine;
    CameraParamEngine* mParamEngine = nullptr;

    CameraSequenceType mCurrentType = CameraSequenceType::None;
    std::vector<CameraKeyframe> mKeyframes;
    size_t mCurrentKeyframeIdx = 0;

    int mCurrentFrame = 0;
    int mTotalFrames = 0;
    int mFrameInKeyframe = 0;
    bool mIsPlaying = false;
    bool mIsFinished = false;

    CameraParams mActiveCameraTV;
    CameraParams mActiveCameraDRC;
};

} // namespace Game
