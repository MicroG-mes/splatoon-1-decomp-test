#include "Game/Npc/PlazaNewsBroadcast.h"
#include <cstdio>

namespace Game {

PlazaNewsBroadcast::PlazaNewsBroadcast()
    : mState(NewsBroadcastState::cOffAir)
    , mCurrentLineIndex(0)
    , mStateTimer(0.0f)
    , mTextScrollTimer(0.0f)
    , mCalliePos(sead::Vector3f(-1.2f, 1.0f, 0.0f))
    , mMariePos(sead::Vector3f(1.2f, 1.0f, 0.0f))
{
    mSchedule = {};
}

PlazaNewsBroadcast::~PlazaNewsBroadcast() {}

void PlazaNewsBroadcast::init(const RotationSchedule& schedule) {
    mSchedule = schedule;
    mState = NewsBroadcastState::cStudioIntro;
    mCurrentLineIndex = 0;
    mStateTimer = 0.0f;
    mTextScrollTimer = 0.0f;
    mLines.clear();

    buildDialogueScript();
}

void PlazaNewsBroadcast::buildDialogueScript() {
    mLines.clear();

    // 1. Studio Intro
    mLines.push_back({ "Callie", "Hi there, squids! It's Squid Sisters time!" });
    mLines.push_back({ "Marie", "Another day, another battle. Let's see what's on the schedule." });

    // 2. Splatfest Announcement (if active)
    if (mSchedule.isSplatfestActive) {
        std::string festIntro = std::string("Splatfest is NOW ACTIVE! Theme: ") +
            (mSchedule.splatfestThemeAlpha ? mSchedule.splatfestThemeAlpha : "Team A") + " vs " +
            (mSchedule.splatfestThemeBravo ? mSchedule.splatfestThemeBravo : "Team B") + "!";
        mLines.push_back({ "Callie", festIntro });
        mLines.push_back({ "Marie", "Head over to the voting booth in the plaza to pick your side and claim your tee!" });
    }

    // 3. Regular Battle (Turf War)
    mLines.push_back({ "Callie", "First up, Regular Battle stages!" });
    addStageBanter(mSchedule.regularStageIdA);
    addStageBanter(mSchedule.regularStageIdB);

    // 4. Ranked Battle
    std::string ruleName = "Ranked Battle";
    switch (mSchedule.rankedRule) {
        case RankedModeType::cSplatZones:   ruleName = "Splat Zones"; break;
        case RankedModeType::cTowerControl: ruleName = "Tower Control"; break;
        case RankedModeType::cRainmaker:    ruleName = "Rainmaker"; break;
        default: break;
    }
    std::string rankedIntro = "And now for the Ranked Battle mode: " + ruleName + "!";
    mLines.push_back({ "Callie", rankedIntro });
    addStageBanter(mSchedule.rankedStageIdA);
    addStageBanter(mSchedule.rankedStageIdB);

    // 5. Sign-off Catchphrase
    mLines.push_back({ "Callie", "That's all for now squids! Until next time..." });
    mLines.push_back({ "Marie", "Stay fresh!" });
}

void PlazaNewsBroadcast::addStageBanter(u32 stageId) {
    const auto* stage = StageDef::getStageInfo(static_cast<StageId>(stageId));
    std::string stageName = stage ? stage->displayName : "Mystery Stage";

    // Authentic Splatoon 1 retail stage banter
    if (stageName.find("Warehouse") != std::string::npos || stageId == 0) {
        mLines.push_back({ "Callie", "Walleye Warehouse! Watch out for forklifts and flying rollers!" });
        mLines.push_back({ "Marie", "I love the smell of cardboard in the morning." });
    } else if (stageName.find("Underpass") != std::string::npos || stageId == 2) {
        mLines.push_back({ "Callie", "Urchin Underpass! High-speed battles under the highway!" });
        mLines.push_back({ "Marie", "Watch out for pigeons on the center grating." });
    } else if (stageName.find("Skatepark") != std::string::npos || stageId == 1) {
        mLines.push_back({ "Callie", "Blackbelly Skatepark! Ride the ramps and conquer the tower!" });
        mLines.push_back({ "Marie", "Make sure you stick the landing, or you're fish food." });
    } else if (stageName.find("Rig") != std::string::npos || stageId == 3) {
        mLines.push_back({ "Callie", "Saltspray Rig! Breathe in that fresh ocean breeze!" });
        mLines.push_back({ "Marie", "And don't fall off the catwalks into the brine." });
    } else if (stageName.find("Mall") != std::string::npos || stageId == 4) {
        mLines.push_back({ "Callie", "Arowana Mall! Splatting and shopping all in one place!" });
        mLines.push_back({ "Marie", "My wallet hurts just thinking about it." });
    } else if (stageName.find("Towers") != std::string::npos || stageId == 5) {
        mLines.push_back({ "Callie", "Moray Towers! A steep climb with a breathtaking drop!" });
        mLines.push_back({ "Marie", "Seriously, do NOT look down." });
    } else {
        std::string cLine = stageName + "! Time to ink every inch of turf!";
        mLines.push_back({ "Callie", cLine });
        mLines.push_back({ "Marie", "Good luck out there, you'll need it." });
    }
}

void PlazaNewsBroadcast::update(f32 dt) {
    if (mState == NewsBroadcastState::cOffAir || mState == NewsBroadcastState::cFinished) {
        return;
    }

    mStateTimer += dt;
    mTextScrollTimer += dt;

    // Update broadcast state based on line progression
    if (mCurrentLineIndex < 2) {
        mState = NewsBroadcastState::cStudioIntro;
    } else if (mSchedule.isSplatfestActive && mCurrentLineIndex < 4) {
        mState = NewsBroadcastState::cAnnounceSplatfest;
    } else if (mCurrentLineIndex < (mSchedule.isSplatfestActive ? 8 : 6)) {
        mState = NewsBroadcastState::cAnnounceRegular;
    } else if (mCurrentLineIndex < mLines.size() - 2) {
        mState = NewsBroadcastState::cAnnounceRanked;
    } else {
        mState = NewsBroadcastState::cSignOff;
    }

    // Auto-advance after 5.0 seconds per line if unpressed
    if (mTextScrollTimer > 5.0f) {
        advanceDialogue();
    }
}

bool PlazaNewsBroadcast::advanceDialogue() {
    mTextScrollTimer = 0.0f;
    if (mCurrentLineIndex + 1 < mLines.size()) {
        mCurrentLineIndex++;
        return true;
    } else {
        mState = NewsBroadcastState::cFinished;
        return false;
    }
}

void PlazaNewsBroadcast::skipBroadcast() {
    mCurrentLineIndex = mLines.empty() ? 0 : mLines.size() - 1;
    mState = NewsBroadcastState::cFinished;
}

const NewsDialogueLine* PlazaNewsBroadcast::getCurrentLine() const {
    if (mCurrentLineIndex < mLines.size()) {
        return &mLines[mCurrentLineIndex];
    }
    return nullptr;
}

} // namespace Game
