#include "Game/Effect/ELinkDatabase.h"
#include "sead/resource/seadResource.h"
#include <fstream>
#include <algorithm>
#include <cstring>

namespace Game {

std::unique_ptr<ELinkDatabase> ELinkDatabase::sInstance = nullptr;

static inline u32 readBE32(const u8* ptr) {
    return (static_cast<u32>(ptr[0]) << 24) |
           (static_cast<u32>(ptr[1]) << 16) |
           (static_cast<u32>(ptr[2]) << 8) |
           static_cast<u32>(ptr[3]);
}

ELinkDatabase::ELinkDatabase() {
}

ELinkDatabase::~ELinkDatabase() {
    clear();
}

ELinkDatabase* ELinkDatabase::instance() {
    if (!sInstance) {
        sInstance = std::make_unique<ELinkDatabase>();
    }
    return sInstance.get();
}

void ELinkDatabase::clear() {
    mIsLoaded = false;
    mStringPoolOffset = 0;
    mBuffer.clear();
    mUsers.clear();
    mUserMap.clear();
}

std::string ELinkDatabase::getStringFromPool(u32 relOffset) const {
    if (mBuffer.empty() || mStringPoolOffset == 0) return "";
    size_t absOffset = mStringPoolOffset + relOffset;
    if (absOffset >= mBuffer.size()) return "";

    const char* strStart = reinterpret_cast<const char*>(mBuffer.data() + absOffset);
    size_t maxLen = mBuffer.size() - absOffset;
    size_t len = 0;
    while (len < maxLen && strStart[len] != '\0') {
        len++;
    }
    return std::string(strStart, len);
}

bool ELinkDatabase::load(const u8* data, size_t size) {
    if (!data || size < 0x80) return false;

    // Check 'XLNK' magic (0x584c4e4b)
    if (readBE32(data) != 0x584c4e4b) {
        return false;
    }

    clear();
    mBuffer.assign(data, data + size);

    // Read string pool offset from word[15] (0x3c)
    mStringPoolOffset = readBE32(mBuffer.data() + 0x3c);
    if (mStringPoolOffset >= size) {
        clear();
        return false;
    }

    size_t poolSize = size - mStringPoolOffset;

    // Read user pointer count from word[12] (0x30)
    u32 ptrCount = readBE32(mBuffer.data() + 0x30);
    if (ptrCount == 0 || ptrCount > 2000) {
        ptrCount = 376;
    }

    // Collect all valid pointers
    std::vector<u32> ptrs;
    ptrs.reserve(ptrCount);
    for (u32 i = 0; i < ptrCount; ++i) {
        size_t off = 0x80 + i * 4;
        if (off + 4 > size) break;
        u32 p = readBE32(mBuffer.data() + off);
        ptrs.push_back(p);
    }

    // Parse each user definition
    for (size_t i = 0; i < ptrs.size(); ++i) {
        u32 p = ptrs[i];
        if (p < 0x1000 || p + 30 * 4 >= size) continue;

        u32 nameRel = readBE32(mBuffer.data() + p + 23 * 4);
        std::string userName = getStringFromPool(nameRel);
        if (userName.empty()) continue;

        u32 parentRel = readBE32(mBuffer.data() + p + 24 * 4);
        std::string userParent = getStringFromPool(parentRel);

        u32 drcsRel = readBE32(mBuffer.data() + p + 28 * 4);
        std::string userDrcs = getStringFromPool(drcsRel);

        ELinkUser user;
        user.name = userName;
        user.parent = userParent;
        user.drcs = userDrcs;
        user.offset = p;

        // Find boundary for effect scanning
        u32 nextP = static_cast<u32>(mStringPoolOffset);
        for (u32 otherP : ptrs) {
            if (otherP > p && otherP < nextP) {
                nextP = otherP;
            }
        }
        if (nextP - p > 2048) {
            nextP = p + 2048;
        }

        // Scan effect cues
        for (u32 wOff = p + 35 * 4; wOff + 4 <= nextP && wOff + 4 <= size; wOff += 4) {
            u32 strRel = readBE32(mBuffer.data() + wOff);
            if (strRel > 0 && strRel < poolSize) {
                std::string cueStr = getStringFromPool(strRel);
                if (cueStr.length() >= 3 &&
                    cueStr.find("FOLDER") == std::string::npos &&
                    cueStr.front() != ' ' &&
                    cueStr != userName &&
                    cueStr != userParent &&
                    cueStr != userDrcs) {
                    if (std::find(user.effects.begin(), user.effects.end(), cueStr) == user.effects.end()) {
                        user.effects.push_back(cueStr);
                    }
                }
            }
        }

        size_t userIdx = mUsers.size();
        mUsers.push_back(user);
        mUserMap[userName] = userIdx;
    }

    mIsLoaded = true;
    return true;
}

bool ELinkDatabase::loadFromSzs(const char* szsPath) {
    if (!szsPath) return false;

    std::ifstream file(szsPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<u8> fileBuf(static_cast<size_t>(fileSize));
    if (!file.read(reinterpret_cast<char*>(fileBuf.data()), fileSize)) {
        return false;
    }

    if (fileSize >= 4 && readBE32(fileBuf.data()) == 0x584c4e4b) {
        return load(fileBuf.data(), fileBuf.size());
    }

    std::vector<u8> decompBuf;
    const u8* sarcData = fileBuf.data();
    size_t sarcSize = fileBuf.size();

    if (fileSize >= 4 && readBE32(fileBuf.data()) == 0x59617A30) {
        if (!sead::Yaz0::decompress(fileBuf.data(), fileBuf.size(), decompBuf)) {
            return false;
        }
        sarcData = decompBuf.data();
        sarcSize = decompBuf.size();
    }

    sead::SarcArchive arc;
    if (!arc.load(sarcData, sarcSize)) {
        return load(sarcData, sarcSize);
    }

    size_t belnkSize = 0;
    const u8* belnkData = arc.getFile("ELink2DB.belnk", &belnkSize);
    if (!belnkData && arc.getFileCount() > 0) {
        const sead::SarcFileInfo* info = arc.getFileInfo(0);
        if (info) {
            belnkData = info->data;
            belnkSize = info->size;
        }
    }

    if (!belnkData || belnkSize == 0) return false;
    return load(belnkData, belnkSize);
}

const ELinkUser* ELinkDatabase::findUser(const std::string& name) const {
    auto it = mUserMap.find(name);
    if (it != mUserMap.end()) {
        return &mUsers[it->second];
    }
    return nullptr;
}

bool ELinkDatabase::hasUser(const std::string& name) const {
    return mUserMap.find(name) != mUserMap.end();
}

std::vector<std::string> ELinkDatabase::getUserNames() const {
    std::vector<std::string> names;
    names.reserve(mUsers.size());
    for (const auto& u : mUsers) {
        names.push_back(u.name);
    }
    return names;
}

std::vector<const ELinkUser*> ELinkDatabase::getChildren(const std::string& parentName) const {
    std::vector<const ELinkUser*> children;
    for (const auto& u : mUsers) {
        if (u.parent == parentName) {
            children.push_back(&u);
        }
    }
    return children;
}

std::vector<std::string> ELinkDatabase::resolveHierarchy(const std::string& name) const {
    std::vector<std::string> hierarchy;
    std::string current = name;
    while (!current.empty()) {
        hierarchy.push_back(current);
        const ELinkUser* u = findUser(current);
        if (!u || u->parent.empty() || u->parent == current) {
            break;
        }
        current = u->parent;
    }
    return hierarchy;
}

bool ELinkDatabase::hasEffect(const std::string& userName, const std::string& effectName) const {
    const ELinkUser* u = findUser(userName);
    if (!u) return false;
    return std::find(u->effects.begin(), u->effects.end(), effectName) != u->effects.end();
}

size_t ELinkDatabase::getEffectCount(const std::string& userName) const {
    const ELinkUser* u = findUser(userName);
    return u ? u->effects.size() : 0;
}

} // namespace Game
