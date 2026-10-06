#pragma once

#include "types.h"

namespace sead {

class Heap {
public:
    virtual ~Heap();
    virtual void* alloc(size_t size, s32 alignment = 4) = 0;
    virtual void free(void* ptr) = 0;
    virtual void* resize(void* ptr, size_t newSize) = 0;
    virtual void destroy() = 0;
    virtual size_t getMaxAllocatableSize(s32 alignment = 4) const = 0;

    const char* getName() const { return mName; }

protected:
    const char* mName;
    void* mStart;
    size_t mSize;
};

} // namespace sead
