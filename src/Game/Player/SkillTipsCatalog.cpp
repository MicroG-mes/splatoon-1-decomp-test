#include "Game/Player/SkillTipsCatalog.h"
#include "Game/Map/BymlParser.h"
#include <fstream>

namespace Game {

SkillTipsCatalog::SkillTipsCatalog() {}

SkillTipsCatalog::~SkillTipsCatalog() {}

static bool loadBymlBuffer(const char* path, std::vector<u8>& outBuffer) {
    if (!path) return false;
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    outBuffer.resize(static_cast<size_t>(fileSize));
    return file.read(reinterpret_cast<char*>(outBuffer.data()), fileSize) ? true : false;
}

bool SkillTipsCatalog::loadFromByml(const char* skillIconByml, const char* tipsByml) {
    bool okSkills = false;
    bool okTips = false;

    // 1. Skill_Icon.byaml
    std::vector<u8> bufSkills;
    if (loadBymlBuffer(skillIconByml, bufSkills)) {
        BymlParser parser;
        if (parser.parse(bufSkills.data(), bufSkills.size()) && parser.getRoot() && parser.getRoot()->isArray()) {
            mSkills.clear();
            const auto* root = parser.getRoot();
            size_t count = root->getArraySize();
            for (size_t i = 0; i < count; ++i) {
                const auto* elem = root->getElement(i);
                if (!elem || !elem->isDictionary()) continue;

                SkillIconEntry entry;
                entry.id = elem->getInt("Id", -1);
                entry.name = elem->getString("Name", "");
                entry.type = static_cast<u32>(elem->getInt("Type", 0));
                mSkills.push_back(entry);
            }
            okSkills = !mSkills.empty();
        }
    }

    // 2. TipsTextInfo.byaml
    std::vector<u8> bufTips;
    if (loadBymlBuffer(tipsByml, bufTips)) {
        BymlParser parser;
        if (parser.parse(bufTips.data(), bufTips.size()) && parser.getRoot() && parser.getRoot()->isArray()) {
            mTips.clear();
            const auto* root = parser.getRoot();
            size_t count = root->getArraySize();
            for (size_t i = 0; i < count; ++i) {
                const auto* elem = root->getElement(i);
                if (!elem || !elem->isDictionary()) continue;

                TipEntry entry;
                entry.id = static_cast<u32>(elem->getInt("Id", 0));
                entry.label = elem->getString("Label", "");
                entry.minRank = static_cast<u32>(elem->getInt("MinRank", 1));
                entry.maxRank = static_cast<u32>(elem->getInt("MaxRank", 20));
                entry.isSpecial = (elem->getString("Special", "no") == "yes");
                mTips.push_back(entry);
            }
            okTips = !mTips.empty();
        }
    }

    return okSkills && okTips;
}

const SkillIconEntry* SkillTipsCatalog::findSkillById(s32 id) const {
    for (const auto& s : mSkills) {
        if (s.id == id) return &s;
    }
    return nullptr;
}

const SkillIconEntry* SkillTipsCatalog::findSkillByName(const char* name) const {
    if (!name) return nullptr;
    for (const auto& s : mSkills) {
        if (s.name == name) return &s;
    }
    return nullptr;
}

std::vector<const TipEntry*> SkillTipsCatalog::getTipsForPlayerLevel(u32 level) const {
    std::vector<const TipEntry*> result;
    for (const auto& t : mTips) {
        if (level >= t.minRank && level <= t.maxRank) {
            result.push_back(&t);
        }
    }
    return result;
}

} // namespace Game
