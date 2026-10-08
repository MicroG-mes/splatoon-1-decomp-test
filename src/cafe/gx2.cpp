#include "cafe/gx2.h"
#include <cstdio>

extern "C" {

void GX2Init(u32* initAttribs) {
    (void)initAttribs;
}

void GX2Shutdown() {
}

void GX2SetViewport(f32 x, f32 y, f32 width, f32 height, f32 nearZ, f32 farZ) {
    (void)x; (void)y; (void)width; (void)height; (void)nearZ; (void)farZ;
}

void GX2SetScissor(u32 x, u32 y, u32 width, u32 height) {
    (void)x; (void)y; (void)width; (void)height;
}

void GX2SwapScanBuffers() {
    // Native PC frame presentation bridge
}

void GX2Flush() {
}

void GX2DrawDone() {
}

} // extern "C"
