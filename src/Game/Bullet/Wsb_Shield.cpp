#include "Game/Bullet/Wsb_Shield.h"
#include <cstring>

namespace Game {

Wsb_Shield::Wsb_Shield()
    : mPosition(0.0f, 0.0f, 0.0f),
      mHitDeflectionFlag(0),
      mMaxHp(800.0f),
      mCurtainEffectPtr(nullptr),
      mState(ShieldState::cThrowing),
      mStateTimer(0),
      mYaw(0.0f),
      mTeam(0),
      mCurrentHp(800.0f),
      mWallWidth(3.0f),
      mWallHeight(2.2f) {
    std::memset(mReserved0_0x8, 0, sizeof(mReserved0_0x8));
    std::memset(mReserved1_0xD0, 0, sizeof(mReserved1_0xD0));
    std::memset(mReserved2_0x129, 0, sizeof(mReserved2_0x129));
    std::memset(mReserved3_0x188, 0, sizeof(mReserved3_0x188));
}

Wsb_Shield::~Wsb_Shield() {
}

// 0x0221EC80: Decompiled vfunc_3 (Shield resource setup)
void Wsb_Shield::vfunc_3() {
    mHitDeflectionFlag = 0;
    mCurrentHp = mMaxHp;
}

// 0x0221EDDC: Decompiled vfunc_7 (Water curtain ink deflection tick)
void Wsb_Shield::vfunc_7() {
    // Clear deflection hit detection flags each frame
    mHitDeflectionFlag = 0;
}

void Wsb_Shield::init() {
    GambitActor::init();
    mState = ShieldState::cThrowing;
    mCurrentHp = mMaxHp;
    vfunc_3();
}

void Wsb_Shield::deploy(const sead::Vector3f& pos, f32 yawAngle, u32 team) {
    mPosition = pos;
    mYaw = yawAngle;
    mTeam = team;
    mCurrentHp = mMaxHp;
    mState = ShieldState::cDeploying;
    mStateTimer = 0;
}

void Wsb_Shield::applyDamage(f32 damage) {
    if (mState != ShieldState::cActiveWall) {
        return;
    }

    mCurrentHp -= damage;
    if (mCurrentHp <= 0.0f) {
        mCurrentHp = 0.0f;
        mState = ShieldState::cBreaking;
        mStateTimer = 0;
    }
}

bool Wsb_Shield::blocksBullet(const sead::Vector3f& bulletPos, u32 bulletTeam) const {
    if (mState != ShieldState::cActiveWall) {
        return false;
    }

    // Friendly bullets pass through
    if (bulletTeam == mTeam) {
        return false;
    }

    // Proximity check to planar shield wall
    f32 dx = bulletPos.x - mPosition.x;
    f32 dz = bulletPos.z - mPosition.z;
    f32 distSq = dx * dx + dz * dz;

    if (distSq <= (mWallWidth * 0.5f) * (mWallWidth * 0.5f) &&
        bulletPos.y >= mPosition.y && bulletPos.y <= mPosition.y + mWallHeight) {
        return true;
    }

    return false;
}

void Wsb_Shield::update() {
    mStateTimer++;
    vfunc_7();

    switch (mState) {
        case ShieldState::cDeploying:
            if (mStateTimer >= 20) { // 20 frames unfold animation
                mState = ShieldState::cActiveWall;
                mStateTimer = 0;
            }
            break;

        case ShieldState::cActiveWall:
            // Natural decay: 800 HP over 420 frames (~1.9 HP per frame)
            mCurrentHp -= (mMaxHp / 420.0f);
            if (mCurrentHp <= 0.0f) {
                mCurrentHp = 0.0f;
                mState = ShieldState::cBreaking;
                mStateTimer = 0;
            }
            break;

        case ShieldState::cBreaking:
            if (mStateTimer >= 15) {
                mState = ShieldState::cFinished;
            }
            break;

        default:
            break;
    }
}

void Wsb_Shield::draw() {
    GambitActor::draw();
}

} // namespace Game
