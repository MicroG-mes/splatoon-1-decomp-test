#pragma once

#include "types.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace Game {

enum class BymlNodeType : u8 {
    cNull        = 0x00,
    cString      = 0xA0,
    cBinary      = 0xA1,
    cArray       = 0xC0,
    cDictionary  = 0xC1,
    cBool        = 0xD0,
    cInt32       = 0xD1,
    cFloat       = 0xD2,
    cUInt32      = 0xD3,
    cInt64       = 0xD4,
    cUInt64      = 0xD5,
    cDouble      = 0xD6
};

class BymlNode;

class BymlParser {
public:
    BymlParser();
    ~BymlParser();

    bool parse(const u8* data, size_t size);
    void clear();

    bool isValid() const { return mRoot != nullptr; }
    const BymlNode* getRoot() const { return mRoot.get(); }

private:
    std::shared_ptr<BymlNode> mRoot;
    std::vector<std::string> mHashKeys;
    std::vector<std::string> mStringTable;
    bool mIsBigEndian;

    bool parseStringTable(const u8* data, size_t size, u32 offset, std::vector<std::string>& outStrings);
    std::shared_ptr<BymlNode> parseNode(const u8* data, size_t size, u8 type, u32 valueOrOffset);
    std::shared_ptr<BymlNode> parseArray(const u8* data, size_t size, u32 offset);
    std::shared_ptr<BymlNode> parseDictionary(const u8* data, size_t size, u32 offset);
};

class BymlNode {
public:
    BymlNode(BymlNodeType type) : mType(type), mIntVal(0), mFloatVal(0.0f), mBoolVal(false) {}

    BymlNodeType getType() const { return mType; }
    bool isDictionary() const { return mType == BymlNodeType::cDictionary; }
    bool isArray() const { return mType == BymlNodeType::cArray; }
    bool isString() const { return mType == BymlNodeType::cString; }
    bool isInt() const { return mType == BymlNodeType::cInt32 || mType == BymlNodeType::cUInt32; }
    bool isFloat() const { return mType == BymlNodeType::cFloat; }
    bool isBool() const { return mType == BymlNodeType::cBool; }

    // Primitive getters
    s32 getInt(s32 defaultVal = 0) const { return isInt() ? mIntVal : defaultVal; }
    f32 getFloat(f32 defaultVal = 0.0f) const { return isFloat() ? mFloatVal : (isInt() ? static_cast<f32>(mIntVal) : defaultVal); }
    bool getBool(bool defaultVal = false) const { return isBool() ? mBoolVal : defaultVal; }
    const std::string& asString(const std::string& defaultVal = "") const { return isString() ? mStringVal : defaultVal; }

    // Dictionary access
    bool hasKey(const std::string& key) const;
    const BymlNode* getChild(const std::string& key) const;
    s32 getInt(const std::string& key, s32 defaultVal = 0) const;
    f32 getFloat(const std::string& key, f32 defaultVal = 0.0f) const;
    bool getBool(const std::string& key, bool defaultVal = false) const;
    std::string getString(const std::string& key, const std::string& defaultVal = "") const;

    const std::map<std::string, std::shared_ptr<BymlNode>>& getDictMembers() const { return mDict; }

    // Array access
    size_t getArraySize() const { return mArray.size(); }
    const BymlNode* getElement(size_t index) const;

    // Mutators used during parsing
    void setInt(s32 v) { mIntVal = v; }
    void setFloat(f32 v) { mFloatVal = v; }
    void setBool(bool v) { mBoolVal = v; }
    void setString(const std::string& s) { mStringVal = s; }
    void addDictChild(const std::string& key, std::shared_ptr<BymlNode> child) { mDict[key] = child; }
    void addArrayElement(std::shared_ptr<BymlNode> elem) { mArray.push_back(elem); }

private:
    BymlNodeType mType;
    s32 mIntVal;
    f32 mFloatVal;
    bool mBoolVal;
    std::string mStringVal;
    std::map<std::string, std::shared_ptr<BymlNode>> mDict;
    std::vector<std::shared_ptr<BymlNode>> mArray;
};

} // namespace Game
