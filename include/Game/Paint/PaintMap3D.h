#pragma once

#include "types.h"
#include "sead/math/seadVector.h"
#include <vector>

namespace Game {

struct PaintTexel {
    u8 team;      // 0 = Team Alpha, 1 = Team Bravo, 255 = Neutral/Empty
    u8 intensity; // 0..255 (wetness/coverage)
    u8 normalX;   // 128 = neutral normal perturb
    u8 normalY;   // 128 = neutral normal perturb

    PaintTexel() : team(255), intensity(0), normalX(128), normalY(128) {}
};

struct PaintStats {
    u32 totalPaintableTexels;
    u32 alphaTexels;
    u32 bravoTexels;
    f32 alphaPercent;
    f32 bravoPercent;

    PaintStats()
        : totalPaintableTexels(0)
        , alphaTexels(0)
        , bravoTexels(0)
        , alphaPercent(0.0f)
        , bravoPercent(0.0f)
    {}
};

class PaintMap3D {
public:
    PaintMap3D();
    ~PaintMap3D();

    void init(u32 width, u32 height, f32 worldMinX, f32 worldMinZ, f32 worldMaxX, f32 worldMaxZ);
    void reset();

    // Splats circular ink spot in world coordinates
    void splatWorldSphere(const sead::Vector3f& center, f32 radius, u8 teamId, f32 intensity = 1.0f);

    // Queries ink property at world position
    bool sampleInkAtWorldPos(const sead::Vector3f& worldPos, u8* outTeam, f32* outIntensity) const;
    bool canSwimAtWorldPos(const sead::Vector3f& worldPos, u8 teamId) const;

    // Calculates real-time Turf War ink coverage
    PaintStats calculateStats() const;

    u32 getWidth() const { return mWidth; }
    u32 getHeight() const { return mHeight; }

    // Raw RGBA buffer suitable for direct upload to DirectX 11 ID3D11Texture2D
    const u8* getRgbaBuffer() const { return mRgbaBuffer.data(); }
    size_t getRgbaBufferSize() const { return mRgbaBuffer.size(); }

private:
    void worldToTexel(f32 wx, f32 wz, s32& tx, s32& ty) const;
    void updateRgbaTexel(u32 index);

    u32 mWidth;
    u32 mHeight;
    f32 mWorldMinX;
    f32 mWorldMinZ;
    f32 mWorldMaxX;
    f32 mWorldMaxZ;

    std::vector<PaintTexel> mTexels;
    std::vector<u8> mRgbaBuffer; // 4 bytes per texel for DX11 texture upload
};

} // namespace Game
