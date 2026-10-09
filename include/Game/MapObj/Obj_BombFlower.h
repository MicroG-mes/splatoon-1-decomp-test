#pragma once

#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include <string>

namespace Game {

enum class BombFlowerState {
    cState_Growing = 0,     // Sprouting from bulb (mAppearFrame = 15f)
    cState_Ready = 1,       // Fully grown balloon flower, vulnerable to ink
    cState_Burst = 2,       // Detonating in 12.0m blast of ink & 120 HP damage
    cState_Respawning = 3   // Depleted bulb waiting to respawn (mBurstWaitFrame = 180f)
};

class Obj_BombFlower : public GambitActor {
public:
    struct Params {
        float mMaxHp = 2.0f;                    // Health threshold before bursting (2.0f)
        float mBalloonRadius = 13.0f;           // Flower head collision radius
        float mBalloonOffsetY = 20.0f;          // Height above stem
        int mBurstWaitFrame = 180;              // Respawn delay (180 frames = 3.0s)
        int mAppearFrame = 15;                  // Growth animation frames
        int mAppearWaitFrame = 60;
        float mBombCorePaintRadius = 120.0f;    // Paint blast radius (12.0m)
        float mBombCoreDamageRadius = 90.0f;    // Lethal blast damage radius (9.0m)
        float mBombCoreDamage = 12.0f;          // Detonation damage (120 HP OHKO)
        int mSplashNum = 15;                    // Droplet splash count
        float mBalloonKp = 0.04f;               // Spring stiffness
        float mBalloonKd = 0.30f;               // Spring damping
    };

    Obj_BombFlower();
    virtual ~Obj_BombFlower() = default;

    virtual void init() override;
    virtual void update() override;

    bool loadParams(const char* filePath = "content/Static/Obj_BombFlower.params");

    BombFlowerState getState() const { return mState; }
    const Params& getParams() const { return mParams; }
    const std::string& getName() const { return mName; }

    float getHealth() const { return mHealth; }
    u32 getLastHitterTeam() const { return mLastHitterTeam; }
    bool isDetonated() const { return mState == BombFlowerState::cState_Burst; }
    bool isReady() const { return mState == BombFlowerState::cState_Ready; }

    bool applyDamage(float damage, u32 teamId = 0);
    bool checkBlastHit(const sead::Vector3f& targetPos, float& outDamage) const;

private:
    std::string mName = "Obj_BombFlower";
    BombFlowerState mState = BombFlowerState::cState_Ready;
    Params mParams;

    float mHealth = 2.0f;
    u32 mLastHitterTeam = 0;
    int mTimer = 0;
};

} // namespace Game
