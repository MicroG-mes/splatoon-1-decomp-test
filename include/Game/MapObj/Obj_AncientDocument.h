#pragma once

#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include <string>

namespace Game {

enum class AncientDocState {
    cState_Hidden = 0,    // Concealed until spawned
    cState_Idle = 1,      // Floating, spinning with ItemPillar light beam
    cState_Collected = 2  // Picked up by player
};

class Obj_AncientDocument : public GambitActor {
public:
    struct Params {
        float mRotationSpeed = 0.05f;           // Yaw spin speed (rad/frame) (DAT_1009d7bc: 0.05f)
        float mBobbingAmplitude = 1.1f;         // Vertical floating amplitude (DAT_1009d7c0: 1.1f)
        float mPillarHeight = 100.0f;           // Light beam height (DAT_1009d7d8: 100.0f)
        float mPillarRadius = 3.8f;             // Light beam radius (DAT_1009d7dc: 3.8f)
        float mCollectRadius = 3.0f;            // Direct pickup touch radius (DAT_1009d7e0: 3.0f)
        float mHitboxRadius = 10.0f;            // Bounding collision radius (DAT_1009d790: 10.0f)
        float mPillarGlowRate = 0.5f;           // Glow intensity ramp per frame (DAT_1009d7e8: 0.5f)
        float mMaxGlowIntensity = 10.0f;        // Peak beam intensity (DAT_1009d790: 10.0f)
    };

    Obj_AncientDocument(int documentId = 1, bool isDummy = false);
    virtual ~Obj_AncientDocument() = default;

    virtual void init() override;
    virtual void update() override;

    void spawn(const sead::Vector3f& pos, int documentId = 1, bool isDummy = false);

    AncientDocState getState() const { return mState; }
    const Params& getParams() const { return mParams; }
    const std::string& getName() const { return mName; }

    int getDocumentId() const { return mDocumentId; }
    bool isDummy() const { return mIsDummy; }
    bool isCollected() const { return mState == AncientDocState::cState_Collected; }

    float getYaw() const { return mYaw; }
    float getBobbingOffset() const { return mBobbingY; }
    float getPillarIntensity() const { return mPillarIntensity; }
    float getPillarHeight() const { return mParams.mPillarHeight; }
    float getPillarRadius() const { return mParams.mPillarRadius; }

    bool tryCollect(const sead::Vector3f& playerPos, float playerRadius = 2.0f, u32 playerId = 0);

private:
    std::string mName = "Obj_AncientDocument";
    AncientDocState mState = AncientDocState::cState_Idle;
    Params mParams;

    int mDocumentId = 1;      // Octo Valley Sunken Scroll Index (1..27)
    bool mIsDummy = false;    // True if already collected previously in save data
    float mYaw = 0.0f;
    float mBobbingY = 0.0f;
    float mPillarIntensity = 0.0f;
    int mFrameCounter = 0;
    u32 mCollectorId = 0;
};

} // namespace Game
