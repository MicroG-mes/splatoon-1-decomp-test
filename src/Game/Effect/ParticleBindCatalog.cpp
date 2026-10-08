#include "Game/Effect/ParticleBindCatalog.h"
#include "Game/Map/BymlParser.h"
#include <fstream>

namespace Game {

ParticleBindCatalog::ParticleBindCatalog() {}
ParticleBindCatalog::~ParticleBindCatalog() {}

bool ParticleBindCatalog::loadFromByml(const char* bymlPath) {
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

    mEntries.clear();
    mUniqueParents.clear();
    const auto* root = parser.getRoot();
    size_t count = root->getArraySize();

    for (size_t i = 0; i < count; ++i) {
        const auto* elem = root->getElement(i);
        if (!elem || !elem->isDictionary()) continue;

        ParticleBindEntry entry;
        entry.modelIndex = static_cast<u32>(elem->getInt("ModelIndex", static_cast<s32>(i)));
        entry.modelName = elem->getString("ModelName", "");
        entry.parentModelName = elem->getString("ParentModelName", "");
        entry.pattern = static_cast<u32>(elem->getInt("Pattern", 0));
        entry.se = elem->getString("SE", "");

        if (!entry.parentModelName.empty()) {
            mUniqueParents.insert(entry.parentModelName);
        }

        mEntries.push_back(entry);
    }

    return !mEntries.empty();
}

const ParticleBindEntry* ParticleBindCatalog::getEntryByIndex(u32 modelIndex) const {
    for (const auto& entry : mEntries) {
        if (entry.modelIndex == modelIndex) {
            return &entry;
        }
    }
    return nullptr;
}

std::vector<const ParticleBindEntry*> ParticleBindCatalog::getEntriesByParent(const std::string& parentModelName) const {
    std::vector<const ParticleBindEntry*> result;
    for (const auto& entry : mEntries) {
        if (entry.parentModelName == parentModelName) {
            result.push_back(&entry);
        }
    }
    return result;
}

std::vector<const ParticleBindEntry*> ParticleBindCatalog::getEntriesByDebrisModel(const std::string& debrisModelName) const {
    std::vector<const ParticleBindEntry*> result;
    for (const auto& entry : mEntries) {
        if (entry.modelName == debrisModelName) {
            result.push_back(&entry);
        }
    }
    return result;
}

std::vector<const ParticleBindEntry*> ParticleBindCatalog::getEntriesWithSoundEffect() const {
    std::vector<const ParticleBindEntry*> result;
    for (const auto& entry : mEntries) {
        if (entry.hasAudio()) {
            result.push_back(&entry);
        }
    }
    return result;
}

} // namespace Game
