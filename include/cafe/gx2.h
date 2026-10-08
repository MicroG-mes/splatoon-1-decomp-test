#pragma once

#include "types.h"

extern "C" {

struct GX2ColorBuffer {
    u32 width;
    u32 height;
    u32 format;
    void* image;
};

void GX2Init(u32* initAttribs);
void GX2Shutdown();
void GX2SetViewport(f32 x, f32 y, f32 width, f32 height, f32 nearZ, f32 farZ);
void GX2SetScissor(u32 x, u32 y, u32 width, u32 height);
void GX2SwapScanBuffers();
void GX2Flush();
void GX2DrawDone();

} // extern "C"
