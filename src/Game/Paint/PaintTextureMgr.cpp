#include "Game/Paint/PaintTextureMgr.h"
#include <cmath>
#include <cstring>
#include <algorithm>

namespace Game {

PaintTextureMgr* PaintTextureMgr::sInstance = nullptr;

PaintTextureMgr::PaintTextureMgr()
    : mMapWidth(512),
      mMapHeight(512),
      mPaintBuffer(nullptr),
      mTotalPaintableCells(512 * 512),
      mAlphaCellCount(0),
      mBravoCellCount(0),
      mHeap(nullptr) {
    sInstance = this;
}

PaintTextureMgr::~PaintTextureMgr() {
    clear();
    if (sInstance == this) {
        sInstance = nullptr;
    }
}

void PaintTextureMgr::init(sead::Heap* heap, u32 mapWidth, u32 mapHeight) {
    mHeap = heap;
    mMapWidth = mapWidth;
    mMapHeight = mapHeight;
    mTotalPaintableCells = mapWidth * mapHeight;

    if (mPaintBuffer) {
        delete[] mPaintBuffer;
    }
    mPaintBuffer = new u8[mTotalPaintableCells];
    std::memset(mPaintBuffer, 0, mTotalPaintableCells);
}

void PaintTextureMgr::clear() {
    if (mPaintBuffer) {
        delete[] mPaintBuffer;
        mPaintBuffer = nullptr;
    }
    mAlphaCellCount = 0;
    mBravoCellCount = 0;
}

void PaintTextureMgr::update() {}

void PaintTextureMgr::paintSplat(const sead::Vector3f& worldPos, f32 radius, PaintColor color) {
    if (!mPaintBuffer) return;

    // Convert world coordinate to normalized map cell coordinates
    s32 centerX = static_cast<s32>(worldPos.x + (mMapWidth / 2.0f));
    s32 centerY = static_cast<s32>(worldPos.z + (mMapHeight / 2.0f));
    s32 radCells = static_cast<s32>(radius * 4.0f); // Scale factor

    s32 minX = std::max(0, centerX - radCells);
    s32 maxX = std::min(static_cast<s32>(mMapWidth) - 1, centerX + radCells);
    s32 minY = std::max(0, centerY - radCells);
    s32 maxY = std::min(static_cast<s32>(mMapHeight) - 1, centerY + radCells);

    f32 rSq = radCells * radCells;

    for (s32 y = minY; y <= maxY; ++y) {
        for (s32 x = minX; x <= maxX; ++x) {
            f32 dx = static_cast<f32>(x - centerX);
            f32 dy = static_cast<f32>(y - centerY);
            if ((dx * dx) + (dy * dy) <= rSq) {
                u32 idx = (y * mMapWidth) + x;
                u8 oldColor = mPaintBuffer[idx];
                u8 newColor = static_cast<u8>(color);

                if (oldColor != newColor) {
                    if (oldColor == static_cast<u8>(PaintColor::TeamAlpha)) mAlphaCellCount--;
                    else if (oldColor == static_cast<u8>(PaintColor::TeamBravo)) mBravoCellCount--;

                    if (newColor == static_cast<u8>(PaintColor::TeamAlpha)) mAlphaCellCount++;
                    else if (newColor == static_cast<u8>(PaintColor::TeamBravo)) mBravoCellCount++;

                    mPaintBuffer[idx] = newColor;
                }
            }
        }
    }
}

PaintColor PaintTextureMgr::getColorAt(const sead::Vector3f& worldPos) const {
    if (!mPaintBuffer) return PaintColor::Neutral;

    s32 cellX = static_cast<s32>(worldPos.x + (mMapWidth / 2.0f));
    s32 cellY = static_cast<s32>(worldPos.z + (mMapHeight / 2.0f));

    if (cellX < 0 || cellX >= static_cast<s32>(mMapWidth) || cellY < 0 || cellY >= static_cast<s32>(mMapHeight)) {
        return PaintColor::Neutral;
    }

    u32 idx = (cellY * mMapWidth) + cellX;
    return static_cast<PaintColor>(mPaintBuffer[idx]);
}

bool PaintTextureMgr::isFriendlyInk(const sead::Vector3f& worldPos, u32 teamId) const {
    PaintColor c = getColorAt(worldPos);
    return (teamId == 0 && c == PaintColor::TeamAlpha) || (teamId == 1 && c == PaintColor::TeamBravo);
}

bool PaintTextureMgr::isEnemyInk(const sead::Vector3f& worldPos, u32 teamId) const {
    PaintColor c = getColorAt(worldPos);
    return (teamId == 0 && c == PaintColor::TeamBravo) || (teamId == 1 && c == PaintColor::TeamAlpha);
}

f32 PaintTextureMgr::getTurfPercentage(PaintColor color) const {
    if (mTotalPaintableCells == 0) return 0.0f;
    if (color == PaintColor::TeamAlpha) {
        return (static_cast<f32>(mAlphaCellCount) / static_cast<f32>(mTotalPaintableCells)) * 100.0f;
    } else if (color == PaintColor::TeamBravo) {
        return (static_cast<f32>(mBravoCellCount) / static_cast<f32>(mTotalPaintableCells)) * 100.0f;
    }
    return 0.0f;
}

} // namespace Game
