#pragma once

#include "types.h"
#include "sead/math/seadVector.h"
#include <vector>
#include <string>

namespace Game {

// Authentic 0x178 (376 bytes) Splatoon Map Parameter block
// Recovered directly from Ghidra functions 0x0201CB54, 0x0201CC64, 0x0201CCB4
struct MapParam {
    s32 mapId;                           // 0x00: Numerical Stage ID
    u32 mapIndex;                        // 0x04: Table sequential index
    u32 category;                        // 0x08: StageCategory
    const char* mapFileName;             // 0x0C: e.g. "Fld_Warehouse00_Vss"
    const char* displayName;             // 0x10: e.g. "Walleye Warehouse"
    sead::Vector3f mapCameraTrans;       // 0x14 - 0x20: DRC Map Camera Translation (X, Y, Z)
    f32 mapCameraRotPitchDeg;            // 0x20: DRC Camera Pitch
    f32 mapCameraRotYawDeg;              // 0x24: DRC Camera Yaw
    f32 mapCameraScale;                  // 0x28: DRC Minimap Scale factor
    u32 mapCameraBravoInversionType;     // 0x2C: 0=None, 1=Rotate180, 2=MirrorX
    f32 boundsMinX;                      // 0x30
    f32 boundsMinZ;                      // 0x34
    f32 boundsMaxX;                      // 0x38
    f32 boundsMaxZ;                      // 0x3C
    sead::Vector3f spawnAlpha;           // 0x40 - 0x4C
    sead::Vector3f spawnBravo;           // 0x4C - 0x58
    sead::Vector3f objectiveCenter;      // 0x58 - 0x64
#if defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__)
    u8 reserved[0x178 - 0x70];           // 64-bit host pointer adjustment
#else
    u8 reserved[0x178 - 0x64];           // 32-bit target PowerPC espresso layout
#endif
};

static_assert(sizeof(MapParam) == 0x178, "MapParam size must be exactly 0x178 (376 bytes) matching Gambit.elf");

class MapTable {
public:
    MapTable();
    ~MapTable();

    void initDefaultTables();
    bool loadFromByml(const u8* data, size_t size);

    // Authentic decompiled lookup methods (from 0x0201CB54, 0x0201CC64, 0x0201CCB4)
    const MapParam* findStageById(s32 stageId) const;            // 0x0201CB54
    const MapParam* getStageByIndex(u32 index) const;            // 0x0201CC64
    const MapParam* findStageByName(const char* mapFileName) const;// 0x0201CCB4

    u32 getStageCount() const { return static_cast<u32>(mStages.size()); }
    const std::vector<MapParam>& getAllStages() const { return mStages; }

    static MapTable* getInstance();

private:
    std::vector<MapParam> mStages;
    static MapTable* sInstance;
};

} // namespace Game
