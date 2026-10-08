#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class IkastoneState : u32 {
    cState_Idle = 0,
    cState_GuideActive,
    cState_Activated,
    cState_Reacting
};

struct IkastoneParams {
    f32 actionRadius;               // mActionRadius (50.0)
    f32 actionAngleDeg;             // mActionAngleDeg (60.0)
    sead::Vector3f actionGuideOffset;// mActionGuideOffset (0, 18.6, 2.5)
    sead::Vector3f spawnPosOffset;  // mSpawnPosOffset (0, -18.594, 25.0)
    s32 reactionAnimCancelFrame;    // mReactionAnimCancelFrame (20)
    f32 startAnimFrames;            // Start (60.0)

    bool load(const char* paramsPath, const char* anmPath);
};

/**
 * Obj_Ikastone
 * Squid Stone ancient monument / directional guidance obelisk.
 * Activates when approached within 50m and 60 degrees cone, revealing path mechanisms.
 */
class Obj_Ikastone : public GambitActor {
public:
    Obj_Ikastone();
    virtual ~Obj_Ikastone() override;

    virtual void init() override;
    void init(const sead::Vector3f& pos, f32 facingAngleDeg = 0.0f);
    virtual void update() override;

    // Proximity and cone detection
    bool checkPlayerApproach(const sead::Vector3f& playerPos, f32 playerFacingDeg);

    // Interaction activation
    bool activate();

    // Getters
    IkastoneState getState() const { return mState; }
    bool isGuideActive() const { return mState == IkastoneState::cState_GuideActive; }
    bool isActivated() const { return mState == IkastoneState::cState_Activated; }
    sead::Vector3f getGuideWorldPosition() const { return mPosition + mParams.actionGuideOffset; }
    sead::Vector3f getSpawnWorldPosition() const { return mPosition + mParams.spawnPosOffset; }
    const IkastoneParams& getParams() const { return mParams; }

private:
    IkastoneState mState;
    IkastoneParams mParams;
    f32 mFacingAngleDeg;
    s32 mActivationTimer;
};

} // namespace Game
