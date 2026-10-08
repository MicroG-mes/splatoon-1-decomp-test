#include "Game/Net/NetPacketStream.h"
#include <cstring>

namespace Game {

NetPacketStream::NetPacketStream()
    : mBuffer(mInternalBuffer),
      mCapacity(cDefaultBufferSize),
      mCursor(0),
      mOwnsBuffer(true) {
    std::memset(mInternalBuffer, 0, sizeof(mInternalBuffer));
}

NetPacketStream::~NetPacketStream() {
}

void NetPacketStream::init() {
    GambitActor::init();
    mBuffer = mInternalBuffer;
    mCapacity = cDefaultBufferSize;
    mCursor = 0;
    mOwnsBuffer = true;
    std::memset(mInternalBuffer, 0, sizeof(mInternalBuffer));
}

void NetPacketStream::setBuffer(u8* buffer, u32 capacity) {
    if (buffer && capacity > 0) {
        mBuffer = buffer;
        mCapacity = capacity;
        mOwnsBuffer = false;
    } else {
        mBuffer = mInternalBuffer;
        mCapacity = cDefaultBufferSize;
        mOwnsBuffer = true;
    }
    mCursor = 0;
}

bool NetPacketStream::writeU8(u8 value) {
    if (mCursor + sizeof(u8) > mCapacity) {
        return false;
    }
    mBuffer[mCursor++] = value;
    return true;
}

bool NetPacketStream::writeU16(u16 value) {
    if (mCursor + sizeof(u16) > mCapacity) {
        return false;
    }
    // Big-endian network byte order
    mBuffer[mCursor++] = static_cast<u8>((value >> 8) & 0xFF);
    mBuffer[mCursor++] = static_cast<u8>(value & 0xFF);
    return true;
}

bool NetPacketStream::writeU32(u32 value) {
    if (mCursor + sizeof(u32) > mCapacity) {
        return false;
    }
    // Big-endian network byte order
    mBuffer[mCursor++] = static_cast<u8>((value >> 24) & 0xFF);
    mBuffer[mCursor++] = static_cast<u8>((value >> 16) & 0xFF);
    mBuffer[mCursor++] = static_cast<u8>((value >> 8) & 0xFF);
    mBuffer[mCursor++] = static_cast<u8>(value & 0xFF);
    return true;
}

bool NetPacketStream::writeFloat(f32 value) {
    u32 rawBits = 0;
    std::memcpy(&rawBits, &value, sizeof(f32));
    return writeU32(rawBits);
}

bool NetPacketStream::writeVector3f(const sead::Vector3f& vec) {
    return writeFloat(vec.x) && writeFloat(vec.y) && writeFloat(vec.z);
}

bool NetPacketStream::readU8(u8* outValue) {
    if (!outValue || mCursor + sizeof(u8) > mCapacity) {
        return false;
    }
    *outValue = mBuffer[mCursor++];
    return true;
}

bool NetPacketStream::readU16(u16* outValue) {
    if (!outValue || mCursor + sizeof(u16) > mCapacity) {
        return false;
    }
    u16 b0 = mBuffer[mCursor++];
    u16 b1 = mBuffer[mCursor++];
    *outValue = (b0 << 8) | b1;
    return true;
}

bool NetPacketStream::readU32(u32* outValue) {
    if (!outValue || mCursor + sizeof(u32) > mCapacity) {
        return false;
    }
    u32 b0 = mBuffer[mCursor++];
    u32 b1 = mBuffer[mCursor++];
    u32 b2 = mBuffer[mCursor++];
    u32 b3 = mBuffer[mCursor++];
    *outValue = (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
    return true;
}

bool NetPacketStream::readFloat(f32* outValue) {
    if (!outValue) {
        return false;
    }
    u32 rawBits = 0;
    if (!readU32(&rawBits)) {
        return false;
    }
    std::memcpy(outValue, &rawBits, sizeof(f32));
    return true;
}

bool NetPacketStream::readVector3f(sead::Vector3f* outVec) {
    if (!outVec) {
        return false;
    }
    return readFloat(&outVec->x) && readFloat(&outVec->y) && readFloat(&outVec->z);
}

void NetPacketStream::update() {
    // Stream serialization utility actor
}

void NetPacketStream::draw() {
    // No visual representation
}

} // namespace Game
