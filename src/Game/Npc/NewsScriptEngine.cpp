#include "Game/Npc/NewsScriptEngine.h"
#include "Game/Map/BymlParser.h"
#include "sead/resource/seadResource.h"
#include <fstream>

namespace Game {

std::unique_ptr<NewsScriptEngine> NewsScriptEngine::sInstance = nullptr;

NewsScriptEngine::NewsScriptEngine() {
}

NewsScriptEngine::~NewsScriptEngine() {
    clear();
}

NewsScriptEngine* NewsScriptEngine::instance() {
    if (!sInstance) {
        sInstance = std::make_unique<NewsScriptEngine>();
    }
    return sInstance.get();
}

void NewsScriptEngine::clear() {
    mIsLoaded = false;
    mScenarios.clear();
    mScenarioMap.clear();
}

bool NewsScriptEngine::load(const u8* bymlData, size_t bymlSize) {
    if (!bymlData || bymlSize == 0) return false;

    BymlParser parser;
    if (!parser.parse(bymlData, bymlSize)) {
        return false;
    }

    const BymlNode* root = parser.getRoot();
    if (!root || !root->isDictionary()) {
        return false;
    }

    const BymlNode* newsArray = root->getChild("News");
    if (!newsArray || !newsArray->isArray()) {
        return false;
    }

    clear();
    size_t count = newsArray->getArraySize();
    mScenarios.reserve(count);

    for (size_t i = 0; i < count; ++i) {
        const BymlNode* scnNode = newsArray->getElement(i);
        if (!scnNode || !scnNode->isDictionary()) continue;

        NewsScenario scn;
        scn.newsType = scnNode->getString("NewsType");

        const BymlNode* cmdArray = scnNode->getChild("Commands");
        if (cmdArray && cmdArray->isArray()) {
            for (size_t c = 0; c < cmdArray->getArraySize(); ++c) {
                const BymlNode* cmdNode = cmdArray->getElement(c);
                if (cmdNode) {
                    NewsCommandEntry cmd;
                    if (cmdNode->isDictionary()) {
                        cmd.name = cmdNode->getString("Command");
                        cmd.subState = cmdNode->getString("SubState");
                    } else if (cmdNode->isString()) {
                        cmd.name = cmdNode->asString();
                    }
                    scn.commands.push_back(cmd);
                }
            }
        }

        size_t idx = mScenarios.size();
        mScenarios.push_back(scn);
        if (!scn.newsType.empty()) {
            mScenarioMap[scn.newsType] = idx;
        }
    }

    mIsLoaded = !mScenarios.empty();
    return mIsLoaded;
}

bool NewsScriptEngine::loadFromSzs(const char* szsPath) {
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

    size_t bymlSize = 0;
    const u8* bymlData = arc.getFile("NewsScript.byml", &bymlSize);
    if (!bymlData && arc.getFileCount() > 0) {
        const sead::SarcFileInfo* info = arc.getFileInfo(0);
        if (info) {
            bymlData = info->data;
            bymlSize = info->size;
        }
    }

    if (!bymlData || bymlSize == 0) return false;
    return load(bymlData, bymlSize);
}

const NewsScenario* NewsScriptEngine::getScenario(size_t index) const {
    if (index < mScenarios.size()) {
        return &mScenarios[index];
    }
    return nullptr;
}

const NewsScenario* NewsScriptEngine::findScenario(const std::string& newsType) const {
    auto it = mScenarioMap.find(newsType);
    if (it != mScenarioMap.end()) {
        return &mScenarios[it->second];
    }
    return nullptr;
}

bool NewsScriptEngine::hasScenario(const std::string& newsType) const {
    return mScenarioMap.find(newsType) != mScenarioMap.end();
}

} // namespace Game
