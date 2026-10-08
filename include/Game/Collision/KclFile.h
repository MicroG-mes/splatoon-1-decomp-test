#pragma once

#include "types.h"
#include "sead/math/seadVector.h"
#include "sead/resource/BfresParser.h"
#include <vector>

namespace Game {

#pragma pack(push, 1)
struct KclHeaderRaw {
    u32 posTableOffset;
    u32 nrmTableOffset;
    u32 prismTableOffset;
    u32 octreeTableOffset;
    f32 thickness;
    sead::Vector3f minCoord;
    u32 xMask;
    u32 yMask;
    u32 zMask;
    u32 xShift;
    u32 yShift;
    u32 zShift;
};

struct KclPrismRaw {
    f32 length;
    u16 posIndex;
    u16 dirIndex;
    u16 nrmAIndex;
    u16 nrmBIndex;
    u16 nrmCIndex;
    u16 attribute;
    u32 flags;
};
#pragma pack(pop)

// Authentic Splatoon Collision Material Bitflags
enum KclAttributeFlags : u16 {
    cKclAttr_Floor          = 0x0000,
    cKclAttr_Wall           = 0x0001,
    cKclAttr_Slope45        = 0x0002,
    cKclAttr_SteepSlope     = 0x0003,
    cKclAttr_Grate          = 0x0020, // Bit 5: Grate / iron mesh
    cKclAttr_Uninkable      = 0x0040, // Bit 6: Glass / metal / uninkable
    cKclAttr_WaterDeath     = 0x0080, // Bit 7: Water pitfall
    cKclAttr_VoidOut        = 0x0100, // Bit 8: Out of bounds fall death
    cKclAttr_Conveyor       = 0x0200, // Bit 9: Moving conveyor
    cKclAttr_Paintable      = 0x0400  // Bit 10: "gambit_paintable"
};

struct KclHitResult {
    bool hit;
    sead::Vector3f hitPoint;
    sead::Vector3f hitNormal;
    f32 distance;
    u16 attribute;

    bool isFloor() const { return (attribute & 0x0003) == cKclAttr_Floor; }
    bool isWall() const { return (attribute & 0x0003) == cKclAttr_Wall; }
    bool isGrate() const { return (attribute & cKclAttr_Grate) != 0; }
    bool isUninkable() const { return (attribute & cKclAttr_Uninkable) != 0; }
    bool isWater() const { return (attribute & cKclAttr_WaterDeath) != 0; }
    bool isVoidOut() const { return (attribute & cKclAttr_VoidOut) != 0; }
    bool isPaintable() const { return (attribute & cKclAttr_Paintable) != 0; }
};

class KclFile {
public:
    KclFile();
    ~KclFile();

    bool load(const u8* data, size_t size);
    void clear();

    // 3D Collision Queries
    bool raycast(const sead::Vector3f& origin, const sead::Vector3f& direction, f32 maxDist, KclHitResult& outHit) const;
    bool checkSphere(const sead::Vector3f& center, f32 radius, KclHitResult& outHit) const;

    // Converts KCL collision triangles into a renderable 3D BfresModel
    sead::BfresModel toBfresModel(const char* modelName = "StageCollisionMesh") const;

    // Loads KCL from a Nintendo SZS archive
    bool loadFromSzsFile(const char* szsFilePath);

    size_t getVertexCount() const { return mPositions.size(); }
    size_t getNormalCount() const { return mNormals.size(); }
    size_t getPrismCount() const { return mPrisms.size(); }

    const sead::Vector3f& getMinBounds() const { return mMinBounds; }
    const sead::Vector3f& getMaxBounds() const { return mMaxBounds; }

private:
    std::vector<sead::Vector3f> mPositions;
    std::vector<sead::Vector3f> mNormals;
    std::vector<KclPrismRaw> mPrisms;
    std::vector<u32> mOctree;

    sead::Vector3f mMinBounds;
    sead::Vector3f mMaxBounds;
    f32 mThickness;
    bool mIsBigEndian;
};

} // namespace Game
