#pragma once

#include "types.h"
#include "sead/math/seadVector.h"
#include "Game/Map/StageDef.h"
#include "Game/Rule/GameRuleRanked.h"
#include <string>
#include <vector>

namespace Game {

enum class NewsBroadcastState : u32 {
    cOffAir           = 0,
    cStudioIntro      = 1,
    cAnnounceRegular  = 2,
    cAnnounceRanked   = 3,
    cAnnounceSplatfest= 4,
    cSignOff          = 5,
    cFinished         = 6
};

struct NewsDialogueLine {
    const char* speaker; // "Callie" or "Marie"
    std::string text;
};

struct RotationSchedule {
    u32 regularStageIdA;
    u32 regularStageIdB;
    u32 rankedStageIdA;
    u32 rankedStageIdB;
    RankedModeType rankedRule;
    bool isSplatfestActive;
    const char* splatfestThemeAlpha;
    const char* splatfestThemeBravo;
};

/**
 * PlazaNewsBroadcast (Inkopolis News / Talk_DemoIdol)
 * Reverse engineered from Splatoon 1 retail binary (0x02095210 / Talk_DemoIdol_%02d_%s).
 * Simulates the iconic Squid Sisters studio broadcast with authentic stage rotation commentary.
 */
class PlazaNewsBroadcast {
public:
    PlazaNewsBroadcast();
    ~PlazaNewsBroadcast();

    void init(const RotationSchedule& schedule);
    void update(f32 dt = 0.01667f);

    // Player inputs to advance dialogue or skip
    bool advanceDialogue();
    void skipBroadcast();

    NewsBroadcastState getState() const { return mState; }
    bool isBroadcasting() const { return mState != NewsBroadcastState::cOffAir && mState != NewsBroadcastState::cFinished; }
    
    // Current dialogue line display
    const NewsDialogueLine* getCurrentLine() const;
    size_t getCurrentLineIndex() const { return mCurrentLineIndex; }
    size_t getTotalLines() const { return mLines.size(); }

    // Studio positions for Callie and Marie
    const sead::Vector3f& getCalliePos() const { return mCalliePos; }
    const sead::Vector3f& getMariePos() const { return mMariePos; }

    const RotationSchedule& getSchedule() const { return mSchedule; }

private:
    void buildDialogueScript();
    void addStageBanter(u32 stageId);

    NewsBroadcastState mState;
    RotationSchedule mSchedule;
    std::vector<NewsDialogueLine> mLines;
    size_t mCurrentLineIndex;
    f32 mStateTimer;
    f32 mTextScrollTimer;

    sead::Vector3f mCalliePos;
    sead::Vector3f mMariePos;
};

} // namespace Game
