#include "Game/Weapon/Wsp_Shachihoko.h"
#include "Game/Paint/PaintTextureMgr.h"

namespace Game {

Wsp_Shachihoko::Wsp_Shachihoko()
    : mPosition(0.0f, 0.0f, 0.0f),
      mState(ShachihokoShieldState::cShieldCharging),
      mCarrierPlayerId(-1),
      mCarrierTeamId(-1),
      mCarrierTimer(cCarrierTimeLimit),
      mShieldAlphaHp(0.0f),
      mShieldBravoHp(0.0f),
      mBurstTimer(0) {
}

Wsp_Shachihoko::~Wsp_Shachihoko() {
}

void Wsp_Shachihoko::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mState = ShachihokoShieldState::cShieldCharging;
    mCarrierPlayerId = -1;
    mCarrierTeamId = -1;
    mCarrierTimer = cCarrierTimeLimit;
    mShieldAlphaHp = 0.0f;
    mShieldBravoHp = 0.0f;
    mBurstTimer = 0;
}

void Wsp_Shachihoko::applyInkToShield(s32 teamId, f32 amount) {
    if (mState != ShachihokoShieldState::cShieldCharging) {
        return;
    }

    if (teamId == 0) {
        mShieldAlphaHp += amount;
        mShieldBravoHp -= amount * 0.5f;
        if (mShieldBravoHp < 0.0f) mShieldBravoHp = 0.0f;

        if (mShieldAlphaHp >= cBurstThreshold) {
            triggerShieldBurst(0);
        }
    } else if (teamId == 1) {
        mShieldBravoHp += amount;
        mShieldAlphaHp -= amount * 0.5f;
        if (mShieldAlphaHp < 0.0f) mShieldAlphaHp = 0.0f;

        if (mShieldBravoHp >= cBurstThreshold) {
            triggerShieldBurst(1);
        }
    }
}

void Wsp_Shachihoko::triggerShieldBurst(s32 winningTeam) {
    mState = ShachihokoShieldState::cShieldBursting;
    mBurstTimer = 0;

    PaintTextureMgr* paint = PaintTextureMgr::instance();
    if (paint) {
        paint->splatInk(mPosition, cBurstRadius, static_cast<u32>(winningTeam));
    }
}

bool Wsp_Shachihoko::pickup(u32 playerId, s32 teamId) {
    if (mState != ShachihokoShieldState::cFreePickup) {
        return false;
    }

    mCarrierPlayerId = static_cast<s32>(playerId);
    mCarrierTeamId = teamId;
    mCarrierTimer = cCarrierTimeLimit;
    mState = ShachihokoShieldState::cCarried;

    return true;
}

void Wsp_Shachihoko::drop(const sead::Vector3f& dropPos) {
    mPosition = dropPos;
    mCarrierPlayerId = -1;
    mCarrierTeamId = -1;
    mCarrierTimer = cCarrierTimeLimit;
    mShieldAlphaHp = 0.0f;
    mShieldBravoHp = 0.0f;
    mState = ShachihokoShieldState::cShieldCharging;
}

void Wsp_Shachihoko::update() {
    switch (mState) {
        case ShachihokoShieldState::cShieldBursting:
            mBurstTimer++;
            if (mBurstTimer >= 30) {
                mState = ShachihokoShieldState::cFreePickup;
                mBurstTimer = 0;
            }
            break;

        case ShachihokoShieldState::cCarried:
            mCarrierTimer--;
            if (mCarrierTimer <= 0) {
                // Time up! Carrier self-destructs in a blast of enemy ink
                PaintTextureMgr* paint = PaintTextureMgr::instance();
                if (paint) {
                    paint->splatInk(mPosition, 5.0f, (mCarrierTeamId == 0) ? 1 : 0);
                }
                drop(mPosition);
            }
            break;

        case ShachihokoShieldState::cShieldCharging:
        case ShachihokoShieldState::cFreePickup:
        default:
            break;
    }
}

void Wsp_Shachihoko::draw() {
    if (mState != ShachihokoShieldState::cCarried) {
        GambitActor::draw();
    }
}

} // namespace Game
