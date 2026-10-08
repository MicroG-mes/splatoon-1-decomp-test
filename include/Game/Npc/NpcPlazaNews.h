#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class NewsState : u32 {
    Init = 0,
    Wait = 1,
    Talk = 2,
    StageAnnouncement = 3,
    SplatfestAnnouncement = 4,
    FadeOut = 5,
};

class NpcPlazaNews : public GambitActor {
public:
    NpcPlazaNews();
    virtual ~NpcPlazaNews() override;

    // Actor lifecycle
    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // News broadcast flow
    void changeState(NewsState state);
    void startBroadcast();
    void endBroadcast();

    // Stage rotation & dialogue
    void setStages(const char* regA, const char* regB, const char* gachiA, const char* gachiB);
    void playTsukkomiTalk(u32 dialogueId);

protected:
    NewsState mCurrentState;
    s32 mStateTimer;
    u32 mCurrentDialogueIndex;

    // Stage rotation names
    const char* mRegularStageA;
    const char* mRegularStageB;
    const char* mGachiStageA;
    const char* mGachiStageB;

    // Squid Sisters (Aori/Callie & Hotaru/Marie) animation flags
    bool mIsCallieTalking;
    bool mIsMarieTalking;
    undefined mReserved[0x30];
};

} // namespace Game
