#pragma once

#include "types.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace Game {

struct SLinkUser {
    std::string name;
    std::string parent;
    std::string drcs; // Distance Reduction Curve Set
    u32 offset = 0;
    std::vector<std::string> triggers;
};

class SLinkDatabase {
public:
    SLinkDatabase();
    ~SLinkDatabase();

    // Load from decompressed XLNK buffer or from .szs archive file
    bool load(const u8* data, size_t size);
    bool loadFromSzs(const char* szsPath);

    void clear();

    bool isLoaded() const { return mIsLoaded; }
    size_t getUserCount() const { return mUsers.size(); }
    size_t getNamedUserCount() const { return mUserMap.size(); }
    size_t getTotalStringCount() const { return mTotalStrings; }
    u32 getStringPoolOffset() const { return mStringPoolOffset; }

    const SLinkUser* findUser(const std::string& name) const;
    bool hasUser(const std::string& name) const;

    std::vector<std::string> getUserNames() const;
    std::vector<const SLinkUser*> getChildren(const std::string& parentName) const;
    std::vector<std::string> resolveHierarchy(const std::string& name) const;

    bool hasTrigger(const std::string& userName, const std::string& triggerName) const;
    size_t getTriggerCount(const std::string& userName) const;

    // Singleton access
    static SLinkDatabase* instance();

private:
    std::string getStringFromPool(u32 relOffset) const;

    bool mIsLoaded = false;
    u32 mStringPoolOffset = 0;
    size_t mTotalStrings = 0;
    std::vector<u8> mBuffer;
    std::vector<SLinkUser> mUsers;
    std::unordered_map<std::string, size_t> mUserMap;
    static std::unique_ptr<SLinkDatabase> sInstance;
};

} // namespace Game
