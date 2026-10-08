#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class FestVoteState : u32 {
    cUnvoted         = 0,
    cPromptSelection = 1,
    cTeamConfirmed   = 2
};

/**
 * Fld_PlazaEvent03_SelectB (Splatfest Voting Booth Actor)
 * Decompiled from PowerPC retail executable Gambit.elf:
 * Vtable @ 0x1005E230
 *
 * Authentic PowerPC Methods:
 *   vfunc_3  @ 0x0259E588: Model setup and billboard initialization
 *   vfunc_9  @ 0x0259E698: Interaction trigger evaluation
 *   vfunc_11 @ 0x0259EC3C: Affine transform sync & voting selection update
 *   vfunc_47 @ 0x0259FD0C: Stage collision registration
 */
class Fld_PlazaEvent03_SelectB : public GambitActor {
public:
    Fld_PlazaEvent03_SelectB();
    virtual ~Fld_PlazaEvent03_SelectB() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic Ghidra Decompiled vfuncs
    virtual void vfunc_3();  // Setup
    virtual void vfunc_9();  // Proximity check
    virtual void vfunc_11(); // Affine matrix sync & choice update
    virtual void vfunc_47(); // Collision register

    void setupBooth(const sead::Vector3f& pos, const char* teamAlphaName, const char* teamBetaName);
    bool checkPlayerInteraction(const sead::Vector3f& playerPos, f32 interactDist = 2.0f);
    void selectTeam(u32 teamId); // 0 = Alpha (e.g. Cats), 1 = Beta (e.g. Dogs)

    FestVoteState getVoteState() const { return mVoteState; }
    u32 getSelectedTeamId() const { return mSelectedTeamId; }
    bool hasVoted() const { return mVoteState == FestVoteState::cTeamConfirmed; }
    const char* getTeamAlphaName() const { return mTeamAlpha; }
    const char* getTeamBetaName() const { return mTeamBeta; }
    const sead::Vector3f& getPosition() const { return mPosition; }

protected:
    sead::Vector3f mPosition;
    f32 mTransformMatrix[3][4]; // 0x54 - 0x80: 3x4 affine transform matrix
    f32 mRenderMatrix[3][4];    // 0x1F8 - 0x220: duplicated render matrix

    bool mIsActive;             // 0x238
    u32 mSelectedTeamId;        // 0x23C
    FestVoteState mVoteState;
    bool mIsPlayerNearby;

    const char* mTeamAlpha;
    const char* mTeamBeta;

    u8 mReserved[0x30];
};

} // namespace Game
