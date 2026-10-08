#include "Game/Enemy/Enm_Flyswatter.h"
#include "Game/Paint/PaintTextureMgr.h"

namespace Game {

Enm_Flyswatter::Enm_Flyswatter()
    : mPosition(0.0f, cApexHeight, 0.0f),
      mTargetPlayerPos(0.0f, 0.0f, 0.0f),
      mHp(cMaxHp),
      mState(FlyswatterState::cHoverIdle),
      mStateTimer(0) {
}

Enm_Flyswatter::~Enm_Flyswatter() {
}

void Enm_Flyswatter::init() {
    GambitActor::init();
    mPosition.set(0.0f, cApexHeight, 0.0f);
    mTargetPlayerPos.set(0.0f, 0.0f, 0.0f);
    mHp = cMaxHp;
    mState = FlyswatterState::cHoverIdle;
    mStateTimer = 0;
}

void Enm_Flyswatter::applyDamage(f32 damage) {
    if (mState == FlyswatterState::cDefeated) {
        return;
    }

    // Only takes full damage when flat on ground with exposed back joint
    f32 damageMultiplier = (mState == FlyswatterState::cFlatRest) ? 1.0f : 0.25f;
    mHp -= damage * damageMultiplier;

    if (mHp <= 0.0f) {
        mState = FlyswatterState::cDefeated;
        mStateTimer = 0;

        PaintTextureMgr* paint = PaintTextureMgr::instance();
        if (paint) {
            paint->splatInk(mPosition, 4.0f, 0); // Player ink burst upon destruction
        }
    }
}

void Enm_Flyswatter::slamImpact() {
    mPosition.y = 0.0f;
    mPosition.x = mTargetPlayerPos.x;
    mPosition.z = mTargetPlayerPos.z;

    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        paint->splatInk(mPosition, cSlamRadius, 1); // Enemy ink shockwave
    }
}

void Enm_Flyswatter::updateAi(const sead::Vector3f& playerPos) {
    if (mState == FlyswatterState::cDefeated) {
        return;
    }

    switch (mState) {
        case FlyswatterState::cHoverIdle:
            // Stalk player overhead
            mPosition.x += (playerPos.x - mPosition.x) * 0.10f;
            mPosition.z += (playerPos.z - mPosition.z) * 0.10f;

            if (mStateTimer >= 90) { // 1.5s hovering
                mTargetPlayerPos = playerPos;
                mState = FlyswatterState::cWindupRise;
                mStateTimer = 0;
            }
            break;

        case FlyswatterState::cWindupRise:
            mPosition.y += 0.12f;
            if (mPosition.y >= cApexHeight + 1.5f) {
                mState = FlyswatterState::cDropSlam;
                mStateTimer = 0;
            }
            break;

        case FlyswatterState::cDropSlam:
            mPosition.y -= 0.65f; // Fast drop
            if (mPosition.y <= 0.0f) {
                slamImpact();
                mState = FlyswatterState::cFlatRest;
                mStateTimer = 0;
            }
            break;

        case FlyswatterState::cFlatRest:
            if (mStateTimer >= 120) { // 2 seconds flat rest
                mState = FlyswatterState::cRecoverRise;
                mStateTimer = 0;
            }
            break;

        case FlyswatterState::cRecoverRise:
            mPosition.y += 0.10f;
            if (mPosition.y >= cApexHeight) {
                mPosition.y = cApexHeight;
                mState = FlyswatterState::cHoverIdle;
                mStateTimer = 0;
            }
            break;

        case FlyswatterState::cDefeated:
        default:
            break;
    }
}

void Enm_Flyswatter::update() {
    mStateTimer++;
}

void Enm_Flyswatter::draw() {
    if (mState != FlyswatterState::cDefeated) {
        GambitActor::draw();
    }
}

} // namespace Game
