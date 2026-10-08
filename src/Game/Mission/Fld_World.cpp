#include "Game/Mission/Fld_World.h"

namespace Game {

Fld_World::Fld_World()
    : mCurrentSector(0),
      mStateTimer(0) {
    for (int i = 0; i < 28; ++i) {
        mKettles[i].missionId = i + 1;
        mKettles[i].pos = sead::Vector3f(0.0f, 0.0f, 0.0f);
        mKettles[i].isDiscovered = false;
        mKettles[i].isCleared = false;
        mKettles[i].hasSunkenScroll = false;
    }
}

Fld_World::~Fld_World() {
}

void Fld_World::init() {
    GambitActor::init();
    mCurrentSector = 0;
    // Kettle 1 in Sector 1 is pre-discovered
    mKettles[0].isDiscovered = true;
}

void Fld_World::selectSector(u32 sectorIndex) {
    if (sectorIndex < 5) {
        mCurrentSector = sectorIndex;
    }
}

void Fld_World::discoverKettle(u32 missionId) {
    if (missionId >= 1 && missionId <= 28) {
        mKettles[missionId - 1].isDiscovered = true;
    }
}

void Fld_World::clearKettle(u32 missionId, bool foundScroll) {
    if (missionId >= 1 && missionId <= 28) {
        mKettles[missionId - 1].isCleared = true;
        if (foundScroll) {
            mKettles[missionId - 1].hasSunkenScroll = true;
        }
    }
}

bool Fld_World::isSectorBossUnlocked(u32 sectorIndex) const {
    // Sector 1: Missions 1-3
    // Sector 2: Missions 4-9
    // Sector 3: Missions 10-15
    // Sector 4: Missions 16-21
    // Sector 5: Missions 22-27
    u32 start = 0, count = 0;
    switch (sectorIndex) {
        case 0: start = 0; count = 3; break;
        case 1: start = 3; count = 6; break;
        case 2: start = 9; count = 6; break;
        case 3: start = 15; count = 6; break;
        case 4: start = 21; count = 6; break;
        default: return false;
    }

    for (u32 i = start; i < start + count; ++i) {
        if (!mKettles[i].isCleared) {
            return false;
        }
    }
    return true;
}

u32 Fld_World::getTotalZapfishRescued() const {
    u32 count = 0;
    for (int i = 0; i < 28; ++i) {
        if (mKettles[i].isCleared) count++;
    }
    return count;
}

KettleInfo* Fld_World::getKettle(u32 missionId) {
    if (missionId >= 1 && missionId <= 28) {
        return &mKettles[missionId - 1];
    }
    return nullptr;
}

void Fld_World::update() {
    mStateTimer++;
}

void Fld_World::draw() {
    GambitActor::draw();
}

} // namespace Game
