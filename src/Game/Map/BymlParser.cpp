#include "Game/Map/BymlParser.h"
#include "sead/resource/seadResource.h"
#include <cstring>

namespace Game {

BymlParser::BymlParser() : mIsBigEndian(true) {}

BymlParser::~BymlParser() {
    clear();
}

void BymlParser::clear() {
    mRoot.reset();
    mHashKeys.clear();
    mStringTable.clear();
}

bool BymlParser::parseStringTable(const u8* data, size_t size, u32 offset, std::vector<std::string>& outStrings) {
    if (offset + 4 > size) return false;
    u8 type = data[offset];
    if (type != 0xC2) return false; // 0xC2 = String Table

    u32 count = 0;
    if (mIsBigEndian) {
        count = (static_cast<u32>(data[offset + 1]) << 16) |
                (static_cast<u32>(data[offset + 2]) << 8)  |
                (static_cast<u32>(data[offset + 3]));
    } else {
        count = (static_cast<u32>(data[offset + 1])) |
                (static_cast<u32>(data[offset + 2]) << 8)  |
                (static_cast<u32>(data[offset + 3]) << 16);
    }

    outStrings.clear();
    outStrings.reserve(count);

    size_t offsetsStart = offset + 4;
    size_t stringsBase = offsetsStart + (count + 1) * 4;
    if (stringsBase > size) return false;

    for (u32 i = 0; i < count; ++i) {
        u32 strRelOff = 0;
        std::memcpy(&strRelOff, data + offsetsStart + i * 4, 4);
        strRelOff = sead::Endian::toHostU32(strRelOff, mIsBigEndian);

        size_t strAbsOff = offset + strRelOff;
        if (strAbsOff < size) {
            outStrings.push_back(std::string(reinterpret_cast<const char*>(data + strAbsOff)));
        } else {
            outStrings.push_back("");
        }
    }

    return true;
}

std::shared_ptr<BymlNode> BymlParser::parseDictionary(const u8* data, size_t size, u32 offset) {
    if (offset + 4 > size) return nullptr;
    u8 type = data[offset];
    if (type != 0xC1) return nullptr;

    u32 count = 0;
    if (mIsBigEndian) {
        count = (static_cast<u32>(data[offset + 1]) << 16) |
                (static_cast<u32>(data[offset + 2]) << 8)  |
                (static_cast<u32>(data[offset + 3]));
    } else {
        count = (static_cast<u32>(data[offset + 1])) |
                (static_cast<u32>(data[offset + 2]) << 8)  |
                (static_cast<u32>(data[offset + 3]) << 16);
    }

    auto dictNode = std::make_shared<BymlNode>(BymlNodeType::cDictionary);
    size_t entryStart = offset + 4;

    for (u32 i = 0; i < count; ++i) {
        size_t entryOffset = entryStart + i * 8;
        if (entryOffset + 8 > size) break;

        u32 keyIdx = 0;
        u8 childType = 0;
        if (mIsBigEndian) {
            keyIdx = (static_cast<u32>(data[entryOffset]) << 16) |
                     (static_cast<u32>(data[entryOffset + 1]) << 8) |
                     (static_cast<u32>(data[entryOffset + 2]));
            childType = data[entryOffset + 3];
        } else {
            keyIdx = (static_cast<u32>(data[entryOffset])) |
                     (static_cast<u32>(data[entryOffset + 1]) << 8) |
                     (static_cast<u32>(data[entryOffset + 2]) << 16);
            childType = data[entryOffset + 3];
        }

        u32 valueOrOffset = 0;
        std::memcpy(&valueOrOffset, data + entryOffset + 4, 4);
        valueOrOffset = sead::Endian::toHostU32(valueOrOffset, mIsBigEndian);

        std::string keyName = (keyIdx < mHashKeys.size()) ? mHashKeys[keyIdx] : "";
        auto child = parseNode(data, size, childType, valueOrOffset);
        if (child && !keyName.empty()) {
            dictNode->addDictChild(keyName, child);
        }
    }

    return dictNode;
}

std::shared_ptr<BymlNode> BymlParser::parseArray(const u8* data, size_t size, u32 offset) {
    if (offset + 4 > size) return nullptr;
    u8 type = data[offset];
    if (type != 0xC0) return nullptr;

    u32 count = 0;
    if (mIsBigEndian) {
        count = (static_cast<u32>(data[offset + 1]) << 16) |
                (static_cast<u32>(data[offset + 2]) << 8)  |
                (static_cast<u32>(data[offset + 3]));
    } else {
        count = (static_cast<u32>(data[offset + 1])) |
                (static_cast<u32>(data[offset + 2]) << 8)  |
                (static_cast<u32>(data[offset + 3]) << 16);
    }

    auto arrayNode = std::make_shared<BymlNode>(BymlNodeType::cArray);
    size_t typesStart = offset + 4;
    size_t valuesStart = (typesStart + count + 3) & ~3; // 4-byte aligned

    if (valuesStart + count * 4 > size) return nullptr;

    for (u32 i = 0; i < count; ++i) {
        u8 childType = data[typesStart + i];
        u32 valueOrOffset = 0;
        std::memcpy(&valueOrOffset, data + valuesStart + i * 4, 4);
        valueOrOffset = sead::Endian::toHostU32(valueOrOffset, mIsBigEndian);

        auto elem = parseNode(data, size, childType, valueOrOffset);
        if (elem) {
            arrayNode->addArrayElement(elem);
        }
    }

    return arrayNode;
}

std::shared_ptr<BymlNode> BymlParser::parseNode(const u8* data, size_t size, u8 type, u32 valueOrOffset) {
    BymlNodeType nodeType = static_cast<BymlNodeType>(type);

    switch (nodeType) {
        case BymlNodeType::cString: {
            auto node = std::make_shared<BymlNode>(nodeType);
            if (valueOrOffset < mStringTable.size()) {
                node->setString(mStringTable[valueOrOffset]);
            }
            return node;
        }
        case BymlNodeType::cBinary: {
            auto node = std::make_shared<BymlNode>(nodeType);
            return node;
        }
        case BymlNodeType::cArray: {
            return parseArray(data, size, valueOrOffset);
        }
        case BymlNodeType::cDictionary: {
            return parseDictionary(data, size, valueOrOffset);
        }
        case BymlNodeType::cBool: {
            auto node = std::make_shared<BymlNode>(nodeType);
            node->setBool(valueOrOffset != 0);
            return node;
        }
        case BymlNodeType::cInt32: {
            auto node = std::make_shared<BymlNode>(nodeType);
            node->setInt(static_cast<s32>(valueOrOffset));
            return node;
        }
        case BymlNodeType::cUInt32: {
            auto node = std::make_shared<BymlNode>(nodeType);
            node->setInt(static_cast<s32>(valueOrOffset));
            return node;
        }
        case BymlNodeType::cFloat: {
            auto node = std::make_shared<BymlNode>(nodeType);
            f32 fVal;
            std::memcpy(&fVal, &valueOrOffset, sizeof(f32));
            node->setFloat(fVal);
            return node;
        }
        default:
            return nullptr;
    }
}

bool BymlParser::parse(const u8* data, size_t size) {
    clear();
    if (!data || size < 16) return false;

    // Check magic: 'BY' (0x4259 Big Endian) or 'YB' (0x5942 Little Endian)
    if (data[0] == 'B' && data[1] == 'Y') {
        mIsBigEndian = true;
    } else if (data[0] == 'Y' && data[1] == 'B') {
        mIsBigEndian = false;
    } else {
        return false;
    }

    u32 hashKeysOffset = 0;
    u32 stringTableOffset = 0;
    u32 rootOffset = 0;

    std::memcpy(&hashKeysOffset, data + 4, 4);
    std::memcpy(&stringTableOffset, data + 8, 4);
    std::memcpy(&rootOffset, data + 12, 4);

    hashKeysOffset = sead::Endian::toHostU32(hashKeysOffset, mIsBigEndian);
    stringTableOffset = sead::Endian::toHostU32(stringTableOffset, mIsBigEndian);
    rootOffset = sead::Endian::toHostU32(rootOffset, mIsBigEndian);

    if (hashKeysOffset != 0) {
        parseStringTable(data, size, hashKeysOffset, mHashKeys);
    }
    if (stringTableOffset != 0) {
        parseStringTable(data, size, stringTableOffset, mStringTable);
    }

    if (rootOffset != 0 && rootOffset < size) {
        u8 rootType = data[rootOffset];
        if (rootType == 0xC1) {
            mRoot = parseDictionary(data, size, rootOffset);
        } else if (rootType == 0xC0) {
            mRoot = parseArray(data, size, rootOffset);
        }
    }

    return mRoot != nullptr;
}

// BymlNode helper methods
bool BymlNode::hasKey(const std::string& key) const {
    return mDict.find(key) != mDict.end();
}

const BymlNode* BymlNode::getChild(const std::string& key) const {
    auto it = mDict.find(key);
    return (it != mDict.end()) ? it->second.get() : nullptr;
}

s32 BymlNode::getInt(const std::string& key, s32 defaultVal) const {
    const BymlNode* child = getChild(key);
    return child ? child->getInt(defaultVal) : defaultVal;
}

f32 BymlNode::getFloat(const std::string& key, f32 defaultVal) const {
    const BymlNode* child = getChild(key);
    return child ? child->getFloat(defaultVal) : defaultVal;
}

bool BymlNode::getBool(const std::string& key, bool defaultVal) const {
    const BymlNode* child = getChild(key);
    return child ? child->getBool(defaultVal) : defaultVal;
}

std::string BymlNode::getString(const std::string& key, const std::string& defaultVal) const {
    const BymlNode* child = getChild(key);
    return child ? child->asString(defaultVal) : defaultVal;
}

const BymlNode* BymlNode::getElement(size_t index) const {
    if (index >= mArray.size()) return nullptr;
    return mArray[index].get();
}

} // namespace Game
