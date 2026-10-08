#include "Game/Shop/Npc_WeaponsShop.h"

namespace Game {

Npc_WeaponsShop::Npc_WeaponsShop()
    : mPosition(0.0f, 0.0f, 0.0f),
      mIsTestFiring(false),
      mTestWeaponId(0),
      mDialogueTimer(0) {
    setupCatalog();
}

Npc_WeaponsShop::~Npc_WeaponsShop() = default;

void Npc_WeaponsShop::init() {
    GambitActor::init();
    mPosition.set(0.0f, 0.0f, 0.0f);
    mIsTestFiring = false;
    mTestWeaponId = 0;
    mDialogueTimer = 0;
    setupCatalog();
}

void Npc_WeaponsShop::update() {
    GambitActor::update();
    if (mDialogueTimer > 0) mDialogueTimer--;
}

void Npc_WeaponsShop::draw() {
    GambitActor::draw();
}

void Npc_WeaponsShop::setupCatalog() {
    mCatalog.clear();
    mCatalog.push_back({ 0, "Splattershot Jr.", 1, 0, true });
    mCatalog.push_back({ 1, "Splattershot", 2, 500, false });
    mCatalog.push_back({ 2, "Splat Roller", 3, 1000, false });
    mCatalog.push_back({ 3, "Splat Charger", 3, 1000, false });
    mCatalog.push_back({ 4, "Aerospray MG", 13, 4500, false });
    mCatalog.push_back({ 5, "Dynamo Roller", 20, 7900, false });
    mCatalog.push_back({ 6, "E-liter 3K", 18, 12500, false });
}

const WeaponListing* Npc_WeaponsShop::findWeapon(u32 weaponId) const {
    for (const auto& w : mCatalog) {
        if (w.weaponId == weaponId) return &w;
    }
    return nullptr;
}

/**
 * Npc_WeaponsShop__vfunc_63 @ 0x02838080
 * Purchase weapon transaction: checks coins, deducts price, marks purchased.
 */
bool Npc_WeaponsShop::vfunc_63(u32 weaponId, u32* pPlayerCoins) {
    if (!pPlayerCoins) return false;

    for (auto& w : mCatalog) {
        if (w.weaponId == weaponId) {
            if (w.isPurchased) return true; // Already owned
            if (*pPlayerCoins >= w.priceCoins) {
                *pPlayerCoins -= w.priceCoins;
                w.isPurchased = true;
                return true;
            }
            return false; // Not enough coins
        }
    }
    return false;
}

/**
 * Npc_WeaponsShop__vfunc_64 @ 0x02838140
 * Checks if player level meets or exceeds weapon requirement.
 */
bool Npc_WeaponsShop::vfunc_64(u32 weaponId, u32 playerLevel) {
    const auto* w = findWeapon(weaponId);
    if (!w) return false;
    return playerLevel >= w->requiredLevel;
}

/**
 * Npc_WeaponsShop__vfunc_68 @ 0x028381B8
 * Enters Test Fire mode for weaponId.
 */
bool Npc_WeaponsShop::vfunc_68(u32 weaponId) {
    const auto* w = findWeapon(weaponId);
    if (!w) return false;

    mIsTestFiring = true;
    mTestWeaponId = weaponId;
    return true;
}

/**
 * Npc_WeaponsShop__vfunc_72 @ 0x02838178
 * Sheldon dialogue monologue describing weapon strengths.
 */
const char* Npc_WeaponsShop::vfunc_72(u32 weaponId) {
    mDialogueTimer = 60;
    switch (weaponId) {
    case 0: return "The Splattershot Jr.! High ink efficiency and a wide spread, perfect for beginners!";
    case 1: return "The standard Splattershot! A balanced weapon that excels at both turf inking and splatting!";
    case 2: return "The Splat Roller! Coat turf as you run, or fling ink for mid-range damage!";
    case 3: return "The Splat Charger! Hold down ZR to charge up a high-pressure ink laser!";
    case 4: return "The Aerospray MG! Incredible fire rate for supreme Turf War painting speed!";
    case 5: return "The Dynamo Roller! Heavy swing, but flings devastatingly massive waves of ink!";
    case 6: return "The E-liter 3K! The longest range in the game, devastating from high perches!";
    default: return "Come back anytime to test out our latest weapons at Ammo Knights!";
    }
}

bool Npc_WeaponsShop::purchaseWeapon(u32 weaponId, u32 playerLevel, u32& playerCoins) {
    if (!vfunc_64(weaponId, playerLevel)) {
        return false; // Level locked
    }
    return vfunc_63(weaponId, &playerCoins);
}

bool Npc_WeaponsShop::canTestFire(u32 weaponId) const {
    return findWeapon(weaponId) != nullptr;
}

} // namespace Game
