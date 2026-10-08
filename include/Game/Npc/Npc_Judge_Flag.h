#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class JudgeState : u32 {
    cSleeping    = 0, // Sleeping peacefully on cushion pillow
    cAwake       = 1, // Sitting up and greeting player
    cJudgingFlag = 2, // Raising Team Alpha or Bravo victory flag
    cRewarding   = 3  // Dispensing Super Sea Snails based on Vibe rating
};

enum class JudgeVibeRank : u32 {
    cDry       = 0, // < 4.0 win streak points
    cWarm      = 1, // 4.0 - 9.5 win streak points
    cSmokin    = 2, // 10.0 - 14.5 win streak points
    cSoHot     = 3  // 15.0+ win streak points (bonus cash / snails)
};

/**
 * Npc_Judge_Flag (Judd the Cat / Referee NPC)
 * Address: vtable @ 0x100E6D6C
 * Authentic path: D:/home/Cafe/Gambit/App/Program/Game/Npc/Npc_Judge_Flag.cpp
 *
 * Real PowerPC methods from Gambit.elf:
 *   vfunc_3  @ 0x026186C4 (size 648): AI decision cycle, sleep/wake transitions, flag gesture
 *   vfunc_11 @ 0x02618958 (size 8): State verification
 *   vfunc_60 @ 0x02618FB8 (size 124): Super Sea Snail bonus calculation from Vibe level
 *   vfunc_47 @ 0x026191AC (size 64): Victory flag decision (Alpha vs Bravo)
 *   vfunc_1  @ 0x02619058 (size 252): Destructor & actor cleanup
 */
class Npc_Judge_Flag : public GambitActor {
public:
    Npc_Judge_Flag();
    virtual ~Npc_Judge_Flag() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic PPC vfuncs
    virtual void vfunc_3();
    virtual u32 vfunc_11();
    virtual u32 vfunc_60();
    virtual void vfunc_47(u8 winningTeam);

    void setup(const sead::Vector3f& cushionPos);
    void updateProximity(const sead::Vector3f& playerPos, f32 wakeRadius = 4.0f);
    void judgeMatch(f32 alphaPercent, f32 bravoPercent);
    void triggerBattleResult(bool alphaWon) { vfunc_47(alphaWon ? 0 : 1); }
    u32 awardSuperSeaSnails(f32 vibePoints);

    JudgeState getState() const { return mState; }
    JudgeVibeRank getVibeRank() const { return mVibeRank; }
    u8 getWinningTeam() const { return mWinningTeam; }
    u32 getAwardedSnails() const { return mAwardedSnails; }
    bool isNearby() const { return mIsPlayerNearby; }
    const sead::Vector3f& getPosition() const { return mPosition; }

protected:
    sead::Vector3f mPosition;
    JudgeState mState;          // +0x204
    u32 mStateCode;             // +0x2C
    JudgeVibeRank mVibeRank;
    u8 mWinningTeam;            // 0 = Alpha (Orange), 1 = Bravo (Cyan), 255 = Draw
    u32 mAwardedSnails;
    s32 mFlagAnimationTimer;
    bool mIsPlayerNearby;
};

} // namespace Game
