#include "Game/Paint/PaintMap3D.h"
#include <algorithm>
#include <cmath>

namespace Game {

PaintMap3D::PaintMap3D()
    : mWidth(0)
    , mHeight(0)
    , mWorldMinX(-50.0f)
    , mWorldMinZ(-50.0f)
    , mWorldMaxX(50.0f)
    , mWorldMaxZ(50.0f)
{
}

PaintMap3D::~PaintMap3D() {
}

void PaintMap3D::init(u32 width, u32 height, f32 worldMinX, f32 worldMinZ, f32 worldMaxX, f32 worldMaxZ) {
    mWidth = width;
    mHeight = height;
    mWorldMinX = worldMinX;
    mWorldMinZ = worldMinZ;
    mWorldMaxX = worldMaxX;
    mWorldMaxZ = worldMaxZ;

    size_t count = static_cast<size_t>(mWidth) * mHeight;
    mTexels.assign(count, PaintTexel());
    mRgbaBuffer.assign(count * 4, 0);

    for (size_t i = 0; i < count; ++i) {
        updateRgbaTexel(static_cast<u32>(i));
    }
}

void PaintMap3D::reset() {
    size_t count = static_cast<size_t>(mWidth) * mHeight;
    mTexels.assign(count, PaintTexel());
    mRgbaBuffer.assign(count * 4, 0);

    for (size_t i = 0; i < count; ++i) {
        updateRgbaTexel(static_cast<u32>(i));
    }
}

void PaintMap3D::worldToTexel(f32 wx, f32 wz, s32& tx, s32& ty) const {
    if (mWorldMaxX <= mWorldMinX || mWorldMaxZ <= mWorldMinZ || mWidth == 0 || mHeight == 0) {
        tx = 0; ty = 0;
        return;
    }

    f32 u = (wx - mWorldMinX) / (mWorldMaxX - mWorldMinX);
    f32 v = (wz - mWorldMinZ) / (mWorldMaxZ - mWorldMinZ);

    u = std::max(0.0f, std::min(1.0f, u));
    v = std::max(0.0f, std::min(1.0f, v));

    tx = static_cast<s32>(u * (mWidth - 1));
    ty = static_cast<s32>(v * (mHeight - 1));
}

void PaintMap3D::updateRgbaTexel(u32 index) {
    if (index >= mTexels.size()) return;

    size_t byteOffset = static_cast<size_t>(index) * 4;
    const auto& tex = mTexels[index];

    if (tex.team == 0) {
        // Team Alpha (Neon Orange)
        mRgbaBuffer[byteOffset + 0] = 255; // R
        mRgbaBuffer[byteOffset + 1] = 85;  // G
        mRgbaBuffer[byteOffset + 2] = 0;   // B
        mRgbaBuffer[byteOffset + 3] = tex.intensity; // A
    } else if (tex.team == 1) {
        // Team Bravo (Neon Cyan/Blue)
        mRgbaBuffer[byteOffset + 0] = 0;   // R
        mRgbaBuffer[byteOffset + 1] = 160; // G
        mRgbaBuffer[byteOffset + 2] = 255; // B
        mRgbaBuffer[byteOffset + 3] = tex.intensity; // A
    } else {
        // Neutral / Unpainted concrete
        mRgbaBuffer[byteOffset + 0] = 40;  // R
        mRgbaBuffer[byteOffset + 1] = 40;  // G
        mRgbaBuffer[byteOffset + 2] = 40;  // B
        mRgbaBuffer[byteOffset + 3] = 0;   // A
    }
}

void PaintMap3D::splatWorldSphere(const sead::Vector3f& center, f32 radius, u8 teamId, f32 intensity) {
    if (mWidth == 0 || mHeight == 0 || radius <= 0.0f) return;

    s32 minTx, minTy, maxTx, maxTy;
    worldToTexel(center.x - radius, center.z - radius, minTx, minTy);
    worldToTexel(center.x + radius, center.z + radius, maxTx, maxTy);

    f32 invWorldWidth = (mWorldMaxX - mWorldMinX) / static_cast<f32>(mWidth);
    f32 invWorldHeight = (mWorldMaxZ - mWorldMinZ) / static_cast<f32>(mHeight);
    f32 rSq = radius * radius;
    u8 clampedIntensity = static_cast<u8>(std::max(0.0f, std::min(255.0f, intensity * 255.0f)));

    for (s32 y = minTy; y <= maxTy; ++y) {
        f32 wz = mWorldMinZ + (y + 0.5f) * invWorldHeight;
        f32 dz = wz - center.z;

        for (s32 x = minTx; x <= maxTx; ++x) {
            f32 wx = mWorldMinX + (x + 0.5f) * invWorldWidth;
            f32 dx = wx - center.x;

            f32 distSq = dx * dx + dz * dz;
            if (distSq <= rSq) {
                u32 idx = static_cast<u32>(y * mWidth + x);
                mTexels[idx].team = teamId;
                mTexels[idx].intensity = clampedIntensity;
                updateRgbaTexel(idx);
            }
        }
    }
}

bool PaintMap3D::sampleInkAtWorldPos(const sead::Vector3f& worldPos, u8* outTeam, f32* outIntensity) const {
    if (mWidth == 0 || mHeight == 0) return false;

    s32 tx, ty;
    worldToTexel(worldPos.x, worldPos.z, tx, ty);
    u32 idx = static_cast<u32>(ty * mWidth + tx);

    if (idx < mTexels.size()) {
        if (outTeam) *outTeam = mTexels[idx].team;
        if (outIntensity) *outIntensity = static_cast<f32>(mTexels[idx].intensity) / 255.0f;
        return mTexels[idx].team != 255;
    }
    return false;
}

bool PaintMap3D::canSwimAtWorldPos(const sead::Vector3f& worldPos, u8 teamId) const {
    u8 team = 255;
    f32 intensity = 0.0f;
    if (sampleInkAtWorldPos(worldPos, &team, &intensity)) {
        return (team == teamId && intensity >= 0.5f);
    }
    return false;
}

PaintStats PaintMap3D::calculateStats() const {
    PaintStats stats;
    stats.totalPaintableTexels = static_cast<u32>(mTexels.size());

    if (stats.totalPaintableTexels == 0) return stats;

    for (const auto& tex : mTexels) {
        if (tex.team == 0 && tex.intensity > 0) {
            stats.alphaTexels++;
        } else if (tex.team == 1 && tex.intensity > 0) {
            stats.bravoTexels++;
        }
    }

    stats.alphaPercent = (static_cast<f32>(stats.alphaTexels) / static_cast<f32>(stats.totalPaintableTexels)) * 100.0f;
    stats.bravoPercent = (static_cast<f32>(stats.bravoTexels) / static_cast<f32>(stats.totalPaintableTexels)) * 100.0f;
    return stats;
}

} // namespace Game
