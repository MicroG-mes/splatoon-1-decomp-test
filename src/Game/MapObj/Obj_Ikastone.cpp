#include "Game/MapObj/Obj_Ikastone.h"
#include "Game/System/AglParameter.h"
#include <cmath>

namespace Game {

bool IkastoneParams::load(const char* paramsPath, const char* anmPath) {
    actionRadius = 50.00000000f;
    actionAngleDeg = 60.00000000f;
    actionGuideOffset.set(0.00000000f, 18.60000038f, 2.50000000f);
    spawnPosOffset.set(0.00000000f, -18.59399986f, 25.00000000f);
    reactionAnimCancelFrame = 20;
    startAnimFrames = 60.00000000f;

    if (paramsPath) {
        AglParameterObj obj;
        if (obj.loadFromFile(paramsPath)) {
            actionRadius = obj.getFloat("mActionRadius", actionRadius);
            actionAngleDeg = obj.getFloat("mActionAngleDeg", actionAngleDeg);
            reactionAnimCancelFrame = obj.getInt("mReactionAnimCancelFrame", reactionAnimCancelFrame);

            auto guideArr = obj.getFloatArray("mActionGuideOffset");
            if (guideArr.size() >= 3) {
                actionGuideOffset.set(guideArr[0], guideArr[1], guideArr[2]);
            }

            auto spawnArr = obj.getFloatArray("mSpawnPosOffset");
            if (spawnArr.size() >= 3) {
                spawnPosOffset.set(spawnArr[0], spawnArr[1], spawnArr[2]);
            }
        }
    }

    if (anmPath) {
        AglParameterObj anmObj;
        if (anmObj.loadFromFile(anmPath)) {
            startAnimFrames = anmObj.getFloat("Start", startAnimFrames);
        }
    }

    return true;
}

Obj_Ikastone::Obj_Ikastone()
    : mState(IkastoneState::cState_Idle)
    , mFacingAngleDeg(0.0f)
    , mActivationTimer(0) {
    mPosition.set(0.0f, 0.0f, 0.0f);
    mParams.load("content/Static/Obj_Ikastone.params", "content/Static/Obj_Ikastone_AnmItp.params");
}

Obj_Ikastone::~Obj_Ikastone() {}

void Obj_Ikastone::init() {
    init(mPosition, 0.0f);
}

void Obj_Ikastone::init(const sead::Vector3f& pos, f32 facingAngleDeg) {
    mPosition = pos;
    mFacingAngleDeg = facingAngleDeg;
    mState = IkastoneState::cState_Idle;
    mActivationTimer = 0;
    mParams.load("content/Static/Obj_Ikastone.params", "content/Static/Obj_Ikastone_AnmItp.params");
}

bool Obj_Ikastone::checkPlayerApproach(const sead::Vector3f& playerPos, f32 playerFacingDeg) {
    f32 dx = playerPos.x - mPosition.x;
    f32 dz = playerPos.z - mPosition.z;
    f32 dist = std::sqrt(dx * dx + dz * dz);

    if (dist <= mParams.actionRadius) {
        // Calculate relative direction angle to player in degrees
        f32 angleToPlayer = std::atan2(dx, dz) * (180.0f / 3.14159265f);
        f32 diffAngle = std::abs(angleToPlayer - mFacingAngleDeg);
        while (diffAngle > 180.0f) diffAngle -= 360.0f;
        diffAngle = std::abs(diffAngle);

        if (diffAngle <= (mParams.actionAngleDeg * 0.5f)) {
            if (mState == IkastoneState::cState_Idle) {
                mState = IkastoneState::cState_GuideActive;
            }
            return true;
        }
    }

    if (mState == IkastoneState::cState_GuideActive) {
        mState = IkastoneState::cState_Idle;
    }
    return false;
}

bool Obj_Ikastone::activate() {
    mState = IkastoneState::cState_Activated;
    mActivationTimer = 0;
    return true;
}

void Obj_Ikastone::update() {
    if (mState == IkastoneState::cState_Activated) {
        mActivationTimer++;
        if (mActivationTimer >= static_cast<s32>(mParams.startAnimFrames)) {
            mState = IkastoneState::cState_Reacting;
        }
    }
}

} // namespace Game
