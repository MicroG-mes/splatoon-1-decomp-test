#pragma once

#include "types.h"

// Cafe OS coreinit definitions
extern "C" {

void OSReport(const char* fmt, ...);
void OSPanic(const char* file, s32 line, const char* fmt, ...);
u32 OSGetTick();
u64 OSGetSystemTime();

void* OSAllocFromSystem(u32 size, u32 align);
void OSFreeToSystem(void* ptr);

struct OSThread {
    undefined field[0x600];
};

struct OSMutex {
    undefined field[0x20];
};

void OSMutex_Init(OSMutex* mutex);
void OSMutex_Lock(OSMutex* mutex);
void OSMutex_Unlock(OSMutex* mutex);

}
