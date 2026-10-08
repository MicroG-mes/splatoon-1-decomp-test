#include "Game/Plaza/PlazaNpcPresetCatalog.h"
#include "sead/resource/seadResource.h"
#include <fstream>
#include <algorithm>

namespace Game {

std::unique_ptr<PlazaNpcPresetCatalog> PlazaNpcPresetCatalog::sInstance = nullptr;

static inline u16 readBE16(const u8* ptr) {
    return (static_cast<u16>(ptr[0]) << 8) | static_cast<u16>(ptr[1]);
}

static inline u32 readBE32(const u8* ptr) {
    return (static_cast<u32>(ptr[0]) << 24) |
           (static_cast<u32>(ptr[1]) << 16) |
           (static_cast<u32>(ptr[2]) << 8) |
           static_cast<u32>(ptr[3]);
}

PlazaNpcPresetCatalog::PlazaNpcPresetCatalog() {
}

PlazaNpcPresetCatalog::~PlazaNpcPresetCatalog() {
    clear();
}

PlazaNpcPresetCatalog* PlazaNpcPresetCatalog::instance() {
    if (!sInstance) {
        sInstance = std::make_unique<PlazaNpcPresetCatalog>();
    }
    return sInstance.get();
}

void PlazaNpcPresetCatalog::clear() {
    mIsLoaded = false;
    mPresets.clear();
    mNameMap.clear();
}

bool PlazaNpcPresetCatalog::loadFromSzs(const char* szsPath) {
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
    // Gather all PlazaNpcPreset*.bin files
    std::vector<std::pair<std::string, const u8*>> presetFiles;
    for (size_t i = 0; i < arc.getFileCount(); ++i) {
        const auto* info = arc.getFileInfo(i);
        if (info && info->size >= 236 && info->name.find("PlazaNpcPreset") != std::string::npos) {
            presetFiles.push_back({info->name, info->data});
        }
    }

    // Sort by name so PlazaNpcPreset00 .. PlazaNpcPreset29 are in order
    std::sort(presetFiles.begin(), presetFiles.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });

    mPresets.reserve(presetFiles.size());
    for (size_t i = 0; i < presetFiles.size(); ++i) {
        const u8* data = presetFiles[i].second;

        PlazaNpcPresetEntry entry;
        entry.index = static_cast<u32>(i);

        // Parse UTF-16 BE name from offset 6
        std::string charName;
        for (size_t c = 6; c + 2 <= 36; c += 2) {
            u16 ch = readBE16(data + c);
            if (ch == 0) break;
            if (ch < 128) {
                charName.push_back(static_cast<char>(ch));
            }
        }
        entry.name = charName;

        entry.gender = readBE32(data + 14 * 4);
        entry.rank = readBE32(data + 15 * 4);
        entry.weaponId = readBE32(data + 16 * 4);
        entry.headGearId = readBE32(data + 17 * 4);
        entry.clothesGearId = readBE32(data + 18 * 4);
        entry.shoesGearId = readBE32(data + 19 * 4);

        size_t idx = mPresets.size();
        mPresets.push_back(entry);
        if (!entry.name.empty()) {
            mNameMap[entry.name] = idx;
        }
    }

    mIsLoaded = (mPresets.size() == 30);
    return mIsLoaded;
}

const PlazaNpcPresetEntry* PlazaNpcPresetCatalog::getPreset(size_t index) const {
    if (index < mPresets.size()) {
        return &mPresets[index];
    }
    return nullptr;
}

const PlazaNpcPresetEntry* PlazaNpcPresetCatalog::getPresetByName(const std::string& name) const {
    auto it = mNameMap.find(name);
    if (it != mNameMap.end()) {
        return &mPresets[it->second];
    }
    return nullptr;
}

} // namespace Game
