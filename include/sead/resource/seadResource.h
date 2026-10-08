#pragma once

#include "types.h"
#include <vector>
#include <string>
#include <cstring>

namespace sead {

class Endian {
public:
    static inline u16 swap16(u16 val) {
        return static_cast<u16>((val << 8) | (val >> 8));
    }

    static inline u32 swap32(u32 val) {
        return ((val << 24) & 0xFF000000) |
               ((val <<  8) & 0x00FF0000) |
               ((val >>  8) & 0x0000FF00) |
               ((val >> 24) & 0x000000FF);
    }

    static inline f32 swapFloat(f32 val) {
        u32 temp;
        std::memcpy(&temp, &val, sizeof(f32));
        temp = swap32(temp);
        f32 result;
        std::memcpy(&result, &temp, sizeof(f32));
        return result;
    }

    static inline u16 toHostU16(u16 val, bool isBigEndian) {
        return isBigEndian ? swap16(val) : val;
    }

    static inline u32 toHostU32(u32 val, bool isBigEndian) {
        return isBigEndian ? swap32(val) : val;
    }

    static inline f32 toHostF32(f32 val, bool isBigEndian) {
        return isBigEndian ? swapFloat(val) : val;
    }
};

class Yaz0 {
public:
    // Decompresses Yaz0 encoded .szs archive buffer into uncompressed output vector
    static bool decompress(const u8* src, size_t srcSize, std::vector<u8>& dst);
};

struct SarcFileInfo {
    std::string name;
    u32 nameHash;
    const u8* data;
    size_t size;
};

class SarcArchive {
public:
    SarcArchive();
    ~SarcArchive();

    bool load(const u8* data, size_t size);
    void clear();

    const u8* getFile(const char* path, size_t* outSize = nullptr) const;
    const u8* getFileByHash(u32 hash, size_t* outSize = nullptr) const;
    bool hasFile(const char* path) const;

    size_t getFileCount() const { return mFiles.size(); }
    const SarcFileInfo* getFileInfo(size_t index) const;

    static u32 calcHash(const char* name, u32 key = 0x65);

private:
    std::vector<u8> mBuffer;
    std::vector<SarcFileInfo> mFiles;
    bool mIsBigEndian;
};

} // namespace sead
