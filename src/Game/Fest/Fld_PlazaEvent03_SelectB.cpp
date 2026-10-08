#include "Game/Fest/Fld_PlazaEvent03_SelectB.h"
#include <cstring>
#include <cmath>

namespace Game {

Fld_PlazaEvent03_SelectB::Fld_PlazaEvent03_SelectB()
    : mPosition(10.5f, 0.0f, -4.2f),
      mIsActive(true),
      mSelectedTeamId(0),
      mVoteState(FestVoteState::cUnvoted),
      mIsPlayerNearby(false),
      mTeamAlpha("Alpha"),
      mTeamBeta("Beta") {
    std::memset(mTransformMatrix, 0, sizeof(mTransformMatrix));
    std::memset(mRenderMatrix, 0, sizeof(mRenderMatrix));
    std::memset(mReserved, 0, sizeof(mReserved));

    mTransformMatrix[0][0] = 1.0f;
    mTransformMatrix[1][1] = 1.0f;
    mTransformMatrix[2][2] = 1.0f;
}

Fld_PlazaEvent03_SelectB::~Fld_PlazaEvent03_SelectB() {
}

void Fld_PlazaEvent03_SelectB::init() {
    GambitActor::init();
    mIsActive = true;
    mSelectedTeamId = 0;
    mVoteState = FestVoteState::cUnvoted;
    mIsPlayerNearby = false;

    vfunc_3();
    vfunc_47();
}

// 0x0259E588: Decompiled vfunc_3 (Model & billboard setup)
void Fld_PlazaEvent03_SelectB::vfunc_3() {
    // Identity transform setup
    mTransformMatrix[0][0] = 1.0f;
    mTransformMatrix[1][1] = 1.0f;
    mTransformMatrix[2][2] = 1.0f;
    mTransformMatrix[0][3] = mPosition.x;
    mTransformMatrix[1][3] = mPosition.y;
    mTransformMatrix[2][3] = mPosition.z;
}

// 0x0259E698: Decompiled vfunc_9 (Interaction check)
void Fld_PlazaEvent03_SelectB::vfunc_9() {
    if (mIsPlayerNearby && mVoteState == FestVoteState::cUnvoted) {
        mVoteState = FestVoteState::cPromptSelection;
    }
}

// 0x0259EC3C: Decompiled vfunc_11 (Affine transform sync & voting choice update)
void Fld_PlazaEvent03_SelectB::vfunc_11() {
    if (!mIsActive) {
        return;
    }

    // Direct translation of Ghidra decompilation:
    // Copies 3x4 affine transform matrix into render matrix buffer
    std::memcpy(mRenderMatrix, mTransformMatrix, sizeof(mRenderMatrix));
}

// 0x0259FD0C: Decompiled vfunc_47 (Collision registration)
void Fld_PlazaEvent03_SelectB::vfunc_47() {
    // Registered with Stage physics mesh
}

void Fld_PlazaEvent03_SelectB::setupBooth(const sead::Vector3f& pos, const char* teamAlphaName, const char* teamBetaName) {
    mPosition = pos;
    mTeamAlpha = teamAlphaName;
    mTeamBeta = teamBetaName;
    mTransformMatrix[0][3] = mPosition.x;
    mTransformMatrix[1][3] = mPosition.y;
    mTransformMatrix[2][3] = mPosition.z;
    mVoteState = FestVoteState::cUnvoted;
}

bool Fld_PlazaEvent03_SelectB::checkPlayerInteraction(const sead::Vector3f& playerPos, f32 interactDist) {
    f32 dx = playerPos.x - mPosition.x;
    f32 dy = playerPos.y - mPosition.y;
    f32 dz = playerPos.z - mPosition.z;
    f32 distSq = dx * dx + dy * dy + dz * dz;

    mIsPlayerNearby = (distSq <= interactDist * interactDist);
    if (mIsPlayerNearby) {
        vfunc_9();
    }
    return mIsPlayerNearby;
}

void Fld_PlazaEvent03_SelectB::selectTeam(u32 teamId) {
    mSelectedTeamId = (teamId > 0) ? 1 : 0;
    mVoteState = FestVoteState::cTeamConfirmed;
}

void Fld_PlazaEvent03_SelectB::update() {
    vfunc_11();
}

void Fld_PlazaEvent03_SelectB::draw() {
    GambitActor::draw();
}

} // namespace Game
