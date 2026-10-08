#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

/**
 * NetPacketStream
 * High-performance binary bitstream serializer/deserializer.
 * Provides endian-safe encoding/decoding of network packets between
 * native PC x86_64 and Splatoon network replication protocols.
 */
class NetPacketStream : public GambitActor {
public:
    static constexpr u32 cDefaultBufferSize = 1024;

    NetPacketStream();
    virtual ~NetPacketStream() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setBuffer(u8* buffer, u32 capacity);

    bool writeU8(u8 value);
    bool writeU16(u16 value);
    bool writeU32(u32 value);
    bool writeFloat(f32 value);
    bool writeVector3f(const sead::Vector3f& vec);

    bool readU8(u8* outValue);
    bool readU16(u16* outValue);
    bool readU32(u32* outValue);
    bool readFloat(f32* outValue);
    bool readVector3f(sead::Vector3f* outVec);

    u32 getCursor() const { return mCursor; }
    u32 getCapacity() const { return mCapacity; }
    void resetCursor() { mCursor = 0; }

protected:
    u8* mBuffer;
    u32 mCapacity;
    u32 mCursor;
    bool mOwnsBuffer;
    u8 mInternalBuffer[cDefaultBufferSize];
};

} // namespace Game
