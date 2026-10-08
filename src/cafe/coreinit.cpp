#include "cafe/coreinit.h"
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <chrono>
#include <mutex>
#include <windows.h>

extern "C" {

void OSReport(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    OutputDebugStringA(buffer);
    printf("%s", buffer);
}

void OSPanic(const char* file, s32 line, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    fprintf(stderr, "\n[OSPanic] %s:%d: %s\n", file, line, buffer);
    abort();
}

u32 OSGetTick() {
    using namespace std::chrono;
    static auto start = steady_clock::now();
    auto now = steady_clock::now();
    return static_cast<u32>(duration_cast<milliseconds>(now - start).count());
}

u64 OSGetSystemTime() {
    using namespace std::chrono;
    auto now = system_clock::now();
    return static_cast<u64>(duration_cast<microseconds>(now.time_since_epoch()).count());
}

void* OSAllocFromSystem(u32 size, u32 align) {
    if (align == 0) align = 4;
    return _aligned_malloc(size, align);
}

void OSFreeToSystem(void* ptr) {
    if (ptr) {
        _aligned_free(ptr);
    }
}

// Win32 CriticalSection bridge for Cafe OSMutex
struct Win32MutexBridge {
    CRITICAL_SECTION cs;
};

void OSMutex_Init(OSMutex* mutex) {
    if (!mutex) return;
    auto* bridge = reinterpret_cast<Win32MutexBridge*>(mutex);
    InitializeCriticalSection(&bridge->cs);
}

void OSMutex_Lock(OSMutex* mutex) {
    if (!mutex) return;
    auto* bridge = reinterpret_cast<Win32MutexBridge*>(mutex);
    EnterCriticalSection(&bridge->cs);
}

void OSMutex_Unlock(OSMutex* mutex) {
    if (!mutex) return;
    auto* bridge = reinterpret_cast<Win32MutexBridge*>(mutex);
    LeaveCriticalSection(&bridge->cs);
}

} // extern "C"
