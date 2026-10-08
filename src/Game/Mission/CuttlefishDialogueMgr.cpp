#include "Game/Mission/CuttlefishDialogueMgr.h"
#include "Game/Map/BymlParser.h"
#include <fstream>

namespace Game {

CuttlefishDialogueMgr::CuttlefishDialogueMgr()
    : mMaxArea(0) {
}

CuttlefishDialogueMgr::~CuttlefishDialogueMgr() {}

bool CuttlefishDialogueMgr::loadFromByml(const char* bymlPath) {
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

    mDialogues.clear();
    mMaxArea = 0;
    const auto* root = parser.getRoot();
    size_t count = root->getArraySize();

    for (size_t i = 0; i < count; ++i) {
        const auto* elem = root->getElement(i);
        if (!elem || !elem->isDictionary()) continue;

        CuttlefishDialogueEntry entry;
        entry.id = static_cast<u32>(elem->getInt("Id", 0));
        entry.areaIdx = static_cast<u32>(elem->getInt("AreaIdx", 0));
        entry.clearNum = static_cast<u32>(elem->getInt("ClearNum", 0));
        entry.messageLabel = elem->getString("MessageLabel", "");
        entry.isFreeTalk = (elem->getString("IsInFreeTalk", "false") == "true");
        entry.isPermanent = (elem->getString("IsPermanent", "false") == "true");
        entry.imageId = elem->getInt("ImageId", -1);

        if (entry.areaIdx > mMaxArea) {
            mMaxArea = entry.areaIdx;
        }

        mDialogues.push_back(entry);
    }

    return !mDialogues.empty();
}

const CuttlefishDialogueEntry* CuttlefishDialogueMgr::getDialogue(u32 areaIdx, u32 clearNum) const {
    for (const auto& d : mDialogues) {
        if (d.areaIdx == areaIdx && d.clearNum == clearNum) {
            return &d;
        }
    }
    return nullptr;
}

std::vector<const CuttlefishDialogueEntry*> CuttlefishDialogueMgr::getDialoguesForArea(u32 areaIdx) const {
    std::vector<const CuttlefishDialogueEntry*> result;
    for (const auto& d : mDialogues) {
        if (d.areaIdx == areaIdx) {
            result.push_back(&d);
        }
    }
    return result;
}

} // namespace Game
