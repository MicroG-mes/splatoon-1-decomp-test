#pragma once

#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include <string>

namespace Game {

class Obj_MissilePosition : public GambitActor {
public:
    enum class State {
        cWait = 0,   // Idle: action guide displayed when player within mActionGuideRadius
        cBind = 1,   // Bind: player in cockpit, aiming, warm-up mBindNoShootFrame
        cShot = 2    // Shot: firing projectile, cooling down / reloading
    };

    struct Params {
        int mBindNoShootFrame = 30;      // Solo: 30f, VS: 15f
        float mMissileSpeed = 5.0f;       // Launch velocity: 5.0 m/s
        int mMinShotInterval = 60;        // Minimum refire cadence: 60f (solo), 70f (VS)
        float mActionGuideRadius = 50.0f; // Prompt radius: 50.0m (solo), 75.0m (VS)
        float mActionGuideOffsetY = 10.0f;// Prompt vertical offset: 10.0m
        int mCoolingFrame = 600;          // Standard cooling duration (VS): 600f (10.0s)
        int mCoolingFrameOverheat = 600;  // Overheat penalty cooling duration: 600f
        float mHeatRatePerShoot = 0.0f;   // Heat generated per rocket launch (0.0% - 100.0%)
        int mFirstPersonCameraSeFrame = 5;// First-person camera SFX trigger frame
        float mMuzzlePitchRangeDeg = 45.0f; // Max barrel elevation angle
    };

    struct BoneHierarchy {
        sead::Vector3f mCockpitPos{0.0f, 0.0f, 0.0f};
        sead::Vector3f mCannonPos{0.0f, 1.5f, 0.0f};
        sead::Vector3f mBarrelPos{0.0f, 2.0f, 0.5f};
        sead::Vector3f mMuzzlePoint{0.0f, 2.2f, 1.8f};
        float mBarrelPitchDeg = 0.0f;
        float mTurretYawDeg = 0.0f;
    };

    Obj_MissilePosition();
    virtual ~Obj_MissilePosition() = default;

    virtual void init() override;
    virtual void update() override;

    bool loadParams(const char* filePath = "content/Static/Obj_MissilePosition.params");
    bool loadParamsVS(const char* filePath = "content/Static/Obj_MissilePositionVS.params");

    State getState() const { return mState; }
    const Params& getParams() const { return mParams; }
    const BoneHierarchy& getBones() const { return mBones; }

    bool isOccupied() const { return mState != State::cWait; }
    int getOccupantPlayerId() const { return mOccupantPlayerId; }

    bool enterCockpit(int playerId);
    void exitCockpit();

    void setAim(float pitchDeg, float yawDeg);
    bool fireMissile(const sead::Vector3f& targetPos);

    bool isPlayerInGuideRadius(const sead::Vector3f& playerPos) const;
    sead::Vector3f getGuideMarkerPos() const;

    float getHeatRate() const { return mHeatRate; }
    bool isOverheated() const { return mIsOverheated; }
    int getFiredMissileCount() const { return mFiredMissileCount; }
    const std::string& getName() const { return mName; }
    bool isVsMode() const { return mIsVsMode; }

private:
    std::string mName = "Obj_MissilePosition";
    State mState = State::cWait;
    Params mParams;
    BoneHierarchy mBones;

    bool mIsVsMode = false;
    int mOccupantPlayerId = -1;
    int mBindFrameCounter = 0;
    int mShotCooldownTimer = 0;
    int mOverheatTimer = 0;
    float mHeatRate = 0.0f;
    bool mIsOverheated = false;
    int mFiredMissileCount = 0;
};

} // namespace Game
