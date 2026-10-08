#pragma once

#include "types.h"
#include <string>
#include <vector>

namespace Game {

enum class GearCategory : u32 {
    cHead    = 0,
    cClothes = 1,
    cShoes   = 2
};

struct GearCatalogItem {
    u32 id;
    std::string name;
    GearCategory category;
    std::string brand;
    u32 price;
    u32 rarityStars; // 1 to 3 stars (0, 1, 2 in BYML + 1)
    std::string mainAbility;
    std::string modelName;
    std::string howToGet;
};

/**
 * GearCatalog
 * Manages the entire 344-item authentic retail Splatoon 1 gear catalog.
 * Loads directly from GearInfo_Head.byaml, GearInfo_Clothes.byaml, and GearInfo_Shoes.byaml.
 */
class GearCatalog {
public:
    GearCatalog();
    ~GearCatalog();

    bool loadAllGear(const char* headByml, const char* clothesByml, const char* shoesByml);
    bool loadCategoryByml(const char* bymlPath, GearCategory category);

    size_t getHeadgearCount() const { return mHeadgear.size(); }
    size_t getClothesCount() const { return mClothes.size(); }
    size_t getShoesCount() const { return mShoes.size(); }
    size_t getTotalGearCount() const { return mHeadgear.size() + mClothes.size() + mShoes.size(); }

    const GearCatalogItem* getGear(GearCategory category, size_t index) const;
    const GearCatalogItem* findGearById(GearCategory category, u32 id) const;
    const GearCatalogItem* findGearByName(const char* name) const;
    std::vector<const GearCatalogItem*> getGearByBrand(const char* brand) const;

private:
    std::vector<GearCatalogItem> mHeadgear;
    std::vector<GearCatalogItem> mClothes;
    std::vector<GearCatalogItem> mShoes;
};

} // namespace Game
