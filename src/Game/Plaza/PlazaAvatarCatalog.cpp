#include "Game/Plaza/PlazaAvatarCatalog.h"
#include "Game/Map/BymlParser.h"
#include <fstream>

namespace Game {

PlazaAvatarCatalog::PlazaAvatarCatalog()
    : mGirlCount(0), mBoyCount(0) {}

PlazaAvatarCatalog::~PlazaAvatarCatalog() {}

bool PlazaAvatarCatalog::loadFromByml(const char* bymlPath) {
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

    mAvatars.clear();
    mGirlCount = 0;
    mBoyCount = 0;

    const auto* root = parser.getRoot();
    size_t count = root->getArraySize();

    for (size_t i = 0; i < count; ++i) {
        const auto* elem = root->getElement(i);
        if (!elem || !elem->isDictionary()) continue;

        PlazaAvatarEntry entry;
        entry.name = elem->getString("Name", "");
        entry.sex = elem->getString("Sex", "Girl");
        entry.skin = static_cast<u32>(elem->getInt("Skin", 0));
        entry.eye = elem->getString("Eye", "");
        entry.head = elem->getString("Head", "");
        entry.clothes = elem->getString("Clothes", "");
        entry.shoes = elem->getString("Shoes", "");
        entry.weaponSet = elem->getString("WeaponSet", "");
        entry.dataName = elem->getString("DataName", "");
        entry.index = static_cast<u32>(elem->getInt("Index", 0));

        if (entry.isGirl()) {
            mGirlCount++;
        } else if (entry.isBoy()) {
            mBoyCount++;
        }

        mAvatars.push_back(entry);
    }

    return !mAvatars.empty();
}

const PlazaAvatarEntry* PlazaAvatarCatalog::getAvatar(size_t index) const {
    if (index >= mAvatars.size()) return nullptr;
    return &mAvatars[index];
}

const PlazaAvatarEntry* PlazaAvatarCatalog::findByName(const std::string& name) const {
    for (const auto& a : mAvatars) {
        if (a.name == name) {
            return &a;
        }
    }
    return nullptr;
}

std::vector<const PlazaAvatarEntry*> PlazaAvatarCatalog::getAvatarsByGroup(const std::string& dataName) const {
    std::vector<const PlazaAvatarEntry*> result;
    for (const auto& a : mAvatars) {
        if (a.dataName == dataName) {
            result.push_back(&a);
        }
    }
    return result;
}

std::vector<const PlazaAvatarEntry*> PlazaAvatarCatalog::getNpcPresets() const {
    return getAvatarsByGroup("NpcPreset");
}

std::vector<const PlazaAvatarEntry*> PlazaAvatarCatalog::getAmiiboPresets() const {
    return getAvatarsByGroup("amiibo");
}

} // namespace Game
