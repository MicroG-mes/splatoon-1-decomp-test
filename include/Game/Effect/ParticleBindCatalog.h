#pragma once

#include "types.h"
#include <string>
#include <vector>
#include <set>

namespace Game {

/**
 * ParticleBindEntry
 * Defines a 3D broken debris model and particle emission binding for destructible
 * props, Octarian infantry, and bosses (e.g. DJ Octavio punch deflection debris).
 */
struct ParticleBindEntry {
    u32 modelIndex = 0;
    std::string modelName;       // Debris mesh (e.g. Obj_Break00, Enm_Break02)
    std::string parentModelName; // Parent model (e.g. Obj_Box00L, Enm_Stamp, Enm_BallKing)
    u32 pattern = 0;             // Emission pattern index
    std::string se;              // Trigger sound effect (e.g. BrokenPiece, BrokenPiece_RailKingPunch)

    bool hasAudio() const { return !se.empty(); }
};

/**
 * ParticleBindCatalog
 * Loads and catalogs the 64 particle bind definitions from ParticleBindModel.byaml.
 */
class ParticleBindCatalog {
public:
    ParticleBindCatalog();
    ~ParticleBindCatalog();

    bool loadFromByml(const char* bymlPath);

    size_t getEntryCount() const { return mEntries.size(); }
    const ParticleBindEntry* getEntryByIndex(u32 modelIndex) const;
    std::vector<const ParticleBindEntry*> getEntriesByParent(const std::string& parentModelName) const;
    std::vector<const ParticleBindEntry*> getEntriesByDebrisModel(const std::string& debrisModelName) const;
    std::vector<const ParticleBindEntry*> getEntriesWithSoundEffect() const;
    size_t getUniqueParentCount() const { return mUniqueParents.size(); }

private:
    std::vector<ParticleBindEntry> mEntries;
    std::set<std::string> mUniqueParents;
};

} // namespace Game
