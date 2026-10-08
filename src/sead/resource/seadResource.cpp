#include "sead/resource/seadResource.h"
#include <cstdio>
#include <algorithm>

namespace sead {

#pragma pack(push, 1)
struct SarcHeaderRaw {
    u32 magic;          // 'SARC' (0x53415243)
    u16 headerSize;     // 0x14
    u16 byteOrder;      // 0xFEFF (Big) or 0xFFFE (Little)
    u32 fileSize;
    u32 dataOffset;
    u16 version;        // 0x0100
    u16 reserved;
};

struct SfatHeaderRaw {
    u32 magic;          // 'SFAT' (0x53464154)
    u16 headerSize;     // 0x0C
    u16 nodeCount;
    u32 hashKey;
};

struct SfatNodeRaw {
    u32 nameHash;
    u32 nameOffsetAndFlags;
    u32 dataStart;
    u32 dataEnd;
};

struct SfntHeaderRaw {
    u32 magic;          // 'SFNT' (0x53464E54)
    u16 headerSize;     // 0x08
    u16 reserved;
};
#pragma pack(pop)

bool Yaz0::decompress(const u8* src, size_t srcSize, std::vector<u8>& dst) {
    if (!src || srcSize < 16) return false;

    // Check 'Yaz0' magic (0x59617A30)
    if (src[0] != 'Y' || src[1] != 'a' || src[2] != 'z' || src[3] != '0') {
        return false;
    }

    u32 uncompressedSize = (static_cast<u32>(src[4]) << 24) |
                           (static_cast<u32>(src[5]) << 16) |
                           (static_cast<u32>(src[6]) << 8)  |
                           (static_cast<u32>(src[7]));

    dst.resize(uncompressedSize);
    size_t srcPos = 16;
    size_t dstPos = 0;
    u8 validBitCount = 0;
    u8 currCodeByte = 0;

    while (dstPos < uncompressedSize && srcPos < srcSize) {
        if (validBitCount == 0) {
            currCodeByte = src[srcPos++];
            validBitCount = 8;
        }

        if (currCodeByte & 0x80) {
            // Direct copy
            dst[dstPos++] = src[srcPos++];
        } else {
            // Back reference copy
            if (srcPos + 1 >= srcSize) break;
            u8 b1 = src[srcPos++];
            u8 b2 = src[srcPos++];
            u32 dist = ((static_cast<u32>(b1 & 0x0F) << 8) | b2) + 1;
            u32 copyLen = b1 >> 4;

            if (copyLen == 0) {
                if (srcPos >= srcSize) break;
                copyLen = static_cast<u32>(src[srcPos++]) + 0x12;
            } else {
                copyLen += 2;
            }

            if (dist > dstPos) break;
            size_t copySrc = dstPos - dist;
            for (u32 i = 0; i < copyLen && dstPos < uncompressedSize; ++i) {
                dst[dstPos++] = dst[copySrc++];
            }
        }

        currCodeByte <<= 1;
        validBitCount--;
    }

    return dstPos == uncompressedSize;
}

u32 SarcArchive::calcHash(const char* name, u32 key) {
    u32 hash = 0;
    while (*name) {
        hash = hash * key + static_cast<u8>(*name++);
    }
    return hash;
}

SarcArchive::SarcArchive() : mIsBigEndian(true) {}

SarcArchive::~SarcArchive() {
    clear();
}

void SarcArchive::clear() {
    mBuffer.clear();
    mFiles.clear();
}

bool SarcArchive::load(const u8* data, size_t size) {
    clear();
    if (!data || size < sizeof(SarcHeaderRaw)) return false;

    // Check if the data is Yaz0-compressed (.szs)
    if (data[0] == 'Y' && data[1] == 'a' && data[2] == 'z' && data[3] == '0') {
        if (!Yaz0::decompress(data, size, mBuffer)) {
            return false;
        }
        data = mBuffer.data();
        size = mBuffer.size();
    } else {
        mBuffer.assign(data, data + size);
        data = mBuffer.data();
    }

    if (size < sizeof(SarcHeaderRaw)) return false;
    const SarcHeaderRaw* sarcHdr = reinterpret_cast<const SarcHeaderRaw*>(data);

    // Verify SARC magic ('SARC' = 0x53415243)
    if (sarcHdr->magic != 0x53415243 && sarcHdr->magic != 0x43524153) {
        return false;
    }

    // Byte 6 & 7: 0xFE 0xFF = Big Endian (Wii U), 0xFF 0xFE = Little Endian
    mIsBigEndian = (data[6] == 0xFE && data[7] == 0xFF);

    u32 dataOffset = Endian::toHostU32(sarcHdr->dataOffset, mIsBigEndian);
    u16 sarcHdrSize = Endian::toHostU16(sarcHdr->headerSize, mIsBigEndian);

    if (sarcHdrSize + sizeof(SfatHeaderRaw) > size) return false;
    const SfatHeaderRaw* sfatHdr = reinterpret_cast<const SfatHeaderRaw*>(data + sarcHdrSize);

    u16 nodeCount = Endian::toHostU16(sfatHdr->nodeCount, mIsBigEndian);
    u32 hashKey = Endian::toHostU32(sfatHdr->hashKey, mIsBigEndian);
    (void)hashKey;

    size_t sfatOffset = sarcHdrSize + sizeof(SfatHeaderRaw);
    size_t sfntOffset = sfatOffset + nodeCount * sizeof(SfatNodeRaw);

    if (sfntOffset + sizeof(SfntHeaderRaw) > size) return false;
    const SfntHeaderRaw* sfntHdr = reinterpret_cast<const SfntHeaderRaw*>(data + sfntOffset);
    u16 sfntHdrSize = Endian::toHostU16(sfntHdr->headerSize, mIsBigEndian);
    const char* strTable = reinterpret_cast<const char*>(data + sfntOffset + sfntHdrSize);

    mFiles.reserve(nodeCount);

    const SfatNodeRaw* nodes = reinterpret_cast<const SfatNodeRaw*>(data + sfatOffset);
    for (u16 i = 0; i < nodeCount; ++i) {
        u32 nameHash = Endian::toHostU32(nodes[i].nameHash, mIsBigEndian);
        u32 nameAttr = Endian::toHostU32(nodes[i].nameOffsetAndFlags, mIsBigEndian);
        u32 dataStart = Endian::toHostU32(nodes[i].dataStart, mIsBigEndian);
        u32 dataEnd = Endian::toHostU32(nodes[i].dataEnd, mIsBigEndian);

        SarcFileInfo info;
        info.nameHash = nameHash;
        info.data = data + dataOffset + dataStart;
        info.size = (dataEnd >= dataStart) ? (dataEnd - dataStart) : 0;

        // Bit 24 indicates name presence: name offset is (nameAttr & 0x00FFFFFF) * 4
        if ((nameAttr & 0x01000000) != 0 || (nameAttr & 0xFF000000) == 0x01000000) {
            u32 strOff = (nameAttr & 0x00FFFFFF) * 4;
            if (sfntOffset + sfntHdrSize + strOff < size) {
                info.name = std::string(strTable + strOff);
            }
        }

        mFiles.push_back(info);
    }

    return true;
}

const u8* SarcArchive::getFile(const char* path, size_t* outSize) const {
    if (!path) return nullptr;
    u32 hash = calcHash(path);
    return getFileByHash(hash, outSize);
}

const u8* SarcArchive::getFileByHash(u32 hash, size_t* outSize) const {
    for (const auto& file : mFiles) {
        if (file.nameHash == hash) {
            if (outSize) *outSize = file.size;
            return file.data;
        }
    }
    return nullptr;
}

bool SarcArchive::hasFile(const char* path) const {
    return getFile(path) != nullptr;
}

const SarcFileInfo* SarcArchive::getFileInfo(size_t index) const {
    if (index >= mFiles.size()) return nullptr;
    return &mFiles[index];
}

} // namespace sead
