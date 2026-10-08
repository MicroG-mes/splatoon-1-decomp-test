#include "Game/Player/UdemaeGradeMgr.h"
#include "Game/Map/BymlParser.h"
#include <fstream>

namespace Game {

UdemaeGradeMgr::UdemaeGradeMgr()
    : mPlayerLevel(1)
    , mCurrentExp(0)
    , mGradeIndex(0) // Starts at C-
    , mGradePoints(0)
{
}

UdemaeGradeMgr::~UdemaeGradeMgr() {}

bool UdemaeGradeMgr::loadFromByml(const char* gradeBymlPath, const char* rankBymlPath) {
    bool okGrades = false;
    bool okRanks = false;

    // 1. Load UdemaeGrade.byaml
    if (gradeBymlPath) {
        std::ifstream file(gradeBymlPath, std::ios::binary | std::ios::ate);
        if (file.is_open()) {
            std::streamsize sz = file.tellg();
            file.seekg(0, std::ios::beg);
            std::vector<u8> buf(static_cast<size_t>(sz));
            if (file.read(reinterpret_cast<char*>(buf.data()), sz)) {
                BymlParser parser;
                if (parser.parse(buf.data(), buf.size()) && parser.getRoot() && parser.getRoot()->isArray()) {
                    mGrades.clear();
                    const auto* rootArr = parser.getRoot();
                    for (size_t i = 0; i < rootArr->getArraySize(); ++i) {
                        const auto* item = rootArr->getElement(i);
                        if (!item || !item->isDictionary()) continue;

                        UdemaeGradeEntry entry;
                        entry.name = item->getString("Name", "C-");
                        entry.basePoint = item->getInt("BasePoint", 0);
                        entry.winBonusCash = static_cast<u32>(item->getInt("WinBonus", 1000));
                        entry.knockOutBonusCash = static_cast<u32>(item->getInt("KnockOutBonus", 1300));
                        mGrades.push_back(entry);
                    }
                    okGrades = !mGrades.empty();
                }
            }
        }
    }

    // 2. Load PlayerRank.byaml
    if (rankBymlPath) {
        std::ifstream file(rankBymlPath, std::ios::binary | std::ios::ate);
        if (file.is_open()) {
            std::streamsize sz = file.tellg();
            file.seekg(0, std::ios::beg);
            std::vector<u8> buf(static_cast<size_t>(sz));
            if (file.read(reinterpret_cast<char*>(buf.data()), sz)) {
                BymlParser parser;
                if (parser.parse(buf.data(), buf.size()) && parser.getRoot() && parser.getRoot()->isArray()) {
                    mRanks.clear();
                    const auto* rootArr = parser.getRoot();
                    for (size_t i = 0; i < rootArr->getArraySize(); ++i) {
                        const auto* item = rootArr->getElement(i);
                        if (!item || !item->isDictionary()) continue;

                        PlayerRankThreshold r;
                        r.rank = static_cast<u32>(i + 1);
                        r.nextRankExp = static_cast<u32>(item->getInt("NextRankExp", 1000));
                        mRanks.push_back(r);
                    }
                    okRanks = !mRanks.empty();
                }
            }
        }
    }

    return okGrades && okRanks;
}

u32 UdemaeGradeMgr::getNextLevelExp() const {
    if (mPlayerLevel > 0 && mPlayerLevel <= mRanks.size()) {
        return mRanks[mPlayerLevel - 1].nextRankExp;
    }
    return 24000; // Cap default
}

bool UdemaeGradeMgr::addExp(u32 expGained) {
    mCurrentExp += expGained;
    u32 needed = getNextLevelExp();
    bool leveledUp = false;

    while (mCurrentExp >= needed && mPlayerLevel < 50) {
        mCurrentExp -= needed;
        mPlayerLevel++;
        leveledUp = true;
        needed = getNextLevelExp();
    }
    return leveledUp;
}

const std::string& UdemaeGradeMgr::getGradeName() const {
    static const std::string sFallback = "C-";
    if (mGradeIndex >= 0 && static_cast<size_t>(mGradeIndex) < mGrades.size()) {
        return mGrades[mGradeIndex].name;
    }
    return sFallback;
}

void UdemaeGradeMgr::applyMatchOutcome(bool won, s32 pointsDelta, bool isKnockout, u32& outCashAwarded) {
    outCashAwarded = 0;
    if (mGradeIndex < 0 || static_cast<size_t>(mGradeIndex) >= mGrades.size()) {
        return;
    }

    const auto& currentGrade = mGrades[mGradeIndex];

    if (won) {
        outCashAwarded = isKnockout ? currentGrade.knockOutBonusCash : currentGrade.winBonusCash;
        mGradePoints += pointsDelta;
        if (mGradePoints >= 100) {
            // Promotion to next rank!
            if (static_cast<size_t>(mGradeIndex + 1) < mGrades.size()) {
                mGradeIndex++;
                mGradePoints = 30; // Retail Splatoon 1 promotion starting points buffer
            } else {
                mGradePoints = 99; // Cap at S+ 99
            }
        }
    } else {
        outCashAwarded = 0;
        mGradePoints -= pointsDelta;
        if (mGradePoints < 0) {
            // Demotion to previous rank!
            if (mGradeIndex > 0) {
                mGradeIndex--;
                mGradePoints = 70; // Retail Splatoon 1 demotion buffer
            } else {
                mGradePoints = 0;  // Floor at C- 0
            }
        }
    }
}

} // namespace Game
