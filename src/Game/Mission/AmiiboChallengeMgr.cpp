#include "Game/Mission/AmiiboChallengeMgr.h"
#include "Game/Map/BymlParser.h"
#include <fstream>

namespace Game {

AmiiboChallengeMgr::AmiiboChallengeMgr() {}

AmiiboChallengeMgr::~AmiiboChallengeMgr() {}

bool AmiiboChallengeMgr::loadFromByml(const char* bymlPath) {
    if (!bymlPath) return false;

    std::ifstream file(bymlPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<u8> buffer(static_cast<size_t>(fileSize));
    if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
        return false;
    }

    BymlParser parser;
    if (!parser.parse(buffer.data(), buffer.size()) || !parser.getRoot() || !parser.getRoot()->isArray()) {
        return false;
    }

    mMissions.clear();
    const auto* root = parser.getRoot();
    size_t count = root->getArraySize();

    for (size_t i = 0; i < count; ++i) {
        const auto* elem = root->getElement(i);
        if (!elem || !elem->isDictionary()) continue;

        AmiiboMissionEntry entry;
        entry.amiiboType = elem->getString("AmiiboType", "");
        entry.uiButtonId = static_cast<u32>(elem->getInt("UIButtonId", 0));
        entry.challengeMapFile = elem->getString("ChallengeMapFileName", "");
        entry.clearMapFile = elem->getString("ClearMapFileName", "");
        entry.weapon = elem->getString("Weapon", "");
        entry.inkLimit = elem->getFloat("InkLimit", -1.0f);
        entry.timeLimit = elem->getInt("TimeLimit", -1);
        entry.kingSquid = (elem->getString("KingSquid", "OFF") == "ON");
        entry.noSuit = (elem->getString("NoSuit", "OFF") == "ON");
        entry.moneyFirstClear = static_cast<u32>(elem->getInt("Money", 0));
        entry.moneyRepeatClear = static_cast<u32>(elem->getInt("MoneyAfterClear", 100));
        entry.prizeType = elem->getString("PrizeType", "Money");
        entry.prizeName = elem->getString("PrizeName", "");
        entry.prizeMiniGame = elem->getString("PrizeMiniGame", "None");

        mMissions.push_back(entry);
    }

    return !mMissions.empty();
}

const AmiiboMissionEntry* AmiiboChallengeMgr::getMission(size_t index) const {
    if (index < mMissions.size()) {
        return &mMissions[index];
    }
    return nullptr;
}

const AmiiboMissionEntry* AmiiboChallengeMgr::findMission(const char* amiiboType, u32 uiButtonId) const {
    if (!amiiboType) return nullptr;
    for (const auto& m : mMissions) {
        if (m.amiiboType == amiiboType && m.uiButtonId == uiButtonId) {
            return &m;
        }
    }
    return nullptr;
}

std::vector<const AmiiboMissionEntry*> AmiiboChallengeMgr::getMissionsByFigure(const char* amiiboType) const {
    std::vector<const AmiiboMissionEntry*> result;
    if (!amiiboType) return result;
    for (const auto& m : mMissions) {
        if (m.amiiboType == amiiboType) {
            result.push_back(&m);
        }
    }
    return result;
}

} // namespace Game
