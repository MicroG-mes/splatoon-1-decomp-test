#include "Game/MapObj/Obj_AncientDocument.h"
#include <cmath>

namespace Game {

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Obj_AncientDocument::Obj_AncientDocument(int documentId, bool isDummy)
    : mDocumentId(documentId)
    , mIsDummy(isDummy)
{
    mName = mIsDummy ? "Obj_AncientDocumentDummy" : "Obj_AncientDocument";
}

void Obj_AncientDocument::init() {
    mState = AncientDocState::cState_Idle;
    mYaw = 0.0f;
    mBobbingY = 0.0f;
    mPillarIntensity = 0.0f;
    mFrameCounter = 0;
    mCollectorId = 0;
}

void Obj_AncientDocument::spawn(const sead::Vector3f& pos, int documentId, bool isDummy) {
    mPosition = pos;
    mDocumentId = documentId;
    mIsDummy = isDummy;
    mName = mIsDummy ? "Obj_AncientDocumentDummy" : "Obj_AncientDocument";
    init();
}

void Obj_AncientDocument::update() {
    if (mState != AncientDocState::cState_Idle) {
        return;
    }

    mFrameCounter++;

    // Continuous yaw rotation: spins at 0.05 rad/frame (DAT_1009d7bc)
    mYaw += mParams.mRotationSpeed;
    if (mYaw > static_cast<float>(M_PI * 2.0)) {
        mYaw -= static_cast<float>(M_PI * 2.0);
    }

    // Gentle vertical bobbing float: amplitude 1.1 units (DAT_1009d7c0)
    // 60-frame cycle for smooth breathing motion
    float bobbingAngle = static_cast<float>(mFrameCounter) * (2.0f * static_cast<float>(M_PI) / 60.0f);
    mBobbingY = std::sin(bobbingAngle) * mParams.mBobbingAmplitude;

    // ItemPillar beam glow intensity ramp-up: increases by 0.5 per frame up to 10.0 (DAT_1009d7e8, DAT_1009d790)
    if (mPillarIntensity < mParams.mMaxGlowIntensity) {
        mPillarIntensity += mParams.mPillarGlowRate;
        if (mPillarIntensity > mParams.mMaxGlowIntensity) {
            mPillarIntensity = mParams.mMaxGlowIntensity;
        }
    }
}

bool Obj_AncientDocument::tryCollect(const sead::Vector3f& playerPos, float playerRadius, u32 playerId) {
    if (mState != AncientDocState::cState_Idle) {
        return false;
    }

    sead::Vector3f docPos = mPosition;
    docPos.y += mBobbingY;

    float dx = playerPos.x - docPos.x;
    float dy = playerPos.y - docPos.y;
    float dz = playerPos.z - docPos.z;
    float distSq = dx * dx + dy * dy + dz * dz;

    float pickupDist = mParams.mCollectRadius + playerRadius;
    if (distSq <= (pickupDist * pickupDist)) {
        mState = AncientDocState::cState_Collected;
        mCollectorId = playerId;
        mPillarIntensity = 0.0f; // Beam extinguished upon acquisition
        return true;
    }

    return false;
}

} // namespace Game
