#pragma once

#include "types.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace Game {

struct ELinkUser {
    std::string name;
    std::string parent;
    std::string drcs;
    u32 offset = 0;
    std::vector<std::string> effects;
};

class ELinkDatabase {
public:
    ELinkDatabase();
    ~ELinkDatabase();

    bool load(const u8* data, size_t size);
    bool loadFromSzs(const char* szsPath);

    void clear();

    bool isLoaded() const { return mIsLoaded; }
    size_t getUserCount() const { return mUsers.size(); }
    size_t getNamedUserCount() const { return mUserMap.size(); }
    u32 getStringPoolOffset() const { return mStringPoolOffset; }

    const ELinkUser* findUser(const std::string& name) const;
    bool hasUser(const std::string& name) const;

    std::vector<std::string> getUserNames() const;
    std::vector<const ELinkUser*> getChildren(const std::string& parentName) const;
    std::vector<std::string> resolveHierarchy(const std::string& name) const;

    bool hasEffect(const std::string& userName, const std::string& effectName) const;
    size_t getEffectCount(const std::string& userName) const;

    static ELinkDatabase* instance();

private:
    std::string getStringFromPool(u32 relOffset) const;

    bool mIsLoaded = false;
    u32 mStringPoolOffset = 0;
    std::vector<u8> mBuffer;
    std::vector<ELinkUser> mUsers;
    std::unordered_map<std::string, size_t> mUserMap;
    static std::unique_ptr<ELinkDatabase> sInstance;
};

} // namespace Game
