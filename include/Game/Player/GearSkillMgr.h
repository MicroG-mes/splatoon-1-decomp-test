#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"

namespace Game {

enum class GearSkillKind : u32 {
    cNone               = 0,
    cDamageUp           = 1,
    cDefenseUp          = 2,
    cInkSaverMain       = 3,
    cInkSaverSub        = 4,
    cInkRecoveryUp      = 5,
    cRunSpeedUp         = 6,
    cSwimSpeedUp        = 7,
    cSpecialChargeUp    = 8,
    cSpecialSaver       = 9,
    cSpecialDurationUp  = 10,
    cQuickRespawn       = 11,
    cQuickSuperJump     = 12,
    cBombRangeUp        = 13,
    cOpeningGambit      = 14,
    cLastDitchEffort    = 15,
    cTenacity           = 16,
    cComeback           = 17,
    cNinjaSquid         = 18,
    cHaunt              = 19,
    cRecon              = 20,
    cBombSniffer        = 21,
    cStealthJump        = 22,
    cInkResistanceUp    = 23,
    cCount              = 24
};

struct GearSlotConfig {
    GearSkillKind mainSkill;
    GearSkillKind subSkills[3];
};

class GearSkillMgr : public GambitActor {
public:
    static constexpr u32 cMainSkillAp = 10;
    static constexpr u32 cSubSkillAp  = 3;
    static constexpr u32 cMaxTotalAp  = 57;

    GearSkillMgr();
    virtual ~GearSkillMgr() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setHeadgear(const GearSlotConfig& config);
    void setClothes(const GearSlotConfig& config);
    void setShoes(const GearSlotConfig& config);
    void recalculateAbilityPoints();

    u32 getAbilityPoints(GearSkillKind skill) const;
    f32 getDamageMultiplier() const;
    f32 getDefenseMultiplier() const;
    f32 getSwimSpeedMultiplier() const;
    f32 getRunSpeedMultiplier() const;
    f32 getInkSaverMainMultiplier() const;
    f32 getSpecialChargeMultiplier() const;

    bool hasAbility(GearSkillKind skill) const { return getAbilityPoints(skill) > 0; }

protected:
    static f32 calcDiminishingCurve(u32 ap, f32 maxBonus);

    GearSlotConfig mHead;
    GearSlotConfig mClothes;
    GearSlotConfig mShoes;

    u32 mTotalAp[static_cast<u32>(GearSkillKind::cCount)];

    undefined mReserved[0x38];
};

} // namespace Game
