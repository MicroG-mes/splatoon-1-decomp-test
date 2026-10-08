#include "Game/Enemy/Shield.h"
#include <cmath>

namespace Game {

Shield::Shield()
    : mPosition(0.0f, 0.0f, 0.0f),
      mFacingYaw(0.0f),
      mHitFlashTimer(0) {
}

Shield::~Shield() {
}

void Shield::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mFacingYaw = 0.0f;
    mHitFlashTimer = 0;
}

void Shield::attachToOwner(const sead::Vector3f& ownerPos, f32 facingYaw) {
    mFacingYaw = facingYaw;
    // Position shield 0.8m directly in front of the owner
    f32 dirX = std::sin(facingYaw);
    f32 dirZ = std::cos(facingYaw);
    mPosition.set(ownerPos.x + dirX * 0.8f, ownerPos.y, ownerPos.z + dirZ * 0.8f);
}

bool Shield::checkFrontalDeflection(const sead::Vector3f& shotIncomingDir) const {
    // Forward vector of the shield
    f32 shieldFwdX = std::sin(mFacingYaw);
    f32 shieldFwdZ = std::cos(mFacingYaw);

    // Dot product between shield forward and reverse shot incoming direction
    f32 dot = (shieldFwdX * -shotIncomingDir.x) + (shieldFwdZ * -shotIncomingDir.z);

    // If angle is within forward arc (cos(75 deg) = ~0.258)
    if (dot > 0.258f) {
        return true; // Shot blocked and deflected!
    }

    return false; // Flanking hit or rear hit
}

void Shield::update() {
    if (mHitFlashTimer > 0) {
        mHitFlashTimer--;
    }
}

void Shield::draw() {
    GambitActor::draw();
}

} // namespace Game
