#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"

namespace Game {

struct HeroGearUpgrades {
    u32 heroShotLevel;   // 1 to 3
    u32 inkTankLevel;    // 1 to 3
    bool hasBurstBomb;
    bool hasSuctionBomb;
    u32 powerEggsBank;
};

class PlayerCustomPartMission : public GambitActor {
public:
    PlayerCustomPartMission();
    virtual ~PlayerCustomPartMission() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void addPowerEggs(u32 count);
    bool upgradeHeroShot();
    bool upgradeInkTank();
    bool unlockBurstBomb();
    bool unlockSuctionBomb();

    u32 getHeroShotLevel() const { return mUpgrades.heroShotLevel; }
    u32 getInkTankLevel() const { return mUpgrades.inkTankLevel; }
    u32 getPowerEggs() const { return mUpgrades.powerEggsBank; }

    f32 getHeroShotFireRate() const;
    f32 getInkTankMultiplier() const;

protected:
    HeroGearUpgrades mUpgrades;
    s32 mStateTimer;

    undefined mReserved[0x38];
};

} // namespace Game
