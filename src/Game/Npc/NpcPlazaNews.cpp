#include "Game/Npc/NpcPlazaNews.h"

namespace Game {

NpcPlazaNews::NpcPlazaNews()
    : mCurrentState(NewsState::Init),
      mStateTimer(0),
      mCurrentDialogueIndex(0),
      mRegularStageA("Fld_SeaPlant00"),  // Urchin Underpass
      mRegularStageB("Fld_Warehouse00"), // Walleye Warehouse
      mGachiStageA("Fld_UpDown00"),      // Saltspray Rig
      mGachiStageB("Fld_Skatepark00"),   // Blackbelly Skatepark
      mIsCallieTalking(false),
      mIsMarieTalking(false) {}

NpcPlazaNews::~NpcPlazaNews() = default;

void NpcPlazaNews::init() {
    GambitActor::init();
    changeState(NewsState::Init);
}

void NpcPlazaNews::changeState(NewsState state) {
    mCurrentState = state;
    mStateTimer = 0;

    switch (mCurrentState) {
        case NewsState::Init:
            mIsCallieTalking = false;
            mIsMarieTalking = false;
            break;
        case NewsState::Talk:
            playTsukkomiTalk(mCurrentDialogueIndex);
            break;
        case NewsState::StageAnnouncement:
            mIsCallieTalking = true;
            break;
        case NewsState::FadeOut:
            endBroadcast();
            break;
        default:
            break;
    }
}

void NpcPlazaNews::update() {
    mStateTimer++;

    switch (mCurrentState) {
        case NewsState::Init:
            if (mStateTimer > 30) {
                changeState(NewsState::StageAnnouncement);
            }
            break;

        case NewsState::StageAnnouncement:
            // After announcing stages, trigger Squid Sisters banter
            if (mStateTimer > 180) {
                changeState(NewsState::Talk);
            }
            break;

        case NewsState::Talk:
            // Advance dialogue lines
            if (mStateTimer > 150) {
                changeState(NewsState::FadeOut);
            }
            break;

        case NewsState::FadeOut:
            break;

        default:
            break;
    }
}

void NpcPlazaNews::draw() {
    GambitActor::draw();
}

void NpcPlazaNews::startBroadcast() {
    changeState(NewsState::Init);
}

void NpcPlazaNews::endBroadcast() {
    // Transition to free-roam in Inkopolis Plaza
}

void NpcPlazaNews::setStages(const char* regA, const char* regB, const char* gachiA, const char* gachiB) {
    mRegularStageA = regA;
    mRegularStageB = regB;
    mGachiStageA = gachiA;
    mGachiStageB = gachiB;
}

void NpcPlazaNews::playTsukkomiTalk(u32 dialogueId) {
    // Alternate dialogue animations between Callie and Marie
    if ((dialogueId % 2) == 0) {
        mIsCallieTalking = true;
        mIsMarieTalking = false;
    } else {
        mIsCallieTalking = false;
        mIsMarieTalking = true;
    }
}

} // namespace Game
