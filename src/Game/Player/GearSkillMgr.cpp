#include "Game/Player/GearSkillMgr.h"

namespace Game {

GearSkillMgr::GearSkillMgr() {
    init();
}

GearSkillMgr::~GearSkillMgr() {
}

void GearSkillMgr::init() {
    GambitActor::init();

    mHead.mainSkill = GearSkillKind::cNone;
    mClothes.mainSkill = GearSkillKind::cNone;
    mShoes.mainSkill = GearSkillKind::cNone;

    for (u32 i = 0; i < 3; ++i) {
        mHead.subSkills[i] = GearSkillKind::cNone;
        mClothes.subSkills[i] = GearSkillKind::cNone;
        mShoes.subSkills[i] = GearSkillKind::cNone;
    }

    recalculateAbilityPoints();
}

void GearSkillMgr::setHeadgear(const GearSlotConfig& config) {
    mHead = config;
    recalculateAbilityPoints();
}

void GearSkillMgr::setClothes(const GearSlotConfig& config) {
    mClothes = config;
    recalculateAbilityPoints();
}

void GearSkillMgr::setShoes(const GearSlotConfig& config) {
    mShoes = config;
    recalculateAbilityPoints();
}

void GearSkillMgr::recalculateAbilityPoints() {
    for (u32 i = 0; i < static_cast<u32>(GearSkillKind::cCount); ++i) {
        mTotalAp[i] = 0;
    }

    const GearSlotConfig* slots[3] = { &mHead, &mClothes, &mShoes };

    for (u32 g = 0; g < 3; ++g) {
        const GearSlotConfig* item = slots[g];
        // Add 10 AP for Main Ability
        u32 mainIdx = static_cast<u32>(item->mainSkill);
        if (mainIdx < static_cast<u32>(GearSkillKind::cCount)) {
            mTotalAp[mainIdx] += cMainSkillAp;
        }

        // Add 3 AP for each Sub Ability
        for (u32 s = 0; s < 3; ++s) {
            u32 subIdx = static_cast<u32>(item->subSkills[s]);
            if (subIdx < static_cast<u32>(GearSkillKind::cCount)) {
                mTotalAp[subIdx] += cSubSkillAp;
            }
        }
    }
}

u32 GearSkillMgr::getAbilityPoints(GearSkillKind skill) const {
    u32 idx = static_cast<u32>(skill);
    if (idx < static_cast<u32>(GearSkillKind::cCount)) {
        return mTotalAp[idx];
    }
    return 0;
}

f32 GearSkillMgr::calcDiminishingCurve(u32 ap, f32 maxBonus) {
    if (ap == 0) {
        return 1.0f;
    }

    f32 fAp = static_cast<f32>(ap > cMaxTotalAp ? cMaxTotalAp : ap);
    // Splatoon 1 diminish quadratic progression: 0.033 * AP - 0.00027 * AP^2
    f32 progress = (0.033f * fAp) - (0.00027f * fAp * fAp);
    if (progress > 1.0f) {
        progress = 1.0f;
    }

    return 1.0f + (maxBonus - 1.0f) * progress;
}

f32 GearSkillMgr::getDamageMultiplier() const {
    // Damage Up caps at 1.30x (capped below 1-shot thresholds)
    return calcDiminishingCurve(getAbilityPoints(GearSkillKind::cDamageUp), 1.30f);
}

f32 GearSkillMgr::getDefenseMultiplier() const {
    // Defense Up reduces incoming damage down to 0.70x
    u32 ap = getAbilityPoints(GearSkillKind::cDefenseUp);
    if (ap == 0) return 1.0f;
    f32 fAp = static_cast<f32>(ap > cMaxTotalAp ? cMaxTotalAp : ap);
    f32 progress = (0.033f * fAp) - (0.00027f * fAp * fAp);
    return 1.0f - (0.30f * progress);
}

f32 GearSkillMgr::getSwimSpeedMultiplier() const {
    // Swim Speed Up scales up to 1.25x
    return calcDiminishingCurve(getAbilityPoints(GearSkillKind::cSwimSpeedUp), 1.25f);
}

f32 GearSkillMgr::getRunSpeedMultiplier() const {
    // Run Speed Up scales up to 1.35x
    return calcDiminishingCurve(getAbilityPoints(GearSkillKind::cRunSpeedUp), 1.35f);
}

f32 GearSkillMgr::getInkSaverMainMultiplier() const {
    // Ink Saver Main reduces ink consumption down to 0.60x (40% discount)
    u32 ap = getAbilityPoints(GearSkillKind::cInkSaverMain);
    if (ap == 0) return 1.0f;
    f32 fAp = static_cast<f32>(ap > cMaxTotalAp ? cMaxTotalAp : ap);
    f32 progress = (0.033f * fAp) - (0.00027f * fAp * fAp);
    return 1.0f - (0.40f * progress);
}

f32 GearSkillMgr::getSpecialChargeMultiplier() const {
    // Special Charge Up increases gauge gain up to 1.30x
    return calcDiminishingCurve(getAbilityPoints(GearSkillKind::cSpecialChargeUp), 1.30f);
}

void GearSkillMgr::update() {
}

void GearSkillMgr::draw() {
}

} // namespace Game
