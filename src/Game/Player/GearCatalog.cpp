#include "Game/Player/GearCatalog.h"
#include "Game/Map/BymlParser.h"
#include <fstream>

namespace Game {

GearCatalog::GearCatalog() {}

GearCatalog::~GearCatalog() {}

bool GearCatalog::loadAllGear(const char* headByml, const char* clothesByml, const char* shoesByml) {
    bool okHead = loadCategoryByml(headByml, GearCategory::cHead);
    bool okClt = loadCategoryByml(clothesByml, GearCategory::cClothes);
    bool okShs = loadCategoryByml(shoesByml, GearCategory::cShoes);
    return okHead && okClt && okShs;
}

bool GearCatalog::loadCategoryByml(const char* bymlPath, GearCategory category) {
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

    std::vector<GearCatalogItem>& targetVec =
        (category == GearCategory::cHead) ? mHeadgear :
        (category == GearCategory::cClothes) ? mClothes : mShoes;

    targetVec.clear();
    const auto* rootArray = parser.getRoot();
    size_t count = rootArray->getArraySize();

    for (size_t i = 0; i < count; ++i) {
        const auto* item = rootArray->getElement(i);
        if (!item || !item->isDictionary()) continue;

        GearCatalogItem gear;
        gear.id = static_cast<u32>(item->getInt("Id", 0));
        gear.name = item->getString("Name", "");
        gear.category = category;
        gear.brand = item->getString("Brand", "NoBrand");
        gear.price = static_cast<u32>(item->getInt("Price", 0));
        gear.rarityStars = static_cast<u32>(item->getInt("Rarity", 0)) + 1; // 0-based in file
        gear.mainAbility = item->getString("Skill0", "None");
        gear.modelName = item->getString("ModelName", "");
        gear.howToGet = item->getString("HowToGet", "cShop");

        targetVec.push_back(gear);
    }

    return !targetVec.empty();
}

const GearCatalogItem* GearCatalog::getGear(GearCategory category, size_t index) const {
    const std::vector<GearCatalogItem>& targetVec =
        (category == GearCategory::cHead) ? mHeadgear :
        (category == GearCategory::cClothes) ? mClothes : mShoes;

    if (index < targetVec.size()) {
        return &targetVec[index];
    }
    return nullptr;
}

const GearCatalogItem* GearCatalog::findGearById(GearCategory category, u32 id) const {
    const std::vector<GearCatalogItem>& targetVec =
        (category == GearCategory::cHead) ? mHeadgear :
        (category == GearCategory::cClothes) ? mClothes : mShoes;

    for (const auto& g : targetVec) {
        if (g.id == id) return &g;
    }
    return nullptr;
}

const GearCatalogItem* GearCatalog::findGearByName(const char* name) const {
    if (!name) return nullptr;
    for (const auto& g : mHeadgear) if (g.name == name || g.modelName == name) return &g;
    for (const auto& g : mClothes) if (g.name == name || g.modelName == name) return &g;
    for (const auto& g : mShoes) if (g.name == name || g.modelName == name) return &g;
    return nullptr;
}

std::vector<const GearCatalogItem*> GearCatalog::getGearByBrand(const char* brand) const {
    std::vector<const GearCatalogItem*> result;
    if (!brand) return result;
    for (const auto& g : mHeadgear) if (g.brand == brand) result.push_back(&g);
    for (const auto& g : mClothes) if (g.brand == brand) result.push_back(&g);
    for (const auto& g : mShoes) if (g.brand == brand) result.push_back(&g);
    return result;
}

} // namespace Game
