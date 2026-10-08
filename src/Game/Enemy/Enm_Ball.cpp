#include "Game/Enemy/Enm_Ball.h"
#include "Game/Paint/PaintTextureMgr.h"
#include <cmath>

namespace Game {

Enm_Ball::Enm_Ball()
    : mPosition(0.0f, 0.0f, 0.0f),
      mMoveDir(0.0f, 0.0f, 1.0f),
      mHp(cMaxHp),
      mState(OctoballState::cIdleStand),
      mStateTimer(0) {
}

Enm_Ball::~Enm_Ball() {
}

void Enm_Ball::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mMoveDir.set(0.0f, 0.0f, 1.0f);
    mHp = cMaxHp;
    mState = OctoballState::cIdleStand;
    mStateTimer = 0;
}

void Enm_Ball::applyDamage(f32 damage) {
    if (mState == OctoballState::cDefeated) {
        return;
    }

    mHp -= damage;
    if (mHp <= 0.0f) {
        mState = OctoballState::cDefeated;
        mStateTimer = 0;

        PaintTextureMgr* paint = PaintTextureMgr::instance();
        if (paint) {
            paint->splatInk(mPosition, 2.5f, 0); // Player ink burst upon defeat
        }
    } else {
        mState = OctoballState::cDazedUncurl;
        mStateTimer = 0;
    }
}

void Enm_Ball::stepRollingMovement() {
    mPosition.x += mMoveDir.x * cRollSpeed;
    mPosition.z += mMoveDir.z * cRollSpeed;

    // Ink the floor with purple ink along rolling trajectory
    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint && (mStateTimer % 3 == 0)) {
        paint->splatInk(mPosition, 1.4f, 1);
    }
}

void Enm_Ball::updateAi(const sead::Vector3f& playerPos, bool isRollingOnPlayerInk) {
    if (mState == OctoballState::cDefeated) {
        return;
    }

    f32 dx = playerPos.x - mPosition.x;
    f32 dz = playerPos.z - mPosition.z;
    f32 dist = std::sqrt(dx * dx + dz * dz);

    switch (mState) {
        case OctoballState::cIdleStand:
            if (dist < 12.0f) {
                if (dist > 0.01f) {
                    mMoveDir.set(dx / dist, 0.0f, dz / dist);
                }
                mState = OctoballState::cCurlingUp;
                mStateTimer = 0;
            }
            break;

        case OctoballState::cCurlingUp:
            if (mStateTimer >= 20) {
                mState = OctoballState::cRollingDash;
                mStateTimer = 0;
            }
            break;

        case OctoballState::cRollingDash:
            stepRollingMovement();

            // Losing traction on player ink
            if (isRollingOnPlayerInk) {
                mState = OctoballState::cDazedUncurl;
                mStateTimer = 0;
                break;
            }

            if (mStateTimer >= 180) { // Roll for 3 seconds then rest
                mState = OctoballState::cIdleStand;
                mStateTimer = 0;
            }
            break;

        case OctoballState::cDazedUncurl:
            if (mStateTimer >= 60) {
                mState = OctoballState::cIdleStand;
                mStateTimer = 0;
            }
            break;

        case OctoballState::cWallBounce:
            mMoveDir.set(-mMoveDir.x, 0.0f, -mMoveDir.z);
            mState = OctoballState::cRollingDash;
            mStateTimer = 0;
            break;

        case OctoballState::cDefeated:
        default:
            break;
    }
}

void Enm_Ball::update() {
    mStateTimer++;
}

void Enm_Ball::draw() {
    if (mState != OctoballState::cDefeated) {
        GambitActor::draw();
    }
}

} // namespace Game
