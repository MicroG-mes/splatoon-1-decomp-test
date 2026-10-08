#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include <string>

namespace Game {

enum class OctavioPilotState : u32 {
    cState_Idle = 0,
    cState_TrackingPlayer,
    cState_Damaged,
    cState_ShiokaraHypnotized,
    cState_Defeated
};

struct RailKingPilotHouseParams {
    s32 damageFrame;            // mDamageFrame (12)
    f32 searchRadius;           // mSearchRadius (100.0)
    f32 rotSpeedDeg;            // mRotSpeedDeg (1.0)
    f32 eyeMaxAngleDeg;         // mEyeUVDirectableParam angle (45.0)
    f32 eyeLerpRate;            // mEyeUVDirectableParam rate (0.10)
    f32 worldWait;              // WorldWait (20.0)
    f32 worldWaitHit;           // WorldWaitHit (10.0)

    bool load(const char* paramsPath, const char* anmPath);
};

/**
 * Obj_RailKingPilotHouse
 * DJ Octavio's armored DJ booth cockpit aboard the Octobot King.
 * Features 360-degree player tracking, reactive eye UV deflection,
 * and hypnotic Calamari Inkantation (Shiokara) groove mechanics.
 */
class Obj_RailKingPilotHouse : public GambitActor {
public:
    Obj_RailKingPilotHouse();
    virtual ~Obj_RailKingPilotHouse() override;

    virtual void init() override;
    void init(const sead::Vector3f& pos);
    virtual void update() override;

    // Player tracking & target acquisition
    bool trackPlayer(const sead::Vector3f& playerPos);

    // Hit by reflected giant rocket punch
    void takePunchDamage();

    // Trigger Calamari Inkantation (Squid Sisters broadcast)
    void startShiokaraGroove(u32 phase = 1);

    // Getters
    OctavioPilotState getState() const { return mState; }
    f32 getCurrentYawDeg() const { return mCurrentYawDeg; }
    f32 getEyeDeflectionDeg() const { return mEyeDeflectionDeg; }
    bool isHypnotized() const { return mState == OctavioPilotState::cState_ShiokaraHypnotized; }
    u32 getShiokaraPhase() const { return mShiokaraPhase; }
    const RailKingPilotHouseParams& getParams() const { return mParams; }

private:
    OctavioPilotState mState;
    RailKingPilotHouseParams mParams;
    f32 mCurrentYawDeg;
    f32 mTargetYawDeg;
    f32 mEyeDeflectionDeg;
    s32 mDamageTimer;
    u32 mShiokaraPhase;
    f32 mGrooveTimer;
};

} // namespace Game
