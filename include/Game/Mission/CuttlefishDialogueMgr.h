#pragma once

#include "types.h"
#include <string>
#include <vector>

namespace Game {

struct CuttlefishDialogueEntry {
    u32 id;
    u32 areaIdx;
    u32 clearNum;
    std::string messageLabel;
    bool isFreeTalk;
    bool isPermanent;
    s32 imageId;
};

/**
 * CuttlefishDialogueMgr
 * Authentic Cap'n Cuttlefish (Atari) single-player campaign dialogue progression system
 * across Octo Valley Areas 1 through 5, loaded from content/Static/WorldTalkTextInfo.byaml.
 */
class CuttlefishDialogueMgr {
public:
    CuttlefishDialogueMgr();
    ~CuttlefishDialogueMgr();

    bool loadFromByml(const char* bymlPath);

    size_t getTotalDialogueCount() const { return mDialogues.size(); }
    const CuttlefishDialogueEntry* getDialogue(u32 areaIdx, u32 clearNum) const;
    std::vector<const CuttlefishDialogueEntry*> getDialoguesForArea(u32 areaIdx) const;

    u32 getMaxArea() const { return mMaxArea; }

private:
    std::vector<CuttlefishDialogueEntry> mDialogues;
    u32 mMaxArea;
};

} // namespace Game
