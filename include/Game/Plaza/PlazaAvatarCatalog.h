#pragma once

#include "types.h"
#include <string>
#include <vector>

namespace Game {

/**
 * PlazaAvatarEntry
 * Defines a curated Inkopolis Plaza offline inhabitant or photo mannequin avatar.
 * Contains full gear loadout, skin/eye palettes, gender, weapon set, and preset group.
 */
struct PlazaAvatarEntry {
    std::string name;
    std::string sex;        // "Girl" or "Boy"
    u32 skin = 0;           // Skin tone palette (0..6)
    std::string eye;        // Eye color style
    std::string head;       // Head gear ID (e.g. HDP000, MET004, VIS001)
    std::string clothes;    // Clothes gear ID (e.g. TES001, PLO000, SWT000)
    std::string shoes;      // Shoes gear ID (e.g. SHT000, SLP000, SLO000)
    std::string weaponSet;  // Weapon set (e.g. Shot_Normal00, Roller_Heavy00)
    std::string dataName;   // Preset category (e.g. Cut23, NpcPreset, amiibo)
    u32 index = 0;          // Index within category

    bool isGirl() const { return sex == "Girl"; }
    bool isBoy() const { return sex == "Boy"; }
};

/**
 * PlazaAvatarCatalog
 * Loads and manages the 81 curated Inkopolis Plaza offline inhabitants and
 * photography mannequins from content/Static/PhotographPlayerInfo.byaml.
 */
class PlazaAvatarCatalog {
public:
    PlazaAvatarCatalog();
    ~PlazaAvatarCatalog();

    bool loadFromByml(const char* bymlPath);

    size_t getTotalAvatarCount() const { return mAvatars.size(); }
    size_t getGirlCount() const { return mGirlCount; }
    size_t getBoyCount() const { return mBoyCount; }

    const PlazaAvatarEntry* getAvatar(size_t index) const;
    const PlazaAvatarEntry* findByName(const std::string& name) const;
    std::vector<const PlazaAvatarEntry*> getAvatarsByGroup(const std::string& dataName) const;
    std::vector<const PlazaAvatarEntry*> getNpcPresets() const;
    std::vector<const PlazaAvatarEntry*> getAmiiboPresets() const;

private:
    std::vector<PlazaAvatarEntry> mAvatars;
    size_t mGirlCount = 0;
    size_t mBoyCount = 0;
};

} // namespace Game
