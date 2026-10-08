#include "Game/Map/GambitStageTreeCatalog.h"
#include "Game/Map/BymlParser.h"
#include "sead/resource/seadResource.h"
#include <fstream>

namespace Game {

std::unique_ptr<GambitStageTreeCatalog> GambitStageTreeCatalog::sInstance = nullptr;

GambitStageTreeCatalog::GambitStageTreeCatalog() {
}

GambitStageTreeCatalog::~GambitStageTreeCatalog() {
    clear();
}

GambitStageTreeCatalog* GambitStageTreeCatalog::instance() {
    if (!sInstance) {
        sInstance = std::make_unique<GambitStageTreeCatalog>();
    }
    return sInstance.get();
}

void GambitStageTreeCatalog::clear() {
    mIsLoaded = false;
    mStages.clear();
    mNameMap.clear();
}

bool GambitStageTreeCatalog::loadFromSzs(const char* szsPath) {
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

    size_t bmutreSize = 0;
    const u8* bmutreData = arc.getFile("Gambit.bmutre", &bmutreSize);
    if (!bmutreData && arc.getFileCount() > 0) {
        const sead::SarcFileInfo* info = arc.getFileInfo(0);
        if (info) {
            bmutreData = info->data;
            bmutreSize = info->size;
        }
    }

    if (!bmutreData || bmutreSize == 0) return false;

    BymlParser parser;
    if (!parser.parse(bmutreData, bmutreSize)) {
        return false;
    }

    const BymlNode* root = parser.getRoot();
    if (!root || !root->isArray()) {
        return false;
    }

    clear();
    size_t count = root->getArraySize();
    mStages.reserve(count);

    for (size_t i = 0; i < count; ++i) {
        const BymlNode* elem = root->getElement(i);
        if (!elem || !elem->isDictionary()) continue;

        StageTreeNode node;
        node.name = elem->getString("Name");
        node.filePath = elem->getString("FilePath");
        node.location = elem->getString("Location");

        size_t idx = mStages.size();
        mStages.push_back(node);
        if (!node.name.empty()) {
            mNameMap[node.name] = idx;
        }
    }

    mIsLoaded = (mStages.size() == 375);
    return mIsLoaded;
}

const StageTreeNode* GambitStageTreeCatalog::getStage(size_t index) const {
    if (index < mStages.size()) {
        return &mStages[index];
    }
    return nullptr;
}

const StageTreeNode* GambitStageTreeCatalog::findStageByName(const std::string& name) const {
    auto it = mNameMap.find(name);
    if (it != mNameMap.end()) {
        return &mStages[it->second];
    }
    return nullptr;
}

std::vector<const StageTreeNode*> GambitStageTreeCatalog::findByLocationPrefix(const std::string& prefix) const {
    std::vector<const StageTreeNode*> results;
    for (const auto& stage : mStages) {
        if (stage.location.rfind(prefix, 0) == 0) {
            results.push_back(&stage);
        }
    }
    return results;
}

} // namespace Game
