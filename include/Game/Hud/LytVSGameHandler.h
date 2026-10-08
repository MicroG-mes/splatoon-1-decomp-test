#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include "cafe/vpad.h"

namespace Game {

struct SquidStatusSlot {
    bool isAlive;
    s32 respawnFrames;
    bool isSpecialActive;
    u32 weaponId;
    sead::Vector3f worldPos;
};

class LytVSGameHandler : public GambitActor {
public:
    LytVSGameHandler();
    virtual ~LytVSGameHandler() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void handleInput(const VPADStatus& vpad);

    void setPlayerStatus(u32 team, u32 slot, bool isAlive, s32 respawnFrames, bool isSpecialActive, const sead::Vector3f& pos);
    void setScores(f32 scoreAlpha, f32 scoreBravo);
    void setTime(s32 remainingSeconds);

    s32 getSuperJumpTarget() const { return mSelectedSuperJumpTarget; }
    void clearSuperJumpTarget() { mSelectedSuperJumpTarget = -1; }

    const SquidStatusSlot& getSlot(u32 team, u32 slot) const;

protected:
    void checkSuperJumpTouch(const VPADStatus& vpad);

    SquidStatusSlot mTeamAlpha[4];
    SquidStatusSlot mTeamBravo[4];

    f32 mScoreAlpha;
    f32 mScoreBravo;
    s32 mRemainingSeconds;

    s32 mSelectedSuperJumpTarget; // -1 = None, 0 = Spawn, 1..3 = Teammates
    s32 mAnimTimer;

    undefined mReserved[0x38];
};

} // namespace Game
