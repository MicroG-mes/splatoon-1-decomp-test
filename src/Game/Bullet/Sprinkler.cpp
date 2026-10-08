#include "Game/Bullet/Sprinkler.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cstring>
#include <cmath>

namespace Game {

Sprinkler::Sprinkler()
    : mState(SprinklerState::cAttaching),
      mStateTimer(0),
      mPosition(0.0f, 0.0f, 0.0f),
      mNormal(0.0f, 1.0f, 0.0f),
      mTeam(0),
      mMaxHp(100.0f),
      mCurrentHp(100.0f),
      mRotationAngle(0.0f),
      mSprayTimer(0),
      mResourceInitToken(0),
      mRotationAngleRaw(0),
      mIsActive(0) {
    std::memset(mPadding1, 0, sizeof(mPadding1));
    std::memset(mPadding2, 0, sizeof(mPadding2));
}

Sprinkler::~Sprinkler() {
}

void Sprinkler::init() {
    GambitActor::init();
    vfunc_3();
    mState = SprinklerState::cAttaching;
    mCurrentHp = mMaxHp;
    mRotationAngle = 0.0f;
    mSprayTimer = 0;
    vfunc_176();
}

/**
 * Sprinkler__vfunc_3 @ 0x0221392c
 * Initializes model & resource token.
 */
void Sprinkler::vfunc_3() {
    mResourceInitToken = 1;
}

/**
 * Sprinkler__vfunc_167 @ 0x02213b50
 * Computes rotating droplet trajectory.
 */
void Sprinkler::vfunc_167() {
    sprayDroplet();
}

/**
 * Sprinkler__vfunc_176 @ 0x022142b8
 * Resets rotation angle (0x248) and active spray state (0x24A).
 */
void Sprinkler::vfunc_176() {
    mRotationAngleRaw = 0;
    mIsActive = 0;
}

void Sprinkler::attachToSurface(const sead::Vector3f& pos, const sead::Vector3f& normal, u32 team) {
    mPosition = pos;
    mNormal = normal;
    mTeam = team;
    mState = SprinklerState::cSpraying;
    mIsActive = 1;
    mStateTimer = 0;
}

void Sprinkler::applyDamage(f32 damage) {
    mCurrentHp -= damage;
    if (mCurrentHp <= 0.0f) {
        mState = SprinklerState::cDestroyed;
        vfunc_176();
    }
}

void Sprinkler::sprayDroplet() {
    mRotationAngle += 0.18f; // ~10 degrees per tick
    if (mRotationAngle > 6.2831853f) {
        mRotationAngle -= 6.2831853f;
    }
    mRotationAngleRaw = static_cast<u16>((mRotationAngle / 6.2831853f) * 65535.0f);

    f32 sprayDist = 2.0f + (std::sin(mRotationAngle * 3.0f) * 1.5f);
    sead::Vector3f dropPos(
        mPosition.x + std::sin(mRotationAngle) * sprayDist,
        mPosition.y,
        mPosition.z + std::cos(mRotationAngle) * sprayDist
    );

    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        paint->splatInk(dropPos, 0.6f, mTeam);
    }
}

void Sprinkler::update() {
    GambitActor::update();

    if (mState == SprinklerState::cSpraying) {
        mStateTimer++;
        mSprayTimer++;
        if (mSprayTimer % 3 == 0) { // Spray droplet every 3 frames (~20 droplets/sec)
            vfunc_167();
        }
    }
}

void Sprinkler::draw() {
    GambitActor::draw();
}

} // namespace Game
