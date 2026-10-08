#include "Game/Mission/MissionStageMapParser.h"
#include "Game/Map/BymlParser.h"
#include "sead/resource/seadResource.h"
#include <fstream>

namespace Game {

static inline sead::Vector3f parseVector3(const BymlNode* node, sead::Vector3f defaultVal = {0.0f, 0.0f, 0.0f}) {
    if (!node) return defaultVal;
    if (node->isDictionary()) {
        f32 x = node->getFloat("X", defaultVal.x);
        f32 y = node->getFloat("Y", defaultVal.y);
        f32 z = node->getFloat("Z", defaultVal.z);
        return {x, y, z};
    } else if (node->isArray() && node->getArraySize() >= 3) {
        f32 x = node->getElement(0) ? node->getElement(0)->getFloat() : defaultVal.x;
        f32 y = node->getElement(1) ? node->getElement(1)->getFloat() : defaultVal.y;
        f32 z = node->getElement(2) ? node->getElement(2)->getFloat() : defaultVal.z;
        return {x, y, z};
    }
    return defaultVal;
}

MissionStageMapParser::MissionStageMapParser() {
}

MissionStageMapParser::~MissionStageMapParser() {
    clear();
}

void MissionStageMapParser::clear() {
    mIsLoaded = false;
    mActors.clear();
    mRails.clear();
    mActorIdMap.clear();
}

bool MissionStageMapParser::load(const u8* bymlData, size_t bymlSize) {
    if (!bymlData || bymlSize == 0) return false;

    BymlParser parser;
    if (!parser.parse(bymlData, bymlSize)) {
        return false;
    }

    const BymlNode* root = parser.getRoot();
    if (!root || !root->isDictionary()) {
        return false;
    }

    clear();

    // 1. Parse Objs array
    const BymlNode* objsNode = root->getChild("Objs");
    if (objsNode && objsNode->isArray()) {
        size_t count = objsNode->getArraySize();
        mActors.reserve(count);
        for (size_t i = 0; i < count; ++i) {
            const BymlNode* elem = objsNode->getElement(i);
            if (!elem || !elem->isDictionary()) continue;

            StageActorObj actor;
            actor.id = elem->getString("Id");
            actor.unitConfigName = elem->getString("UnitConfigName");
            actor.layerConfigName = elem->getString("LayerConfigName");
            actor.team = elem->getInt("Team", 0);
            actor.switch0 = elem->getInt("Switch0", 0);
            actor.switch1 = elem->getInt("Switch1", 0);
            actor.param0 = elem->getInt("Parameter0", -99);

            actor.translate = parseVector3(elem->getChild("Translate"), {0.0f, 0.0f, 0.0f});
            actor.rotate = parseVector3(elem->getChild("Rotate"), {0.0f, 0.0f, 0.0f});
            actor.scale = parseVector3(elem->getChild("Scale"), {1.0f, 1.0f, 1.0f});

            size_t idx = mActors.size();
            mActors.push_back(actor);
            if (!actor.id.empty()) {
                mActorIdMap[actor.id] = idx;
            }
        }
    }

    // 2. Parse Rails array
    const BymlNode* railsNode = root->getChild("Rails");
    if (railsNode && railsNode->isArray()) {
        size_t count = railsNode->getArraySize();
        mRails.reserve(count);
        for (size_t i = 0; i < count; ++i) {
            const BymlNode* elem = railsNode->getElement(i);
            if (!elem || !elem->isDictionary()) continue;

            StageRailObj rail;
            rail.id = elem->getString("Id");
            rail.railType = elem->getString("RailType");
            rail.isClosed = elem->getBool("IsClosed", false);
            rail.priority = elem->getInt("Priority", 0);

            const BymlNode* ptsNode = elem->getChild("RailPoints");
            if (ptsNode && ptsNode->isArray()) {
                size_t ptCount = ptsNode->getArraySize();
                rail.points.reserve(ptCount);
                for (size_t p = 0; p < ptCount; ++p) {
                    const BymlNode* ptElem = ptsNode->getElement(p);
                    if (!ptElem || !ptElem->isDictionary()) continue;

                    StageRailPoint pt;
                    pt.translate = parseVector3(ptElem->getChild("Translate"), {0.0f, 0.0f, 0.0f});
                    pt.control0 = parseVector3(ptElem->getChild("Control0"), {0.0f, 0.0f, 0.0f});
                    pt.control1 = parseVector3(ptElem->getChild("Control1"), {0.0f, 0.0f, 0.0f});
                    rail.points.push_back(pt);
                }
            }

            mRails.push_back(rail);
        }
    }

    mIsLoaded = (!mActors.empty());
    return mIsLoaded;
}

bool MissionStageMapParser::loadFromSzs(const char* szsPath) {
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
        return load(sarcData, sarcSize);
    }

    // Look for .byaml file in archive
    const u8* bymlData = nullptr;
    size_t bymlSize = 0;

    for (size_t i = 0; i < arc.getFileCount(); ++i) {
        const auto* info = arc.getFileInfo(i);
        if (info && info->name.find(".byaml") != std::string::npos) {
            bymlData = info->data;
            bymlSize = info->size;
            break;
        }
    }

    if (!bymlData && arc.getFileCount() > 0) {
        const auto* info = arc.getFileInfo(0);
        if (info) {
            bymlData = info->data;
            bymlSize = info->size;
        }
    }

    if (!bymlData || bymlSize == 0) return false;
    return load(bymlData, bymlSize);
}

const StageActorObj* MissionStageMapParser::getActor(size_t index) const {
    if (index < mActors.size()) {
        return &mActors[index];
    }
    return nullptr;
}

const StageActorObj* MissionStageMapParser::findActorById(const std::string& id) const {
    auto it = mActorIdMap.find(id);
    if (it != mActorIdMap.end()) {
        return &mActors[it->second];
    }
    return nullptr;
}

std::vector<const StageActorObj*> MissionStageMapParser::findActorsByConfig(const std::string& configName) const {
    std::vector<const StageActorObj*> results;
    for (const auto& a : mActors) {
        if (a.unitConfigName == configName) {
            results.push_back(&a);
        }
    }
    return results;
}

const StageRailObj* MissionStageMapParser::getRail(size_t index) const {
    if (index < mRails.size()) {
        return &mRails[index];
    }
    return nullptr;
}

bool MissionStageMapParser::hasSunkenScroll() const {
    return !findActorsByConfig("Obj_AncientDocument").empty();
}

bool MissionStageMapParser::hasGoalZapfish() const {
    return !findActorsByConfig("Obj_Goal").empty();
}

bool MissionStageMapParser::hasRespawnPoint() const {
    return !findActorsByConfig("RespawnPos").empty();
}

bool MissionStageMapParser::hasBoss() const {
    for (const auto& a : mActors) {
        if (a.unitConfigName.find("StampKing") != std::string::npos ||
            a.unitConfigName.find("RailKing") != std::string::npos ||
            a.unitConfigName.find("MouthKing") != std::string::npos ||
            a.unitConfigName.find("BallKing") != std::string::npos ||
            a.unitConfigName.find("CylinderKing") != std::string::npos) {
            return true;
        }
    }
    return false;
}

} // namespace Game
