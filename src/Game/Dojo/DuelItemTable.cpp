#include "Game/Dojo/DuelItemTable.h"
#include "Game/Map/BymlParser.h"
#include <fstream>

namespace Game {

DuelItemTableMgr::DuelItemTableMgr() {}

DuelItemTableMgr::~DuelItemTableMgr() {}

bool DuelItemTableMgr::loadFromByml(const char* itemTableByml, const char* playerSettingByml) {
    bool okTable = false;
    bool okSetting = false;

    // 1. Load DuelItemTable.byaml
    if (itemTableByml) {
        std::ifstream file(itemTableByml, std::ios::binary | std::ios::ate);
        if (file.is_open()) {
            std::streamsize fileSize = file.tellg();
            file.seekg(0, std::ios::beg);
            std::vector<u8> buffer(static_cast<size_t>(fileSize));
            if (file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
                BymlParser parser;
                if (parser.parse(buffer.data(), buffer.size()) && parser.getRoot() && parser.getRoot()->isArray()) {
                    mProbEntries.clear();
                    const auto* root = parser.getRoot();
                    size_t count = root->getArraySize();
                    for (size_t i = 0; i < count; ++i) {
                        const auto* elem = root->getElement(i);
                        if (!elem || !elem->isDictionary()) continue;

                        DuelItemProbEntry entry;
                        entry.minPointDiff = elem->getInt("MinPointDiff", 0);
                        entry.maxPointDiff = elem->getInt("MaxPointDiff", 0);
                        entry.barrier = static_cast<u32>(elem->getInt("Barrier", 0));
                        entry.devil = static_cast<u32>(elem->getInt("Devil", 0));
                        entry.marking = static_cast<u32>(elem->getInt("Marking", 0));
                        entry.powerUp = static_cast<u32>(elem->getInt("PowerUp", 0));
                        entry.sprinkler = static_cast<u32>(elem->getInt("Sprinkler", 0));
                        entry.superJump = static_cast<u32>(elem->getInt("SuperJump", 0));
                        entry.superShot = static_cast<u32>(elem->getInt("SuperShot", 0));
                        entry.tornade = static_cast<u32>(elem->getInt("Tornade", 0));
                        mProbEntries.push_back(entry);
                    }
                    okTable = !mProbEntries.empty();
                }
            }
        }
    }

    // 2. Load DuelPlayerSetting.byaml
    if (playerSettingByml) {
        std::ifstream file(playerSettingByml, std::ios::binary | std::ios::ate);
        if (file.is_open()) {
            std::streamsize fileSize = file.tellg();
            file.seekg(0, std::ios::beg);
            std::vector<u8> buffer(static_cast<size_t>(fileSize));
            if (file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
                BymlParser parser;
                if (parser.parse(buffer.data(), buffer.size()) && parser.getRoot() && parser.getRoot()->isArray()) {
                    mPresetLoadouts.clear();
                    const auto* root = parser.getRoot();
                    size_t count = root->getArraySize();
                    for (size_t i = 0; i < count; ++i) {
                        const auto* elem = root->getElement(i);
                        if (!elem || !elem->isDictionary()) continue;

                        DuelPresetLoadout preset;
                        preset.id = static_cast<u32>(elem->getInt("Id", 0));
                        preset.weaponSet = elem->getString("WeaponSet", "");
                        preset.head = elem->getString("Head", "");
                        preset.clothes = elem->getString("Clothes", "");
                        preset.shoes = elem->getString("Shoes", "");
                        mPresetLoadouts.push_back(preset);
                    }
                    okSetting = !mPresetLoadouts.empty();
                }
            }
        }
    }

    return okTable && okSetting;
}

const DuelItemProbEntry* DuelItemTableMgr::getEntryForPointDiff(s32 pointDiff) const {
    for (const auto& entry : mProbEntries) {
        if (pointDiff >= entry.minPointDiff && pointDiff <= entry.maxPointDiff) {
            return &entry;
        }
    }
    // Fallback: clamp to edges
    if (!mProbEntries.empty()) {
        if (pointDiff > mProbEntries.front().maxPointDiff) {
            return &mProbEntries.front();
        }
        return &mProbEntries.back();
    }
    return nullptr;
}

DuelDropItemType DuelItemTableMgr::rollDropItem(s32 pointDiff, u32 roll100) const {
    const auto* entry = getEntryForPointDiff(pointDiff);
    if (!entry) return DuelDropItemType::cNone;

    u32 roll = roll100 % 100;

    if (roll < entry->barrier) return DuelDropItemType::cBarrier;
    roll -= entry->barrier;

    if (roll < entry->devil) return DuelDropItemType::cDevil;
    roll -= entry->devil;

    if (roll < entry->marking) return DuelDropItemType::cMarking;
    roll -= entry->marking;

    if (roll < entry->powerUp) return DuelDropItemType::cPowerUp;
    roll -= entry->powerUp;

    if (roll < entry->sprinkler) return DuelDropItemType::cSprinkler;
    roll -= entry->sprinkler;

    if (roll < entry->superJump) return DuelDropItemType::cSuperJump;
    roll -= entry->superJump;

    if (roll < entry->superShot) return DuelDropItemType::cSuperShot;
    roll -= entry->superShot;

    if (roll < entry->tornade) return DuelDropItemType::cTornade;

    return DuelDropItemType::cNone;
}

const DuelPresetLoadout* DuelItemTableMgr::getPresetLoadout(u32 id) const {
    for (const auto& p : mPresetLoadouts) {
        if (p.id == id) return &p;
    }
    return nullptr;
}

} // namespace Game
