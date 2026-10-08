#pragma once

#include "types.h"
#include <string>
#include <vector>
#include <memory>

namespace Game {

struct VBallStageInfo {
    u32 stageIndex = 0;
    u32 goalPoint = 0;
    size_t enemyCount = 0;
};

struct RaceStageInfo {
    u32 stageIndex = 0;
    u32 timeLimit = 0;
    std::string strongRival;
    size_t wallCount = 0;
};

struct JukeBoxTrackInfo {
    u32 trackIndex = 0;
    u32 numNotesEasy = 0;
    u32 numNotesNormal = 0;
    u32 numNotesExtreme = 0;
};

class MiniGameCatalog {
public:
    MiniGameCatalog();
    ~MiniGameCatalog();

    bool loadFromSzs(const char* szsPath);
    void clear();

    bool isLoaded() const { return mIsLoaded; }
    size_t getVBallStageCount() const { return mVBallStages.size(); }
    size_t getRaceStageCount() const { return mRaceStages.size(); }
    size_t getJukeBoxTrackCount() const { return mJukeBoxTracks.size(); }

    const VBallStageInfo* getVBallStage(size_t index) const;
    const RaceStageInfo* getRaceStage(size_t index) const;
    const JukeBoxTrackInfo* getJukeBoxTrack(size_t index) const;

    static MiniGameCatalog* instance();

private:
    bool mIsLoaded = false;
    std::vector<VBallStageInfo> mVBallStages;
    std::vector<RaceStageInfo> mRaceStages;
    std::vector<JukeBoxTrackInfo> mJukeBoxTracks;
    static std::unique_ptr<MiniGameCatalog> sInstance;
};

} // namespace Game
