#pragma once

#include "types.h"
#include <string>
#include <vector>

namespace Game {

struct SkillIconEntry {
    s32 id;
    std::string name;
    u32 type;
};

struct TipEntry {
    u32 id;
    std::string label;
    u32 minRank;
    u32 maxRank;
    bool isSpecial;
};

/**
 * SkillTipsCatalog
 * Manages authentic 26-skill gear ability icons and 83 level-gated tips
 * loaded directly from Skill_Icon.byaml and TipsTextInfo.byaml.
 */
class SkillTipsCatalog {
public:
    SkillTipsCatalog();
    ~SkillTipsCatalog();

    bool loadFromByml(const char* skillIconByml, const char* tipsByml);

    size_t getSkillCount() const { return mSkills.size(); }
    size_t getTipCount() const { return mTips.size(); }

    const SkillIconEntry* findSkillById(s32 id) const;
    const SkillIconEntry* findSkillByName(const char* name) const;

    std::vector<const TipEntry*> getTipsForPlayerLevel(u32 level) const;

private:
    std::vector<SkillIconEntry> mSkills;
    std::vector<TipEntry> mTips;
};

} // namespace Game
