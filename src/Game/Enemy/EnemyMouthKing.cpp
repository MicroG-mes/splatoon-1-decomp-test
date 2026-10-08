#include "Game/Enemy/EnemyMouthKing.h"
#include "Game/Paint/PaintTextureMgr.h"

namespace Game {

EnemyMouthKing::EnemyMouthKing()
    : mState(OctomawState::cSubmergedSwim),
      mPhase(OctomawPhase::cPhase1),
      mStateTimer(0),
      mPosition(0.0f, -2.0f, 0.0f),
      mTargetPlayerPos(0.0f, 0.0f, 0.0f),
      mTentacleHp(100.0f) {
}

EnemyMouthKing::~EnemyMouthKing() {
}

void EnemyMouthKing::init() {
    GambitActor::init();
    mPhase = OctomawPhase::cPhase1;
    mTentacleHp = 100.0f;
    mState = OctomawState::cSubmergedSwim;
    mStateTimer = 0;
    mPosition.set(0.0f, -2.0f, 0.0f);

    setupPhaseTeeth();
}

void EnemyMouthKing::setupPhaseTeeth() {
    ToothArmorType armor = ToothArmorType::cNormal;
    if (mPhase == OctomawPhase::cPhase2) {
        armor = ToothArmorType::cReinforced;
    } else if (mPhase == OctomawPhase::cPhase3) {
        armor = ToothArmorType::cGold;
    }

    for (u32 i = 0; i < cToothCount; ++i) {
        mTeeth[i].init();
        mTeeth[i].setupTooth(i, armor);
    }
}

u32 EnemyMouthKing::countBrokenTeeth() const {
    u32 count = 0;
    for (u32 i = 0; i < cToothCount; ++i) {
        if (mTeeth[i].isBroken()) {
            count++;
        }
    }
    return count;
}

void EnemyMouthKing::applyBombToMouth(f32 bombDamage) {
    if (mState != OctomawState::cLeapChomp && mState != OctomawState::cBreachTarget) {
        return;
    }

    // Damage all remaining teeth
    for (u32 i = 0; i < cToothCount; ++i) {
        mTeeth[i].applyDamage(bombDamage);
    }

    // If more than half the teeth are broken, crash down stunned
    if (countBrokenTeeth() >= (cToothCount / 2)) {
        mState = OctomawState::cStunnedExposed;
        mStateTimer = 0;
        mPosition.y = 0.0f; // Rest flat on ground
    }
}

void EnemyMouthKing::applyTentacleDamage(f32 damage) {
    if (mState != OctomawState::cStunnedExposed) {
        return;
    }

    mTentacleHp -= damage;
    if (mTentacleHp <= 0.0f) {
        if (mPhase == OctomawPhase::cPhase1) {
            mPhase = OctomawPhase::cPhase2;
            mTentacleHp = 100.0f;
            mState = OctomawState::cSubmergeRecover;
            mStateTimer = 0;
            setupPhaseTeeth();
        } else if (mPhase == OctomawPhase::cPhase2) {
            mPhase = OctomawPhase::cPhase3;
            mTentacleHp = 100.0f;
            mState = OctomawState::cSubmergeRecover;
            mStateTimer = 0;
            setupPhaseTeeth();
        } else {
            mState = OctomawState::cDefeated;
            mStateTimer = 0;
        }
    }
}

void EnemyMouthKing::updateBossAi(const sead::Vector3f& playerPos) {
    if (mState == OctomawState::cDefeated) {
        return;
    }

    switch (mState) {
        case OctomawState::cSubmergedSwim: {
            // Circle or stalk player underneath purple ink
            f32 speed = (mPhase == OctomawPhase::cPhase1) ? 0.08f : 0.14f;
            mPosition.x += (playerPos.x - mPosition.x) * speed;
            mPosition.z += (playerPos.z - mPosition.z) * speed;
            mPosition.y = -1.5f;

            if (mStateTimer >= 180) { // 3 seconds stalk
                mTargetPlayerPos = playerPos;
                mState = OctomawState::cBreachTarget;
                mStateTimer = 0;
            }
            break;
        }

        case OctomawState::cBreachTarget:
            // Submerged telegraph rumble under target position
            if (mStateTimer >= 45) {
                mPosition.x = mTargetPlayerPos.x;
                mPosition.z = mTargetPlayerPos.z;
                mState = OctomawState::cLeapChomp;
                mStateTimer = 0;
            }
            break;

        case OctomawState::cLeapChomp: {
            // Erupts upward with open mouth
            mPosition.y += 0.35f;
            if (mPosition.y >= 8.0f) {
                // Missed bomb: chomps down and dives back into ink
                mState = OctomawState::cSubmergeRecover;
                mStateTimer = 0;
            }
            break;
        }

        case OctomawState::cStunnedExposed: {
            // Flat on arena with open mouth, tentacle exposed
            mPosition.y = 0.0f;
            s32 stunDuration = (mPhase == OctomawPhase::cPhase1) ? 360 : (mPhase == OctomawPhase::cPhase2 ? 300 : 240);
            if (mStateTimer >= stunDuration) {
                mState = OctomawState::cSubmergeRecover;
                mStateTimer = 0;
            }
            break;
        }

        case OctomawState::cSubmergeRecover: {
            // Retracts down into ink floor
            mPosition.y -= 0.25f;
            if (mPosition.y <= -2.0f) {
                mPosition.y = -2.0f;
                mState = OctomawState::cSubmergedSwim;
                mStateTimer = 0;

                // Splat ink on re-entry
                PaintTextureMgr* paint = PaintTextureMgr::instance();
                if (paint) {
                    paint->splatInk(mPosition, 6.0f, 1);
                }
            }
            break;
        }

        default:
            break;
    }
}

void EnemyMouthKing::update() {
    mStateTimer++;
    for (u32 i = 0; i < cToothCount; ++i) {
        mTeeth[i].update();
    }
}

void EnemyMouthKing::draw() {
    if (mState != OctomawState::cDefeated) {
        GambitActor::draw();
        for (u32 i = 0; i < cToothCount; ++i) {
            mTeeth[i].draw();
        }
    }
}

} // namespace Game
