#include "sead/heap/seadHeap.h"
#include <cstdlib>

namespace sead {

Heap::~Heap() {
}

class ExpHeap : public Heap {
public:
    ExpHeap(const char* name, size_t size) {
        mName = name;
        mSize = size;
        mStart = malloc(size);
    }

    virtual ~ExpHeap() override {
        destroy();
    }

    virtual void* alloc(size_t size, s32 alignment = 4) override {
        (void)alignment;
        return malloc(size);
    }

    virtual void free(void* ptr) override {
        ::free(ptr);
    }

    virtual void* resize(void* ptr, size_t newSize) override {
        return realloc(ptr, newSize);
    }

    virtual void destroy() override {
        if (mStart) {
            ::free(mStart);
            mStart = nullptr;
        }
    }

    virtual size_t getMaxAllocatableSize(s32 alignment = 4) const override {
        (void)alignment;
        return mSize;
    }

    static ExpHeap* create(size_t size, const char* name) {
        return new ExpHeap(name, size);
    }
};

} // namespace sead
