#include "Game/Player/PlayerRankMgr.h"
#include "Game/Map/BymlParser.h"
#include <fstream>

namespace Game {

PlayerRankMgr* PlayerRankMgr::sInstance = nullptr;

PlayerRankMgr::PlayerRankMgr() {
    sInstance = this;
}

PlayerRankMgr::~PlayerRankMgr() {
    if (sInstance == this) {
        sInstance = nullptr;
    }
}

PlayerRankMgr* PlayerRankMgr::instance() {
    return sInstance;
}

void PlayerRankMgr::clear() {
    mIsLoaded = false;
    mRanks.clear();
}

bool PlayerRankMgr::load(const char* byamlPath) {
    if (!byamlPath) return false;

    std::ifstream file(byamlPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<u8> buffer(static_cast<size_t>(fileSize));
    if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
        return false;
    }

    BymlParser parser;
    if (!parser.parse(buffer.data(), buffer.size())) {
        return false;
    }

    const BymlNode* root = parser.getRoot();
    if (!root || !root->isArray()) {
        return false;
    }

    clear();
    size_t count = root->getArraySize();
    mRanks.reserve(count);

    for (size_t i = 0; i < count; ++i) {
        const BymlNode* elem = root->getElement(i);
        if (!elem || !elem->isDictionary()) continue;

        PlayerRankInfo info;
        info.rank = static_cast<u32>(i + 1); // Rank 1-indexed
        info.nextRankExp = static_cast<u32>(elem->getInt("NextRankExp"));

        mRanks.push_back(info);
    }

    mIsLoaded = !mRanks.empty();
    return mIsLoaded;
}

const PlayerRankInfo* PlayerRankMgr::getRankInfo(u32 rank) const {
    if (rank == 0 || rank > mRanks.size()) return nullptr;
    return &mRanks[rank - 1];
}

u32 PlayerRankMgr::getNextRankExp(u32 rank) const {
    const PlayerRankInfo* info = getRankInfo(rank);
    return info ? info->nextRankExp : 0;
}

void PlayerRankMgr::calculateRankFromTotalExp(u32 totalExp, u32& outRank, u32& outCurrentRankExp, u32& outNeededExp) const {
    outRank = 1;
    outCurrentRankExp = totalExp;
    outNeededExp = getNextRankExp(1);

    for (size_t i = 0; i < mRanks.size(); ++i) {
        u32 threshold = mRanks[i].nextRankExp;
        if (totalExp >= threshold && (i + 1 < mRanks.size())) {
            outRank = static_cast<u32>(i + 2);
            outCurrentRankExp = totalExp - threshold;
            outNeededExp = (i + 1 < mRanks.size()) ? (mRanks[i + 1].nextRankExp - threshold) : 0;
        } else {
            break;
        }
    }
}

} // namespace Game
