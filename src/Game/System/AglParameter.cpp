#include "Game/System/AglParameter.h"
#include <fstream>
#include <sstream>
#include <cctype>

namespace Game {

AglParameterObj::AglParameterObj() {}

AglParameterObj::~AglParameterObj() {}

bool AglParameterObj::loadFromFile(const char* filePath) {
    if (!filePath) return false;
    std::ifstream file(filePath);
    if (!file.is_open()) return false;

    std::stringstream buffer;
    buffer << file.rdbuf();
    return parseString(buffer.str());
}

bool AglParameterObj::parseString(const std::string& text) {
    mEntries.clear();

    size_t i = 0;
    size_t len = text.length();

    auto skipWhitespace = [&]() {
        while (i < len && (std::isspace(static_cast<unsigned char>(text[i])) || text[i] == '\r' || text[i] == '\n')) {
            i++;
        }
    };

    auto readQuotedString = [&]() -> std::string {
        if (i >= len || text[i] != '"') return "";
        i++; // skip opening quote
        size_t start = i;
        while (i < len && text[i] != '"') {
            i++;
        }
        std::string s = text.substr(start, i - start);
        if (i < len && text[i] == '"') i++; // skip closing quote
        return s;
    };

    auto readToken = [&]() -> std::string {
        skipWhitespace();
        if (i >= len) return "";
        if (text[i] == '{' || text[i] == '}') {
            char c = text[i++];
            return std::string(1, c);
        }
        if (text[i] == '"') {
            return readQuotedString();
        }
        size_t start = i;
        while (i < len && !std::isspace(static_cast<unsigned char>(text[i])) && text[i] != '{' && text[i] != '}') {
            i++;
        }
        return text.substr(start, i - start);
    };

    // Parse tokens
    std::string currentKey = "";
    bool insideParamBlock = false;

    while (i < len) {
        std::string tok = readToken();
        if (tok.empty()) break;

        if (tok == "{") {
            // New block begins
            currentKey = "";
        } else if (tok == "}") {
            // Block ends
            currentKey = "";
        } else {
            // If currentKey is empty, this token might be the key
            if (currentKey.empty()) {
                currentKey = tok;
                mEntries[currentKey] = std::vector<std::string>();
            } else {
                // Otherwise it's a value associated with currentKey
                mEntries[currentKey].push_back(tok);
            }
        }
    }

    return !mEntries.empty();
}

bool AglParameterObj::hasKey(const char* key) const {
    if (!key) return false;
    return mEntries.find(key) != mEntries.end();
}

f32 AglParameterObj::getFloat(const char* key, f32 defaultVal) const {
    if (!key) return defaultVal;
    auto it = mEntries.find(key);
    if (it != mEntries.end() && !it->second.empty()) {
        try {
            return std::stof(it->second[0]);
        } catch (...) {}
    }
    return defaultVal;
}

s32 AglParameterObj::getInt(const char* key, s32 defaultVal) const {
    if (!key) return defaultVal;
    auto it = mEntries.find(key);
    if (it != mEntries.end() && !it->second.empty()) {
        try {
            return std::stoi(it->second[0]);
        } catch (...) {}
    }
    return defaultVal;
}

std::string AglParameterObj::getString(const char* key, const char* defaultVal) const {
    if (!key) return defaultVal ? defaultVal : "";
    auto it = mEntries.find(key);
    if (it != mEntries.end() && !it->second.empty()) {
        return it->second[0];
    }
    return defaultVal ? defaultVal : "";
}

std::vector<f32> AglParameterObj::getFloatArray(const char* key) const {
    std::vector<f32> result;
    if (!key) return result;
    auto it = mEntries.find(key);
    if (it != mEntries.end()) {
        for (const auto& valStr : it->second) {
            try {
                result.push_back(std::stof(valStr));
            } catch (...) {}
        }
    }
    return result;
}

// Strongly typed loader implementations

bool RollerWeaponParams::load(const char* filePath) {
    AglParameterObj obj;
    if (!obj.loadFromFile(filePath)) return false;

    swingLiftFrame = static_cast<u32>(obj.getInt("mSwingLiftFrame", 20));
    splashNum = static_cast<u32>(obj.getInt("mSplashNum", 12));
    splashInitSpeedBase = obj.getFloat("mSplashInitSpeedBase", 8.0f);
    splashInitSpeedRandomZ = obj.getFloat("mSplashInitSpeedRandomZ", 3.0f);
    splashInitSpeedRandomX = obj.getFloat("mSplashInitSpeedRandomX", 0.4f);
    splashDeg = obj.getFloat("mSplashDeg", 2.0f);

    return true;
}

bool ShieldParams::load(const char* filePath) {
    AglParameterObj obj;
    if (!obj.loadFromFile(filePath)) return false;

    maxHp = obj.getFloat("mMaxHp", 10.0f);
    preparationDurationFrame = static_cast<u32>(obj.getInt("mPreparationDurationFrame", 30));
    noDamageRunningDurationFrame = static_cast<u32>(obj.getInt("mNoDamageRunningDurationFrame", 370));
    damage = obj.getFloat("mDamage", 0.5f);
    boundVelLen = obj.getFloat("mBoundVelLen", 2.0f);
    paintRepeatFrame = static_cast<u32>(obj.getInt("mPaintRepeatFrame", 6));
    paintWidth = obj.getFloat("mPaintWidth", 10.0f);

    return true;
}

bool ShachihokoParams::load(const char* filePath) {
    AglParameterObj obj;
    if (!obj.loadFromFile(filePath)) return false;

    hp = obj.getFloat("mHp", 15.0f);
    offsetY = obj.getFloat("mOffsetY", 10.0f);
    victoryPlayerTimeLimitFrame = static_cast<u32>(obj.getInt("mVictoryPlayerTimeLimitFrame", 3600));
    barrierRadius = obj.getFloat("mBarrierRadius", 15.0f);
    barrierMaxScale = obj.getFloat("mBarrierMaxScale", 3.0f);

    return true;
}

bool TrapParams::load(const char* filePath) {
    AglParameterObj obj;
    if (!obj.loadFromFile(filePath)) return false;

    maxHp = obj.getFloat("mMaxHp", 1.0f);
    paintRadius = obj.getFloat("mPaintRadius", 10.0f);
    playerColRadius = obj.getFloat("mPlayerColRadius", 20.0f);
    timerFrame = static_cast<u32>(obj.getInt("mTimerFrame", 600));
    presageFrame = static_cast<u32>(obj.getInt("mPresageFrame", 60));

    return true;
}

} // namespace Game
