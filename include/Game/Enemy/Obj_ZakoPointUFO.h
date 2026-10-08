#pragma once

#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include <string>
#include <vector>

namespace Game {

class Obj_ZakoPointUFO : public GambitActor {
public:
    enum class UfoType {
        OctUfoBox = 0,    // Standard Octarian UFO cargo box transport (Lft_OctUfoBox)
        OctUfoWall = 1,   // Octarian vertical wall transport craft (Lft_OctUfoWall)
        RvlUfoMini = 2    // Octoling assault mini-UFO invasion craft (Lft_RvlUfoMini)
    };

    enum class State {
        Hovering = 0,     // Cruising/hovering at altitude along patrol track
        Deploying = 1,    // Hatch open, beaming/dropping enemy reinforcements
        CoolingDown = 2,  // Waiting between spawn waves
        Retreating = 3,   // Ascending out of arena
        Destroyed = 4     // HP depleted, crashing/exploding
    };

    struct Params {
        float mLife = 1000.0f;           // Durability from Obj_ZakoPointUFO.params (1000.0 HP)
        int mSpawnIntervalFrames = 180;  // 3.0 seconds between drops
        int mMaxActiveEnemies = 4;       // Maximum active concurrent spawned grunts
        float mCruiseAltitudeY = 15.0f;  // Hover height above terrain
        float mDropSpeed = 0.5f;         // Reinforcement descent speed
    };

    Obj_ZakoPointUFO(UfoType type = UfoType::OctUfoBox);
    virtual ~Obj_ZakoPointUFO() = default;

    virtual void init() override;
    virtual void update() override;

    bool loadParams(const char* filePath = "content/Static/Obj_ZakoPointUFO.params");

    UfoType getUfoType() const { return mUfoType; }
    void setUfoType(UfoType type) { mUfoType = type; }

    State getState() const { return mState; }
    const Params& getParams() const { return mParams; }

    float getHealth() const { return mHealth; }
    float getMaxHealth() const { return mParams.mLife; }
    bool isAlive() const { return mHealth > 0.0f && mState != State::Destroyed; }

    void applyDamage(float damage);
    bool triggerSpawnWave();

    int getTotalSpawnedCount() const { return mTotalSpawnedCount; }
    int getActiveEnemyCount() const { return mActiveEnemyCount; }
    void onEnemyDefeated();

    void setHoverAltitude(float altY) { mHoverY = altY; }
    float getHoverAltitude() const { return mHoverY; }
    const std::string& getName() const { return mName; }

private:
    std::string mName = "Obj_ZakoPointUFO";
    UfoType mUfoType = UfoType::OctUfoBox;
    State mState = State::Hovering;
    Params mParams;

    float mHealth = 1000.0f;
    float mHoverY = 15.0f;
    int mSpawnTimer = 0;
    int mTotalSpawnedCount = 0;
    int mActiveEnemyCount = 0;
};

} // namespace Game
