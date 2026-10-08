#include "Game/MapObj/Obj_GeneralBox.h"
#include "Game/System/PcAudioDriver.h"
#include <cmath>

namespace Game {

Obj_GeneralBox::Obj_GeneralBox()
    : mPosition(0.0f, 0.0f, 0.0f),
      mScale(1.0f),
      mMaxHp(80.0f),
      mCurrentHp(80.0f),
      mIsBroken(false),
      mBreakTimer(0),
      mCoveredTeam(0),
      mHasKcl(false) {
}

Obj_GeneralBox::~Obj_GeneralBox() {
}

void Obj_GeneralBox::init() {
    GambitActor::init();
    mCurrentHp = mMaxHp;
    mIsBroken = false;
    mBreakTimer = 0;
}

void Obj_GeneralBox::setup(const sead::Vector3f& position, f32 maxHp, f32 scale) {
    mPosition = position;
    mMaxHp = maxHp;
    mCurrentHp = maxHp;
    mScale = scale;
    mIsBroken = false;
    mBreakTimer = 0;
}

bool Obj_GeneralBox::loadCollision(const char* szsFilePath) {
    mHasKcl = mKcl.loadFromSzsFile(szsFilePath);
    return mHasKcl;
}

bool Obj_GeneralBox::checkBulletCollision(const sead::Vector3f& bulletPos, f32 radius, f32 damage, u8 teamId) {
    if (mIsBroken) return false;

    f32 halfSize = (cDefaultSize * mScale) * 0.5f;
    f32 minX = mPosition.x - halfSize - radius;
    f32 maxX = mPosition.x + halfSize + radius;
    f32 minY = mPosition.y - radius;
    f32 maxY = mPosition.y + (cDefaultSize * mScale) + radius;
    f32 minZ = mPosition.z - halfSize - radius;
    f32 maxZ = mPosition.z + halfSize + radius;

    if (bulletPos.x >= minX && bulletPos.x <= maxX &&
        bulletPos.y >= minY && bulletPos.y <= maxY &&
        bulletPos.z >= minZ && bulletPos.z <= maxZ) {
        applyDamage(damage, teamId);
        return true;
    }
    return false;
}

void Obj_GeneralBox::applyDamage(f32 damage, u8 teamId) {
    if (mIsBroken) return;

    mCurrentHp -= damage;
    mCoveredTeam = teamId;

    if (mCurrentHp <= 0.0f) {
        mCurrentHp = 0.0f;
        mIsBroken = true;
        mBreakTimer = 0;
        PcAudioDriver::instance().playSound(cSoundId_Crate_Break, 1.0f, 0.0f);
    } else {
        PcAudioDriver::instance().playSound(cSoundId_Hit_Confirm, 0.7f, 0.0f);
    }
}

void Obj_GeneralBox::update() {
    if (mIsBroken) {
        mBreakTimer++;
    }
}

void Obj_GeneralBox::draw() {
}

} // namespace Game
