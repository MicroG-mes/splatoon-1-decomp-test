#include "Game/MiniGame/MiniGameCatalog.h"
#include "Game/Map/BymlParser.h"
#include "sead/resource/seadResource.h"
#include <fstream>

namespace Game {

std::unique_ptr<MiniGameCatalog> MiniGameCatalog::sInstance = nullptr;

MiniGameCatalog::MiniGameCatalog() {
}

MiniGameCatalog::~MiniGameCatalog() {
    clear();
}

MiniGameCatalog* MiniGameCatalog::instance() {
    if (!sInstance) {
        sInstance = std::make_unique<MiniGameCatalog>();
    }
    return sInstance.get();
}

void MiniGameCatalog::clear() {
    mIsLoaded = false;
    mVBallStages.clear();
    mRaceStages.clear();
    mJukeBoxTracks.clear();
}

bool MiniGameCatalog::loadFromSzs(const char* szsPath) {
    if (!szsPath) return false;

    std::ifstream file(szsPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<u8> fileBuf(static_cast<size_t>(fileSize));
    if (!file.read(reinterpret_cast<char*>(fileBuf.data()), fileSize)) {
        return false;
    }

    std::vector<u8> decompBuf;
    const u8* sarcData = fileBuf.data();
    size_t sarcSize = fileBuf.size();

    if (fileSize >= 4 && (fileBuf[0] == 'Y' && fileBuf[1] == 'a' && fileBuf[2] == 'z' && fileBuf[3] == '0')) {
        if (!sead::Yaz0::decompress(fileBuf.data(), fileBuf.size(), decompBuf)) {
            return false;
        }
        sarcData = decompBuf.data();
        sarcSize = decompBuf.size();
    }

    sead::SarcArchive arc;
    if (!arc.load(sarcData, sarcSize)) {
        return false;
    }

    clear();

    // 1. Parse VBallStage.byml (Squid Ball)
    size_t vballSize = 0;
    const u8* vballData = arc.getFile("VBallStage.byml", &vballSize);
    if (vballData && vballSize > 0) {
        BymlParser parser;
        if (parser.parse(vballData, vballSize) && parser.getRoot() && parser.getRoot()->isArray()) {
            size_t count = parser.getRoot()->getArraySize();
            mVBallStages.reserve(count);
            for (size_t i = 0; i < count; ++i) {
                const BymlNode* elem = parser.getRoot()->getElement(i);
                if (!elem || !elem->isDictionary()) continue;

                VBallStageInfo info;
                info.stageIndex = static_cast<u32>(i);
                info.goalPoint = static_cast<u32>(elem->getInt("GoalPoint"));
                const BymlNode* enemies = elem->getChild("Enemys");
                if (enemies && enemies->isArray()) {
                    info.enemyCount = enemies->getArraySize();
                }
                mVBallStages.push_back(info);
            }
        }
    }

    // 2. Parse RaceStage.byml (Squid Racer)
    size_t raceSize = 0;
    const u8* raceData = arc.getFile("RaceStage.byml", &raceSize);
    if (raceData && raceSize > 0) {
        BymlParser parser;
        if (parser.parse(raceData, raceSize) && parser.getRoot() && parser.getRoot()->isArray()) {
            size_t count = parser.getRoot()->getArraySize();
            mRaceStages.reserve(count);
            for (size_t i = 0; i < count; ++i) {
                const BymlNode* elem = parser.getRoot()->getElement(i);
                if (!elem || !elem->isDictionary()) continue;

                RaceStageInfo info;
                info.stageIndex = static_cast<u32>(i);
                info.timeLimit = static_cast<u32>(elem->getInt("Time"));
                info.strongRival = elem->getString("StrongRival");
                const BymlNode* walls = elem->getChild("Walls");
                if (walls && walls->isArray()) {
                    info.wallCount = walls->getArraySize();
                }
                mRaceStages.push_back(info);
            }
        }
    }

    // 3. Parse JukeBoxNoteDataEasy.byml, Normal, Extreme (Squid Beatz)
    size_t jbEasySize = 0, jbNormSize = 0, jbExtSize = 0;
    const u8* jbEasyData = arc.getFile("JukeBoxNoteDataEasy.byml", &jbEasySize);
    const u8* jbNormData = arc.getFile("JukeBoxNoteDataNormal.byml", &jbNormSize);
    const u8* jbExtData = arc.getFile("JukeBoxNoteDataExtreme.byml", &jbExtSize);

    BymlParser easyParser, normParser, extParser;
    bool hasEasy = jbEasyData && easyParser.parse(jbEasyData, jbEasySize);
    bool hasNorm = jbNormData && normParser.parse(jbNormData, jbNormSize);
    bool hasExt = jbExtData && extParser.parse(jbExtData, jbExtSize);

    if (hasEasy && easyParser.getRoot() && easyParser.getRoot()->isArray()) {
        size_t count = easyParser.getRoot()->getArraySize();
        mJukeBoxTracks.reserve(count);
        for (size_t i = 0; i < count; ++i) {
            JukeBoxTrackInfo track;
            track.trackIndex = static_cast<u32>(i);

            const BymlNode* easyElem = easyParser.getRoot()->getElement(i);
            if (easyElem && easyElem->isDictionary()) {
                track.numNotesEasy = static_cast<u32>(easyElem->getInt("NumNote"));
            }

            if (hasNorm && normParser.getRoot() && normParser.getRoot()->isArray()) {
                const BymlNode* normElem = normParser.getRoot()->getElement(i);
                if (normElem && normElem->isDictionary()) {
                    track.numNotesNormal = static_cast<u32>(normElem->getInt("NumNote"));
                }
            }

            if (hasExt && extParser.getRoot() && extParser.getRoot()->isArray()) {
                const BymlNode* extElem = extParser.getRoot()->getElement(i);
                if (extElem && extElem->isDictionary()) {
                    track.numNotesExtreme = static_cast<u32>(extElem->getInt("NumNote"));
                }
            }

            mJukeBoxTracks.push_back(track);
        }
    }

    mIsLoaded = (!mVBallStages.empty() && !mRaceStages.empty() && !mJukeBoxTracks.empty());
    return mIsLoaded;
}

const VBallStageInfo* MiniGameCatalog::getVBallStage(size_t index) const {
    if (index < mVBallStages.size()) {
        return &mVBallStages[index];
    }
    return nullptr;
}

const RaceStageInfo* MiniGameCatalog::getRaceStage(size_t index) const {
    if (index < mRaceStages.size()) {
        return &mRaceStages[index];
    }
    return nullptr;
}

const JukeBoxTrackInfo* MiniGameCatalog::getJukeBoxTrack(size_t index) const {
    if (index < mJukeBoxTracks.size()) {
        return &mJukeBoxTracks[index];
    }
    return nullptr;
}

} // namespace Game
