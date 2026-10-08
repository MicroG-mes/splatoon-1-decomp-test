#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "Game/Sequence/StartupScreen.h"
#include "Game/Sequence/TitleView.h"
#include "Game/Npc/NpcPlazaNews.h"
#include "cafe/vpad.h"

namespace Game {

enum class PlazaSeqState : u32 {
    WaitSceneFade       = 0, // StateId::cWaitSceneFade
    StartupSplash       = 1, // Controller reminder splash
    TitleView           = 2, // StateId::cTitleView
    NewsBroadcast       = 3, // Plaza_News (Squid Sisters Callie & Marie)
    StageInDemoPlaza    = 4, // Loading Model/Fld_Plaza00.szs
    PlazaFreeRoam       = 5, // StateId::cPlaza
    DoorInLobby         = 6, // StateId::cDoorIn (Inkopolis Tower)
    GotoWorldOctoValley = 7, // StateId::cGotoWorld (Grate to single-player)
    CustomShopLook      = 8, // Plaza_CustomShopLook (Booyah Base)
    VideoGameLook       = 9, // Plaza_VideoGameLook (Arcade Cabinet)
    JudgeLook           = 10, // Plaza_JudgeLook (Judd Vibe Check)
    SpykeTalk           = 11, // Alleyway Spyke (Gear order & reroll)
    InMatchLobby        = 12  // Inside multiplayer tower lobby
};

class LytShopHandler;
class Npc_Judge_Flag;
class LytPlazaGearOrderMgr;
class Obj_PlazaGame;
class LytMiniGameHandler;
class Fld_PlazaLobby;
class Lobby;

class SeqMgrPlaza : public GambitActor {
public:
    SeqMgrPlaza();
    virtual ~SeqMgrPlaza() override;

    static SeqMgrPlaza* instance() { return sInstance; }

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void handleInput(const VPADStatus& vpad);
    void changeState(PlazaSeqState state);

    PlazaSeqState getState() const { return mCurrentState; }

    static SeqMgrPlaza* sInstance;

protected:
    PlazaSeqState mCurrentState;
    s32 mStateTimer;

    // Sub-screens & sequences
    StartupScreen mStartupScreen;
    TitleView mTitleView;
    NpcPlazaNews mNewsBroadcast;

    bool mIsFirstBoot;
    undefined mReserved[0x40];
};

} // namespace Game
