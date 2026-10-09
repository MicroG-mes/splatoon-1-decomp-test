#pragma once

#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include <string>

namespace Game {

enum class DefenseTowerState {
    Intact = 0,
    Damaged = 1,
    Destroyed = 2
};

class Obj_DefenseTower : public GambitActor {
public:
    struct Params {
        float mLife = 35.0f;        // Core HP (35.0 = 350 hit points)
        float mCurableRate = 0.50f; // Regen recovery rate factor (0.50)
    };

    Obj_DefenseTower();
    virtual ~Obj_DefenseTower() = default;

    virtual void init() override;
    virtual void update() override;

    bool loadParams(const char* filePath = "content/Static/Obj_DefenseTower.params");

    DefenseTowerState getState() const { return mState; }
    const Params& getParams() const { return mParams; }
    const std::string& getName() const { return mName; }

    float getHealth() const { return mHealth; }
    float getMaxHealth() const { return mParams.mLife; }
    bool isAlive() const { return mState != DefenseTowerState::Destroyed && mHealth > 0.0f; }

    bool applyDamage(float damage);
    void repair(float amount);

private:
    std::string mName = "Obj_DefenseTower";
    DefenseTowerState mState = DefenseTowerState::Intact;
    Params mParams;

    float mHealth = 35.0f;
    int mDamageCooldownTimer = 0;
};

} // namespace Game
