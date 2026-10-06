#pragma once

#include "types.h"
#include "sead/math/seadVector.h"
#include "sead/heap/seadHeap.h"

namespace Game {

enum class PaintColor : u8 {
    Neutral = 0,
    TeamAlpha = 1,
    TeamBravo = 2,
};

class PaintTextureMgr {
public:
    PaintTextureMgr();
    virtual ~PaintTextureMgr();

    static PaintTextureMgr* instance() { return sInstance; }

    virtual void init(sead::Heap* heap, u32 mapWidth, u32 mapHeight);
    virtual void update();
    virtual void clear();

    // Painting commands
    void paintSplat(const sead::Vector3f& worldPos, f32 radius, PaintColor color);

    // Queries (used by Player to check if standing in friendly or enemy ink)
    PaintColor getColorAt(const sead::Vector3f& worldPos) const;
    bool isFriendlyInk(const sead::Vector3f& worldPos, u32 teamId) const;
    bool isEnemyInk(const sead::Vector3f& worldPos, u32 teamId) const;

    // Turf War percentage calculation
    f32 getTurfPercentage(PaintColor color) const;

    static PaintTextureMgr* sInstance;

protected:
    u32 mMapWidth;
    u32 mMapHeight;
    u8* mPaintBuffer; // 2D grid storing ink colors per cell
    u32 mTotalPaintableCells;
    u32 mAlphaCellCount;
    u32 mBravoCellCount;
    sead::Heap* mHeap;
};

} // namespace Game
